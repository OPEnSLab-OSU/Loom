
#include <SPI.h>
#include <OSCBundle.h>
//#include <RF24Network.h>
//#include <RF24.h>


#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x) //to concatenate a predefined number to a string literal, use STR(x)

#define FAMILY "/LOOM"
#define DEVICE "/Ishield"
#define INSTANCE_NUM 0  // Unique instance number for this device, useful when using more than one of the same device type in same space

#define IDString FAMILY DEVICE STR(INSTANCE_NUM) // C interprets subsequent string literals as concatenation: "/Loom" "/Ishield" "0" becomes "/Loom/Ishield0"

//RF24 radio(5,6);

//RF24Network network(radio);

const uint16_t this_node = 00;
const uint16_t other_node = 01;

const unsigned long interval = 2000;

unsigned long last_sent;
unsigned long packets_sent;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  delay(1000);
  Serial.print("Starting up...");
  //SPI.begin();
  //radio.begin();
  //network.begin(90, this_node);
}

void loop() {
  //network.update();

  //unsigned long now = millis();
  //if (now - last_sent >= interval)
  //{
    //last_sent = now;
  OSCBundle bndl;
  bndl.add(IDString "/VWC").add((float)1.0);
  bndl.add(IDString "/Temp").add((int32_t)-1);
  bndl.add(IDString "/ElecCond").add((float)3.0);

  delay(2000);

  char *string = get_OSC_string(bndl);
  Serial.println(string);

  delay(2000);

  get_OSC_bundle(string, &bndl);
  delay(2000);
  //}
  free(string);
}
