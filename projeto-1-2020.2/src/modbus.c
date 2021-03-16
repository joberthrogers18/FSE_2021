#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <crc16.h>
#include <stdlib.h>

#define DEVICE_ADDRESS 0x01
#define CODE_FUNCTION 0X23
#define SUB_CODE_TI 0XC1
#define SUB_CODE_TR 0xC2


int uart0_filestream = -1;

char *getSubstring(unsigned char *string, int position, int length)
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

void initUART() {
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
}

int requestFloat(char subCode) {
    int count = 0;
    unsigned char code_float[7] = {DEVICE_ADDRESS, CODE_FUNCTION, subCode, 8, 0, 1, 3};
    short crc = calcula_CRC(code_float, 7);

    unsigned char msg[9];
    memcpy(msg, &code_float, 7);
    memcpy(&msg[7], &crc, 2);
    count = write(uart0_filestream, &msg[0], 9);

    return count;
}

void getMessageModbus(float *temperature) {
    if (uart0_filestream != -1) {
        // buffer to read
        unsigned char rx_buffer[256];
        // read from buffer
        int rx_length = read(uart0_filestream, (void *)rx_buffer, 255);

        char *new_string = malloc(sizeof(char)* 4);

        printf("%d\n", rx_length);
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

            memcpy(temperature, &rx_buffer[3], 4);
        }

        free(new_string);
    }
}

void closeUART() {
    close(uart0_filestream);
}

void getInformationModbus(float *TI, float *TR) {
    initUART();
    printf("Temperatura Externa");
    requestFloat(SUB_CODE_TI);
    sleep(1);
    getMessageModbus(TI);

    printf("Temperatura Potenciometro");
    requestFloat(SUB_CODE_TR);
    sleep(1);
    getMessageModbus(TR);

    closeUART();
}