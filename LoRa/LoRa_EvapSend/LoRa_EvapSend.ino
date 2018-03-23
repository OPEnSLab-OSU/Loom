/* Authors: Chet Udell, Marissa Kwon, Thomas DeBell
    Date: 06/26/2017
    THIS CODE IS IN COMPLETE AS OF 6/28/17 at 5:37 p.m do
  Description: Code for "Transmitter" LoRa radio Feather Board; initilizes
  LoRa radio transmitter and sends data to LoRa receiver on the same
  frequency.

  This code originated from Adafruit's website and has been
  adjusted for use in the Internet of Agriculture project.
  Beginning Researcher student Research at OPEnS Lab at Oregon State
  University.
  // Important Example code found at https://learn.adafruit.com/adafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts/rfm9x-test

  APPENDIX *****************************
  SET-UP loop = 87, Void Loop(main) = 146, RTC Subroutines and loop= 241,
  Alarm Function = 277, 298,  Wakeup ISR= 311, InteruptPin set-up= 313,323
  ****************************************************8
*/

// LoRa 9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client(transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example LoRa9x_RX

#include <SPI.h>
#include <RH_RF95.h> // Important Example code found at https://learn.adafruit.com/adafruit-rfm69hcw-and-rfm96-rfm95-rfm98-lora-packet-padio-breakouts/rfm9x-test
#include <RHReliableDatagram.h>
#include "HX711.h"  //https://learn.sparkfun.com/tutorials/load-cell-amplifier-hx711-breakout-hookup-guide
#include "LowPower.h" // from sparkfun low power library found here https://github.com/rocketscream/Low-Power
#include "RTClibExtended.h"// from sparkfun low power library found here https://github.com/FabioCuomo/FabioCuomo-DS3231/
//------------------------------------------------------------------------
// Humidity/Temperature Sensor Settings--------------------
//------------------------------------------------------------------------
//#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_SHT31.h" // https://github.com/adafruit/Adafruit_SHT31
// ------------------------------------------------------------------------
// Infrared/Full Light Sensor Settings--------------------
//------------------------------------------------------------------------
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h" // https://github.com/adafruit/Adafruit_TSL2591_Library
//------------------------------------------------------------------------
// Debug Mode, Set flag to 0 for normal operation
//------------------------------------------------------------------------
#define DEBUG 1
//------------------------------------------------------------------------
// LORA pins --------------------
//------------------------------------------------------------------------
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7

#define SERVER_ADDRESS 2
//battery voltage read pin
#define VBATPIN A9

//super validator calibration variable//
#define calibration_factor 430000//This value is obtained using the Calibration sketch (grams)

//load cell variables//
#define DOUT 5 //connecting the out and clock pins for the load cell
#define CLK 6


//Global Set-up//
float temp, humidity, loadCell, measuredvbat;
String IDstring, tempString, humidityString, loadCellString, lightRString, lightGString, lightBString, lightIRString, lightFullString, vbatString, RTC_monthString, RTC_dayString, RTC_hrString, RTC_minString, RTC_timeString, stringTransmit;
int transmitBufLen; // length of transmit buffer
const int ID = 100;
uint16_t lightR, lightG, lightB, lightIR, lightFull;
//char transmitBuf[23]; // this needs to be the length of the transmission buffer
float temp_ar[5], humidity_ar[5], loadcell_ar[5];
uint16_t lightIR_ar[5], lightFull_ar[5];

HX711 scale(DOUT, CLK);

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(rf95, SERVER_ADDRESS);

// Evap code from Manuel instance of temp/humidity sensor
Adafruit_SHT31 sht31 = Adafruit_SHT31();

// Create instance of TSL2591 light sensor
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591); // can pass in a number for the sensor identifier (for your use later)

// Create instance of DS3231 called RTC
RTC_DS3231 RTC; //we are using the DS3231 RTC

// declare/init RTC variables//
volatile bool TakeSampleFlag = false; // Flag is set with external Pin A0 Interrupt by RTC
volatile bool LEDState = false; // flag t toggle LED
volatile int HR = 8; // Hr of the day we want alarm to go off
volatile int MIN = 0; // Min of each hour we want alarm to go off
volatile int WakePeriodMin = 1;  // Period of time to take sample in Min, reset alarm based on this period (Bo - 5 min)
const byte wakeUpPin = 11;

////////////////////////
void setup()
{
  // convert ID into String, done up here because we only need this done once
  IDstring = String(ID, DEC);
  pinMode(wakeUpPin, INPUT_PULLUP);

  // Turns on temp and humid sensor //
  sht31.begin(0x44);
  // load cell calibration
  scale.set_scale(calibration_factor); //This value is obtained by using the Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0
  scale.power_down(); // Go into low power mode

  //LoRa transmission//
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  Serial.begin(9600);
  //while (!Serial); // waits for serial hardware to start up
#if DEBUG == 1
  //report all sensors present on system
  Serial.println(" LoRa Feather Transmitter Test!");
  Serial.println("HX711 scale");
  Serial.println("Starting Adafruit TSL2591 Test!");
#endif
  
  //check light sensor init
  while (!tsl.begin())
  {
  #if DEBUG == 1
    Serial.println("No sensor found ... check your wiring?");
  #endif  
    while (1);
  }
  #if DEBUG == 1
  Serial.println("Found a TSL2591 sensor");
  #endif
  /* Configure the sensor */
  configureSensor();
  //there's an option to print sensor details in example code - this requires addit'l functions and takes up too much space

  //manually reset LoRa
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  while (!manager.init()) {
    #if DEBUG == 1
      Serial.println("LoRa manager init failed"); //if print wiring may be wrong
    #endif
    while (1);
  }
  #if DEBUG == 1
    Serial.println("LoRa radio init OK!");
  #endif
  // checks if frequency is initialized
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    #if DEBUG == 1
      Serial.println("setFrequency failed");
    #endif
    while (1);
  }
  #if DEBUG == 1
    Serial.print("Set Freq to: ");
    Serial.println(RF95_FREQ);
  #endif

  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips / symbol, CRC on
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);


  //RTC stuff init//

  InitalizeRTC();
  #if DEBUG == 1
    Serial.print("Alarm set to go off every "); Serial.print(WakePeriodMin); Serial.println("min from program time");
  #endif
  delay(1000);
}
////////////////////////// MAIN //////////////////////
void loop() {
  if (!DEBUG)
  {
    // Sleep the radio until needed
    rf95.sleep();
    // Enable SQW pin interrupt
    // enable interrupt for PCINT7...
    pciSetup(11);

    // Enter into Low Power mode here[RTC]:
    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    // <----  Wait in sleep here until pin interrupt
    // On Wakeup, proceed from here:
    PCICR = 0x00;         // Disable PCINT interrupt
    clearAlarmFunction(); // Clear RTC Alarm
    scale.power_up();
  }
  else
  {
    delay(30000); // period in DEBUG mode to wait between samples
    scale.power_up();
    TakeSampleFlag = 1;
  }
  if (TakeSampleFlag)
  {
    // get RTC timestamp string
    DateTime now = RTC.now();
    uint8_t mo = now.month();
    uint8_t d = now.day();
    uint8_t h = now.hour();
    uint8_t mm = now.minute();

    RTC_monthString = String(mo, DEC);
    RTC_dayString = String(d, DEC);
    RTC_hrString = String(h, DEC);
    RTC_minString = String(mm, DEC);
    RTC_timeString = RTC_hrString + ":" + RTC_minString + "_" + RTC_monthString + "/" + RTC_dayString;

    // Read sensors
	for(int i = 0; i < 5; i++) {
		temp_ar[i] = sht31.readTemperature();
	}
	temp = (temp_ar[0]+temp_ar[1]+temp_ar[2]+temp_ar[3]+temp_ar[4])/5; // degrees C
	
	for(int i = 0; i < 5; i++) {
		humidity_ar[i] = sht31.readHumidity();
	}
	humidity = (humidity_ar[0]+humidity_ar[1]+humidity_ar[2]+humidity_ar[3]+humidity_ar[4])/5; // relative as a percent
	
    for(int i = 0; i < 5; i++) {
		loadcell_ar[i] = scale.get_units();
	}
    loadCell = (loadcell_ar[0]+loadcell_ar[1]+loadcell_ar[2]+loadcell_ar[3]+loadcell_ar[4])/5;
		
    advancedRead();  // gets full and IR light values, save to global vars lightFull, lightIR

    measuredvbat = analogRead(VBATPIN); // reading battery voltage
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage

    // RGB read - WIP

    // Manually power down the loadcell (wakes up when MCU wakes from sleep
    scale.power_down();
    tsl.disable();

    // convert sensor data into string for concatenation
    tempString =  String(temp, 2); // 2 decimal places
    humidityString =  String(humidity, 2); // same
    loadCellString =  String(loadCell, 6); // 6 decimal places
    lightIRString = String(lightIR, DEC);
    lightFullString = String(lightFull, DEC);
    vbatString = String(measuredvbat, 1); //1 decimal place
    // RGB string - not yet possible with TSL2591

    //concatenate RGB and IR strings to stringTransmit
    stringTransmit = String(IDstring + "," + RTC_timeString + "," + tempString + "," + humidityString + "," + loadCellString + "," + lightIRString + "," + lightFullString + "," + vbatString + "\0");//concates all strings into a big string
   
    // Calc len of transmit buffer:
    transmitBufLen = 1 + (char)stringTransmit.length(); // add 2 here to include last actual char

    // instantiate a transmit buffer of x len based on len of concat string above
    char transmitBuf[transmitBufLen];
    // converts long string of data into a character array to be transmitted
    stringTransmit.toCharArray(transmitBuf, transmitBufLen);
#if DEBUG == 1
    Serial.println(stringTransmit);
    Serial.println(transmitBufLen);
    Serial.println("Reading...");
    Serial.println(transmitBuf); // print to confirm transmit buff matches above string
    Serial.println("Sending to rf95_server");
#endif
    //begin sending to data to receiver (loops 3x)
    if(manager.sendtoWait((uint8_t*)transmitBuf, transmitBufLen, SERVER_ADDRESS))
      Serial.println("Ok");
    else
      Serial.println("Send Failure");

    // End big If statement from Sleep/Wake
    
    // Stuff that NEEDS to happen at the end
    // Reset alarm1 for next period
    setAlarmFunction();
    delay(75);  // delay so serial stuff has time to print out all the way
    TakeSampleFlag = false; // Clear Sample Flag
  }// endif takeSample Flag
} //endif Loop


//******************
// TSL2591 Subroutines
//******************
/**************************************************************************
    Configures the gain and integration time for the TSL2591
**************************************************************************/
void configureSensor(void)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  tsl.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
  //tsl.setGain(TSL2591_GAIN_MED);      // 25x gain
  //tsl.setGain(TSL2591_GAIN_HIGH);   // 428x gain

  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_200MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_300MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_400MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_500MS);
  //tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

#if DEBUG == 1
  /* Display the gain and integration time for reference sake */
  Serial.println("------------------------------------");
  Serial.print  ("Gain:         ");
  tsl2591Gain_t gain = tsl.getGain();
  switch (gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println("1x (Low)");
      break;
    case TSL2591_GAIN_MED:
      Serial.println("25x (Medium)");
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println("428x (High)");
      break;
    case TSL2591_GAIN_MAX:
      Serial.println("9876x (Max)");
      break;
  }
  Serial.print  ("Timing:       ");
  Serial.print((tsl.getTiming() + 1) * 100, DEC);
  Serial.println(" ms");
  Serial.println("------------------------------------");
  Serial.println("");
#endif
}
//**************************************************************************/
//    Show how to read IR and Full Spectrum at once and convert to lux
//**************************************************************************/
void advancedRead(void)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum;
  for(int i = 0; i < 5; i++) {
	lum = tsl.getFullLuminosity();
	lightIR_ar[i] = lum >> 16;
	lightFull_ar[i] = lum & 0xFFFF;
  }
  //uint16_t ir, full;
  lightIR = (lightIR_ar[0]+lightIR_ar[1]+lightIR_ar[2]+lightIR_ar[3]+lightIR_ar[4])/5;
  lightFull = (lightFull_ar[0]+lightFull_ar[1]+lightFull_ar[2]+lightFull_ar[3]+lightFull_ar[4])/5;
  //lightIR = lum >> 16;
  //lightFull = lum & 0xFFFF;
  //Serial.print("[ "); Serial.print(millis()); Serial.print(" ms ] ");
  //Serial.print("IR: "); Serial.print(ir);  Serial.print("  ");
  //Serial.print("Full: "); Serial.print(full); Serial.print("  ");
  // Serial.print("Visible: "); Serial.print(full - ir); Serial.print("  ");
  // Serial.print("Lux: "); Serial.println(tsl.calculateLux(full, ir));

}


//******************
// RTC Subroutines
//******************
void InitalizeRTC()
{
  // RTC Timer settings here
  if (! RTC.begin()) {
#if DEBUG == 1
    Serial.println("Couldn't find RTC");
#endif
    while (1);
  }
  // This may end up causing a problem in practice - what if RTC looses power in field? Shouldn't happen with coin cell batt backup
  if (RTC.lostPower()) {
#if DEBUG == 1
    Serial.println("RTC lost power, lets set the time!");
#endif
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //clear any pending alarms
  clearAlarmFunction();

  // Querry Time and print
  DateTime now = RTC.now();
#if DEBUG == 1
  Serial.print("RTC Time is: ");
  Serial.print(now.hour(), DEC); Serial.print(':'); Serial.print(now.minute(), DEC); Serial.print(':'); Serial.print(now.second(), DEC); Serial.println();
#endif
  //Set SQW pin to OFF (in my case it was set by default to 1Hz)
  //The output of the DS3231 INT pin is connected to this pin
  //It must be connected to arduino Interrupt pin for wake-up
  RTC.writeSqwPinMode(DS3231_OFF);

  //Set alarm1
  setAlarmFunction();
}

// *********
// RTC helper function
// Function to query current RTC time and add the period to set next alarm cycle
// *********
void setAlarmFunction()
{
  DateTime now = RTC.now(); // Check the current time

  // Calculate new time
  MIN = (now.minute() + WakePeriodMin) % 60; // wrap-around using modulo every 60 sec
  HR  = (now.hour() + ((now.minute() + WakePeriodMin) / 60)) % 24; // quotient of now.min+periodMin added to now.hr, wraparound every 24hrs
#if DEBUG == 1
  Serial.print("Resetting Alarm 1 for: "); Serial.print(HR); Serial.print(":"); Serial.println(MIN);
#endif

  //Set alarm1
  RTC.setAlarm(ALM1_MATCH_HOURS, MIN, HR, 0);   //set your wake-up time here
  RTC.alarmInterrupt(1, true);

}

//*********
// RTC helper function
// When exiting the sleep mode we clear the alarm
//*********
void clearAlarmFunction()
{
  //clear any pending alarms
  RTC.armAlarm(1, false);
  RTC.clearAlarm(1);
  RTC.alarmInterrupt(1, false);
  RTC.armAlarm(2, false);
  RTC.clearAlarm(2);
  RTC.alarmInterrupt(2, false);
}
//**********************
// Wakeup in SQW ISR
//********************
// Function to init PCI interrupt pin
// Pulled from: https://playground.arduino.cc/Main/PinChangeInterrupt

void pciSetup(byte pin)
{
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
  PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}


// Use one Routine to handle each group

ISR (PCINT0_vect) // handle pin change interrupt for D8 to D13 here
{
  if (digitalRead(11) == LOW)
    TakeSampleFlag = true;
}




