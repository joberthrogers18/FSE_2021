#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "dht11.h"
#include "gpio.h"

void app_main()
{
    DHT11_init(GPIO_NUM_4);

    // configuraBotao();
    configuraLed();
    while(1) {
        ligaLED(0);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        ligaLED(1);
    }
}