#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "dht11.h"
#include "gpio.h"
#include "mqtt.h"
#include "wifi.h"

#define TAG "Main"
#define FLOAT_SIZE_MAX 318

#define clear() printf("\e[1;1H\e[2J")

void atualizaSensores(void * params){
    DHT11_init(GPIO_NUM_4);
    while(1) {
        if(DHT11_read().status >= 0){
            printf("Temperature is %d \n", DHT11_read().temperature);
            printf("Humidity is %d\n", DHT11_read().humidity);
            printf("Status code is %d\n\n\n", DHT11_read().status);

            char temperatura[sizeof(DHT11_read().temperature)];
            char umidade[sizeof(DHT11_read().humidity)];
            char statusCode[sizeof(DHT11_read().status)];

            sprintf(temperatura, "%d", DHT11_read().temperature);
            sprintf(umidade, "%d", DHT11_read().humidity);
            sprintf(statusCode, "%d", DHT11_read().status);

            mqtt_envia_mensagem("fse2020/160121817/sala/temperatura", temperatura);
            mqtt_envia_mensagem("fse2020/160121817/sala/umidade", umidade);
            mqtt_envia_mensagem("fse2020/160121817/sala/status", statusCode);

        } else {
            ESP_LOGI(TAG, "Erro ao recuperar temperatura");
        }
        // clear();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(){
    // configura wifi
    // conecta na rede
    // se a conexão funcionar, inicia a conexão MQTT
    configure_wifi();

    xTaskCreate(&atualizaSensores, "Leitura de Sensores", 2048, NULL, 2, NULL);
    configuraLed(LED_1);
    configuraBotao(BOTAO);
}
