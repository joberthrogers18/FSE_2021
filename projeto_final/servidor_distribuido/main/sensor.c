#include <stdio.h>
#include <string.h>

#include "esp_log.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "mqtt.h"
#include "dht11.h"
#include "sensor.h"

#define TAG "SENSOR"

void atualiza_dados_sensores(void * comodo){
    char comodo_s[COMODO_MAX_LENGTH_NAME] = {0};
    memcpy(comodo_s, comodo, COMODO_MAX_LENGTH_NAME);

    ESP_LOGI(TAG, "CÔMODO: %s", comodo_s);

    DHT11_init(GPIO_NUM_4);
    while(1) {
        if(DHT11_read().status >= 0){
            char temperatura[sizeof(DHT11_read().temperature)];
            char umidade[sizeof(DHT11_read().humidity)];
            char statusCode[sizeof(DHT11_read().status)];

            sprintf(temperatura, "%d", DHT11_read().temperature);
            sprintf(umidade, "%d", DHT11_read().humidity);
            sprintf(statusCode, "%d", DHT11_read().status);

            char topic_temp[MAX_SIZE_TOPIC_LENGTH] = {0};
            char topic_umi[MAX_SIZE_TOPIC_LENGTH] = {0};
            char topic_stt[MAX_SIZE_TOPIC_LENGTH] = {0};

            strcpy(topic_temp, BASE_PATH);
            strcat(topic_temp, comodo_s);
            strcat(topic_temp, "/temperatura");

            strcpy(topic_umi, BASE_PATH);
            strcat(topic_umi, comodo_s);
            strcat(topic_umi, "/umidade");

            strcpy(topic_stt, BASE_PATH);
            strcat(topic_stt, comodo_s);
            strcat(topic_stt, "/status");

            mqtt_envia_mensagem(topic_temp, temperatura);
            mqtt_envia_mensagem(topic_umi, umidade);
            mqtt_envia_mensagem(topic_stt, statusCode);

        } else {
            ESP_LOGI(TAG, "Erro ao recuperar temperatura");
        }
        // clear();
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}