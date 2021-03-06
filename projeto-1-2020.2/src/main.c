#include <stdio.h>
#include <lcd_control.h>
#include <read_bme280.h>
#include <modbus.h>
#include <unistd.h>
#include <wiringPi.h>
#include <stdio.h>
#include <gpio.h>
#include <pid.h>
#include <utils.h>
#include <stdlib.h>
#include <signal.h>

#define KP 5.0
#define KI 1.0
#define KD 5.0 
#define ADDRESS_BME280 0x76
#define CHANNEL_BME280 1

int useRefManual = 0;
double controlSignal = 0.0;
float internalTemperature = 0.0;
float referenceTemperature = 0.0;
int temperatureExternal = 0.0;

void getManualReference(float *internalTemp, float *referenceTemp);
void menuhandler();

void initResources() {
    initUART();
    lcd_init();
    csvCreation();

    int i = bme280Init(CHANNEL_BME280, ADDRESS_BME280);
	if (i != 0)
	{
		printf("Erro to open\n");
	}

    // before: 10, 1, 5
    pid_configure_consts(KP, KI, KD);
    wiringPiSetup();
    turnOFFFAN();
    turnOFFResistor();
}

void finishResources() {
    turnOFFFAN();
    turnOFFResistor();
    closeUART();
    exit(0);
}

void getManualReference(float *internalTemp, float *referenceTemp) {
    float RefTemp = 0.0;

    getInformationModbus(internalTemp, referenceTemp, useRefManual);

    getInformationBME280(&temperatureExternal);

    float temperatureExt = (float)temperatureExternal/100;

    system("clear");
    printf("Digite a temperatura de referência desejada:\n");
    scanf("%f", &RefTemp);
    
    if (RefTemp < temperatureExt || RefTemp > 100) {
        system("clear");
        printf("A temperatura não pode ser menor que %.2f Cº e maior que 100 Cº\n", temperatureExt);
        sleep(2);
        menuhandler(internalTemp, referenceTemp);
    } else {
        *referenceTemp = RefTemp;
        useRefManual = 1;
    }
}

void menuhandler() {
    char option;
    printMenu();
    
    option = getchar();

    switch (option)
    {
        case '1':
            turnOFFFAN();
            turnOFFResistor();
            useRefManual = 0;
            break;

        case '2':
            turnOFFFAN();
            turnOFFResistor();
            getManualReference(&internalTemperature, &referenceTemperature);
            break;

        case '3':
            finishResources();
            break;
        
        default:
            system("clear");
            printf("Opção errada, escolha novamente\n");
            sleep(1);
            menuhandler();
            break;
    }
}

int main(int argc, char const *argv[])
{
    signal(SIGINT, menuhandler);

    initResources();

    menuhandler();

    while (1)
    {  
        sleep(1);  
        getInformationModbus(&internalTemperature, &referenceTemperature, useRefManual);

        getInformationBME280(&temperatureExternal);

        float temperatureExt = (float)temperatureExternal/100;

        printInScreen(&temperatureExt, &internalTemperature, &referenceTemperature);

        pid_update_reference(referenceTemperature);

        controlSignal = pid_control(internalTemperature);

        printLog(
            internalTemperature, 
            temperatureExt, 
            referenceTemperature, 
            controlSignal
        );

        controlFanResistorPWM((int) controlSignal);
    }

    finishResources();

    return 0;
}