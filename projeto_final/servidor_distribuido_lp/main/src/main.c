#include <stdio.h>

#include "freertos/FreeRTOS.h"

#include "../inc/gpio.h"
#include "../inc/mqtt.h"
#include "../inc/wifi.h"
#include "../inc/nvs.h"

#define clear() printf("\e[1;1H\e[2J")

void app_main(){
    configuraBotao(BOTAO);
    // configura wifi
    // conecta na rede
    // se a conexão funcionar, inicia a conexão MQTT
    configure_wifi();
}
