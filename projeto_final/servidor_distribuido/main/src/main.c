#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "../inc/gpio.h"
#include "../inc/mqtt.h"
#include "../inc/wifi.h"
#include "../inc/nvs.h"

#define TAG "Main"
#define FLOAT_SIZE_MAX 318

#define clear() printf("\e[1;1H\e[2J")

void app_main(){
    // configura wifi
    // conecta na rede
    // se a conexão funcionar, inicia a conexão MQTT
    configure_wifi();
}
