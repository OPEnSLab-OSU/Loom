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
#include <string.h>
#include <vector>

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
SDI12Sensor::SDI12Sensor(const char* module_name, const uint8_t	num_samples, int sdiPin) 
: Sensor(module_name, num_samples), SDI12Interface(sdiPin) {
	SDI12Interface.begin();
	delay(500); // Wait for things to settle

	// Spray the entire list of addresses to find which ones are active
	scanAddressSpace();
}

// Basic .begin call to the SDI 12 interface
void SDI12Sensor::power_up(){
	
}

//Scan the entire address space to find active devices, should take about 2 seconds cause SDI12 is slow
void SDI12Sensor::scanAddressSpace(){
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
		addrs[j] = '-'
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
		if(sendCommand(i, "!").length > 1) return true;
	}

	SDI12Interface.clearBuffer();
	return false;
}


// Sets the bit in the proper location within the addressRegister to record that the sensor is active and taken
bool SDI12Sensor::setTaken(byte i){
	bool initStatus = isTaken(i);
	i = charToDec(i);
	byte j = i / 8; // Byte #
	byte k = i % 8; // Bit #
	addressRegister[j] |= (1 << k)
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

// Measures and then Reads the most recent measurements from the sensor
void SDI12Sensor::measure(){
	String response = "";
	readData = "";
	char* activeAddrs = getTaken();
	for(int i = 0; i < sizeof(activeAddrs)/sizeof(activeAddrs[0]); i++){
		// Send the measure command to the specified address as long as it is a valid address
		if(activeAddrs[i] != '-')
		{
			// Find out the wait time for the sensor
			response = sendCommand(activeAddrs[i], "M!");

			// How long we need to wait for data in seconds
			unsigned int waitTime = 0;
			waitTime = response.substring(1,4).toInt();

			// When we got the data
			unsigned long timerStart = millis();

			// Hold until the time is complete or the sensor interrupts us
			while((millis() - timerStart) < (1000*wait)){
				if(SDI12Interface.available()){
					SDI12Interface.clearBuffer();
					break;
				}
			}

			// Clear buffer before sending data request
			SDI12Interface.clearBuffer();

			// Requests the data from the last measurement
			response = sendCommand(activeAddr[i], "D0!");

			// If the length of the returned data was larger than a given value
			if(response.length() > 1){

				// [SDI12 Address]:[Sensor Data],...
				readData += activeAddr[i] +":" + response + ",";
			}
		}
		else{
			break;
		}
		
	}

}

// Sends a command over SDI12 to a device and returns the result
String SDI12Sensor::sendCommand(char addr, String command){
	String fullCommand = "";
	String commandResult = "";

	fullCommand += addr;
	fullCommand += command; // [address][command]


	// Send the given command to the interface
	SDI12Interface.sendCommand(fullCommand);
	LMark;
	// Delay for 30ms cause SDI12 is slow af
	delay(30);

	// While there is data to be read, read it
	while (SDI12Interface.available()){
		char c = SDI12Interface.read();

		// Make sure the character is not a new line character
		if ((c != '\n') && (c != '\r')){
			commandResult += c;
			delay(10); // Wait 10ms because it takes rougly 7.5ms per character
		}
	}
	LMark;

	// Clear the serial buffer
	SDI12Interface.clearBuffer();

	// If data was actually stored in the commandResult then return the result
	if(commandResult.length > 1){
		return commandResult;
	}

	// If no data was recieived return an empty string
	return "";
}

void K30::print_measurements() const {

	// Print the name of the module
	print_module_label();
    vector<string> out;
    tokenize(readData, ',', out);


	// Print out the sensor and address combo
	for (size_t = 0; i < out.size(); i++){
		LPrintLn(out[i]);
	}
}

void K30::package(JsonObject json){
	LMark;
	JsonObject data = get_module_data_object(json, module_name);

	// Comma seperated segments
	vector<string> sensors;
    tokenize(readData, ',', sensors);

	// Colon seperated address and sensor reading
	for (size_t = 0; i < out.size(); i++){
		vector<string> splitSensor;
		tokenize(out[i], ':', splitSensor);


		// JSON Output
		data[splitSensor[0]] = splitSensor[1];
	}

}

// Splits a string at a delimiter and returns a vector
void tokenize(std::string const &str, const char delim,
            std::vector<std::string> &out)
{
    size_t start;
    size_t end = 0;
 
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS