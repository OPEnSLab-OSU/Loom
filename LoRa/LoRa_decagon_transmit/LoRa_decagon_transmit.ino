
#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <OSCBundle.h>
#include "SDI12.h"
#include <string.h>
#include "SensorList.h"
#include "LowPower.h"
#include <Wire.h>

//NOTE: Must include the following line in the RTClibExtended.h file to use with M0:
//#define _BV(bit) (1 << (bit))
#include <RTClibExtended.h>

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>

#ifdef __SAMD21G18A__
#define is_M0
#endif

#ifdef __AVR_ATmega32U4__
#define is_32U4
#pragma message("Warning: 32u4 can only interface with one Decagon device on pin 11")
#endif

#define DEBUG 1
#ifndef DEBUG
#define DEBUG 0
#endif

//===== Decagon Initializations =====

#define DATAPIN1 A0  // change to the proper pin
#define DATAPIN2 A1
#define DATAPIN3 A3
#define DATAPIN4 A4
#define DATAPIN5 A5
#define DATAPIN6 10
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

// ===== RTC Initializations =====

#define RTC3231

#ifdef RTC3231
RTC_DS3231 RTC_DS;

// declare/init RTC_DS variables//
volatile bool TakeSampleFlag = false; // Flag is set with external Pin A0 Interrupt by RTC
volatile bool LEDState = false; // flag t toggle LED
volatile int HR = 8; // Hr of the day we want alarm to go off
volatile int MIN = 0; // Min of each hour we want alarm to go off
volatile int WakePeriodMin = 1;  // Period of time to take sample in Min, reset alarm based on this period (Bo - 5 min)
const byte wakeUpPin = 11;
#endif

//===== Setup =====

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(2000);
  Serial.println("Beginning setup");
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

  // RTC Setup

#ifdef RTC3231
  pinMode(wakeUpPin, INPUT_PULLUP);
  attachInterrupt(wakeUpPin, wake, FALLING);

  InitalizeRTC();
#if DEBUG == 1
    Serial.print("Alarm set to go off every "); Serial.print(WakePeriodMin); Serial.println("min from program time");
#endif
  delay(10000);
#endif
}

void loop() {
#ifdef RTC3231
  attachInterrupt(digitalPinToInterrupt(wakeUpPin), wake, FALLING);  
  // Enter power down state with ADC and BOD module disabled.
  // Wake up when wake up pin is low.
  LowPower.idle(IDLE_2);
  // <----  Wait in sleep here until pin interrupt
    
  // On Wakeup, proceed from here:
  //detachInterrupt(digitalPinToInterrupt(wakeUpPin)); //
  //clearAlarmFunction(); // Clear RTC Alarm

#endif

#ifdef RTC3231
  if(TakeSampleFlag)
  {
    detachInterrupt(digitalPinToInterrupt(wakeUpPin)); //
    clearAlarmFunction(); // Clear RTC Alarm
#endif
  // ===== Poll Sensors =====

  data = poll_sensors(mySDI12);
  
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

  print_bundle(&bndl);

  Serial.print("Sending...");
  if (manager.sendtoWait((uint8_t*)message, strlen(message), SERVER_ADDRESS))
    Serial.println("ok");
  else
    Serial.println("failed");

#ifdef RTC3231
  setAlarmFunction();
  delay(75);  // delay so serial stuff has time to print out all the way
  TakeSampleFlag = false; // Clear Sample Flag
  }
#endif
  

  delay(2000);
}
