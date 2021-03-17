#include <stdio.h>
#include <lcd_control.h>
#include <read_bme280.h>
#include <modbus.h>
#include <unistd.h>
#include <wiringPi.h> /* include wiringPi library */
#include <stdio.h>
#include <gpio.h>

int main(int argc, char const *argv[])
{
    double control_signal = 0.0;
    initUART();

    lcd_init();

    int i = bme280Init(1, 0x76);
	if (i != 0)
	{
		printf("Erro to open\n"); // problem - quit
	}

    pid_configura_constantes(10.0, 1.0, 5.0);

    wiringPiSetup();

    while (1)
    {    
        float internalTemperature, referenceTemperature;

        getInformationModbus(&internalTemperature, &referenceTemperature);

        int temperatureExternal;

        getInformationBME280(&temperatureExternal);

        float temperatureExt = (float)temperatureExternal/100;

        printInScreen(&temperatureExt, &internalTemperature, &referenceTemperature);

        usleep(1000);

        pid_atualiza_referencia(referenceTemperature)

        control_signal = pid_controle(internalTemperature);

        controlFanResistorPWM(control_signal);
    }

    closeUART();

    return 0;
}


// #include <wiringPi.h> /* include wiringPi library */
// #include <stdio.h>    
// #include <softPwm.h>  /* include header file for software PWM */
// #include <pid.h>

// int main(){
	// int PWM_pin_RESISTO = 4;		/* GPIO1 as per WiringPi,GPIO18 as per BCM */
    // int PWM_pin_FAN = 5;		/* GPIO1 as per WiringPi,GPIO18 as per BCM */
	// int intensity;
	// wiringPiSetup();		/* initialize wiringPi setup */
	// pinMode(PWM_pin_RESISTO,OUTPUT);	/* set GPIO as output */
	// softPwmCreate(PWM_pin_RESISTO,1,100);	/* set PWM channel along with range*/
	
    // digitalWrite (PWM_pin_RESISTO,  LOW) ; delay (1500) ;
    // digitalWrite (PWM_pin_RESISTO, HIGH) ; delay (1500) ;
    // digitalWrite (PWM_pin_RESISTO,  LOW) ; delay (1500) ;
    // while (1)
	//   {
	// 	for (intensity = 0; intensity < 101; intensity++)
	// 	{
	// 	  softPwmWrite (PWM_pin, intensity); /* change the value of PWM */
	// 	  delay (10) ;
	// 	}
	// 	delay(1);

	// 	for (intensity = 100; intensity >= 0; intensity--)
	// 	{
	// 	  softPwmWrite (PWM_pin, intensity);
	// 	  delay (10);
	// 	}
	// 	delay(1);
		
	// }

//     pid_configura_constantes(10.0, 1.0, 5.0);
    
// }