#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "gpio.h"
#include "mqtt.h"
#include "wifi.h"
#include "nvs.h"

#define TAG "Main"
#define FLOAT_SIZE_MAX 318

#define clear() printf("\e[1;1H\e[2J")

void app_main(){
    configuraLed(LED);
    configuraBotao(BOTAO);
    // configura wifi
    // conecta na rede
    // se a conexão funcionar, inicia a conexão MQTT
    configure_wifi();
}
