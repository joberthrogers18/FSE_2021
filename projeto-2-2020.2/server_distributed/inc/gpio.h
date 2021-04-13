#ifndef GPIO_H_
#define GPIO_H_

// outputs
#define PIN_LAMP_1_KITCHEN     0 // pin 17
#define PIN_LAMP_2_ROOM        1 // pin 18
#define PIN_LAMP_3_BEDROOM_01  2 // pin 27
#define PIN_LAMP_4_BEDROOM_02  3 // pin 22
#define PIN_AR_COND_BEDROOM_01 23 // pin 13
#define PIN_AR_COND_BEDROOM_02 24 // pin 19

// inputs
#define PIN_SENSOR_PRES_01           6 //pin 25
#define PIN_SENSOR_PRES_02           25 //pin 26
#define PIN_SENSOR_DOOR_KITCHEN      21 //pin 05
#define PIN_SENSOR_WINDOW_KITCHEN    22 //pin 06
#define PIN_SENSOR_DOOR_ROOM         26 //pin 12
#define PIN_SENSOR_WINDOW_ROOM       27 //pin 16
#define PIN_SENSOR_WINDOW_BEDROOM_01 28 //pin 20
#define PIN_SENSOR_WINDOW_BEDROOM_02 29 //pin 21

void turnONOrOFFDevice(int pin, int isOn);
// void initPinsGPIO();
// void turnOnGPIO();
// void turnOffGPIO();
// void closeGPIO();


#endif