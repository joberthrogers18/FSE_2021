#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <socket.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cJSON.h>
#include <gpio.h>

#define PORT_SERVER 10018
#define IP_CLIENT "192.168.0.53"
#define NUMBER_QUEUE 20
// #define ON 1
#define OFF 0

#define PORT_SERVER 10018
#define IP_SERVER "192.168.0.4"

int serverSocket;
int socketClient;
struct sockaddr_in serverAddr;
struct sockaddr_in clientAddr;
unsigned int clientLength;
extern char *buffer;

void initSocket() {
    // create a Socket 
    if ((serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printf("Error when start socket. Try again\n");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(PORT_SERVER);

    // Bind
	if(bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0) {
		printf("Failed tried bind. Try again later!\n");
        exit(1);
    }

    if (listen(serverSocket, NUMBER_QUEUE) < 0) {
        printf("Failed in listening\n");
    }
}

void turnOFFAllDevices() {
    turnONOrOFFDevice(PIN_LAMP_1_KITCHEN, OFF);
    turnONOrOFFDevice(PIN_LAMP_2_ROOM, OFF);
    turnONOrOFFDevice(PIN_LAMP_3_BEDROOM_01, OFF);
    turnONOrOFFDevice(PIN_LAMP_4_BEDROOM_02, OFF);
    turnONOrOFFDevice(PIN_AR_COND_BEDROOM_01, OFF);
    turnONOrOFFDevice(PIN_AR_COND_BEDROOM_02, OFF);
}

void handleDeviceState() {
    printf("lamp1: %d; lamp2: %d;lamp3: %d; lamp4: %d\n", state.lamp2->valueint, state.lamp2->valueint, state.lamp3->valueint, state.lamp4->valueint);
    printf("ar1: %d; ar1: %d;\n", state.arCondition1->valueint, state.arCondition2->valueint);
    printf("sp1: %d; lamp2: %d\n", state.sensorPres1->valueint, state.sensorPres2->valueint);
    // turnONOrOFFDevice(PIN_LAMP_1_KITCHEN, state.lamp1->valueint);
    // turnONOrOFFDevice(PIN_LAMP_2_ROOM, state.lamp2->valueint);
    // turnONOrOFFDevice(PIN_LAMP_3_BEDROOM_01, state.lamp3->valueint);
    // turnONOrOFFDevice(PIN_LAMP_4_BEDROOM_02, state.lamp4->valueint);
    // turnONOrOFFDevice(PIN_AR_COND_BEDROOM_01, state.arCondition1->valueint);
    // turnONOrOFFDevice(PIN_AR_COND_BEDROOM_02, state.arCondition2->valueint);
}

void handlerCLientRequest() {
    buffer = malloc(sizeof(char) * 1100);
    // int sizeReceive;

    // if ((sizeReceive = recv(socketClient, buffer, 1100, 0)) < 0) {
    //     printf("Error when receive request\n");
    // }

    read(socketClient , buffer, 1100);

    cJSON *dataStatus = cJSON_Parse(buffer);
    state.lamp1 = cJSON_GetObjectItemCaseSensitive(dataStatus, "lamp1");
    state.lamp2 = cJSON_GetObjectItemCaseSensitive(dataStatus, "lamp2");
    state.lamp3 = cJSON_GetObjectItemCaseSensitive(dataStatus, "lamp3");
    state.lamp4 = cJSON_GetObjectItemCaseSensitive(dataStatus, "lamp4");
    state.arCondition1 = cJSON_GetObjectItemCaseSensitive(dataStatus, "ar1");
    state.arCondition2 = cJSON_GetObjectItemCaseSensitive(dataStatus, "ar2");

    handleDeviceState();

    cJSON *sendDataStatus = cJSON_CreateObject();
    cJSON_AddItemToObject(sendDataStatus, "temperature", cJSON_CreateNumber(state.temperature->valuedouble));
    cJSON_AddItemToObject(sendDataStatus, "humidity", cJSON_CreateNumber(state.humidity->valuedouble));
    cJSON_AddItemToObject(sendDataStatus, "lamp1", cJSON_CreateNumber(state.sensorPres1->valueint));
    cJSON_AddItemToObject(sendDataStatus, "lamp2", cJSON_CreateNumber(state.lamp2->valueint));
    cJSON_AddItemToObject(sendDataStatus, "lamp3", cJSON_CreateNumber(state.lamp3->valueint));
    cJSON_AddItemToObject(sendDataStatus, "lamp4", cJSON_CreateNumber(state.lamp4->valueint));
    cJSON_AddItemToObject(sendDataStatus, "ar1", cJSON_CreateNumber(state.arCondition1->valueint));
    cJSON_AddItemToObject(sendDataStatus, "ar2", cJSON_CreateNumber(state.arCondition2->valueint));
    cJSON_AddItemToObject(sendDataStatus, "sp1", cJSON_CreateNumber(state.sensorPres2->valueint));
    cJSON_AddItemToObject(sendDataStatus, "sp2", cJSON_CreateNumber(state.sensorPres2->valueint));
    cJSON_AddItemToObject(sendDataStatus, "sdk", cJSON_CreateNumber(state.sensorDoorKitchen->valueint));
    cJSON_AddItemToObject(sendDataStatus, "swk", cJSON_CreateNumber(state.sensorWindowKitchen->valueint));
    cJSON_AddItemToObject(sendDataStatus, "swr", cJSON_CreateNumber(state.sensorWindowRoom->valueint));
    cJSON_AddItemToObject(sendDataStatus, "sdr", cJSON_CreateNumber(state.sensorDoorRoom->valueint));
    cJSON_AddItemToObject(sendDataStatus, "swb1", cJSON_CreateNumber(state.sensorWindowBedroom1->valueint));
    cJSON_AddItemToObject(sendDataStatus, "swb2", cJSON_CreateNumber(state.sensorWindowBedroom2->valueint));

    char *parseDataStatus = cJSON_Print(sendDataStatus);
    unsigned sizeMessage = strlen(parseDataStatus);

    if (send(socketClient, parseDataStatus, strlen(parseDataStatus), 0) != sizeMessage){
        printf("Error when send message: number of bytes different from was send.");
    }

    // while (sizeReceive > 0) {
    //     if (send(socketClient, buffer, sizeReceive, 0) != sizeReceive) {
    //         printf("Error when send try send message\n");
    //     }

    //     if ((sizeReceive = recv(socketClient, buffer, 16, 0)) < 0) {
    //         printf("Error when try receive responde \n");
    //     }
    // }

    free(buffer);
}

void *listenSocket() {
    clientLength = sizeof(clientAddr);

    if ((socketClient = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientLength)) < 0) {
        printf("Failed in accept\n");    
    }

    while(1) {
        handlerCLientRequest();
    }
}

void closeSocket() {
    close(socketClient);
    close(serverSocket);
}
