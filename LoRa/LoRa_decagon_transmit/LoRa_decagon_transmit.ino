
#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <OSCBundle.h>
#include "SDI12.h"
#include <string.h>
#include "SensorList.h"

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>

#ifdef __SAMD21G18A__
#define is_M0
#endif

#ifdef __AVR_ATmega32U4__
#define is_32U4
#pragma message("Warning: 32u4 can only interface with one Decagon device on pin 11")
#endif

//===== Decagon Initializations =====

#define DATAPIN1 A0  // change to the proper pin
#define DATAPIN2 A1
#define DATAPIN3 A3
#define DATAPIN4 A4
#define DATAPIN5 A5
#define DATAPIN6 11
#define SENSOR_ADDRESS "?"

//Declare 6 SDI-12 objects initialized with DATAPIN1-6
SDI12 mySDI12[6] = {DATAPIN1, DATAPIN2, DATAPIN3, DATAPIN4, DATAPIN5, DATAPIN6};

//Struct used to manage sensor state data
struct SensorList data;

String sdiResponse = "";

//===== LoRa Initializations =====

#ifdef is_M0
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#endif

#ifdef is_32U4
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#endif

#define SERVER_ADDRESS 88

//IDString constructor

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x) //to concatenate a predefined number to a string literal, use STR(x)

#define FAMILY "/LOOM"
#define DEVICE "/DShield"
#define INSTANCE_NUM 0  // Unique instance number for this device, useful when using more than one of the same device type in same space

#define IDString FAMILY DEVICE STR(INSTANCE_NUM) // C interprets subsequent string literals as concatenation: "/Loom" "/Ishield" "0" becomes "/Loom/Ishield0"
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

RHReliableDatagram manager(rf95, INSTANCE_NUM);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(2000);
  lora_setup(&rf95, &manager);

  // Decagon Setup

  for (int i = 0; i < 6; i++) {
    mySDI12[i].begin();
    mySDI12[i].forceHold();
  }

#ifdef SDI12_EXTERNAL_PCINT
  pinMode(DATAPIN1, INPUT_PULLUP);
  enableInterrupt(DATAPIN1, SDI12::handleInterrupt, CHANGE);
  pinMode(DATAPIN2, INPUT_PULLUP);
  enableInterrupt(DATAPIN2, SDI12::handleInterrupt, CHANGE);
  pinMode(DATAPIN3, INPUT_PULLUP);
  enableInterrupt(DATAPIN3, SDI12::handleInterrupt, CHANGE);
  pinMode(DATAPIN4, INPUT_PULLUP);
  enableInterrupt(DATAPIN4, SDI12::handleInterrupt, CHANGE);
  pinMode(DATAPIN5, INPUT_PULLUP);
  enableInterrupt(DATAPIN5, SDI12::handleInterrupt, CHANGE);
  pinMode(DATAPIN6, INPUT_PULLUP);
  enableInterrupt(DATAPIN6, SDI12::handleInterrupt, CHANGE);
#endif

  // Poll Sensors

  data = poll_sensors(mySDI12);

}

void loop() {
  
  // ===== Send Measure to Active Sensors =====

  for (int i = 0; i < data.count; i++) {
    int num = data.list[i];
    get_measure(&mySDI12[num]);
  }

  delay(1000);     // delay between taking reading and requesting data

  // ===== Retrieve Data from Active Sensors =====

  for (int i = 0; i < data.count; i++) {
    int num = data.list[i];
    sdiResponse = get_data(&mySDI12[num]);

    set_data(&data, sdiResponse, i);
    /*Serial.print("Data from pin A");
    Serial.print(num);
    Serial.print(": ");
    Serial.println(sdiResponse);*/
    sdiResponse = "";
  }

  // ===== Package data =====
  OSCBundle bndl;
  package_data(&bndl, data);

  // ===== Transmit =====

  char message[201];

  memset(message, '\0', 201);

  get_OSC_string(&bndl, message);

  delay(2000);

  Serial.print("Sending...");
  if (manager.sendtoWait((uint8_t*)message, strlen(message), SERVER_ADDRESS))
    Serial.println("ok");
  else
    Serial.println("failed");

  delay(2000);
}
