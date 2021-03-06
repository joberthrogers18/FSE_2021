#include <stdio.h> 
#include <time.h>  
#include <utils.h>
#include <stdlib.h>

void csvCreation() {
    FILE* csvFile;

    csvFile = fopen("project1.csv", "w");
    fprintf(csvFile, "Time, TempExt, TempInt, TempRef, ControleSignal\n");
    fclose(csvFile);
}

void addInfoFileCsv(float TI, float TE, float TR, double controlSignal) {
    FILE* csvFile;
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];

    csvFile = fopen("project1.csv", "a+");

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer,80,"%F %X",timeinfo);

    fprintf(csvFile, "%s, %.3f, %.3f, %.3f, %.3lf\n", buffer, TE, TI, TR, controlSignal);

    fclose(csvFile);
}

void printLog(float TI, float TE, float TR, double controlSignal) {
    printf("\n\n========== VALORES COLETADOS ==========\n");
    printf("TI: %f TE: %f TR: %f\n", TI, TE, TR);
    addInfoFileCsv(TI, TE, TR, controlSignal);
}

void printMenu() {
    system("clear");
    printf("======== Projeto 1 FSE ========\n");
    printf("\nEscolha uma das opções abaixo:\n\n");
    printf("1) Executar experimento usando a temperatura do potenciometro\n");
    printf("2) Entrar com um temperatura de referência\n");
    printf("3) Sair do programa\n");
}