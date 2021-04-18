#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <socket.h>
#include <cJSON.h>


#define PORT_SERVER 10018
#define IP_SERVER "192.168.0.4"
#define NUMBER_QUEUE 20

#define IP_CLIENT "192.168.0.53"

struct sockaddr_in serverAddr;
int clientSocket;
char *buffer;
unsigned int tamanhoMensagem;

int bytesRecebidos;
int totalBytesRecebidos;
int totalBytesReceived;
int bytesReceived;
extern struct pinState state;

void initSocket() {
    // create a Socket 
    if ((clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
        printf("Error when start socket. Try again\n");
        exit(1);
    }

    // Build struct sockaddr in
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(IP_SERVER);
    serverAddr.sin_port = htons(PORT_SERVER);

    // connect server
    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0){
        printf("Error when connect with server\n");
        exit(1);
    }
}

void sendMessage() {
    buffer = malloc(sizeof(char) * 2400);

    cJSON *dataStatus = cJSON_CreateObject();
    // cJSON_AddItemToObject(dataStatus, "temperature", cJSON_CreateNumber(state.temperature));
    // cJSON_AddItemToObject(dataStatus, "humidity", cJSON_CreateNumber(state.humidity));
    cJSON_AddItemToObject(dataStatus, "lamp1", cJSON_CreateNumber(state.lamp1->valueint));
    cJSON_AddItemToObject(dataStatus, "lamp2", cJSON_CreateNumber(state.lamp2->valueint));
    cJSON_AddItemToObject(dataStatus, "lamp3", cJSON_CreateNumber(state.lamp3->valueint));
    cJSON_AddItemToObject(dataStatus, "lamp4", cJSON_CreateNumber(state.lamp4->valueint));
    cJSON_AddItemToObject(dataStatus, "ar1", cJSON_CreateNumber(state.arCondition1->valueint));
    cJSON_AddItemToObject(dataStatus, "ar2", cJSON_CreateNumber(state.arCondition2->valueint));
    cJSON_AddItemToObject(dataStatus, "sp1", cJSON_CreateNumber(state.sensorPres1->valueint));
    cJSON_AddItemToObject(dataStatus, "sp2", cJSON_CreateNumber(state.sensorPres2->valueint));
    cJSON_AddItemToObject(dataStatus, "sdk", cJSON_CreateNumber(state.sensorDoorKitchen->valueint));
    cJSON_AddItemToObject(dataStatus, "swk", cJSON_CreateNumber(state.sensorWindowKitchen->valueint));
    cJSON_AddItemToObject(dataStatus, "swr", cJSON_CreateNumber(state.sensorWindowRoom->valueint));
    cJSON_AddItemToObject(dataStatus, "sdr", cJSON_CreateNumber(state.sensorDoorRoom->valueint));
    cJSON_AddItemToObject(dataStatus, "swb1", cJSON_CreateNumber(state.sensorWindowBedroom1->valueint));
    cJSON_AddItemToObject(dataStatus, "swb2", cJSON_CreateNumber(state.sensorWindowBedroom2->valueint));


    char *parseDataStatus = cJSON_Print(dataStatus);
    unsigned sizeMessage = strlen(parseDataStatus);

    if (send(clientSocket, parseDataStatus, strlen(parseDataStatus), 0) != sizeMessage){
        printf("Error when send message: number of bytes different from was send.");
    }

    totalBytesReceived = 0;

    read(clientSocket, buffer, 2400);

    printf("%s\n", buffer);

    state.lamp1 = cJSON_GetObjectItemCaseSensitive(dataStatus, "lamp1");
    state.lamp2 = cJSON_GetObjectItemCaseSensitive(dataStatus, "lamp2");
    state.lamp3 = cJSON_GetObjectItemCaseSensitive(dataStatus, "lamp3");
    state.lamp4 = cJSON_GetObjectItemCaseSensitive(dataStatus, "lamp4");
    state.arCondition1 = cJSON_GetObjectItemCaseSensitive(dataStatus, "ar1");
    state.arCondition2 = cJSON_GetObjectItemCaseSensitive(dataStatus, "ar2");
    state.sensorPres1 = cJSON_GetObjectItemCaseSensitive(dataStatus, "sp1");
    state.sensorPres2 = cJSON_GetObjectItemCaseSensitive(dataStatus, "sp2");
    state.sensorDoorKitchen = cJSON_GetObjectItemCaseSensitive(dataStatus, "sdk");
    state.sensorWindowKitchen = cJSON_GetObjectItemCaseSensitive(dataStatus, "swk");
    state.sensorWindowRoom = cJSON_GetObjectItemCaseSensitive(dataStatus, "swr");
    state.sensorDoorRoom = cJSON_GetObjectItemCaseSensitive(dataStatus, "sdr");
    state.sensorWindowBedroom1 = cJSON_GetObjectItemCaseSensitive(dataStatus, "swb1");
    state.sensorWindowBedroom2 = cJSON_GetObjectItemCaseSensitive(dataStatus, "swb2");

	// while(totalBytesReceived < sizeMessage) {
	// 	if((bytesReceived = recv(clientSocket, buffer, 16-1, 0)) <= 0) {
	// 		printf("The bytes sended was not receive!\n");
    //     }

	// 	totalBytesReceived += bytesReceived;
	// 	buffer[bytesReceived] = '\0';
		
    //     printf("%s\n", buffer);
	// }

    free(buffer);
}

void closeSocket(){
    close(clientSocket);
    exit(0);
}