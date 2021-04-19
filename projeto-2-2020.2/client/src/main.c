#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <socket.h>
#include <signal.h>
#include <display_menu.h>
#include <utils.h>

struct pinState state;

void initializeStates() {
    state.temperature = cJSON_CreateNumber(0.0);
    state.humidity = cJSON_CreateNumber(0.0);
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

void finishResources(){
    closeSocket();
    exit(0);
}

int main() {
    pthread_t thread_display_menu, thread_read_menu, thread_message;

    signal(SIGINT, finishResources);

    initializeStates();
    csvCreation();
    initSocket();
    init_menu();

    pthread_create(&thread_display_menu, NULL, display_states, NULL);
    pthread_create(&thread_read_menu, NULL, menu_handler, NULL);
    pthread_create(&thread_message, NULL, sendMessageBySecond, NULL);

    pthread_detach(thread_message);
    pthread_join(thread_display_menu, NULL);
    pthread_join(thread_read_menu, NULL);


    shut_down_menu();

    return 0;
}