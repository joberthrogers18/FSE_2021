#include <stdio.h> 
#include <time.h>  
#include <utils.h>
#include <stdlib.h>
#include <socket.h>

void csvCreation() {
    FILE* csvFile;

    csvFile = fopen("measurements.csv", "w");
    fprintf(csvFile, "Time, Temperature, Humidity, lamp1, lamp2, lamp3, lamp4, ar1, ar2\n");
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

    fprintf(csvFile, "%s, %.3f, %.3f\n", 
        buffer, 
        temperature, 
        humidity
    );

    fclose(csvFile);
}