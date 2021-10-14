///////////////////////////////////////////////////////////////////////////////
///
/// @file		SDI12_Sensor.cpp
/// @brief		File for SDI12Sensor implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "SDI12_Sensor.h"
#include <Arduino.h>

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
SDI12Sensor::SDI12Sensor(const char* module_name, const uint8_t	num_samples, int sdiPin) 
: Sensor(module_name, num_samples) : SDI12Interface(sdiPin) {}

// Basic .begin call to the SDI 12 interface
void SDI12Sensor::power_up(){
	SDI12Interface.begin();
}

//Scan over the available addresses finding ones that return some value when queried and adding them to the list of valid addresses
void SDI12Sensor::scanAddresses(){
	for (byte i = 'A'; i <= 'D'; i++){
		if(SDI12Sensor::checkActive(i))
			SDI12Sensor::setTaken(i);
	}
}

// Check if there is a device at the address
bool SDI12Sensor::checkActive(char i){

	// Send a basic 'acknowledge' command to verify that there is a device listening there format: [address][!]
	String ackCommand = "";
	ackCommand = "";
	ackCommand += i;
	ackCommand += "!";

	// Attempt to contact the sensor 3 times
	for (int j =0; j < 3; j++){

		// Send an acknowledge command and if there is any response from the sensor break out of the loop
		SDI12Interface.sendCommand(ackCommand);
		if(SDI12Interface.available()>1) break;
	}

	// If anything is returned we assume there is a sensor occupying that address
	if(SDI12Inteface.available() > 2){
		SDI12Inteface.clearBuffer();
		return true;
	}

	// If not clear the buffer and return false
	SDI12Interface.clearBuffer();
	return false;
}


// Sets the bit in the proper location within the addressRegister to record that the sensor is active and taken
bool setTaken(byte i){
	bool initStatus = isTaken(i);
	i = charToDec(i);
	byte j = i / 8; // Byte #
	byte k = i % 8; // Bit #
	addressRegister[j] |= (1 << k)
	return !initStatus;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS