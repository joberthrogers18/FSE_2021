#include <stdio.h>
#include <read_bme280.h>
#include <gpio.h>
#include <unistd.h>
#include <stdlib.h>
#include <socket.h>
#include <unistd.h>
#include <signal.h>

#define ADDRESS_BME280 0x76
#define CHANNEL_BME280 1

int temperatureExternal = 0.0;
int humidity = 0.0;

void initResources() {
    int i = bme280Init(CHANNEL_BME280, ADDRESS_BME280);
	if (i != 0)
	{
		printf("Erro to open\n");
        exit(1);
	}
}

void finishResources(){
    closeSocket();
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, finishResources);

    // initResources();
    // getInformationBME280(&temperatureExternal, &humidity);

    // float temperatureExt = (float)temperatureExternal/100;
    // float humidityParser = (float)humidity/1000;
    // printf("Temperature: %.2f Humidity: %.2f\n", temperatureExt, humidityParser);

    // initPinsGPIO();

    // turnOnGPIO();

    // sleep(10);

    // turnOffGPIO();

    // closeGPIO();

    initSocket();

    void listenSocket();

    return 0;
}
