#include <wiringPiI2C.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef LCD_CONTROL_H_
#define LCD_CONTROL_H_

void printInScreen(float *TE, float *TI, float *TR);
void lcd_init();

#endif