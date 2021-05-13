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

#define TAG "MQTT"

char * device_topic;

void register_esp(){
  uint8_t mac[6]; 
  char mac_address[19]; 

  esp_efuse_mac_get_default(mac);
  sprintf(mac_address ,"%02x:%02x:%02x:%02x:%02x:%02x",
          mac[0] & 0xff, mac[1] & 0xff, mac[2] & 0xff,
          mac[3] & 0xff, mac[4] & 0xff, mac[5] & 0xff);

  ESP_LOGI(TAG, "MAC ADDRESS: [%s]", mac_address);

  char topico[sizeof(ADD_DEVICE_PATH) + sizeof(mac_address)];
  sprintf(topico, "%s%s", ADD_DEVICE_PATH, mac_address);

  ESP_LOGI(TAG, "Topico: [%s]", topico);

  mqtt_start();
  mqtt_envia_mensagem(topico, "testando");
  sleep(2);
  mqtt_assinar_canal(topico);
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
      ESP_LOGI(TAG, "MQTT_EVENT_DATA");
      memcpy(message, event->data, event->data_len);
      memcpy(topic, event->topic, event->topic_len);
      message[event->data_len] = '\0';

      if(strcmp(topic, device_topic)){
        printf("MENSAGEM NO CANAL DO DISPOSITIVO: %s\n", message);
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

void mqtt_envia_mensagem(char * topico, char * mensagem){
  int message_id = esp_mqtt_client_publish(client, topico, mensagem, 0, 1, 0);
  ESP_LOGI(TAG, "Mensagem enviada, ID: %d", message_id);
}

void mqtt_assinar_canal(char * topico){
  int id_msg_device = esp_mqtt_client_subscribe(client, topico, 0);
  device_topic = topico;
}
