#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <OSCBundle.h>
#include "LOOM_OSC_Scheme.h"
 
#define RFM95_CS 8
#define RFM95_RST 4
//#define RFM95_INT 3 //Use this for the M0
#define RFM95_INT 7 //Use this for the 32u4

#define CLIENT_ADDRESS 3
#define SERVER_ADDRESS 2

#define FAMILY "/LOOM"
#define DEVICE "/LoRaNode"
#define INSTANCE_NUM 0  // Unique instance number for this device, useful when using more than one of the same device type in same space
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
#define MESSAGE_SIZE RH_RF95_MAX_MESSAGE_LEN

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

RHReliableDatagram manager(rf95, CLIENT_ADDRESS);

String sendString = String("IDstring,RTC_timeString,tempString,humidityString,loadCellString,lightIRString,lightFullString,vbatString");

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

void loop() {
  /*OSCBundle bndl;
  bndl.add(IDString).add("Date").add("3/6/2018").add("IDtag").add((int32_t) INSTANCE_NUM)
  .add("TimeStamp").add("2018").add("TempC").add((int32_t)32).add("Humidity").add((float)46.4)
  .add("LoadCell").add((int32_t)1000).add("IRLight").add((int32_t)2000).add("FullLight").add((int32_t)3000)
  .add("BatVolt").add((float)4.2);*/

  char message[RH_RF95_MAX_MESSAGE_LEN];
  memset(message, '\0', sizeof(message));
  sendString.toCharArray(message, sizeof(message)-1);
  //get_OSC_string(&bndl, message);

  /*
  for(int i = 0; i < MESSAGE_SIZE; i++) {
    if(message[i] == '\0' || i == MESSAGE_SIZE-1) {
      Serial.print("Message uses ");
      Serial.print(i);
      Serial.println(" byes of the message buffer.");
      break;
    }
  }*/
  

  Serial.println(message);
  /*Serial.print("Message length: ");
  Serial.println(strlen(message));
  Serial.print("Max message length: ");
  Serial.println(RH_RF95_MAX_MESSAGE_LEN);*/
  
  Serial.print("Sending...");
  if (manager.sendtoWait((uint8_t*)message, strlen(message), SERVER_ADDRESS))
    Serial.println("ok");
  else
    Serial.println("failed");

  delay(10000);
}
