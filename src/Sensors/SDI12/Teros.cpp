///////////////////////////////////////////////////////////////////////////////
///
/// @file		Decagon_GS3.cpp
/// @brief		File for Teros implementation.
/// @author		Will Richards
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "Teros.h"
#include "Module_Factory.h"
#include <Arduino.h>

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
Teros::Teros(const uint8_t addr, const uint8_t num_samples)
	: SDI12Sensor(addr, "Teros", num_samples), pinAddr(addr)
{
	LMark;

	// Try to get the first address in the address buffer
	sensorAddresses = getTaken();


	// Check if the sensor address actually changed
	if(sensorAddresses.size() > 0 ){
		Serial.print("\n=== Decagon initialized successfully ===\n");  
  	} else {
    	Serial.print("\n=== Decagon failed to initialize ===\n"); 
  	}
	
}

///////////////////////////////////////////////////////////////////////////////
Teros::Teros(JsonArrayConst p)
	: Teros(EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Teros::print_config() const
{
	SDI12Sensor::print_config();
}

///////////////////////////////////////////////////////////////////////////////
void Teros::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");

	for (int i = 0; i < sensorAddresses.size(); i++){
		LPrintln("Device #", i);
		LPrintln("\tTemperature: ", temp[i]);
		LPrintln("\tMoisture: ", moisture[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
void Teros::measure()
{
	LMark;
	
	// Loop over the SDI addresses and pull measurement data from each sensor
	for (char i : sensorAddresses){
		// Measure the data from the sensor
		sendCommand(i, "M!");
		LMark;

		// Poll data from the sensor
		sdiResponse = sendCommand(i, "D0!");

		LMark;
		parse_results();
	}
	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Teros::package(JsonObject json)
{
	LMark;
	JsonObject data = get_module_data_object(json, module_name);

	if(sensorAddresses.size() > 1){
		for (int i = 0; i < sensorAddresses.size(); i++){
			String sensorInst = String("Sensor #") + String(i);
			data[sensorInst]["moisture"]	= moisture[i];
			data[sensorInst]["temperature"]	= temp[i];
		}
	}
	else{
		data["moisture"] = moisture[0];
		data["temperature"] = temp[0];
	}
}

/**
 * Called by the module manager when the feather needs to wake back up
 */ 
void Teros::power_up(){
	pinMode(pinAddr, OUTPUT);
}


/**
 * Called by the module manager when the feather is sleeping
 */ 
void Teros::power_down(){
	pinMode(pinAddr, INPUT);
}

/**
 * Clear all the sensor vectors
 */ 
void Teros::clear_vectors(){
	moisture.clear();
	temp.clear();
}

///////////////////////////////////////////////////////////////////////////////
void Teros::parse_results(){
	sdiResponse.toCharArray(buf, sizeof(buf));
	p = buf;

	// Read out the results and parse out each of the data readings and pares them to floats
	strtok_r(p, "+", &p);
	moisture.push_back(atof(strtok_r(NULL, "+", &p)));
	temp.push_back(atof(strtok_r(NULL, "+", &p)));
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
