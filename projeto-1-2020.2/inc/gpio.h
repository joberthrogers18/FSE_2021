#ifndef GPIO_H_
#define GPIO_H_

void controlFanResistorPWM(int control_signal);
void turnOFFResistor();
void turnOFFFAN();
void turnONFAN(int newFanValue);
void turnONResistor(int newResistorValue);

#endif /* PID_H_ */