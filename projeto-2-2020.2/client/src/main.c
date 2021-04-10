#include <stdio.h>
#include <socket.h>

int main(int argc, char const *argv[])
{
    initSocket();

    sendMessage("teste");

    closeSocket();
    
    return 0;
}
