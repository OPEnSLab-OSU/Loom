#include <string.h>

union data_value {
  int32_t i;
  float f;
  uint32_t u;
};

void get_OSC_string(OSCBundle *bndl, char *osc_string) {
  char data_type;
  data_value value;
  int addr_len = 40;
  OSCMessage* msg;
  char addr_buf[addr_len];

  memset(osc_string, '\0', sizeof(osc_string));

  for (int i = 0; i < bndl->size(); i++) {
    msg = bndl->getOSCMessage(i);
    memset(addr_buf, '\0', addr_len);
    msg->getAddress(addr_buf, 0);
    strcat(osc_string, addr_buf);

    for (int j = 0; j < msg->size(); j++) {
      data_type = msg->getType(j);
      switch (data_type) {
        case 'f':
          value.f = msg->getFloat(j);
          snprintf(addr_buf, addr_len, ",f%lu", value.u);
          strcat(osc_string, addr_buf);
          break;

        case 'i':
          value.i = msg->getInt(j);
          snprintf(addr_buf, addr_len, ",i%lu", value.u);
          strcat(osc_string, addr_buf);
          break;

        case 's':
          char data_buf[40];
          msg->getString(j, data_buf, sizeof(data_buf));
          snprintf(addr_buf, addr_len, ",s%s", data_buf);
          strcat(osc_string, addr_buf);
          break;

        default:
          if (data_type != '\0')
            Serial.print("Invalid message arg type");
          break;
      }
    }
    if (msg != NULL) strcat(osc_string, " ");
  }
}

void get_OSC_bundle(char *string, OSCBundle* bndl) {
  bndl->empty();
  data_value value_union;
  char buf[strlen(string)+1];
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
  char data_type;
  OSCMessage *msg = bndl->getOSCMessage(n);
  while (msg != NULL) {
    msg->getAddress(buf, 0);
    Serial.print("Address ");
    Serial.print(n + 1);
    Serial.print(": ");
    Serial.println(buf);

    int m = 0;
    data_type = msg->getType(m);
    while (data_type != '\0') {
      Serial.print("Value ");
      Serial.print(m + 1);
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
      data_type = msg->getType(m);
    }
    n++;
    msg = bndl->getOSCMessage(n);
  }
}

String get_data_value(OSCMessage* msg, int pos) {
  switch (msg->getType(pos)) {
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
