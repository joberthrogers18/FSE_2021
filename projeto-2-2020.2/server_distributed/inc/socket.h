#ifndef SOCKET_H_
#define SOCKET_H_

void initSocket();
void handlerCLientRequest();
void *listenSocket();
void closeSocket();
void turnOFFAllDevices();

char *buffer;

#endif