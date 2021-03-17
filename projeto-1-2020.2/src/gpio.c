#include <wiringPi.h> 
#include <softPwm.h> 

#define RESISTOR_GPIO_PIN 4
#define FAN_GPIO_PIN 5
#define MIN_RANGE_PWM 0
#define MAX_RANGE_PWM 100

void controlFanResistorPWM(int control_signal) {
    if (control_signal > 0) {
        turnONResistor((int) control_signal);
        turnOFFFAN();
    }
    else {
        if (control_signal < -20.0) {
            turnONResistor((int) (-1.0 * control_signal));
        }
        else {
            turnOFFFAN();
        }

        turnOFFResistor();
    }
}

void turnONResistor(int newResistorValue){
  pinMode(RESISTOR_GPIO_PIN, OUTPUT);
  softPwmCreate(RESISTOR_GPIO_PIN,  MIN_RANGE_PWM, 100);
  softPwmWrite(RESISTOR_GPIO_PIN, newResistorValue);
}

void turnONFAN(int newFanValue){
  pinMode(FAN_GPIO_PIN,OUTPUT);
  softPwmCreate(FAN_GPIO_PIN, MIN_RANGE_PWM, MAX_RANGE_PWM);
  softPwmWrite(FAN_GPIO_PIN, newFanValue);
}

void turnOFFResistor(){
  softPwmCreate(RESISTOR_GPIO_PIN, MIN_RANGE_PWM, MAX_RANGE_PWM);
  pinMode(RESISTOR_GPIO_PIN,OUTPUT);
  softPwmWrite(RESISTOR_GPIO_PIN, MIN_RANGE_PWM);
}

void turnOFFFAN(){
  pinMode(FAN_GPIO_PIN,OUTPUT);
  softPwmCreate(FAN_GPIO_PIN, MIN_RANGE_PWM, MAX_RANGE_PWM);
  softPwmWrite(FAN_GPIO_PIN, MIN_RANGE_PWM);
}