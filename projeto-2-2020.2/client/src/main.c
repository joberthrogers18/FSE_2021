#include <stdio.h>
#include <socket.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    initSocket();
//    printf("passou aqui \n");
//    sendMessage("teste");
    sendMessage();
    closeSocket();

    // system("omxplayer /home/joberthcosta/client/alarm.mp3");
    
    return 0;
}
