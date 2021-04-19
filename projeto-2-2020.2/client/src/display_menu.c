#include <ncurses.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <display_menu.h>
#include <socket.h>
#include <cJSON.h>
#include <utils.h>

#define LINE_START 1
#define SCAN_LINE 8
#define SCAN_COL 30
#define ON 1
#define OFF 0

int option = -1;
int is_alarm_on = 1; //ON
int yMax, xMax;

WINDOW *logs;
WINDOW *menu;

void init_menu() {
    initscr();

    //get screen size
    getmaxyx(stdscr, yMax, xMax);

    logs = newwin(10, xMax, 0, 0);
    menu = newwin(10, xMax, 10, 0);
    box(logs, 0, 0);
}

void *display_states(void *arg) {
    while (option != 0) {
        sleep(1);
        mvwprintw(logs, LINE_START, (int) xMax / 2, "ESTADOS");
        mvwprintw(logs, LINE_START + 1, 5, "");
        mvwprintw(logs, LINE_START + 2, 5, "Lâmpadas 1: %d; Lâmpadas 2: %d; Lâmpadas 3:% d; Lâmpadas 4: %d", 
            state.lamp1->valueint, state.lamp2->valueint, state.lamp3->valueint, state.lamp4->valueint
        );
        mvwprintw(logs, LINE_START + 3, 5, "AR Condicionado1: %d; AR Condicionado 2: %d; Sensor Presença 1: %d; Sensor Presença 2: %d", 
            state.arCondition1->valueint, state.arCondition2->valueint, state.sensorPres1->valueint, state.sensorPres2->valueint
        );
        mvwprintw(logs, LINE_START + 4, 5, "Sensor Porta C: %d; Sensor Janela C: %d; Sensor Porta S: %d", 
            state.sensorDoorKitchen->valueint, state.sensorWindowKitchen->valueint, state.sensorDoorRoom->valueint
        );
        mvwprintw(logs, LINE_START + 5, 5, "Sensor Janela S: %d; Sensor Janela Q1: %d; Sensor Janela Q2: %d", 
            state.sensorWindowRoom->valueint, state.sensorWindowBedroom1->valueint, state.sensorWindowBedroom2->valueint
        );
        mvwprintw(logs, LINE_START + 6, 5, "Temperatura: %.2lf C°; Umidade: %.2lf %%", (float) state.temperature->valuedouble, (float) state.humidity->valuedouble);

        attron(A_BOLD);
        if (state.sensorWindowRoom->valueint || state.sensorWindowBedroom1->valueint || state.sensorWindowBedroom2->valueint || state.sensorDoorKitchen->valueint || state.sensorWindowKitchen->valueint || state.sensorDoorRoom->valueint || state.sensorPres1->valueint || state.sensorPres2->valueint) {
            mvwprintw(logs, LINE_START + 7, ((int) xMax / 2) - 3, "ALARME ATIVADO");
            playBeep();
        }

        wrefresh(logs);
    }

    return NULL;
}

void display_step_menu(int line_delta) {
    wclear(menu);
    box(menu, 0, 0);
    mvwprintw(menu, LINE_START, (int) xMax / 2, "MENU\n");
    mvwprintw(menu, line_delta, 5, "Aperte 1 para lâmpada seguido do número da lâmpada.");
    mvwprintw(menu, line_delta + 1, 5, "Aperte 2 para ar condicionado seguido do número do ar condicionado.");
    mvwprintw(menu, line_delta + 2, 5, "Exemplo: para ligar a 'Ar Condicionado 1' digite '21'.");

    mvwprintw(menu, SCAN_LINE, 5, "Digite o comando: ");
    wrefresh(menu);
}

void display_alert() {
    wclear(menu);
    box(menu, 0, 0);
    mvwprintw(menu, LINE_START, (int) xMax / 2, "MENU\n");
    mvwprintw(menu, LINE_START + 2, 5, "Não exite esse tipo de dispositivo, tente novamente.");

    wrefresh(menu);
    sleep(5);
    display_step_menu(LINE_START + 2);
}

void changeState(int code, int currentState) {
    switch (code)
    {
        case 11:
            state.lamp1 = cJSON_CreateNumber(currentState);
            break;
        case 12:
            state.lamp2 = cJSON_CreateNumber(currentState);
            break;
        case 13:
            state.lamp3 = cJSON_CreateNumber(currentState);
            break;
        case 14:
            state.lamp4 = cJSON_CreateNumber(currentState);
            break;
        case 21:
            state.arCondition1 = cJSON_CreateNumber(currentState);
            break;
        case 22:
            state.arCondition2 = cJSON_CreateNumber(currentState);
            break;
        default:
            display_alert();
            break;
    }
}

void *menu_handler(void *arg) {
    char value;
    char posfix_code[3];

    while (option != 0) {
        wclear(menu);
        box(menu, 0, 0);
        mvwprintw(menu, LINE_START, (int) xMax / 2, "MENU\n");
        mvwprintw(menu, LINE_START + 1, 5, "1-LIGAR um dispositivo.\n");
        mvwprintw(menu, LINE_START + 2, 5, "2-DESLIGAR um dispositivo.\n");
        mvwprintw(menu, LINE_START + 3, 5, "0-Sair.\n");
        mvwprintw(menu, SCAN_LINE, 5, "Escolha a opção: ");
        wrefresh(menu);
        
        mvwscanw(menu, SCAN_LINE, SCAN_COL, "%d", &option);
        wclear(menu);

        switch (option) {
            case 0:
                closeSocket();
                break;

            case 1:
                mvwprintw(menu, LINE_START + 1, 0, "Escolha o dispositivo para LIGAR: ");
                display_step_menu(LINE_START + 2);

                mvwscanw(menu, SCAN_LINE, SCAN_COL, "%d", &value);
                changeState(value, ON);
                // sendMessage();
                // addInfoFileCsv(0, 0);
                break;

            case 2:
                mvwprintw(menu, LINE_START + 1, 0, "Escolha o dispositivo para DESLIGAR: ");
                display_step_menu(LINE_START + 2);

                mvwscanw(menu, SCAN_LINE, SCAN_COL, "%d", &value);

                sprintf(posfix_code, "%d", value);
                changeState(value, OFF);
                // sendMessage();
                // addInfoFileCsv(0, 0);
                break;
                
            default:
                break;
        }
    }

    return NULL;
}

void shut_down_menu() {
    endwin();
}