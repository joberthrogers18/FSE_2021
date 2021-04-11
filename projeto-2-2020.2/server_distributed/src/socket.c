#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <socket.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT_SERVER 10018
#define IP_CLIENT "192.168.0.53"
#define NUMBER_QUEUE 20

int serverSocket;
int socketClient;
struct sockaddr_in serverAddr;
struct sockaddr_in clientAddr;
unsigned int clientLength;

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

void handlerCLientRequest() {
    char buffer[16];
    int sizeReceive;

    if ((sizeReceive = recv(socketClient, buffer, 16, 0)) < 0) {
        printf("Error when receive request\n");
    }

    while (sizeReceive > 0) {
        if (send(socketClient, buffer, sizeReceive, 0) != sizeReceive) {
            printf("Error when send try send message\n");
        }

        if ((sizeReceive = recv(socketClient, buffer, 16, 0)) < 0) {
            printf("Error when try receive responde \n");
        }

        printf("current message: %s\n", buffer);
    }
}

void listenSocket() {
    printf("Passou aqui\n");
    if (listen(serverSocket, NUMBER_QUEUE) < 0) {
        printf("Failed in listening\n");
    }

    while(1) {
        printf("Aqui\n");
        clientLength = sizeof(clientAddr);

        if ((socketClient = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientLength)) < 0) {
            printf("Failed in accept\n");    
        }

        handlerCLientRequest();
    }
}

void closeSocket() {
    close(socketClient);
    close(serverSocket);
}
