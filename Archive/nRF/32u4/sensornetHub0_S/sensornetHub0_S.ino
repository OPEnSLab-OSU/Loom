/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <avr/pgmspace.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
//#include "nodeconfig.h"
#include "sleep.h"
//#include "printf.h"
#include "S_message.h"
#include "A_message.h"

//------------------------------------------------------------------------------------------------------
// DEBUG MODE: Set to 1 if you want to see serial printouts, else, set to 0 for field use to save memory
//------------------------------------------------------------------------------------------------------
#ifndef DEBUG
#define DEBUG 0
#endif

// uncoment one of these - which format do we want stuff output on serial?
#define OUTPUT_BINARY
//#define OUTPUT_READABLE

RF24 radio(9,10);
RF24Network network(radio);

// Node Address Defs
const uint16_t this_node = 00;        // Address of our node in Octal format
//const uint16_t other_node = 01;       // Address of the other node in Octal format
const uint16_t preamble = 0xFAB4;     // unique preamble message for transmitting binary data in searial.write

// Our node address
//uint16_t this_node;

// The message that we send is just an unsigned int, containing a sensor reading. 
unsigned int message;
byte num_readings; // how many readings does this transmission have?

// Sleep constants.  In this example, the watchdog timer wakes up
// every 1s, and every 4th wakeup we power up the radio and send
// a reading.  In real use, these numbers which be much higher.
// Try wdt_8s and 7 cycles for one reading per minute.> 1
const wdt_prescalar_e wdt_prescalar = wdt_250ms;
const short sleep_cycles_per_transmission = 4;

// Code to convert floating point into binary float
typedef union {
 float floatingPoint;
 byte binary[4];
} binaryFloat;

void setup(void)
{
  //
  // Print preamble
  //
#if DEBUG == 1
  Serial.begin(57600);

#ifdef OUTPUT_READABLE
  Serial.println("RF24Network/examples/sensornetHub0/");
#endif
#endif
  //
  // Pull node address out of eeprom 
  //

  // Which node are we?
//  this_node = nodeconfig_read();

  //
  // Bring up the RF network
  //

  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH); // set powerlevel to max
  radio.setDataRate(RF24_250KBPS); // set to 250kbps for better reception
  network.begin(/*channel*/ 95, /*node address*/ this_node);
}

void loop(void)
{
  // Pump the network regularly
  network.update();

  // If we are the base, is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header; // grab header
    network.peek(header); // Peek at received header without stripping message
    if ( header.type == 'S' ) // If header type 'S', then use 'S' structure
    {
      
      // Read S message struct
      S_message message;
      network.read(header,&message,sizeof(message));
#if DEBUG == 1
    #ifdef OUTPUT_READABLE
      Serial.print("Received S Type packet from node #");
      Serial.println(header.from_node);
      Serial.println("9 packets received");
      Serial.println((float)message.Yaw_reading);
      Serial.println((float)message.Pitch_reading);
      Serial.println((float)message.Roll_reading);
      Serial.println(((float)message.AX_reading)/16000);
      Serial.println(((float)message.AY_reading)/16000);
      Serial.println(((float)message.AZ_reading)/16000);
      Serial.println(((float)message.GX_reading)/16000);
      Serial.println(((float)message.GY_reading)/16000);
      Serial.println(((float)message.GZ_reading)/16000);
    #endif
    
    #ifdef OUTPUT_BINARY
      // node number (02 family is serial/i2c type), num packets
      // sensor1 H, Sensor1 L
      // sensor2 H, Sensor1 2
      // etc
 
      Serial.write((uint8_t)(header.from_node)); Serial.write(9);
      // packet data
      binaryFloat hi;
      hi.floatingPoint = message.Yaw_reading; Serial.write(hi.binary,4);
      hi.floatingPoint = message.Pitch_reading; Serial.write(hi.binary,4);
      hi.floatingPoint = message.Roll_reading; Serial.write(hi.binary,4);
      hi.floatingPoint = message.AX_reading; Serial.write(hi.binary,4);
      hi.floatingPoint = message.AY_reading; Serial.write(hi.binary,4);
      hi.floatingPoint = message.AZ_reading; Serial.write(hi.binary,4);
      hi.floatingPoint = message.GX_reading; Serial.write(hi.binary,4);
      hi.floatingPoint = message.GY_reading; Serial.write(hi.binary,4);
      hi.floatingPoint = message.GZ_reading; Serial.write(hi.binary,4);
  // ********** Get All Readings right!     ********
     
    #endif
#endif
      // Consider Printing 'end transmission' code here to indicate software finished reading transmission
    }
    if ( header.type == 'A' ) // If header type 'A', then use 'A' structure
    {
      
      // Read A message struct
      A_message message;
      network.read(header,&message,sizeof(message));
      num_readings = message.num_readings; // how many sensor readings does this transmission have?
	  
#if DEBUG == 1
   #ifdef OUTPUT_READABLE  // output analog sensor ASCII 
      Serial.print("Received A Type packet from node #");
      Serial.println(header.from_node); 
      switch(num_readings)
      { // needs to be in backwards sequence for "fall-through" effect of not having breaks in switch/case
        case 8:
          Serial.print("A7: ");
          Serial.println(message.A7_reading);
        case 7:
          Serial.print("A6: ");
          Serial.println(message.A6_reading);
        case 6:
          Serial.print("A5: ");
          Serial.println(message.A5_reading);
        case 5:
          Serial.print("A4: ");
          Serial.println(message.A4_reading);
        case 4:
          Serial.print("A3: ");
          Serial.println(message.A3_reading);
        case 3:
          Serial.print("A2: ");
          Serial.println(message.A2_reading);
        case 2:
          Serial.print("A1: ");
          Serial.println(message.A1_reading);
        case 1:
          Serial.print("A0: ");
          Serial.println(message.A0_reading);
      }
    #endif
 
 #ifdef OUTPUT_BINARY  // output analog sensor Binary
      Serial.write((uint8_t)(header.from_node)); Serial.write(num_readings);
      switch(num_readings)
      { // needs to be in backwards sequence for "fall-through" effect of not having breaks in switch/case
        case 8:
          Serial.write((uint8_t)(message.A7_reading >> 8)); Serial.write((uint8_t)(message.A7_reading & 0xFF));
        case 7:
          Serial.write((uint8_t)(message.A6_reading >> 8)); Serial.write((uint8_t)(message.A6_reading & 0xFF));
        case 6:
          Serial.write((uint8_t)(message.A5_reading >> 8)); Serial.write((uint8_t)(message.A5_reading & 0xFF));
        case 5:
          Serial.write((uint8_t)(message.A4_reading >> 8)); Serial.write((uint8_t)(message.A4_reading & 0xFF));
        case 4:
          Serial.write((uint8_t)(message.A3_reading >> 8)); Serial.write((uint8_t)(message.A3_reading & 0xFF));
        case 3:
          Serial.write((uint8_t)(message.A2_reading >> 8)); Serial.write((uint8_t)(message.A2_reading & 0xFF));
        case 2:
          Serial.write((uint8_t)(message.A1_reading >> 8)); Serial.write((uint8_t)(message.A1_reading & 0xFF));
        case 1:
          Serial.write((uint8_t)(message.A0_reading >> 8)); Serial.write((uint8_t)(message.A0_reading & 0xFF));
      }
    #endif    
    //Serial.println("STOP");// Consider Printing 'end transmission' code here to indicate software finished reading transmission
#endif
    }
    
  }

}
// vim:ai:cin:sts=2 sw=2 ft=cpp
