#ifndef SENSORLIST_H
#define SENSORLIST_H

struct SensorList {
  int count;
  int list[6];
  int type[6];
  float readings[6][3];
};

#define D_GS3 0
#define D_5TM 1

#endif
