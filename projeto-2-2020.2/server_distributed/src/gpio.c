#include <stdio.h>
#include <stdlib.h>
#include <gpio.h>
#include <unistd.h>
#include <sys/time.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <cJSON.h>

#define MIN_RANGE_PWM 0
#define MAX_RANGE_PWM 100
#define IGNORE_CHANGE_BELOW_USEC 10000

struct timeval last_change;

void turnONOrOFFDevice(int pin, int isOn){
  pinMode(pin, OUTPUT);
  softPwmCreate(pin,  MIN_RANGE_PWM, MAX_RANGE_PWM);

  if (isOn) {
    softPwmWrite(pin, MAX_RANGE_PWM);
  } else {
    softPwmWrite(pin, MIN_RANGE_PWM);
  }
}

void initializePinSensors() {
  // Setup pinout
  pinMode(PIN_SENSOR_PRES_01, OUTPUT);
  pinMode(PIN_SENSOR_PRES_02, OUTPUT);
  pinMode(PIN_SENSOR_DOOR_KITCHEN, OUTPUT);
  pinMode(PIN_SENSOR_WINDOW_KITCHEN, OUTPUT);
  pinMode(PIN_SENSOR_DOOR_ROOM, OUTPUT);
  pinMode(PIN_SENSOR_WINDOW_ROOM, OUTPUT);
  pinMode(PIN_SENSOR_WINDOW_BEDROOM_01, OUTPUT);
  pinMode(PIN_SENSOR_WINDOW_BEDROOM_02, OUTPUT);
}

void getStateOfPin() {
  state.sensorPres1 = cJSON_CreateNumber(digitalRead(PIN_SENSOR_PRES_01));
  state.sensorPres2 = cJSON_CreateNumber(digitalRead(PIN_SENSOR_PRES_02));
  state.sensorDoorKitchen = cJSON_CreateNumber(digitalRead(PIN_SENSOR_DOOR_KITCHEN));
  state.sensorWindowKitchen = cJSON_CreateNumber(digitalRead(PIN_SENSOR_WINDOW_KITCHEN));
  state.sensorDoorRoom = cJSON_CreateNumber(digitalRead(PIN_SENSOR_DOOR_ROOM));
  state.sensorWindowRoom = cJSON_CreateNumber(digitalRead(PIN_SENSOR_WINDOW_ROOM));
  state.sensorWindowBedroom1 = cJSON_CreateNumber(digitalRead(PIN_SENSOR_WINDOW_BEDROOM_01));
  state.sensorWindowBedroom2 = cJSON_CreateNumber(digitalRead(PIN_SENSOR_WINDOW_BEDROOM_02));
}

void changeState(int currentState) {
  switch (currentState)
  {
    case PIN_SENSOR_PRES_01:
      state.sensorPres1 = cJSON_CreateNumber(!state.sensorPres1->valueint);
      break;
    case PIN_SENSOR_PRES_02:
      state.sensorPres2 = cJSON_CreateNumber(!state.sensorPres2->valueint);
      break;
    case PIN_SENSOR_DOOR_KITCHEN:
      state.sensorDoorKitchen = cJSON_CreateNumber(!state.sensorDoorKitchen->valueint);
      break;
    case PIN_SENSOR_WINDOW_KITCHEN:
      state.sensorWindowKitchen = cJSON_CreateNumber(!state.sensorWindowKitchen->valueint);
      break;
    case PIN_SENSOR_DOOR_ROOM:
      state.sensorDoorRoom = cJSON_CreateNumber(!state.sensorDoorRoom->valueint);
      break;
    case PIN_SENSOR_WINDOW_ROOM:
      state.sensorWindowRoom = cJSON_CreateNumber(!state.sensorWindowRoom->valueint);
      break;
    case PIN_SENSOR_WINDOW_BEDROOM_01:
      state.sensorWindowBedroom1 = cJSON_CreateNumber(!state.sensorWindowBedroom1->valueint);
      break;
    case PIN_SENSOR_WINDOW_BEDROOM_02:
      state.sensorWindowBedroom2 = cJSON_CreateNumber(!state.sensorWindowBedroom2->valueint);
      break;
    default:
      break;
  }
}

void handle(int currentState, int typeState, char* sensor) {
  struct timeval now;
  unsigned long diff;

  // debouncing by time
  gettimeofday(&now, NULL);

  diff = (now.tv_sec * 1000000 + now.tv_usec) - (last_change.tv_sec * 1000000 + last_change.tv_usec);

  // Filter jitter
  if (diff > IGNORE_CHANGE_BELOW_USEC) {
    if (currentState) {
      printf("Turn OFF sensor: %s\n", sensor);
    } else {
      printf("Turn ON sensor: %s\n", sensor);
    }

    changeState(typeState);
  }

  last_change = now;
}

void handleSensorPresence1() {
  handle(state.sensorPres1->valueint, PIN_SENSOR_PRES_01, "Sensor presence 01");
}

void handleSensorPresence2() {
  handle(state.sensorPres2->valueint, PIN_SENSOR_PRES_02, "Sensor presence 02");
}

void handleSensorDoorKitchen() {
  handle(state.sensorDoorKitchen->valueint, PIN_SENSOR_DOOR_KITCHEN, "Sensor door kitchen");
}

void handleSensorWindowKitchen() {
  handle(state.sensorWindowKitchen->valueint, PIN_SENSOR_WINDOW_KITCHEN, "Sensor window kitchen");
}

void handleSensorDoorRoom() {
  handle(state.sensorDoorRoom->valueint, PIN_SENSOR_DOOR_ROOM, "Sensor door room");
}

void handleSensorWindowRoom() {
  handle(state.sensorWindowRoom->valueint, PIN_SENSOR_WINDOW_ROOM, "Sensor window room");
}

void handleSensorWindowBedroom1() {
  handle(state.sensorWindowBedroom1->valueint, PIN_SENSOR_WINDOW_BEDROOM_01, "Sensor bedroom 1");
}

void handleSensorWindowBedroom2() {
  handle(state.sensorWindowBedroom2->valueint, PIN_SENSOR_WINDOW_BEDROOM_02, "Sensor bedroom 2");
}

void bindInterupts() {
  wiringPiISR(PIN_SENSOR_PRES_01, INT_EDGE_BOTH,  &handleSensorPresence1);
  wiringPiISR(PIN_SENSOR_PRES_02, INT_EDGE_BOTH,  &handleSensorPresence2);
  wiringPiISR(PIN_SENSOR_DOOR_KITCHEN, INT_EDGE_BOTH,  &handleSensorDoorKitchen);
  wiringPiISR(PIN_SENSOR_WINDOW_KITCHEN, INT_EDGE_BOTH,  &handleSensorWindowKitchen);
  wiringPiISR(PIN_SENSOR_DOOR_ROOM, INT_EDGE_BOTH,  &handleSensorDoorRoom);
  wiringPiISR(PIN_SENSOR_WINDOW_ROOM, INT_EDGE_BOTH,  &handleSensorWindowRoom);
  wiringPiISR(PIN_SENSOR_WINDOW_BEDROOM_01, INT_EDGE_BOTH,  &handleSensorWindowBedroom1);
  wiringPiISR(PIN_SENSOR_WINDOW_BEDROOM_02, INT_EDGE_BOTH,  &handleSensorWindowBedroom2); 
}

void *initializeStateHandle() {

  // initialize all pins like OUTPUT
  initializePinSensors();

  gettimeofday(&last_change, NULL);

  bindInterupts();

  getStateOfPin();

  for (;;) {
    sleep(1);
  }
}

void *printLog(float temperature, float humidity, struct pinState status) {
    printf("\n\n========== VALORES COLETADOS ==========\n");
    printf("Temperatura: %.3fC° Umidade: %.3f%%\n", temperature, humidity);

    printf(
        "L1: %d L2: %d L3: %d L4: %d AR1: %d AR2: %d\n", 
        state.lamp1->valueint, 
        state.lamp2->valueint, 
        state.lamp3->valueint, 
        state.lamp4->valueint, 
        state.arCondition1->valueint, 
        state.arCondition2->valueint
    );

    printf("SP1: %d SP2: %d SDK: %d SWK: %d SDR: %d SWR: %d SWB1: %d SWB2: %d\n", 
        state.sensorPres1->valueint, 
        state.sensorPres2->valueint, 
        state.sensorDoorKitchen->valueint, 
        state.sensorWindowKitchen->valueint, 
        state.sensorDoorRoom->valueint, 
        state.sensorWindowRoom->valueint, 
        state.sensorWindowBedroom1->valueint, 
        state.sensorWindowBedroom2->valueint
    );
    
    return;
}