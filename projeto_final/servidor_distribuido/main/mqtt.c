#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

#include "mqtt.h"
#include "nvs.h"
#include "cJSON.h"
#include "sensor.h"
#include "gpio.h"

#define TAG "MQTT"

char* btn_topic;
char* device_topic;

int esp_cadastrada;
char smac[6];

void register_esp(){
  // obtem MAC ADDRESS
  char mac_address[19];
  uint8_t mac[6];
  char comodo[COMODO_MAX_LENGTH_NAME] = {0};

  esp_efuse_mac_get_default(mac);
  sprintf(mac_address ,"%02x:%02x:%02x:%02x:%02x:%02x",
          mac[0] & 0xff, mac[1] & 0xff, mac[2] & 0xff,
          mac[3] & 0xff, mac[4] & 0xff, mac[5] & 0xff);

  ESP_LOGI(TAG, "MAC ADDRESS: [%s]", mac_address);
  sprintf(smac, "%hhn", mac);

  char topico[sizeof(ADD_DEVICE_PATH) + sizeof(mac_address)];
  sprintf(topico, "%s%s", ADD_DEVICE_PATH, mac_address);
  ESP_LOGI(TAG, "Topico: [%s]", topico);

  // verifica se a esp já está registrada na memória
  if(le_valor_nvs(smac, comodo) == -2){
    ESP_LOGI(TAG, "ESP NÃO CADASTRADA");
    // Gera o tópico com o MAC ADDRESS para solicitação de cadastro
    mqtt_envia_mensagem(topico, "Cadastro ESP");
    sleep(2);
    mqtt_assinar_canal(ROOM_PATH, 1);

  } else {
    ESP_LOGI(TAG, "ESP CADASTRADA");
    ESP_LOGI(TAG, "COMODO RECUPERADO: %s", comodo);
    xTaskCreate(&atualiza_dados_sensores, "Leitura de Sensores", 2048, (void *)comodo, 2, NULL);
  }

  // padrão: fse2020/160121817/<mac_address>/botao
  // padrão: fse2020/160121817/<mac_address>/led
  char in_topic[strlen(topico) + strlen("/botao")];
  char out_topic[strlen(topico) + strlen("/led")];

  sprintf(in_topic, "%s%s", topico, "/botao");
  sprintf(out_topic, "%s%s", topico, "/led");

  btn_topic = in_topic;
  
  ESP_LOGI(TAG, "TÓPICO BOTÃO: %s", btn_topic);
  ESP_LOGI(TAG, "TÓPICO LED: %s", out_topic);

  mqtt_assinar_canal(out_topic, 0);
}

extern xSemaphoreHandle conexaoMQTTSemaphore;
esp_mqtt_client_handle_t client;

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event){
  esp_mqtt_client_handle_t client = event->client;
  int msg_id;

  char message[event->data_len];
  char topic[event->topic_len];
  
  switch (event->event_id) {
    case MQTT_EVENT_CONNECTED:
      ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
      xSemaphoreGive(conexaoMQTTSemaphore);
      break;
    case MQTT_EVENT_DISCONNECTED:
      ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
      break;
    case MQTT_EVENT_SUBSCRIBED:
      ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
      break;
    case MQTT_EVENT_UNSUBSCRIBED:
      ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
      break;
    case MQTT_EVENT_PUBLISHED:
      ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
      break;
    case MQTT_EVENT_DATA:
      message[event->data_len] = '\0';

      memcpy(topic, event->topic, event->topic_len);
      ESP_LOGI(TAG, "MQTT_EVENT_DATA: %s", topic);

      memcpy(message, event->data, event->data_len);

      cJSON *jsonResponse = cJSON_Parse(message);

      if(strstr(topic, "/led") != NULL){
        ESP_LOGI(TAG, "LED: %s\n", message);
        int estado_led = 0;
        memcpy(&estado_led, (int *) &(cJSON_GetObjectItemCaseSensitive(jsonResponse, "data")->valueint), sizeof(int));
        setNivelDispositivo(LED, estado_led);

      } else {
        ESP_LOGI(TAG, "CÔMODO: %s\n", message);

        char comodo_json[COMODO_MAX_LENGTH_NAME] = {0};
        strcpy(comodo_json, cJSON_GetObjectItemCaseSensitive(jsonResponse, "comodo")->valuestring);
        
        // CADASTRA DISPOSITIVO NVS
        grava_valor_nvs(smac, comodo_json);
        xTaskCreate(&atualiza_dados_sensores, "Leitura de Sensores", 2048, (void *)comodo_json, 2, NULL);
      } 

      break;
    case MQTT_EVENT_ERROR:
      ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
      break;
    default:
      ESP_LOGI(TAG, "Other event id:%d", event->event_id);
      break;
  }
  return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
  ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
  mqtt_event_handler_cb(event_data);
}

void mqtt_start(){
  esp_mqtt_client_config_t mqtt_config = {
    .uri = "mqtt://test.mosquitto.org",
  };
  client = esp_mqtt_client_init(&mqtt_config);
  esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
  esp_mqtt_client_start(client);
}

void mqtt_envia_mensagem(char* topico, char* mensagem){
  int message_id = esp_mqtt_client_publish(client, topico, mensagem, 0, 1, 0);
  ESP_LOGI(TAG, "Mensagem enviada, ID: %d", message_id);
}

void mqtt_assinar_canal(char* topico, int device){
  int id_msg_device = esp_mqtt_client_subscribe(client, topico, 0);

  if(device)
    device_topic = topico;
}
