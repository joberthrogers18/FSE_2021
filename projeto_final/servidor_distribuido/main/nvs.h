#ifndef NVS_H
#define NVS_H

#include <stdio.h>

#define NVS_KEY_NAME_MAX_SIZE 20

void grava_valor_nvs(char* name, char* valor);
int32_t le_valor_nvs(char* name, char* src);

#endif