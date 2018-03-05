#include "SensorList.h"
#include <string.h>

struct SensorList poll_sensors(SDI12 *mySDI12) {
  struct SensorList data;
  String id = "";
  data.count = 0;
  for (int i = 0; i < 6; i++) {
    id = get_identity(&mySDI12[i]);
    if (id.length() > 0) {
      data.type[data.count] = get_type(id);
      data.list[data.count] = i;
      data.count++;
    }
  }
  return data;
}

int get_type(String id) {
    char buffer[50];
    char *p = buffer, *token1 = NULL, *token2 = NULL, *token3 = NULL;
    int return_val = -1;
    id.toCharArray(buffer, sizeof(buffer));
    token1 = strtok_r(p, " ", &p);
    token2 = strtok_r(NULL, " ", &p);
    token3 = strtok_r(NULL, " ", &p);
    /*Serial.print("1: ");
    Serial.println(token1);
    Serial.print("2: ");
    Serial.println(token2);
    Serial.print("3: ");
    Serial.println(token3);*/
    if (strcmp(token2, "GS3") == 0) return_val = D_GS3;
    else if (strcmp(token2, "5TM") == 0) return_val = D_5TM;
    return return_val;
}

String _send_command(SDI12 *mySDI12, String myCommand) {
  String response = "";
  mySDI12->setActive(); //Necessary when using multiple SDI12 objects

  mySDI12->sendCommand(myCommand);
  delay(30);                     // wait a while for a response

  while (mySDI12->available()) {  // build response string
    char c = mySDI12->read();
    if ((c != '\n') && (c != '\r')) {
      response += c;
      delay(5);
    }
  }

  mySDI12->clearBuffer();
  mySDI12->forceHold();
  return response;
}

String get_measure(SDI12 *mySDI12) {
  String response = "";
  String myCommand = String(SENSOR_ADDRESS) + "M!";
  response = _send_command(mySDI12, myCommand);
  return response;
}

String get_identity(SDI12 *mySDI12) {
  String response = "";
  String myCommand = String(SENSOR_ADDRESS) + "I!";
  response = _send_command(mySDI12, myCommand);
  return response;
}

String get_data(SDI12 *mySDI12) {
  String response = "";
  String myCommand = String(SENSOR_ADDRESS) + "D0!";
  response = _send_command(mySDI12, myCommand);
  return response;
}

void set_data(struct SensorList *data, String response, int index) {
  char buffer[20];
  char *p = buffer;
  response.toCharArray(buffer, sizeof(buffer));
  strtok_r(p, "+", &p); //First token is address

  //GS3 case
  if (data->type[index] == D_GS3) {
    data->readings[index][0] = atof(strtok_r(NULL, "+", &p));
    data->readings[index][1] = atof(strtok_r(NULL, "+", &p));
    data->readings[index][2] = atof(strtok_r(NULL, "+", &p));
  }
  //5TM case
  else if (data->type[index] == D_5TM) {
    data->readings[index][0] = atof(strtok_r(NULL, "+", &p));
    data->readings[index][1] = atof(strtok_r(NULL, "+", &p));
  }
}

void print_data(struct SensorList data) {
  int i = 0;
  Serial.println("\n=============================================\n");
  Serial.print("Number of sensors: ");
  Serial.println(data.count);
  Serial.println("");
  while(i < data.count) {
    Serial.print("Sensor type from pin A");
    Serial.print(data.list[i]);
    Serial.print(": ");
    if (data.type[i] == D_GS3) {
      Serial.println("GS3");
      Serial.print("VWC: ");
      Serial.println(data.readings[i][0]);
      Serial.print("Temperature: ");
      Serial.println(data.readings[i][1]);
      Serial.print("Electrical Conductivity: ");
      Serial.println(data.readings[i][2]);
    }
    else if (data.type[i] == D_5TM) {
      Serial.println("5TM");
      Serial.print("VWC: ");
      Serial.println(data.readings[i][0]);
      Serial.print("Temperature: ");
      Serial.println(data.readings[i][1]);
    }
    Serial.println("");
    i++;
  }
}
