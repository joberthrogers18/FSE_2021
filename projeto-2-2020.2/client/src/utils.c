#include <stdio.h> 
#include <time.h>  
#include <utils.h>
#include <stdlib.h>
#include <socket.h>

void csvCreation() {
    FILE* csvFile;

    csvFile = fopen("measurements.csv", "w");
    fprintf(csvFile, "Time, Temperature, Humidity, lamp1, lamp2, lamp3, lamp4, ar1, ar2, sp1, sp2, sdk, swk, sdr, swr, swb1, swb2\n");
    fclose(csvFile);
}

void addInfoFileCsv(float temperature, float humidity) {
    FILE* csvFile;
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    csvFile = fopen("measurements.csv", "a+");

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer,80,"%F %X",timeinfo);

    fprintf(csvFile, "%s, %.3f, %.3f, %d, %d, %d, %d, %d, %d\n", 
        buffer, 
        temperature, 
        humidity,
        state.lamp1->valueint,
        state.lamp2->valueint,
        state.lamp3->valueint,
        state.lamp4->valueint,
        state.arCondition1->valueint,
        state.arCondition2->valueint,
        state.sensorPres1->valueint,
        state.sensorPres2->valueint,
        state.sensorDoorKitchen->valueint,
        state.sensorWindowKitchen->valueint,
        state.sensorDoorRoom->valueint,
        state.sensorWindowRoom->valueint,
        state.sensorWindowBedroom1->valueint,
        state.sensorWindowBedroom2->valueint
    );

    fclose(csvFile);
}