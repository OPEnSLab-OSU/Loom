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

#define SERVER_ADDRESS 2
#define NUM_FIELDS 16
#define MESSAGE_SIZE RH_RF95_MAX_MESSAGE_LEN

//Ethernet / Hub Info
//byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//Use this for OPEnS Lab
byte mac[] = {0x98, 0x76, 0xB6, 0x10, 0x61, 0xD6};  

const char DEVID[] = "v25CCAAB0F709665"; 
char serverName[] = "api.pushingbox.com";

//IP Address
//IPAddress ip(10, 248, 55, 154);
//Use this for OPEnS Lab
IPAddress ip(128,193,56,138);


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
      if(((char)(buf[0])) == '/') {
        OSCBundle bndl;
        get_OSC_bundle((char*)buf, &bndl); 
        for(int i = 0; i < NUM_FIELDS; i++)
          data[i] = get_data_value(bndl.getOSCMessage(0), i);
      }
      else {
        char str[MESSAGE_SIZE];
        String((char*)buf).toCharArray(str, sizeof(str)-1);
        Serial.println(str);
        char *token;
        char *savept = str;
        String cols[8] = {"IDtag", "RTC_time", "temp", "humidity", "loadCell", "lightIR", "lightFull", "vbat"};
        for(int i = 0; i < NUM_FIELDS; i+=2) {
          token = strtok_r(savept, ",", &savept);
          if(token != NULL) {
            data[i] = cols[i/2];
            data[i+1] = String(token);
          }
        }
        for(int i = 0; i < NUM_FIELDS; i++) {
          Serial.println(data[i]);   
        }
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
    for(int i = 0; i < NUM_FIELDS; i++) {
      if((i % 2) == 0)
        client.print("&key" + String(i/2) + "=");
      else
        client.print("&val" + String(i/2) + "=");
      client.print(data[i]);
    }
    client.println(" HTTP/1.1");
    client.print("Host: "); client.println(serverName);
    client.println("User-Agent: Arduino");
    client.println();
   
  } 
  else {
    if(DEBUG){Serial.println("connection failed");}
  }
}
