#include <stdio.h>
#include <lcd_control.h>
#include <read_bme280.h>
#include <modbus.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    // lcd_init();

    // read_from_bme();

    lcd_init();

    float internalTemperature, referenceTemperature;

    getInformationModbus(&internalTemperature, &referenceTemperature);

    int temperatureExternal;

    getInformationBME280(&temperatureExternal);

    float temperatureExt = (float)temperatureExternal/100;

    printInScreen(&temperatureExt, &internalTemperature, &referenceTemperature);

    return 0;
}
