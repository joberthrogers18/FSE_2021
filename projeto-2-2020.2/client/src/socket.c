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

struct sockaddr_in serverAddr;
int clientSocket;
char buffer[16];
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

void sendMessage(){
    cJSON *dataStatus = cJSON_CreateObject();
    // cJSON_AddItemToObject(dataStatus, "temperature", cJSON_CreateNumber(state.temperature));
    // cJSON_AddItemToObject(dataStatus, "humidity", cJSON_CreateNumber(state.humidity));
    cJSON_AddItemToObject(dataStatus, "lamp1", cJSON_CreateNumber(state.lamp1->valueint));
    cJSON_AddItemToObject(dataStatus, "lamp2", cJSON_CreateNumber(state.lamp2->valueint));
    cJSON_AddItemToObject(dataStatus, "lamp3", cJSON_CreateNumber(state.lamp3->valueint));
    cJSON_AddItemToObject(dataStatus, "lamp4", cJSON_CreateNumber(state.lamp4->valueint));
    cJSON_AddItemToObject(dataStatus, "ar1", cJSON_CreateNumber(state.arCondition1->valueint));
    cJSON_AddItemToObject(dataStatus, "ar2", cJSON_CreateNumber(state.arCondition2->valueint));

    char *parseDataStatus = cJSON_Print(dataStatus);
    unsigned sizeMessage = strlen(parseDataStatus);

    if (send(clientSocket, parseDataStatus, strlen(parseDataStatus), 0) != sizeMessage){
        printf("Error when send message: number of bytes different from was send.");
    }

    totalBytesReceived = 0;

	// while(totalBytesReceived < sizeMessage) {
	// 	if((bytesReceived = recv(clientSocket, buffer, 16-1, 0)) <= 0) {
	// 		printf("The bytes sended was not receive!\n");
    //     }

	// 	totalBytesReceived += bytesReceived;
	// 	buffer[bytesReceived] = '\0';
		
    //     printf("%s\n", buffer);
	// }

}

void closeSocket(){
    close(clientSocket);
    exit(0);
}