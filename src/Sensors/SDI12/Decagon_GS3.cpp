///////////////////////////////////////////////////////////////////////////////
///
/// @file		Decagon_GS3.cpp
/// @brief		File for DecagonGS3 implementation.
/// @author		Will Richards
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "Decagon_GS3.h"
#include "Module_Factory.h"
#include <Arduino.h>

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
DecagonGS3::DecagonGS3(const uint8_t addr, const uint8_t num_samples)
	: SDI12Sensor(addr, "GS3", num_samples), pinAddr(addr)
{
	LMark;

	// Try to get the first address in the address buffer
	sensorAddress = getTaken()[0];


	// Check if the sensor address actually changed
	if(sensorAddress != -1){
		Serial.print("\n=== Decagon initialized successfully ===\n");  
  	} else {
    	Serial.print("\n=== Decagon failed to initialize ===\n"); 
  	}
	
}

///////////////////////////////////////////////////////////////////////////////
DecagonGS3::DecagonGS3(JsonArrayConst p)
	: DecagonGS3(EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void DecagonGS3::print_config() const
{
	SDI12Sensor::print_config();
}

///////////////////////////////////////////////////////////////////////////////
void DecagonGS3::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tConductivity: ", elec_cond);
	LPrintln("\tTemperature: ", temp);
	LPrintln("\tMoisture: ", dielec_perm);
}

///////////////////////////////////////////////////////////////////////////////
void DecagonGS3::measure()
{
	LMark;
	// Measure the data from the sensor
	sendCommand(sensorAddress, "M!");
	LMark;

	// Poll data from the sensor
	sdiResponse = sendCommand(sensorAddress, "D0!");

	LMark;
	parse_results();
	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void DecagonGS3::package(JsonObject json)
{
	LMark;
	JsonObject data = get_module_data_object(json, module_name);
	data["moisture"]	= dielec_perm;
	data["temperature"]	= temp;
	data["conductivity"]	= elec_cond;
}

/**
 * Called by the module manager when the feather needs to wake back up
 */ 
void DecagonGS3::power_up(){
	pinMode(pinAddr, OUTPUT);
}


/**
 * Called by the module manager when the feather is sleeping
 */ 
void DecagonGS3::power_down(){
	pinMode(pinAddr, INPUT);
}

///////////////////////////////////////////////////////////////////////////////
void DecagonGS3::parse_results(){
	sdiResponse.toCharArray(buf, sizeof(buf));
	p = buf;

	// Read out the results and parse out each of the data readings and pares them to floats
	strtok_r(p, "+", &p);
	dielec_perm = atof(strtok_r(NULL, "+", &p));
	temp = atof(strtok_r(NULL, "+", &p));
	elec_cond = atof(strtok_r(NULL, "+", &p));
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
