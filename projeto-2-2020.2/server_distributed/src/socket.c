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
    turnONOrOFFDevice(PIN_LAMP_1_KITCHEN, state.lamp1->valueint);
    turnONOrOFFDevice(PIN_LAMP_2_ROOM, state.lamp2->valueint);
    turnONOrOFFDevice(PIN_LAMP_3_BEDROOM_01, state.lamp3->valueint);
    turnONOrOFFDevice(PIN_LAMP_4_BEDROOM_02, state.lamp4->valueint);
    turnONOrOFFDevice(PIN_AR_COND_BEDROOM_01, state.arCondition1->valueint);
    turnONOrOFFDevice(PIN_AR_COND_BEDROOM_02, state.arCondition2->valueint);
}

void handlerCLientRequest() {
    buffer = malloc(sizeof(char) * 1100);
    int sizeReceive;

    if ((sizeReceive = recv(socketClient, buffer, 1100, 0)) < 0) {
        printf("Error when receive request\n");
    }

    cJSON *dataStatus = cJSON_Parse(buffer);
    state.lamp1 = cJSON_GetObjectItemCaseSensitive(dataStatus, "lamp1");
    state.lamp2 = cJSON_GetObjectItemCaseSensitive(dataStatus, "lamp2");
    state.lamp3 = cJSON_GetObjectItemCaseSensitive(dataStatus, "lamp3");
    state.lamp4 = cJSON_GetObjectItemCaseSensitive(dataStatus, "lamp4");
    state.arCondition1 = cJSON_GetObjectItemCaseSensitive(dataStatus, "ar1");
    state.arCondition2 = cJSON_GetObjectItemCaseSensitive(dataStatus, "ar2");

    handleDeviceState();

    while (sizeReceive > 0) {
        if (send(socketClient, buffer, sizeReceive, 0) != sizeReceive) {
            printf("Error when send try send message\n");
        }

        if ((sizeReceive = recv(socketClient, buffer, 16, 0)) < 0) {
            printf("Error when try receive responde \n");
        }
    }

    free(buffer);
}

void listenSocket() {
    if (listen(serverSocket, NUMBER_QUEUE) < 0) {
        printf("Failed in listening\n");
    }

    while(1) {
        clientLength = sizeof(clientAddr);

        if ((socketClient = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientLength)) < 0) {
            printf("Failed in accept\n");    
        }

        handlerCLientRequest();
    }
}

// void *send_message() {

// }

void closeSocket() {
    close(socketClient);
    close(serverSocket);
}
