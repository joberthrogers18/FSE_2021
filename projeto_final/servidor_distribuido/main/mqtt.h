#ifndef MQTT_H
#define MQTT_H

#define BASE_PATH "fse2020/160121817/"
#define ADD_DEVICE_PATH "fse2020/160121817/dispositivos/"

void mqtt_start();

void mqtt_envia_mensagem(char * topico, char * mensagem);

#endif
