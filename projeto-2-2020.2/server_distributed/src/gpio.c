#include <stdio.h>
#include <stdlib.h>
#include <gpio.h>
#include <unistd.h>
#include <sys/time.h>
#include <wiringPi.h>
#include <softPwm.h>

#define MIN_RANGE_PWM 0
#define MAX_RANGE_PWM 100
#define IGNORE_CHANGE_BELOW_USEC 10000

extern struct pinState state;

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
  state.sensorPres1 = digitalRead(PIN_SENSOR_PRES_01);
  state.sensorPres2 = digitalRead(PIN_SENSOR_PRES_02);
  state.sensorDoorKitchen = digitalRead(PIN_SENSOR_DOOR_KITCHEN);
  state.sensorWindowKitchen = digitalRead(PIN_SENSOR_WINDOW_KITCHEN);
  state.sensorDoorRoom = digitalRead(PIN_SENSOR_DOOR_ROOM);
  state.sensorWindowRoom = digitalRead(PIN_SENSOR_WINDOW_ROOM);
  state.sensorWindowBedroom1 = digitalRead(PIN_SENSOR_WINDOW_BEDROOM_01);
  state.sensorWindowBedroom2 = digitalRead(PIN_SENSOR_WINDOW_BEDROOM_02);
}

void changeState(int currentState) {
  switch (currentState)
  {
    case PIN_SENSOR_PRES_01:
      state.sensorPres1 = !state.sensorPres1;
      break;
    case PIN_SENSOR_PRES_02:
      state.sensorPres2 = !state.sensorPres2;
      break;
    case PIN_SENSOR_DOOR_KITCHEN:
      state.sensorDoorKitchen = !state.sensorDoorKitchen;
      break;
    case PIN_SENSOR_WINDOW_KITCHEN:
      state.sensorWindowKitchen = !state.sensorWindowKitchen;
      break;
    case PIN_SENSOR_DOOR_ROOM:
      state.sensorDoorRoom = !state.sensorDoorRoom;
      break;
    case PIN_SENSOR_WINDOW_ROOM:
      state.sensorWindowRoom = !state.sensorWindowRoom;
      break;
    case PIN_SENSOR_WINDOW_BEDROOM_01:
      state.sensorWindowBedroom1 = !state.sensorWindowBedroom1;
      break;
    case PIN_SENSOR_WINDOW_BEDROOM_02:
      state.sensorWindowBedroom2 = !state.sensorWindowBedroom2;
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
  handle(state.sensorPres1, PIN_SENSOR_PRES_01, "Sensor presence 01");
}

void handleSensorPresence2() {
  handle(state.sensorPres2, PIN_SENSOR_PRES_02, "Sensor presence 02");
}

void handleSensorDoorKitchen() {
  handle(state.sensorDoorKitchen, PIN_SENSOR_DOOR_KITCHEN, "Sensor door kitchen");
}

void handleSensorWindowKitchen() {
  handle(state.sensorWindowKitchen, PIN_SENSOR_WINDOW_KITCHEN, "Sensor window kitchen");
}

void handleSensorDoorRoom() {
  handle(state.sensorDoorRoom, PIN_SENSOR_DOOR_ROOM, "Sensor door room");
}

void handleSensorWindowRoom() {
  handle(state.sensorWindowRoom, PIN_SENSOR_WINDOW_ROOM, "Sensor window room");
}

void handleSensorWindowBedroom1() {
  handle(state.sensorWindowBedroom1, PIN_SENSOR_WINDOW_BEDROOM_01, "Sensor bedroom 1");
}

void handleSensorWindowBedroom2() {
  handle(state.sensorWindowBedroom2, PIN_SENSOR_WINDOW_BEDROOM_02, "Sensor bedroom 2");
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

void *initializeInteruption() {
  wiringPiSetup();

  // initialize all pins like OUTPUT
  initializePinSensors();

  gettimeofday(&last_change, NULL);

  bindInterupts();

  getStateOfPin();

  for (;;) {
    sleep(1);
  }
}