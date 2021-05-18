#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_sleep.h"
#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "driver/rtc_io.h"
#include "mqtt_client.h"
#include "esp32/rom/uart.h"

#include "../inc/mqtt.h"
#include "../inc/nvs.h"
#include "../inc/cJSON.h"
#include "../inc/gpio.h"

#define TAG "MQTT"

int btn_topic_len;

RTC_DATA_ATTR char smac[6];
int esp_cadastrada;

RTC_DATA_ATTR char btn_topic[200] = {0};
RTC_DATA_ATTR esp_mqtt_client_handle_t client;

extern xSemaphoreHandle conexaoMQTTSemaphore;

void register_esp(){
  // obtem MAC ADDRESS
  char comodo[COMODO_MAX_LENGTH_NAME] = {0};
  char mac_address[19];
  uint8_t mac[6];

  esp_efuse_mac_get_default(mac);
  sprintf(mac_address ,"%02x:%02x:%02x:%02x:%02x:%02x",
          mac[0] & 0xff, mac[1] & 0xff, mac[2] & 0xff,
          mac[3] & 0xff, mac[4] & 0xff, mac[5] & 0xff);

  ESP_LOGI(TAG, "MAC ADDRESS: [%s]", mac_address);
  sprintf(smac, "%hhn", mac);

  char topico[sizeof(ADD_DEVICE_PATH) + sizeof(mac_address)];
  sprintf(topico, "%s%s", ADD_DEVICE_PATH, mac_address);

  // padrão: fse2020/160121817/<mac_address>/botao
  char in_topic[strlen(topico) + strlen("/botao")];
  sprintf(in_topic, "%s%s", topico, "/botao");
  btn_topic_len = strlen(in_topic);
  memcpy(btn_topic, in_topic, btn_topic_len);

  // verifica se a esp já está registrada na memória
  ESP_LOGI(TAG, "CHAVE: %s", smac);
  if(le_valor_nvs(smac, comodo) == -2){
    ESP_LOGI(TAG, "ESP NÃO CADASTRADA");
    
    // Gera o tópico com o MAC ADDRESS para solicitação de cadastro
    cJSON *json = cJSON_CreateObject();
    cJSON *data = cJSON_CreateObject();

    cJSON_AddItemToObject(data, "lowMode", cJSON_CreateBool(cJSON_True));
    cJSON_AddItemToObject(json, "data", data);

    mqtt_envia_mensagem(topico, cJSON_Print(json));
    sleep(2);
    mqtt_assinar_canal(ROOM_PATH);
  } else {
    configuraBotao(BOTAO);
  }
}

void ativa_sleep(){
  // LOW POWER
  printf("Entrando em modo Light Sleep\n");
  // Configura o modo sleep somente após completar a escrita na UART para finalizar o printf
  uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);
  // Entra em modo Light Sleep
  esp_light_sleep_start();
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event){
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

      if(strstr(message, "comodo") != NULL){
        char comodo_json[COMODO_MAX_LENGTH_NAME] = {0};
        strcpy(comodo_json, cJSON_GetObjectItemCaseSensitive(jsonResponse, "comodo")->valuestring);
        // CADASTRA DISPOSITIVO NVS
        grava_valor_nvs(smac, comodo_json);
        configuraBotao(BOTAO);
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

void mqtt_assinar_canal(char* topico){
  esp_mqtt_client_subscribe(client, topico, 0);
}
