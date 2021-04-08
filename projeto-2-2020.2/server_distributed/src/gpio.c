#include <bcm2835.h>
#include <stdio.h>
#include <stdlib.h>

// outputs
#define PIN_LAMP_1_KITCHEN     RPI_V2_GPIO_P1_11 // pin 17
#define PIN_LAMP_2_ROOM        RPI_V2_GPIO_P1_12 // pin 18
#define PIN_LAMP_3_BEDROOM_01  RPI_V2_GPIO_P1_13 // pin 27
#define PIN_LAMP_4_BEDROOM_02  RPI_V2_GPIO_P1_15 // pin 27
#define PIN_AR_COND_BEDROOM_01 RPI_V2_GPIO_P1_33 // pin 13
#define PIN_AR_COND_BEDROOM_02 RPI_V2_GPIO_P1_35 // pin 19

// inputs
#define PIN_SENSOR_PRES_01           RPI_V2_GPIO_P1_22 //pin 25
#define PIN_SENSOR_PRES_02           RPI_V2_GPIO_P1_37 //pin 26
#define PIN_SENSOR_DOOR_KITCHEN      RPI_V2_GPIO_P1_29 //pin 05
#define PIN_SENSOR_WINDOW_KITCHEN    RPI_V2_GPIO_P1_31 //pin 06
#define PIN_SENSOR_DOOR_ROOM         RPI_V2_GPIO_P1_32 //pin 12
#define PIN_SENSOR_WINDOW_ROOM       RPI_V2_GPIO_P1_36 //pin 16
#define PIN_SENSOR_WINDOW_BEDROOM_01 RPI_V2_GPIO_P1_38 //pin 20
#define PIN_SENSOR_WINDOW_BEDROOM_02 RPI_V2_GPIO_P1_40 //pin 21

void initPinsGPIO() {
    int responseBcm = bcm2835_init();

    if (responseBcm != 1) {
        printf("Error to initialize bcm2835\n");
        exit(1);
    }

    // initialize outputs
    bcm2835_gpio_fsel(PIN_LAMP_1_KITCHEN, BCM2835_GPIO_FSEL_OUTP); 
    bcm2835_gpio_fsel(PIN_LAMP_2_ROOM, BCM2835_GPIO_FSEL_OUTP); 
    bcm2835_gpio_fsel(PIN_LAMP_3_BEDROOM_01, BCM2835_GPIO_FSEL_OUTP); 
    bcm2835_gpio_fsel(PIN_LAMP_4_BEDROOM_02, BCM2835_GPIO_FSEL_OUTP); 
    bcm2835_gpio_fsel(PIN_AR_COND_BEDROOM_01, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_AR_COND_BEDROOM_02, BCM2835_GPIO_FSEL_OUTP);

    // initialize inputs (sensors)
    bcm2835_gpio_fsel(PIN_SENSOR_PRES_01, BCM2835_GPIO_FSEL_OUTP); 
    bcm2835_gpio_fsel(PIN_SENSOR_PRES_02, BCM2835_GPIO_FSEL_OUTP); 
    bcm2835_gpio_fsel(PIN_SENSOR_DOOR_KITCHEN, BCM2835_GPIO_FSEL_OUTP); 
    bcm2835_gpio_fsel(PIN_SENSOR_WINDOW_KITCHEN, BCM2835_GPIO_FSEL_OUTP); 
    bcm2835_gpio_fsel(PIN_SENSOR_DOOR_ROOM, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_SENSOR_WINDOW_ROOM, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_SENSOR_WINDOW_BEDROOM_01, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_fsel(PIN_SENSOR_WINDOW_BEDROOM_02, BCM2835_GPIO_FSEL_OUTP);
}

void turnOnGPIO() {
    bcm2835_gpio_write(PIN_LAMP_1_KITCHEN, HIGH);
    bcm2835_gpio_write(PIN_LAMP_2_ROOM, HIGH);
    bcm2835_gpio_write(PIN_LAMP_3_BEDROOM_01, HIGH);
    bcm2835_gpio_write(PIN_LAMP_4_BEDROOM_02, HIGH);
    bcm2835_gpio_write(PIN_AR_COND_BEDROOM_01, HIGH);
    bcm2835_gpio_write(PIN_AR_COND_BEDROOM_02, HIGH);
}

void turnOffGPIO() {
    bcm2835_gpio_write(PIN_LAMP_1_KITCHEN, LOW);
    bcm2835_gpio_write(PIN_LAMP_2_ROOM, LOW);
    bcm2835_gpio_write(PIN_LAMP_3_BEDROOM_01, LOW);
    bcm2835_gpio_write(PIN_LAMP_4_BEDROOM_02, LOW);
    bcm2835_gpio_write(PIN_AR_COND_BEDROOM_01, LOW);
    bcm2835_gpio_write(PIN_AR_COND_BEDROOM_02, LOW);
}

void closeGPIO() {
    bcm2835_close();
}
