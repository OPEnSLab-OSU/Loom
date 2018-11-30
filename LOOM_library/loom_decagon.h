// ================================================================ 
// ===                ACCESSSING SENSOR STRUCTS                 === 
// ================================================================

// Will be update to use state struct soon

// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include "SDI12.h"
#include <string.h>


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define DATAPIN 11         // change to the proper pin
#define SENSOR_ADDRESS "?" //"?" broadcasts message


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
SDI12 mySDI12(DATAPIN);

String sdiResponse = "";
String myCommand = "";
char   buf[20];
char   *p;
float  dielec_p = 0, temp = 0, elec_c = 0;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_decagon();
void measure_decagon();
void package_decagon(OSCBundle * bndl, char packet_header_string[]);


// ================================================================
// ===                          SETUP                           ===
// ================================================================
//
// Runs any Decagon setup
//
void setup_decagon() 
{
	mySDI12.begin();
	delay(2000);
	//first command to take a measurement
	myCommand = String(SENSOR_ADDRESS) + "I!";
	LOOM_DEBUG_Println2("Decagon: ", myCommand);     // echo command to terminal

	mySDI12.sendCommand(myCommand);
	delay(30);                     // wait a while for a response

	while (mySDI12.available()) {  // build response string
		char c = mySDI12.read();
		if ((c != '\n') && (c != '\r')) {
			sdiResponse += c;
			delay(5);
		}
	}
	LOOM_DEBUG_Println(sdiResponse); //write the response to the screen
	mySDI12.clearBuffer();

	delay(1000);                 // delay between taking reading and requesting data
	sdiResponse = "";            // clear the response string
}



// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- MEASURE DECAGON ---
//
// Gets Decagon sensor readings
//
void measure_decagon() 
{
	// First command to take a measurement
	myCommand = String(SENSOR_ADDRESS) + "M!";
	//Serial.println(myCommand);     // echo command to terminal
	
	mySDI12.sendCommand(myCommand);
	delay(30);                     // wait a while for a response
	
	while (mySDI12.available()) {  // build response string
		char c = mySDI12.read();
		if ((c != '\n') && (c != '\r')) {
			sdiResponse += c;
			delay(5);
		}
	}
	//if (sdiResponse.length() > 1) Serial.println(sdiResponse); //write the response to the screen
	mySDI12.clearBuffer();
	
	delay(1000);                 // delay between taking reading and requesting data
	sdiResponse = "";            // clear the response string
	
	// next command to request data from last measurement
	myCommand = String(SENSOR_ADDRESS) + "D0!";
	//Serial.println(myCommand);  // echo command to terminal
	
	mySDI12.sendCommand(myCommand);
	delay(30);                     // wait a while for a response
	
	while (mySDI12.available()) {  // build string from response
		char c = mySDI12.read();
		if ((c != '\n') && (c != '\r')) {
			sdiResponse += c;
			delay(5);
		}
	}
	
	sdiResponse.toCharArray(buf, sizeof(buf));
	
	p = buf;
	
	strtok_r(p, "+", &p);
	dielec_p = atof(strtok_r(NULL, "+", &p));
	temp     = atof(strtok_r(NULL, "+", &p));
	elec_c   = atof(strtok_r(NULL, "+", &p));
	
	
	//if (sdiResponse.length() > 1) Serial.println(sdiResponse); //write the response to the screen
	/*Serial.print("Dielectric Permittivity: ");
	Serial.println(dielec_p);
	Serial.print("Temperature (C): ");
	Serial.println(temp);
	Serial.print("Electric Conductivity: ");
	Serial.println(elec_c);*/
	mySDI12.clearBuffer();
}



// --- PACKAGE DECAGON ---
//
// Populates OSC bundle with last Decagon readings
//
// @param bndl                  The OSC bundle to be populated
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//
void package_decagon(OSCBundle * bndl, char packet_header_string[]) 
{
	char addressString[255];
	sprintf(addressString, "%s%s", packet_header_string, "/DielecPerm");
	bndl->add(addressString).add(dielec_p);
	sprintf(addressString, "%s%s", packet_header_string, "/Temp");
	bndl->add(addressString).add(temp);
	sprintf(addressString, "%s%s", packet_header_string, "/ElecCond");
	bndl->add(addressString).add(elec_c);
}




