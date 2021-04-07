#include <stdio.h>
#include <read_bme280.h>

#define ADDRESS_BME280 0x76
#define CHANNEL_BME280 1

int temperatureExternal = 0.0;
int humidity = 0.0;

void initResources() {
    int i = bme280Init(CHANNEL_BME280, ADDRESS_BME280);
	if (i != 0)
	{
		printf("Erro to open\n");
	}
}

int main(int argc, char const *argv[])
{
    initResources();
    getInformationBME280(&temperatureExternal, &humidity);

    float temperatureExt = (float)temperatureExternal/100;
    float humidityParser = (float)humidity/100;
    printf("Temperature: %f Humidity: %f\n", temperatureExt, humidityParser);

    return 0;
}
