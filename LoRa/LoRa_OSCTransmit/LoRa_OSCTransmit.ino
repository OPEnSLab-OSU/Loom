#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <OSCBundle.h>
#include <EnableInterrupt.h>
 
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3 //Use this for the M0
//#define RFM95_INT 7 //Use this for the 32u4

#define CLIENT_ADDRESS 0
#define SERVER_ADDRESS 88

//IDString constructor

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x) //to concatenate a predefined number to a string literal, use STR(x)

#define FAMILY "/LOOM"
#define DEVICE "/Ishield"
#define INSTANCE_NUM 0  // Unique instance number for this device, useful when using more than one of the same device type in same space

#define IDString FAMILY DEVICE STR(INSTANCE_NUM) // C interprets subsequent string literals as concatenation: "/Loom" "/Ishield" "0" becomes "/Loom/Ishield0"
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

RHReliableDatagram manager(rf95, CLIENT_ADDRESS);

void setup() {
  Serial.begin(9600);
  delay(2000);

  Serial.println("Initializing manager...");
  if (!manager.init())
    Serial.println("init failed");

  Serial.println("Setting Frequency...");
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }

  Serial.println("Setting power...");
  rf95.setTxPower(23, false);

}

uint8_t data[] = "Hello world";

void loop() {
  OSCBundle bndl;
  bndl.add(IDString "/VWC").add((float)1.0);
  bndl.add(IDString "/Temp").add((int32_t)-1);
  bndl.add(IDString "/ElecCond").add((float)3.0);

  char message[121];

  memset(message, '\0', 121);
  
  get_OSC_string(&bndl, message);

  Serial.println(message);
  Serial.print("Message length: ");
  Serial.println(strlen(message));
  Serial.print("Max message length: ");
  Serial.println(RH_RF95_MAX_MESSAGE_LEN);

  delay(2000);
  
  Serial.print("Sending...");
  if (manager.sendtoWait((uint8_t*)message, strlen(message), SERVER_ADDRESS))
    Serial.println("ok");
  else
    Serial.println("failed");

  delay(2000);
}
