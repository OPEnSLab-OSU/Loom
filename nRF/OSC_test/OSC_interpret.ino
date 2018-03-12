#include <string.h>

union data_vals {
  int32_t i;
  float f;
  uint32_t u;
};

void get_OSC_string(OSCBundle *bndl, char *string) {
  char buf[50];
  char type;
  int n = 0;
  data_vals value;
  OSCMessage* msg = bndl->getOSCMessage(n);

  memset(string, '\0', 121);
  
  while(msg != NULL) {
    msg->getAddress(buf, 0);
    
    /*Serial.print("Address ");
    Serial.print(n+1);
    Serial.print(": ");
    Serial.println(buf);*/

    strcat(string, buf);

    int m = 0;
    type = msg->getType(m);

    while(type != '\0') {

      if (type == 'f') {
        value.f = msg->getFloat(m);
        /*Serial.print("Value ");
        Serial.print(n+1);
        Serial.print(": ");
        Serial.println(value);*/

        snprintf(buf, 50, ",f%lu", value.u);
        strcat(string, buf);
      }
      else if (type == 'i') {
        value.i = msg->getInt(m);
        /*Serial.print("Value ");
        Serial.print(n+1);
        Serial.print(": ");
        Serial.println(value);*/

        snprintf(buf, 50, ",i%lu", value.u);
        strcat(string, buf);
      }
      else if (type == 's') {
        char val_buf[50];
        msg->getString(m, val_buf, 50);

        snprintf(buf, 50, ",s%s", val_buf);
        strcat(string, buf);
      }
      m++;
      type = msg->getType(m);
    }
    n++;
    msg = bndl->getOSCMessage(n);
    if (msg != NULL) strcat(string, " ");
  }
}

void get_OSC_bundle(char *string, OSCBundle* bndl) {
  bndl->empty();
  data_vals value_union;
  char buf[121];
  char *p = buf, *p2 = NULL;
  char *token = NULL, *msg_token = NULL;
  strcpy(buf, string);
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
  char type;
  OSCMessage *msg = bndl->getOSCMessage(n);
  while(msg != NULL) {
    msg->getAddress(buf, 0);
    Serial.print("Address ");
    Serial.print(n+1);
    Serial.print(": ");
    Serial.println(buf);

    int m = 0;
    type = msg->getType(m);
    while(type != '\0') {
      Serial.print("Value ");
      Serial.print(m+1);
      Serial.print(": ");
      if (type == 'f') {
        Serial.println(msg->getFloat(m));
      }
      else if (type == 'i') {
        Serial.println(msg->getInt(m));
      }
      else if (type == 's') {
        msg->getString(m, buf, 50);
        Serial.println(buf);
      }
      
      m++;
      type = msg->getType(m);
    }
    n++;
    msg = bndl->getOSCMessage(n);
  }
}

