#include <stdio.h>
#include <read_bme280.h>
#include <gpio.h>
#include <unistd.h>
#include <stdlib.h>
#include <socket.h>
#include <unistd.h>
#include <signal.h>
#include <wiringPi.h>

#define ON 1
#define OFF 0

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

    wiringPiSetup();

    turnONOrOFFDevice(PIN_LAMP_1_KITCHEN, ON);
    turnONOrOFFDevice(PIN_LAMP_2_ROOM, ON);
    turnONOrOFFDevice(PIN_LAMP_3_BEDROOM_01, ON);
    turnONOrOFFDevice(PIN_LAMP_4_BEDROOM_02, ON);
    turnONOrOFFDevice(PIN_AR_COND_BEDROOM_01, ON);
    turnONOrOFFDevice(PIN_AR_COND_BEDROOM_02, ON);

    sleep(5);

    turnONOrOFFDevice(PIN_LAMP_1_KITCHEN, OFF);
    turnONOrOFFDevice(PIN_LAMP_2_ROOM, OFF);
    turnONOrOFFDevice(PIN_LAMP_3_BEDROOM_01, OFF);
    turnONOrOFFDevice(PIN_LAMP_4_BEDROOM_02, OFF);
    turnONOrOFFDevice(PIN_AR_COND_BEDROOM_01, OFF);
    turnONOrOFFDevice(PIN_AR_COND_BEDROOM_02, OFF);


    // initResources();
    // getInformationBME280(&temperatureExternal, &humidity);

    // float temperatureExt = (float)temperatureExternal/100;
    // float humidityParser = (float)humidity/1000;
    // printf("Temperature: %.2f Humidity: %.2f\n", temperatureExt, humidityParser);

    // initSocket();

    // listenSocket();

    return 0;
}
