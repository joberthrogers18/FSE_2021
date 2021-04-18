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
#include <utils.h>
#include <cJSON.h>

#define ADDRESS_BME280 0x76
#define CHANNEL_BME280 1

struct pinState state;

pthread_t threads[2];
int temperatureExternal = 0.0;
int humidity = 0.0;
float temperatureExt = 0.0;
float humidityParser = 0.0;

void initResources() {
    int i = bme280Init(CHANNEL_BME280, ADDRESS_BME280);
	
    if (i != 0)
	{
		printf("Erro to open\n");
        exit(1);
	}
    
    wiringPiSetup();
    initSocket();
    turnOFFAllDevices();
    // csvCreation();

    pthread_create(&threads[0], NULL, initializeStateHandle, NULL);
    pthread_detach(threads[0]);
    pthread_create(&threads[1], NULL, listenSocket, NULL);
    pthread_detach(threads[0]);
}

void finishResources(){
    closeSocket();
    exit(0);
}

void initializeStates() {
    state.lamp1 = cJSON_CreateNumber(0);
    state.lamp2 = cJSON_CreateNumber(0);
    state.lamp3 = cJSON_CreateNumber(0);
    state.lamp4 = cJSON_CreateNumber(0);
    state.arCondition1 = cJSON_CreateNumber(0);
    state.arCondition2 = cJSON_CreateNumber(0);
    state.sensorPres1 = cJSON_CreateNumber(0);
    state.sensorPres2 = cJSON_CreateNumber(0);
    state.sensorDoorKitchen = cJSON_CreateNumber(0);
    state.sensorWindowKitchen = cJSON_CreateNumber(0);
    state.sensorWindowRoom = cJSON_CreateNumber(0);
    state.sensorDoorRoom = cJSON_CreateNumber(0);
    state.sensorWindowRoom = cJSON_CreateNumber(0);
    state.sensorWindowBedroom1 = cJSON_CreateNumber(0);
    state.sensorWindowBedroom2 = cJSON_CreateNumber(0);
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, finishResources);

    initResources();
    initializeStates();

    while (1)
    {
        getInformationBME280(&temperatureExternal, &humidity);
        temperatureExt = (float)temperatureExternal/100;
        humidityParser = (float)humidity/1000;
        sleep(1);
    }

    return 0;
}
