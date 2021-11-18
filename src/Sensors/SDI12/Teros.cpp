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
	LMark;

	// Try to get the first address in the address buffer
	sensorAddresses = getTaken();
	
	// Loop over all the discovered addresses, and add them to the sensor 
	for (char addr : sensorAddresses){
		sensors.insert(pair<char, int>(addr, get_teros_version(addr)));
	}

	// Check if the sensor address actually changed
	if(sensors.size() > 0 ){
		Serial.print("\n=== Teros initialized successfully ===\n");  
  	} else {
    	Serial.print("\n=== Teros failed to initialize ===\n"); 
  	}
	
}

///////////////////////////////////////////////////////////////////////////////
Teros::Teros(JsonArrayConst p)
	: Teros((uint8_t)p[0], (uint8_t)p[1] ) {}

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

	for (int i = 0; i < sensors.size(); i++){
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
	for (auto i : sensors){
		// Measure the data from the sensor
		sendCommand(i.first, "M!");
		LMark;

		// Poll data from the sensor
		sdiResponse = sendCommand(i.first, "D0!");
		
		LMark;
		parse_results(i.first);
	}
	LMark;
}


///////////////////////////////////////////////////////////////////////////////
void Teros::package(JsonObject json)
{
	LMark;
	JsonObject data = get_module_data_object(json, module_name);

	// Check if we have more than 1 sensor registered 
	if(sensor_addresses.size() > 1){

		// If so for each sensor get the gathered data and add it to the JSON data
		for (int i = 0; i < sensor_addresses.size(); i++){

			// Json key for the specific sensor
			String sensorInst = String("Sensor #") + String(i);
			
			data[sensorInst]["moisture"]	= moisture[i];
			data[sensorInst]["temperature"]	= temp[i];
			if(sensors[sensor_addresses[i]] > 11)
				data[sensorInst]["conductivity"] = elec_cond[i];

		}
	}


	// If only 1 teros sensor is connected pull the first value out of the vector
	else{
		
		data["moisture"] = moisture[0];
		data["temperature"] = temp[0];
		if(sensors[sensor_addresses[0]] > 11)
			data["conductivity"] = elec_cond[0];
	}

	clear_vectors();
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
	elec_cond.clear();
}
/**
 * Get the teros version of the device at the given address
 */
int Teros::get_teros_version(char addr){

	// For right now just print out the sensor reading until I know how to parse it
	LPrintln(sendCommand(addr, "I!"));

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
void Teros::parse_results(char addr){
	sdiResponse.toCharArray(buf, sizeof(buf));
	p = buf;

	if (sensors[addr] > 11){
		// Read out the results and parse out each of the data readings and pares them to floats
		strtok_r(p, "+", &p);
		moisture.push_back(atof(strtok_r(NULL, "+", &p)));
		temp.push_back(atof(strtok_r(NULL, "+", &p)));
		elec_cond.push_back(atof(strtok_r(NULL, "+", &p)));
	}
	else{
		// Read out the results and parse out each of the data readings and pares them to floats
		strtok_r(p, "+", &p);
		moisture.push_back(atof(strtok_r(NULL, "+", &p)));
		temp.push_back(atof(strtok_r(NULL, "+", &p)));
	}
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
