#include <string.h>

void get_OSC_string(OSCBundle bndl) {
  char buf[50];
  char type;
  int n = 0;
  OSCMessage* msg = bndl.getOSCMessage(n);
  
  while(msg != NULL) {
    msg->getAddress(buf, 0);
    type = msg->getType(0);
    
    Serial.print("Address ");
    Serial.print(n+1);
    Serial.print(": ");
    Serial.println(buf);

    

    if (type == 'f') {
      float value = msg->getFloat(0);
      Serial.print("Value ");
      Serial.print(n+1);
      Serial.print(": ");
      Serial.println(value);
    }
    else if (type == 'i') {
      int value = msg->getInt(0);
      Serial.print("Value ");
      Serial.print(n+1);
      Serial.print(": ");
      Serial.println(value);
      
    }
    n++;
    msg = bndl.getOSCMessage(n);
  }

}

