// #include <stdio.h>
// #include <socket.h>
// #include <stdlib.h>
// #include <ncurses.h>

// struct pinState state;

// void initializeStates() {
//     state.lamp1 = cJSON_CreateNumber(0);
//     state.lamp2 = cJSON_CreateNumber(0);
//     state.lamp3 = cJSON_CreateNumber(0);
//     state.lamp4 = cJSON_CreateNumber(0);
//     state.arCondition1 = cJSON_CreateNumber(0);
//     state.arCondition2 = cJSON_CreateNumber(0);
//     state.sensorPres1 = cJSON_CreateNumber(0);
//     state.sensorPres2 = cJSON_CreateNumber(0);
//     state.sensorDoorKitchen = cJSON_CreateNumber(0);
//     state.sensorWindowKitchen = cJSON_CreateNumber(0);
//     state.sensorWindowRoom = cJSON_CreateNumber(0);
//     state.sensorDoorRoom = cJSON_CreateNumber(0);
//     state.sensorWindowRoom = cJSON_CreateNumber(0);
//     state.sensorWindowBedroom1 = cJSON_CreateNumber(0);
//     state.sensorWindowBedroom2 = cJSON_CreateNumber(0);
// }

// int main(int argc, char const *argv[])
// {
//     // initSocket();
//     // initializeStates();

//     // sendMessage();
//     // closeSocket();

//     // system("omxplayer /home/joberthcosta/client/alarm.mp3");
    
//      WINDOW *w;
//     char list[5][7] = { "One", "Two", "Three", "Four", "Five" };
//     char item[7];
//     int ch, i = 0, width = 7;
 
//     initscr(); // initialize Ncurses
//     w = newwin( 10, 12, 1, 1 ); // create a new window
//     box( w, 0, 0 ); // sets default borders for the window
     
// // now print all the menu items and highlight the first one
//     for( i=0; i<5; i++ ) {
//         if( i == 0 ) 
//             wattron( w, A_STANDOUT ); // highlights the first item.
//         else
//             wattroff( w, A_STANDOUT );
//         sprintf(item, "%-7s",  list[i]);
//         mvwprintw( w, i+1, 2, "%s", item );
//     }
 
//     wrefresh( w ); // update the terminal screen
 
//     i = 0;
//     noecho(); // disable echoing of characters on the screen
//     keypad( w, TRUE ); // enable keyboard input for the window.
//     curs_set( 0 ); // hide the default screen cursor.
     
//        // get the input
//     while(( ch = wgetch(w)) != 'q'){ 
         
//                 // right pad with spaces to make the items appear with even width.
//             sprintf(item, "%-7s",  list[i]); 
//             mvwprintw( w, i+1, 2, "%s", item ); 
//               // use a variable to increment or decrement the value based on the input.
//             switch( ch ) {
//                 case KEY_UP:
//                             i--;
//                             i = ( i<0 ) ? 4 : i;
//                             break;
//                 case KEY_DOWN:
//                             i++;
//                             i = ( i>4 ) ? 0 : i;
//                             break;
//             }
//             // now highlight the next item in the list.
//             wattron( w, A_STANDOUT );
             
//             sprintf(item, "%-7s",  list[i]);
//             mvwprintw( w, i+1, 2, "%s", item);
//             wattroff( w, A_STANDOUT );
//     }
 
//     delwin( w );
//     endwin();

//     return 0;
// }

#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <socket.h>
#include <signal.h>
#include <display_menu.h>

struct pinState state;

void initializeStates() {
    state.lamp1 = cJSON_CreateNumber(0);
    state.lamp2 = cJSON_CreateNumber(0);
    state.lamp3 = cJSON_CreateNumber(0);
    state.lamp4 = cJSON_CreateNumber(0);
    state.arCondition1 = cJSON_CreateNumber(0);
    state.arCondition2 = cJSON_CreateNumber(0);
    state.sensorPres1 = cJSON_CreateNumber(0);
    state.sensorPres2 = cJSON_CreateNumber(0);
    state.sensorDoorKitchen = cJSON_CreateNumber(0);
    state.sensorWindowKitchen = cJSON_CreateNumber(0);
    state.sensorWindowRoom = cJSON_CreateNumber(0);
    state.sensorDoorRoom = cJSON_CreateNumber(0);
    state.sensorWindowRoom = cJSON_CreateNumber(0);
    state.sensorWindowBedroom1 = cJSON_CreateNumber(0);
    state.sensorWindowBedroom2 = cJSON_CreateNumber(0);
}

void finishResources(){
    closeSocket();
    exit(0);
}

int main() {
    pthread_t thread_display_menu, thread_read_menu, thread_socket;

    signal(SIGINT, finishResources);

    initializeStates();
    // csvCreation();
    initSocket();
    init_menu();

    pthread_create(&thread_display_menu, NULL, display_states, NULL);
    pthread_create(&thread_read_menu, NULL, menu_handler, NULL);

    pthread_join(thread_display_menu, NULL);
    pthread_join(thread_read_menu, NULL);

    shut_down_menu();

    return 0;
}