#include <stdio.h>
#include <read_bme280.h>
#include <pthread.h>
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

pthread_t threads[2];

void initResources() {
    int i = bme280Init(CHANNEL_BME280, ADDRESS_BME280);
	if (i != 0)
	{
		printf("Erro to open\n");
        exit(1);
	}
}

void finishResources(){
    pthread_join(threads[0], NULL);
    closeSocket();
    exit(0);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, finishResources);

    pthread_create(&threads[0], NULL, initializeInteruption, NULL);
    
    while (1)
    {
        printf("Lamp 1: %d, Lamp 2: %d, Lamp 3: %d, Lamp 4: %d \n", state.lamp1, state.lamp2, state.lamp3, state.lamp4);
        printf("Ar Condition 1: %d, Ar Condition 2: %d \n", state.arCondition1, state.arCondition2);
        printf("Sensor Pres. 1: %d, Sensor Pres. 2: %d, Sensor Door Kitchen: %d, Sensor Window Kitchen: %d \n", state.sensorPres1, state.sensorPres1, state.sensorDoorKitchen, state.sensorWindowKitchen);
        printf("Sensor Door Room: %d, Sensor Window Room: %d, Sensor Window Bedroom1: %d, Sensor Window Bedroom2: %d \n", state.sensorDoorRoom, state.sensorWindowRoom, state.sensorWindowBedroom1, state.sensorWindowBedroom2);
        sleep(1);
    }
    

    // wiringPiSetup();

    // turnONOrOFFDevice(PIN_LAMP_1_KITCHEN, ON);
    // turnONOrOFFDevice(PIN_LAMP_2_ROOM, ON);
    // turnONOrOFFDevice(PIN_LAMP_3_BEDROOM_01, ON);
    // turnONOrOFFDevice(PIN_LAMP_4_BEDROOM_02, ON);
    // turnONOrOFFDevice(PIN_AR_COND_BEDROOM_01, ON);
    // turnONOrOFFDevice(PIN_AR_COND_BEDROOM_02, ON);

    // sleep(5);

    // turnONOrOFFDevice(PIN_LAMP_1_KITCHEN, OFF);
    // turnONOrOFFDevice(PIN_LAMP_2_ROOM, OFF);
    // turnONOrOFFDevice(PIN_LAMP_3_BEDROOM_01, OFF);
    // turnONOrOFFDevice(PIN_LAMP_4_BEDROOM_02, OFF);
    // turnONOrOFFDevice(PIN_AR_COND_BEDROOM_01, OFF);
    // turnONOrOFFDevice(PIN_AR_COND_BEDROOM_02, OFF);




    // initResources();
    // getInformationBME280(&temperatureExternal, &humidity);

    // float temperatureExt = (float)temperatureExternal/100;
    // float humidityParser = (float)humidity/1000;
    // printf("Temperature: %.2f Humidity: %.2f\n", temperatureExt, humidityParser);

    // initSocket();

    // listenSocket();

    return 0;
}
