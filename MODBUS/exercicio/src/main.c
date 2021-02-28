#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <crc16.h>
#include <stdlib.h>

 
char *substring(unsigned char *string, int position, int length)
{
   char *p;
   int c;
 
   p = malloc(length+1);
   
   if (p == NULL)
   {
      printf("Unable to allocate memory.\n");
      exit(1);
   }
 
   for (c = 0; c < length; c++)
   {
      *(p+c) = *(string+position-1);      
      string++;  
   }
 
   *(p+c) = '\0';
 
   return p;
}

int main() {
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

    // // transmition buffer
    // unsigned char tx_buffer[15];
    // // define a pointer to above variable
    // unsigned char *p_tx_buffer;

    // // write a message in buffer
    // p_tx_buffer = &tx_buffer[0];

    // unsigned char code_int[3] = {0x01, 0x23, 0xA1};
    // short crc = calcula_CRC(code_int, 3);
    
    // unsigned char msg[5];
    // memcpy(msg, &code_int, 3);
    // memcpy(&msg[3], &crc, 2);

    int option;
    int count = 0;
    printf("\nEnter the option choose:\n");
    scanf("%i", &option);

    if (option == 1) {
        unsigned char code_int[3] = {0x01, 0x23, 0xA1};
        short crc = calcula_CRC(code_int, 3);

        unsigned char msg[5];
        memcpy(msg, &code_int, 3);
        memcpy(&msg[3], &crc, 2);
        count = write(uart0_filestream, &msg[0], 5);
    }
    else if (option == 2) {
        unsigned char code_float[3] = {0x01, 0x23, 0xA2};
        short crc = calcula_CRC(code_float, 3);

        unsigned char msg[5];
        memcpy(msg, &code_float, 3);
        memcpy(&msg[3], &crc, 2);
        count = write(uart0_filestream, &msg[0], 5);
    }
    else if (option == 3) {
        unsigned char code_string[3] = {0x01, 0x23, 0xA3};
        short crc = calcula_CRC(code_string, 3);

        unsigned char msg[5];
        memcpy(msg, &code_string, 3);
        memcpy(&msg[3], &crc, 2);
        count = write(uart0_filestream, &msg[0], 5);
    }
    else if (option == 4) {
        // unsigned char code_string[9] = {0x01, 0x23, 0xA3, 0x5, "t", "e", "s", "t", "e"};
        // short crc = calcula_CRC(code_string, 9);

        // unsigned char msg[9];
        // memcpy(msg, &code_string, 9);
        // memcpy(&msg[3], &crc, 2);
        // count = write(uart0_filestream, &msg[0], 5);
    }


    printf("Write on UART\n");
    
    if (count < 0)
    {
        printf("Error when send data -- TX\n");
        return 1;
    }


    // READ FROM MODBUS

    sleep(1);

    if (uart0_filestream != -1) {
        // buffer to read
        unsigned char rx_buffer[256];
        // read from buffer
        int rx_length = read(uart0_filestream, (void *)rx_buffer, 255);

        char *new_string = malloc(sizeof(char)*rx_length - 4);

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
            new_string = substring(rx_buffer, 4, rx_length - 2);
            if (option == 1) {
                int currentInteger = 0;
                memcpy(&currentInteger, new_string, strlen(new_string));

                printf("int: %d\n", currentInteger);
            }
            else if (option == 2) {
                float currentFloat = 0;
                memcpy(&currentFloat, new_string, strlen(new_string));

                printf("float: %f\n", currentFloat);
            }
            else if (option == 3) {
                printf("string: %s\n", new_string);
            }
        }

        free(new_string);
    }

    close(uart0_filestream);

    return 0;
}