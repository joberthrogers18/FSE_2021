#include <stdio.h>
#include <lcd_control.h>
#include <read_bme280.h>
#include <modbus.h>
#include <unistd.h>
#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>
#include <gpio.h>
#include <pid.h>
#include <utils.h>

#define KP 5.0
#define KI 1.0
#define KD 5.0 
#define ADDRESS_BME280 0x76
#define CHANNEL_BME280 1

int main(int argc, char const *argv[])
{
    double control_signal = 0.0;
    float internalTemperature = 0.0;
    float referenceTemperature = 0.0;
    int temperatureExternal = 0.0;

    initUART();

    lcd_init();

    csvCreation();

    int i = bme280Init(CHANNEL_BME280, ADDRESS_BME280);
	if (i != 0)
	{
		printf("Erro to open\n"); // problem - quit
	}

    // before: 10, 1, 5
    pid_configura_constantes(KP, KI, KD);

    wiringPiSetup();

    turnOFFFAN();
    turnOFFResistor();

    for (int i = 0; i < 200; i ++)
    {  
        sleep(1);  

        getInformationModbus(&internalTemperature, &referenceTemperature);

        getInformationBME280(&temperatureExternal);

        float temperatureExt = (float)temperatureExternal/100;

        printInScreen(&temperatureExt, &internalTemperature, &referenceTemperature);

        pid_atualiza_referencia(referenceTemperature);

        control_signal = pid_controle(internalTemperature);

        printLog(float TI, float TE, float TR, double controlSignal);

        controlFanResistorPWM((int) control_signal);

    }

    turnOFFFAN();
    turnOFFResistor();
    closeUART();

    return 0;
}