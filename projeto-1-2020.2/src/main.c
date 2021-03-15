#include <stdio.h>
#include <lcd_control.h>
#include <read_bme280.h>
#include <modbus.h>
#include <unistd.h>

// delay between samples in microseconds
#define DELAY 1000000

int main(int argc, char const *argv[])
{
    // lcd_init();

    // printInScreen("Teste 1", "Teste 2");

    // read_from_bme();

    // getInformationModbus();

    int i;
    int T, P, H; // calibrated values


	i = bme280Init(1, 0x76);
	if (i != 0)
	{
		return -1; // problem - quit
	}


	printf("BME280 device successfully opened.\n");
	usleep(1000000); // wait for data to settle for first read

	// for (i=0; i<120; i++) // read values twice a second for 1 minute
	// {
    bme280ReadValues(&T, &P, &H);
    T -= 150; // for some reason, the sensor reports temperatures too high

    printf("Calibrated temp. = %3.2f C, pres. = %6.2f Pa, hum. = %2.2f%%\n", (float)T/100.0, (float)P/256.0, (float)H/1024.0);
    usleep(DELAY);
	// }

    return 0;
}
