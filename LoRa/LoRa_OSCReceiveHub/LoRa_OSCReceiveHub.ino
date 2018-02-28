#include <SPI.h>
#include <Ethernet2.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <OSCBundle.h>
#include "LOOM_OSC_Scheme.h"

#define DEBUG 1
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3 //Use this for the M0
//#define RFM95_INT 7 //Use this for the 32u4

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2
#define NUM_FIELDS 8
#define MESSAGE_SIZE 240

//Ethernet / Hub Info
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
const char DEVID[] = "v25CCAAB0F709665"; 
char serverName[] = "api.pushingbox.com";
IPAddress ip(192, 168, 1, 177);
EthernetClient client;
String data[NUM_FIELDS];


// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

RHReliableDatagram manager(rf95, SERVER_ADDRESS);
uint8_t buf[MESSAGE_SIZE];

void setup() {
  //This is required for using the ethernet featherwing
  //On a LoRa or RFM board.
  pinMode(8, INPUT_PULLUP);
  
  Serial.begin(9600);
  delay(10000);
  if (!manager.init())
    Serial.println("init failed");

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  //Allow for time to connect
  delay(1000);
  
  rf95.setTxPower(23, false);
  Serial.println("Finished setup!");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (manager.available()) {
    uint8_t len = MESSAGE_SIZE;
    uint8_t from;
    memset(buf, '\0', MESSAGE_SIZE);
    if (manager.recvfromAck(buf, &len, &from)) {
      OSCBundle bndl;
      get_OSC_bundle((char*)buf, &bndl);
      Serial.print("Messages in bndl: ");
      Serial.println(bndl.size());
      for(int i = 0; i < bndl.size(); i++) {
        data[i] = get_data_value(bndl.getOSCMessage(i), 0);
      }
      for(int i = 0; i < NUM_FIELDS; i++) {
        Serial.print("Data["); 
        Serial.print(i); 
        Serial.print("]: ");
        Serial.println(data[i]);
      }
      sendToPushingBox();
      Serial.println("");
    }
  }
}

//Function for sending the request to PushingBox
void sendToPushingBox()
{
  client.stop();
  if (client.connect(serverName, 80)) {  
    client.print("GET /pushingbox?devid="); client.print(DEVID); 
    client.print("&IDtag=");client.print(data[0]);
    client.print("&TimeStamp=");client.print(data[1]);
    client.print("&TempC=");client.print(data[2]);
    client.print("&Humid=");client.print(data[3]);
    client.print("&LoadCell=");client.print(data[4]);
    client.print("&IRLight=");client.print(data[5]);
    client.print("&FullLight=");client.print(data[6]);
    client.print("&BatVolt=");client.print(data[7]);
    client.println(" HTTP/1.1");
    client.print("Host: "); client.println(serverName);
    client.println("User-Agent: Arduino");
    client.println();
   
  } 
  else {
    if(DEBUG){Serial.println("connection failed");}
  }
}
