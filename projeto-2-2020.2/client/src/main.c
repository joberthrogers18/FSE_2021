#include <stdio.h>
#include <socket.h>

int main(int argc, char const *argv[])
{
    initSocket();
    printf("passou aqui \n");
    sendMessage("teste");

    closeSocket();
    
    return 0;
}
