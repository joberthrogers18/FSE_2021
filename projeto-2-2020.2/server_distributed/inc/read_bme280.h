
#ifndef BMEREAD280_H_
#define BMEREAD280_H_

int bme280Init(int iChannel, int iAddr);
int bme280ReadValues(int *T, int *P, int *H);
void getInformationBME280(int *temperature, int *humidity);

#endif