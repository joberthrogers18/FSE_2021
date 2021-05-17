#ifndef MQTT_H
#define MQTT_H

#define COMODO_MAX_LENGTH_NAME 20
#define IN_MAX_LENGTH 12

#define BASE_PATH "fse2020/160121817/"

// Recebe cadastro do dispositivo pelo servidor central 
// Padrão: fse2020/160121817/<cômodo>
#define ROOM_PATH "fse2020/160121817/+"
#define ADD_DEVICE_PATH "fse2020/160121817/dispositivos/"

void mqtt_start();
void mqtt_envia_mensagem(char * topico, char * mensagem);
void mqtt_assinar_canal(char* topico, int device);
void register_esp();

#endif
