#include <stdio.h>
#include <socket.h>
#include <stdlib.h>

struct pinState state;

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
    initSocket();
    initializeStates();
//    printf("passou aqui \n");
//    sendMessage("teste");
    sendMessage();
    closeSocket();

    // system("omxplayer /home/joberthcosta/client/alarm.mp3");
    
    return 0;
}
