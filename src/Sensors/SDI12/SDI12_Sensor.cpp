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

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
SDI12Sensor::SDI12Sensor(
		const uint8_t			sdiPin,
		const char*				module_name,
		const uint8_t			num_samples 
	) 
	: Sensor(module_name, num_samples), 
	mySDI12(sdiPin) {

		// Init the SDI12 sens
		mySDI12.begin();
		delay(100);

		// Get the list of active addresses
		scanAddressSpace();
	}

//Scan the entire address space to find active devices, should take about 2 seconds cause SDI12 is slow
void SDI12Sensor::scanAddressSpace(){
	LPrintln("Scanning SDI12 Address Space this make take a little while...");

	// Scan over the characters that can be used as addresses for referencing the sensors
	for (char i = '0'; i <= '9'; i++){
		if(checkActive(i)){
			setTaken(i);
		}
	}
	LMark;
	for(char i = 'a'; i <= 'z'; i++){
		if(checkActive(i)){
			setTaken(i);
		}
	}
	LMark;
	for (char i = 'A'; i <= 'Z'; i++){
		if(checkActive(i)){
			setTaken(i);
		}
	}
	LMark;
}

// Returns a list will all active addresses at the front, that way when a 0 value is hit we can break
char* SDI12Sensor::getTaken(){
	static char addrs[62];

	// Quickly init the addrs list with some character outside the normal range
	for(int j = 0; j < 62; j++){
		addrs[j] = '-';
	}

	int count = 0;
	for (char i = '0'; i <= '9'; i++){
		if(isTaken(i)){
			addrs[count] = i;
			count++;
		}
	}
	LMark;
	for(char i = 'a'; i <= 'z'; i++){
		if(isTaken(i)){
			addrs[count] = i;
			count++;
		}
	}
	LMark;
	for (char i = 'A'; i <= 'Z'; i++){
		if(isTaken(i)){
			addrs[count] = i;
			count++;
		}
	}
	LMark;

	LPrintln("Active SDI-12 Addresses: ", addrs);
	return addrs;
}

// Checks if there is a sensor occupying that address
bool SDI12Sensor::isTaken(byte i){
	i = charToDec(i);
	byte j = i / 8; // Number of bytes
	byte k = i % 8; // Bit #
	return addressRegister[j] & (1<<k); // At the specific byte shift over however many bits

}

// Check if there is a device at the address
bool SDI12Sensor::checkActive(char i){
	// Attempt to contact the sensor 3 times
	for (int j =0; j < 3; j++){
		if(sendCommand(i, "!").length() > 0) return true;
	}

	mySDI12.clearBuffer();
	return false;
}


// Sets the bit in the proper location within the addressRegister to record that the sensor is active and taken
bool SDI12Sensor::setTaken(byte i){
	bool initStatus = isTaken(i);
	i = charToDec(i);
	byte j = i / 8; // Byte #
	byte k = i % 8; // Bit #
	addressRegister[j] |= (1 << k);
	return !initStatus;
}

// Convert ASCII characters into a smooth sequence 0-9 a-z and A-Z between 0 and 61
byte SDI12Sensor::charToDec(char i){
	// If is a number, all the offsets are relative to '0'
	if((i >= '0') && (i <= '9')) return i - '0';
	if((i >= 'a') && (i <= 'z')) return i - 'a' + 10; // Originally should be 0-26 but need to add 10 because of the initial 0-9
	if((i >= 'A') && (i <= 'Z')) return i - 'A' + 37; // Add 37 because 10 plus 26 and then the starting point needs to be non 0
	else return i;
}

// Sends a command over SDI12 to a device and returns the first message
String SDI12Sensor::sendCommand(char addr, String command){
	String fullCommand = "";
	String commandResult = "";

	fullCommand += addr;
	fullCommand += command; // [address][command]


	// Send the given command to the interface
	mySDI12.sendCommand(fullCommand);
	LMark;
	// Delay for 30ms cause SDI12 is slow af
	delay(30);

	commandResult = read_next_message();

	// Clear the serial buffer
	mySDI12.clearBuffer();

	return commandResult;

	// If no data was recieived return an empty string
	return "";
}

// Sends a command over SDI12 to a device and returns the entire buffer
String SDI12Sensor::sendCommand_allBuffer(char addr, String command){
	String fullCommand = "";
	String commandResult = "";

	fullCommand += addr;
	fullCommand += command; // [address][command]


	// Send the given command to the interface
	mySDI12.sendCommand(fullCommand);
	LMark;
	// Delay for 30ms cause SDI12 is slow af
	delay(30);

	// While there is data to be read, read it
	while (mySDI12.available()){
		char c = mySDI12.read();

		// Make sure the character is not a new line character
		if (c == '\n') {
			commandResult += "<LF>";
		}
		else if (c == '\r'){
			commandResult += "<CR>";
		}
		else{
			commandResult += c;
			delay(10); // Wait 10ms because it takes rougly 7.5ms per character
		}
	}
	LMark;

	// Clear the serial buffer
	mySDI12.clearBuffer();

	// If data was actually stored in the commandResult then return the result
	if(commandResult.length() > 1){
		return commandResult;
	}

	// If no data was recieived return an empty string
	return "";
}

/**
 * Read next message in the message queue 
 */ 
String SDI12Sensor::read_next_message(){
	String sdiResponse = "";

	while (mySDI12.available()){
		char c = mySDI12.read();

		// If we reached the end of the line break out of the loop
		if(c == '\n'){
			break;
		}

		// Add the read byte to the response
		sdiResponse += c;

		// Wait 20ms cause SDI12 is slow
		delay(20);
	}
	if(sdiResponse[sdiResponse.length()-1] == '\r'){
		sdiResponse[sdiResponse.length()-1] = '\0'; // Replace carriage return with null terminator byte
	}

	return sdiResponse;
}


///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS