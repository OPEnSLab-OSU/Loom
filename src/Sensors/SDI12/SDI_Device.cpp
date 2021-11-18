///////////////////////////////////////////////////////////////////////////////
///
/// @file		SDI_Device.cpp
/// @brief		File for SDI_Device implementation.
/// @author		Will Richards
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "SDI_Device.h"
#include "Module_Factory.h"
#include <Arduino.h>
#include <SDI12.h>

#include "Decagon_GS3.h";

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
SDI_Device::SDI_Device(const uint8_t addr)
	: SDI12Sensor(new SDI12(addr), "SDIDevice", 1), pinAddr(addr)
{
	LMark;

	get_SDI12_interface().begin();

	// Get the list of active addresses
	scanAddressSpace();

	// Loop over all active addresses, and add them to a map with the sensor address and sensor type
	for (char i : getTaken()){
		parse_sensors(get_sensor_type(i), i);
	}

	// Check if the sensor address actually changed
	if(sensorArray.size() > 0){
		Serial.print("\n=== SDI12 Devices Successfully Discovered ===\n");  
  	} else {
    	Serial.print("\n=== No SDI12 Found! ===\n"); 
  	}
	
}

///////////////////////////////////////////////////////////////////////////////
SDI_Device::SDI_Device(JsonArrayConst p)
	: SDI_Device((uint8_t)p[0]) {}

///////////////////////////////////////////////////////////////////////////////
void SDI_Device::print_config() const
{
	SDI_Device::print_config();
}

///////////////////////////////////////////////////////////////////////////////
void SDI_Device::print_measurements() const
{
	for(SDI12Sensor sensor : sensorArray){
		sensor.print_measurements();
	}
}

///////////////////////////////////////////////////////////////////////////////
void SDI_Device::measure()
{
	for(SDI12Sensor sensor : sensorArray){
		sensor.measure();
	}
}

///////////////////////////////////////////////////////////////////////////////
void SDI_Device::package(JsonObject json)
{
	for(SDI12Sensor sensor : sensorArray){
		sensor.package();
	}
}

/**
 * Called by the module manager when the feather needs to wake back up
 */ 
void SDI_Device::power_up(){
	pinMode(pinAddr, OUTPUT);
}


/**
 * Called by the module manager when the feather is sleeping
 */ 
void SDI_Device::power_down(){
	pinMode(pinAddr, INPUT);
}

/**
 * Get the sensor type at the given address
 */ 
String get_sensor_type(char addr){
	sdiResponse = sendCommand(addr, "I!");
	sdiResponse.toCharArray(buf, sizeof(buf));

	return parse_string_by_delimeter(buf, " ", 1);
}

/**
 * Parse a string given a delimeter to split at and the index we wish to get
 */ 
String SDI_Device::parse_string_by_delimeter(string str, const char* delim, int index){
    char* result = strtok_r((char*)str.c_str(), delim, &p);
    
	// If we are only asking for the 0th index simply return, if not loop replacing the previous value with null until we get to the desired index
    if(index == 0)
        return String(result);
    else{
        for(int i = 0; i < index; i++)
            result = strtok_r(NULL, delim, &p);
        return String(result);
    }
    
}

/**
 * Takes in a vector of SDI12Sensor's and given a sensor name an addr constructs the proper sensors
 */ 
void SDI_Device::parse_sensors(String sensorName, char addr){
	LPrintln("Found ", sensorName, "at address '", addr, "'");
	if(sensorName == "GS3")
		sensorArray.push_back(new DecagonGS3(get_SDI12_interface(), addr, sensorName));
	//else if(sensorName == "TERR11")
	//	sensorArray.push_back(new Teros(addr));
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
