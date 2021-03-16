#include <stdio.h>
#include <lcd_control.h>
#include <read_bme280.h>
#include <modbus.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    initUART();

    lcd_init();

    int i = bme280Init(1, 0x76);
	if (i != 0)
	{
		printf("Erro to open\n"); // problem - quit
	}

    while (1)
    {    
        float internalTemperature, referenceTemperature;

        getInformationModbus(&internalTemperature, &referenceTemperature);

        int temperatureExternal;

        getInformationBME280(&temperatureExternal);

        float temperatureExt = (float)temperatureExternal/100;

        printInScreen(&temperatureExt, &internalTemperature, &referenceTemperature);

        usleep(1000);
    }

    closeUART();

    return 0;
}
