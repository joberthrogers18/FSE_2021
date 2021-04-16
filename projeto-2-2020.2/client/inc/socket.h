#include <cJSON.h>

#ifndef SOCKET_H_
#define SOCKET_H_

struct pinState {
  cJSON *lamp1;
  cJSON *lamp2;
  cJSON *lamp3;
  cJSON *lamp4;
  cJSON *arCondition1;
  cJSON *arCondition2;
  
  cJSON *sensorPres1;
  cJSON *sensorPres2;
  cJSON *sensorDoorKitchen;
  cJSON *sensorWindowKitchen;
  cJSON *sensorDoorRoom;
  cJSON *sensorWindowRoom;
  cJSON *sensorWindowBedroom1;
  cJSON *sensorWindowBedroom2;
};

extern struct pinState state;

void initSocket();
void sendMessage();
void closeSocket();

#endif