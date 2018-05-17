
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
#pragma message("Warning: 32u4 can only interface with one Decagon device on pin 10")
#endif

#define DEBUG 1
#ifndef DEBUG
#define DEBUG 0
#endif

//===== Decagon Initializations =====

#ifdef is_M0
#define SENSORCOUNT 6 //Determines how many pins you actually want to poll for sensors
#endif //is_M0

#ifdef is_32U4
#define SENSORCOUNT 1
#endif //is_32U4

#define DATAPIN1 10  // change to the proper pin
#define DATAPIN2 A0
#define DATAPIN3 A1
#define DATAPIN4 A3
#define DATAPIN5 A4
#define DATAPIN6 A5
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

#define SERVER_ADDRESS 2

//battery voltage read pin
#ifdef is_M0
#define VBATPIN A7
#endif

#ifdef is_32U4
#define VBATPIN A9
#endif

float measuredvbat;

//IDString constructor

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x) //to concatenate a predefined number to a string literal, use STR(x)

#define FAMILY "/LOOM"
#define DEVICE "/DShield"
#define INSTANCE_NUM 0  // Unique instance number for this device, useful when using more than one of the same device type in same space
// $$$$$$$ NOTE: Change instance number for each device above ^ $$$$$$$$


#define IDString FAMILY DEVICE STR(INSTANCE_NUM) // C interprets subsequent string literals as concatenation: "/Loom" "/Ishield" "0" becomes "/Loom/Ishield0"
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

RHReliableDatagram manager(rf95, INSTANCE_NUM + 10);

// ===== RTC Initializations =====

#define RTC3231

#ifdef RTC3231
RTC_DS3231 RTC_DS;

char TimeStamp[20];

// declare/init RTC_DS variables//
volatile bool TakeSampleFlag = true; // Flag is set with external Pin A0 Interrupt by RTC
volatile bool LEDState = false; // flag t toggle LED
volatile int HR = 8; // Hr of the day we want alarm to go off
volatile int MIN = 0; // Min of each hour we want alarm to go off
#if DEBUG == 0
volatile int WakePeriodMin = 15;  // Period of time to take sample in Min, reset alarm based on this period (Bo - 5 min)
#elif DEBUG == 1
volatile int WakePeriodMin = 1;
#endif //DEBUG
const byte wakeUpPin = 11;
#endif //RTC3231

//===== Setup =====

void setup() {
  // put your setup code here, to run once:
#if DEBUG == 1
  Serial.begin(9600);
#endif
  delay(10000); //delay on start up so boards don't immediately sleep :)
  lora_setup(&rf95, &manager);

  // Decagon Setup

  for (int i = 0; i < 6; i++) {
    mySDI12[i].begin();
    mySDI12[i].forceHold();
  }

#ifdef SDI12_EXTERNAL_PCINT
  pinMode(DATAPIN1, INPUT_PULLUP);
  enableInterrupt(DATAPIN1, SDI12::handleInterrupt, CHANGE);
#ifndef is_32U4
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
#endif //not is_32U4
#endif

  // RTC Setup

#ifdef RTC3231


  pinMode(wakeUpPin, INPUT_PULLUP);

  InitalizeRTC();
#if DEBUG == 1
    Serial.print("Alarm set to go off every "); Serial.print(WakePeriodMin); Serial.println("min from program time");
#endif // DEBUG
  delay(10000); //Things break without this, I don't know why
  TakeSampleFlag = true;
#endif // RTC3231
}

void loop() {
  rf95.sleep();
#ifdef RTC3231

#ifdef is_M0
  attachInterrupt(digitalPinToInterrupt(wakeUpPin), wake, LOW);

  LowPower.standby();

#endif //is_M0

#ifdef is_32U4
  pinMode(wakeUpPin, INPUT_PULLUP);
  enableInterrupt(wakeUpPin, wake, FALLING);
  
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  
#endif //is_32U4

#endif //RTC3231

#ifdef RTC3231
  if(TakeSampleFlag)
  { 

#ifdef is_M0
    //detachInterrupt(digitalPinToInterrupt(wakeUpPin));
#endif //is_M0

#ifdef is_32U4
    //disableInterrupt(wakeUpPin);
#endif //is_32U4

    clearAlarmFunction(); // Clear RTC Alarm
    
  // ===== Create RTC Timestamp =====
  DateTime now = RTC_DS.now();
  uint8_t mo = now.month();
  uint8_t d = now.day();
  uint8_t h = now.hour();
  uint8_t mm = now.minute();

  String RTC_monthString = String(mo, DEC);
  String RTC_dayString = String(d, DEC);
  String RTC_hrString = String(h, DEC);
  String RTC_minString = String(mm, DEC);
  String RTC_timeString = RTC_hrString + ":" + RTC_minString + "_" + RTC_monthString + "/" + RTC_dayString;
  RTC_timeString.toCharArray(TimeStamp, 20);

#endif //RTC3231

  // ===== Poll Sensors =====

  data = poll_sensors(mySDI12);

  // ===== Get Battery Voltage =====

  measuredvbat = analogRead(VBATPIN); // reading battery voltage
  measuredvbat *= 2;    // we divided by 2, so multiply back
  measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  measuredvbat /= 1024; // convert to voltage
  
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

  // ===== Package and Transmit data =====
  OSCBundle bndl;
  for(int sensor_number = 0; sensor_number < data.count; sensor_number++) {
    package_data(&bndl, data, sensor_number);

    // Transmit

    char message[RH_RF95_MAX_MESSAGE_LEN + 1];
    memset(message, '\0', RH_RF95_MAX_MESSAGE_LEN + 1);
    get_OSC_string(&bndl, message);
    delay(2000);
#if DEBUG == 1
    print_bundle(&bndl);
    Serial.print("Sending...");
#endif
    if (manager.sendtoWait((uint8_t*)message, strlen(message), SERVER_ADDRESS)) {
#if DEBUG == 1
      Serial.println("ok");
#endif
    }
    else {
#if DEBUG == 1
      Serial.println("failed");
#endif
    }
  }

#ifdef RTC3231
  setAlarmFunction();
  delay(75);  // delay so serial stuff has time to print out all the way
  TakeSampleFlag = false; // Clear Sample Flag
  }
#endif //RTC3231

}
