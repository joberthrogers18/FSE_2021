#ifndef BMEREAD280_H_
#define BMEREAD280_H_

// void read_from_bme();
int bme280Init(int iChannel, int iAddr);
int bme280ReadValues(int *T, int *P, int *H);

#endif