#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

int main(int argc, const char *argv[])
{
    int uart0_filestream = -1;

    // Access the file to serial port       flag to read and deal with file write and read
    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);

    // if happen an error, return a printf error indicated
    if (uart0_filestream == -1)
    {
        printf("Error - Unable to open UART. Ensure it is not in use by another applications\n");
    }
    else 
    {
        printf("UART load successfully\n");
    }

    // create a struct to modeling the options uart
    struct termios options;
    // get attributes from UART
    tcgetattr(uart0_filestream, &options);
    // DEFINE THE FLAGS
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD; // Bound rates
    options.c_iflag = IGNPAR;                        // IGNORE PARITY
    options.c_oflag = 0;
    options.c_lflag = 0; // COMUNICATION PATTERN

    // SAVE THIS OPTIONS IN DEVICE
    tcflush(uart0_filestream, TCIFLUSH);
    // set attributes setup
    tcsetattr(uart0_filestream, TCSANOW, &options);

    // transmition buffer
    unsigned char tx_buffer[15];
    // define a pointer to above variable
    unsigned char *p_tx_buffer;

    // write a message in buffer
    p_tx_buffer = &tx_buffer[0];
    int option;
    printf("\nEnter the option choose:\n");
    scanf("%i", &option);

    if (option == 161) {
        *p_tx_buffer++ = 0xA1;
        *p_tx_buffer++ = 8;
        *p_tx_buffer++ = 0;
        *p_tx_buffer++ = 1;
        *p_tx_buffer++ = 3;
    }
    else if (option == 162) {
        *p_tx_buffer++ = 0xA2;
        *p_tx_buffer++ = 8;
        *p_tx_buffer++ = 0;
        *p_tx_buffer++ = 1;
        *p_tx_buffer++ = 3;
    } 
    else if (option == 163) {
        *p_tx_buffer++ = 0xA3;
        *p_tx_buffer++ = 4;
        *p_tx_buffer++ = 't';
        *p_tx_buffer++ = 'e';
        *p_tx_buffer++ = 's';
        *p_tx_buffer++ = 't';
        *p_tx_buffer++ = 8;
        *p_tx_buffer++ = 0;
        *p_tx_buffer++ = 1;
        *p_tx_buffer++ = 3;
    }

    // write message in uart device
    int count = write(uart0_filestream, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
    printf("Write on UART\n");
    if (count < 0)
    {
        printf("Error when send data -- TX\n");
        return 1;
    }

    // memcpy(&dado, &rx_buffer[0], 4); //copy data to a buffer rx 
    sleep(1);

    if (uart0_filestream != -1) {
        // buffer to read
        unsigned char rx_buffer[100];
        // read from buffer
        int rx_length = read(uart0_filestream, (void *)rx_buffer, 255);


        if (rx_length < 0)
        {
            printf("Error during read of UART - RX\n");
        }
        else if (rx_length == 0)
        {
            printf("There is no content available\n");
        }
        else
        {
            rx_buffer[rx_length] = '\0';
            if (option == 161) {
                int currentInteger;
                memcpy(&currentInteger, rx_buffer, rx_length);

                printf("int %d: %d\n", rx_length, currentInteger);
            }
            else if (option == 162) {
                float currentFloat;
                memcpy(&currentFloat, rx_buffer, rx_length);

                printf("float %d: %f\n", rx_length, currentFloat);
            }
            else {
                char valueStr[10];
                memcpy(valueStr, rx_buffer, rx_length);
                printf("final str: %s\n", valueStr);
            }
        }
    }

    close(uart0_filestream);

    return 0;
}
