#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"

#include "../inc/gpio.h"
#include "../inc/mqtt.h"

#define TAG "GPIO"

xQueueHandle filaDeInterrupcao;

int levelLED = 0;

// MARK: general functions

void setNivelDispositivo(int dispositivo, int intensidade){
  gpio_set_level(dispositivo, intensidade);
}

int getNivelDispositivo(int dispositivo){
  return gpio_get_level(dispositivo);
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
      int estado = gpio_get_level(pino);
      if(estado == 1){
        gpio_isr_handler_remove(pino);

        extern char btn_topic[200];
        ESP_LOGI(TAG, "BOTÃO CLICADO");
        mqtt_envia_mensagem(btn_topic, "{ \"data\": 1 }");

        // Habilitar novamente a interrupção
        vTaskDelay(50 / portTICK_PERIOD_MS);
        gpio_isr_handler_add(pino, gpio_isr_handler, (void *) pino);
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

  filaDeInterrupcao = xQueueCreate(10, sizeof(int));
  xTaskCreate(trataInterrupcaoBotao, "TrataBotao", 2048, NULL, 1, NULL);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(dispositivo, gpio_isr_handler, (void *) dispositivo);
}

// MARK: led functions

void configuraLed(int dispositivo){
  // Configuração dos pinos dos LEDs 
  gpio_pad_select_gpio(dispositivo);   
  // Configura os pinos dos LEDs como Output
  gpio_set_direction(dispositivo, GPIO_MODE_OUTPUT);
}