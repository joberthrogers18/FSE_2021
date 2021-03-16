#ifndef MODBUS_H_
#define MODBUS_H_

void initUART();
void getInformationModbus(float *TE, float *TR);
void closeUART();

#endif