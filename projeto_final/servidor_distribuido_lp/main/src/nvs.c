#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "../inc/nvs.h"

#define TAG "NVS"

int32_t le_valor_nvs(char* name, char* src){
  // Inicia o acesso à partição padrão nvs
  ESP_ERROR_CHECK(nvs_flash_init());

  char valor[1024] = {0};
  nvs_handle particao_padrao_handle;
  
  // Abre o acesso à partição nvs
  esp_err_t res_nvs = nvs_open("armazenamento", NVS_READWRITE, &particao_padrao_handle);
  
  if(res_nvs == ESP_ERR_NVS_NOT_FOUND){
    ESP_LOGE(TAG, "Namespace: não encontrado");
  }
  else{
    size_t size = sizeof(valor);
    esp_err_t res = nvs_get_str(particao_padrao_handle, name, valor, &size);

    switch (res){
      case ESP_OK:
        ESP_LOGI(TAG, "Valor armazenado: [%s]", valor);
        strcpy(src, valor);
        break;
      case ESP_ERR_NOT_FOUND:
        ESP_LOGE(TAG, "Valor não encontrado");
        return -1;
      case ESP_ERR_NVS_NOT_FOUND:
        ESP_LOGE(TAG, "Chave não encontrada");
        return -2;
      default:
        ESP_LOGE(TAG, "Erro ao acessar o NVS (%s)", esp_err_to_name(res));
        return -1;
    }

    nvs_close(particao_padrao_handle);
  }
  return valor;
}

void grava_valor_nvs(char* name, char* valor){
  ESP_ERROR_CHECK(nvs_flash_init());

  nvs_handle particao_padrao_handle;

  esp_err_t res_nvs = nvs_open("armazenamento", NVS_READWRITE, &particao_padrao_handle);
  
  if(res_nvs == ESP_ERR_NVS_NOT_FOUND){
    ESP_LOGE(TAG, "não encontrado");
  }
  esp_err_t res = nvs_set_str(particao_padrao_handle, name, valor);
  
  if(res != ESP_OK){
    ESP_LOGE(TAG, "Não foi possível escrever no NVS (%s)", esp_err_to_name(res));
  }
  nvs_commit(particao_padrao_handle);
  nvs_close(particao_padrao_handle);
}
