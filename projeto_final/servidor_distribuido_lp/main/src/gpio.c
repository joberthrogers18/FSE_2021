#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_sleep.h"

#include "driver/rtc_io.h"
#include "esp32/rom/uart.h"

#include "../inc/gpio.h"
#include "../inc/mqtt.h"
#include "../inc/cJSON.h"

#define TAG "RTCIO"

RTC_DATA_ATTR xQueueHandle filaDeInterrupcao;

// MARK: general functions

void setNivelDispositivo(int dispositivo, int intensidade){
  gpio_set_level(dispositivo, intensidade);
}

int getNivelDispositivo(int dispositivo){
  return rtc_gpio_get_level(dispositivo);
}

// MARK: button functions

void IRAM_ATTR gpio_isr_handler(void *args){
  int pino = (int)args;
  xQueueSendFromISR(filaDeInterrupcao, &pino, NULL);
}

void trataInterrupcaoBotao(void *params){
  int pino;

  while(true){
    if(xQueueReceive(filaDeInterrupcao, &pino, portMAX_DELAY)){
      // De-bouncing
      int estado = rtc_gpio_get_level(pino);
      if(estado == 1){
        gpio_isr_handler_remove(pino);

        extern char btn_topic[200];
        ESP_LOGI(TAG, "BOTÃO CLICADO: %s", btn_topic);

        cJSON *json = cJSON_CreateObject();
        cJSON_AddItemToObject(json, "data", cJSON_CreateNumber(1));
        mqtt_envia_mensagem(btn_topic, cJSON_Print(json));

        // Habilitar novamente a interrupção
        vTaskDelay(50 / portTICK_PERIOD_MS);
        gpio_isr_handler_add(pino, gpio_isr_handler, (void *) pino);

        vTaskDelay(1000 / portTICK_PERIOD_MS);
        printf("Entrando em modo Light Sleep\n");
        uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);
        esp_light_sleep_start();
      }
    }
  }
}

void configuraBotao(int dispositivo){
  // Configuração do pino do Botão
  gpio_pad_select_gpio(dispositivo);
  // Configura o pino do Botão como Entrada
  gpio_set_direction(dispositivo, GPIO_MODE_INPUT);
  // Configura o resistor de Pulldown para o botão (por padrão a entrada estará em Zero)
  gpio_pulldown_en(dispositivo);
  // Desabilita o resistor de Pull-up por segurança.
  gpio_pullup_dis(dispositivo);
  // Configura pino para interrupção
  gpio_set_intr_type(dispositivo, GPIO_INTR_POSEDGE);

  gpio_wakeup_enable(dispositivo, GPIO_INTR_LOW_LEVEL);
  esp_sleep_enable_gpio_wakeup();

  filaDeInterrupcao = xQueueCreate(10, sizeof(int));
  xTaskCreate(trataInterrupcaoBotao, "TrataBotao", 2048, NULL, 1, NULL);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(dispositivo, gpio_isr_handler, (void *) dispositivo);
}