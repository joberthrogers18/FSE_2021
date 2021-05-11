#ifndef GPIO_H_
#define GPIO_H_

#define LED_1 2
#define BOTAO 0

void configuraBotao(int dispositivo);
void configuraLed(int dispositivo);

void setNivelDispositivo(int dispositivo, int intensidade);
int getNivelDispositivo(int dispositivo);

#endif