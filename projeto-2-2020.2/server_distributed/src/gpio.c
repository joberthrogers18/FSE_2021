#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>

#define MIN_RANGE_PWM 0
#define MAX_RANGE_PWM 100

// void initPinsGPIO() {
//     int responseBcm = bcm2835_init();

//     if (responseBcm != 1) {
//         printf("Error to initialize bcm2835\n");
//         exit(1);
//     }

//     // initialize outputs
//     bcm2835_gpio_fsel(PIN_LAMP_1_KITCHEN, BCM2835_GPIO_FSEL_OUTP); 
//     bcm2835_gpio_fsel(PIN_LAMP_2_ROOM, BCM2835_GPIO_FSEL_OUTP); 
//     bcm2835_gpio_fsel(PIN_LAMP_3_BEDROOM_01, BCM2835_GPIO_FSEL_OUTP); 
//     bcm2835_gpio_fsel(PIN_LAMP_4_BEDROOM_02, BCM2835_GPIO_FSEL_OUTP); 
//     bcm2835_gpio_fsel(PIN_AR_COND_BEDROOM_01, BCM2835_GPIO_FSEL_OUTP);
//     bcm2835_gpio_fsel(PIN_AR_COND_BEDROOM_02, BCM2835_GPIO_FSEL_OUTP);

//     // initialize inputs (sensors)
//     bcm2835_gpio_fsel(PIN_SENSOR_PRES_01, BCM2835_GPIO_FSEL_OUTP); 
//     bcm2835_gpio_fsel(PIN_SENSOR_PRES_02, BCM2835_GPIO_FSEL_OUTP); 
//     bcm2835_gpio_fsel(PIN_SENSOR_DOOR_KITCHEN, BCM2835_GPIO_FSEL_OUTP); 
//     bcm2835_gpio_fsel(PIN_SENSOR_WINDOW_KITCHEN, BCM2835_GPIO_FSEL_OUTP); 
//     bcm2835_gpio_fsel(PIN_SENSOR_DOOR_ROOM, BCM2835_GPIO_FSEL_OUTP);
//     bcm2835_gpio_fsel(PIN_SENSOR_WINDOW_ROOM, BCM2835_GPIO_FSEL_OUTP);
//     bcm2835_gpio_fsel(PIN_SENSOR_WINDOW_BEDROOM_01, BCM2835_GPIO_FSEL_OUTP);
//     bcm2835_gpio_fsel(PIN_SENSOR_WINDOW_BEDROOM_02, BCM2835_GPIO_FSEL_OUTP);
// }

// void turnOnGPIO() {
//     bcm2835_gpio_write(PIN_LAMP_1_KITCHEN, HIGH);
//     bcm2835_gpio_write(PIN_LAMP_2_ROOM, HIGH);
//     bcm2835_gpio_write(PIN_LAMP_3_BEDROOM_01, HIGH);
//     bcm2835_gpio_write(PIN_LAMP_4_BEDROOM_02, HIGH);
//     bcm2835_gpio_write(PIN_AR_COND_BEDROOM_01, HIGH);
//     bcm2835_gpio_write(PIN_AR_COND_BEDROOM_02, HIGH);
// }

// void turnOffGPIO() {
//     bcm2835_gpio_write(PIN_LAMP_1_KITCHEN, LOW);
//     bcm2835_gpio_write(PIN_LAMP_2_ROOM, LOW);
//     bcm2835_gpio_write(PIN_LAMP_3_BEDROOM_01, LOW);
//     bcm2835_gpio_write(PIN_LAMP_4_BEDROOM_02, LOW);
//     bcm2835_gpio_write(PIN_AR_COND_BEDROOM_01, LOW);
//     bcm2835_gpio_write(PIN_AR_COND_BEDROOM_02, LOW);
// }

// void closeGPIO() {
//     bcm2835_close();
// }

void turnONOrOFFDevice(int pin, int isOn){
  pinMode(pin, OUTPUT);
  softPwmCreate(pin,  MIN_RANGE_PWM, MAX_RANGE_PWM);

  if (isOn) {
    softPwmWrite(pin, MAX_RANGE_PWM);
  } else {
      softPwmWrite(pin, MIN_RANGE_PWM);
  }
}