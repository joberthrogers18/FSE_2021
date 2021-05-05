#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "freertos/queue.h"

#include "dht11.h"
#include "gpio.h"
#include "wifi.h"

#define clear() printf("\e[1;1H\e[2J")

void atualizaSensores(void * params){
    DHT11_init(GPIO_NUM_4);
    while(1) {
        printf("Temperature is %d \n", DHT11_read().temperature);
        printf("Humidity is %d\n", DHT11_read().humidity);
        printf("Status code is %d\n\n\n", DHT11_read().status);
        clear();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(){
    configure_wifi();

    xTaskCreate(&atualizaSensores, "Leitura de Sensores", 2048, NULL, 2, NULL);
    configuraLed(LED_1);
    configuraBotao(BOTAO);
}
