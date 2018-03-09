#include <string.h>

union data_value {
  int32_t i;
  float f;
  uint32_t u;
  char* c;
};

void get_OSC_string(OSCBundle *bndl, char *osc_string) {
  char buf[50];
  char data_type;
  int n = 0;
  data_value value;
  
  OSCMessage* msg = bndl->getOSCMessage(n);
  memset(osc_string, '\0', strlen(osc_string));
  
  while(msg != NULL) {
    msg->getAddress(buf, 0);
    
    /*Serial.print("Address ");
    Serial.print(n+1);
    Serial.print(": ");
    Serial.println(buf);*/

    strcat(osc_string, buf);

    int m = 0;
    data_type = msg->getdata_type(m);

    while(data_type != '\0') {

      if (data_type == 'f') {
        value.f = msg->getFloat(m);
		
        /*Serial.print("Value ");
        Serial.print(n+1);
        Serial.print(": ");
        Serial.println(value);*/

        snprintf(buf, 50, ",f%lu", value.u);
        strcat(osc_string, buf);
      }
      else if (data_type == 'i') {
        value.i = msg->getInt(m);
		
        /*Serial.print("Value ");
        Serial.print(n+1);
        Serial.print(": ");
        Serial.println(value);*/

        snprintf(buf, 50, ",i%lu", value.u);
        strcat(osc_string, buf);
      }
      else if (data_type == 's') {
        char val_buf[50];
        msg->getString(m, val_buf, 50);

        snprintf(buf, 50, ",s%s", val_buf);
        strcat(osc_string, buf);
      }
      m++;
      data_type = msg->getdata_type(m);
    }
    n++;
    msg = bndl->getOSCMessage(n);
    if (msg != NULL) strcat(osc_string, " ");
  }
}

void get_OSC_bundle(char *osc_string, OSCBundle* bndl) {
  bndl->empty();
  data_value value_union;
  char buf[msg_size];
  char *p = buf, *p2 = NULL;
  char *token = NULL, *msg_token = NULL;
  strcpy(buf, osc_string);
  OSCMessage *msg;
  msg_token = strtok_r(p, " ", &p);
  while (msg_token != NULL & strlen(msg_token) > 0) {
    p2 = msg_token;
    token = strtok_r(p2, ",", &p2);
    msg = &(bndl->add(token));
    token = strtok_r(NULL, ",", &p2);
    int k = 1;
    while (token != NULL & strlen(token) > 0) {
      if (token[0] == 'f') {
        value_union.u = strtoul(&token[1], NULL, 0);
        msg->add(value_union.f);
      }
      else if (token[0] == 'i') {
        value_union.u = strtoul(&token[1], NULL, 0);
        msg->add(value_union.i);
      }
      else if (token[0] == 's') {
        msg->add(&token[1]);
      }
      token = strtok_r(p2, ",", &p2);
    }
    msg_token = strtok_r(p, " ", &p);
  }
}

void print_bundle(OSCBundle *bndl) {
  int n = 0;
  char buf[50];
  char data_type;
  OSCMessage *msg = bndl->getOSCMessage(n);
  while(msg != NULL) {
    msg->getAddress(buf, 0);
    Serial.print("Address ");
    Serial.print(n+1);
    Serial.print(": ");
    Serial.println(buf);

    int m = 0;
    data_type = msg->getdata_type(m);
    while(data_type != '\0') {
      Serial.print("Value ");
      Serial.print(m+1);
      Serial.print(": ");
      if (data_type == 'f') {
        Serial.println(msg->getFloat(m));
      }
      else if (data_type == 'i') {
        Serial.println(msg->getInt(m));
      }
      else if (data_type == 's') {
        msg->getString(m, buf, 50);
        Serial.println(buf);
      }
      
      m++;
      data_type = msg->getdata_type(m);
    }
    n++;
    msg = bndl->getOSCMessage(n);
  }
}

String get_data_valueue(OSCMessage* msg, int pos) {
  switch(msg->getdata_type(pos)) {
    case 'i':
      return String(msg->getInt(pos));
      break;
    case 'f':
      return String(msg->getFloat(pos));
      break;
    case 's':
      char buf[80];
      msg->getString(pos, buf, 80);
      return String(buf);
      break;
    default:
      Serial.println("Unsupported data data_type.");
      return String("");
  }
}
