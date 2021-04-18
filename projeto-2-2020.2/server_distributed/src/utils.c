#include <stdio.h> 
#include <time.h>  
#include <utils.h>
#include <stdlib.h>
#include <gpio.h>

void csvCreation() {
    FILE* csvFile;

    csvFile = fopen("measurements.csv", "w");
    fprintf(csvFile, "Time, Temperature, Humidity\n");
    fclose(csvFile);
}

void addInfoFileCsv() {
    FILE* csvFile;
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    csvFile = fopen("measurements.csv", "a+");

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer,80,"%F %X",timeinfo);

    fprintf(csvFile, "%s, %.3f, %.3f\n", buffer, (float) state.temperature->valuedouble, (float) state.humidity->valuedouble);

    fclose(csvFile);
}