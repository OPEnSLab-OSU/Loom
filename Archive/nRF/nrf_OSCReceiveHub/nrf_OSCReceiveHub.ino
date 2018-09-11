/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 Update 2014 - TMRh20
 */

/**
 * Simplest possible example of using RF24Network,
 *
 * RECEIVER NODE
 * Listens for messages from the transmitter and prints them out.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <OSCBundle.h>
#include <Ethernet2.h>

#define NUM_FIELDS 16
#define DEBUG 1

RF24 radio(5,6);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 01;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node = 00;   // Address of the other node in Octal format

//Ethernet / Hub Info
// byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// IPAddress ip(192, 168, 1, 177);

//Use this for OPEnS Lab
byte mac[] = {0x98, 0x76, 0xB6, 0x10, 0x61, 0xD6};  
IPAddress ip(128,193,56,138);

const char DEVID[] = "v25CCAAB0F709665"; 
char serverName[] = "api.pushingbox.com";

EthernetClient client;
String data[NUM_FIELDS];

char message[300];

void setup(void)
{
  Serial.begin(9600);
	while(!Serial);
	pinMode(8, INPUT_PULLUP);
  Serial.println("RF24Network/examples/helloworld_rx/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
	
		
	if(!setup_ethernet()) {
		#if DEBUG == 1
			Serial.println("Failed to setup ethernet");
		#endif
	}
}

void loop(void){
  
  network.update();                  // Check the network regularly

  
  while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    memset(message, '\0', 300);
    network.read(header,&message,299);

    OSCBundle bndl;
		get_OSC_bundle(message, &bndl);
		for(int i = 0; i < NUM_FIELDS; i++) {
			data[i] = get_data_value(bndl.getOSCMessage(0), i);
			#if DEBUG == 1
				Serial.print("Data ");
				Serial.print(i);
				Serial.print(": ");
				Serial.println(data[i]);
			#endif
		}
		sendToPushingBox();
		
  }
}

bool setup_ethernet() {
	bool is_setup;
	if (Ethernet.begin(mac) == 0) {
		#if DEBUG == 1
			Serial.println("Failed to configure Ethernet using DHCP");
		#endif
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
	
	if(client.connect("www.google.com", 80)) {
		is_setup = true;
		#if DEBUG == 1
			Serial.println("Successfully connected to internet");
		#endif
		client.stop();
	}
	else {
		is_setup = false;
		#if DEBUG == 1
			Serial.println("Failed to connect to internet");
		#endif
	}
	
	return is_setup;
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
		 #if DEBUG == 1
			Serial.println("Failed to connect to PB, attempting to re-setup ethernet.");
		#endif
		if(setup_ethernet()) {
			#if DEBUG == 1
				Serial.println("Successfully re-setup ethernet.");
			#endif
		}
		#if DEBUG == 1 
			else {
				Serial.println("Failed to re-setup ethernet.");
			}
		#endif
  }
}

