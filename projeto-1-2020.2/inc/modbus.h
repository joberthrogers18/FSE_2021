#ifndef MODBUS_H_
#define MODBUS_H_

void initUART();
void getInformationModbus(float *TI, float *TR, int referenceManual);
void closeUART();

#endif