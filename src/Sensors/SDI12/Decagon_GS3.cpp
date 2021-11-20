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

	// Only add the sensor addresses that are actually GS3 sensors
	for (char i : getTaken())
	{
		if(get_sensor_type(i) == "GS3"){
			sensorAddresses.push_back(i);
		}
	}

	// Check if the sensor address actually changed
	if(sensorAddresses.size() > 0){
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

	for (int i = 0; i < sensorAddresses.size(); i++){
		LPrintln("Device #", i);
		LPrintln("\tTemperature: ", temp[i]);
		LPrintln("\tMoisture: ", dielec_perm[i]);
		LPrintln("\tConductivity: ", elec_cond[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
void DecagonGS3::measure()
{
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
void DecagonGS3::package(JsonObject json)
{
	
	LMark;
	JsonObject data = get_module_data_object(json, module_name);

	// Only use multiple sensor format if there is actually more than 1 sensor
	if(sensorAddresses.size() > 1){
		for (int i = 0; i < sensorAddresses.size(); i++){
			String sensorInst = String("Sensor #") + String(i);
			data[sensorInst]["moisture"]	= dielec_perm[i];
			data[sensorInst]["temperature"]	= temp[i];
			data[sensorInst]["conductivity"]= elec_cond[i];
		}
	}
	else{
		data["moisture"] = dielec_perm[0];
		data["temperature"] = temp[0];
		data["conductivity"] = elec_cond[0];
	}

	clear_vectors();
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

/**
 * Clear all the sensor vectors
 */ 
void DecagonGS3::clear_vectors(){
	dielec_perm.clear();
	temp.clear();
	elec_cond.clear();
}

///////////////////////////////////////////////////////////////////////////////
void DecagonGS3::parse_results(){
	sdiResponse.toCharArray(buf, sizeof(buf));
	p = buf;

	// Read out the results and parse out each of the data readings and pares them to floats
	strtok_r(p, "+", &p);
	dielec_perm.push_back(atof(strtok_r(NULL, "+", &p)));
	temp.push_back(atof(strtok_r(NULL, "+", &p)));
	elec_cond.push_back(atof(strtok_r(NULL, "+", &p)));
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
