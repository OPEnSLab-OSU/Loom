///////////////////////////////////////////////////////////////////////////////
///
/// @file		Decagon_5TM.cpp
/// @brief		File for Decagon5TM implementation. Incomplete.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "Decagon_5TM.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
Decagon5TM::Decagon5TM(const uint8_t sdiPin, const uint8_t num_samples)
	: SDI12Sensor(sdiPin, "5TM", num_samples)
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
Decagon5TM::Decagon5TM(JsonArrayConst p)
	: Decagon5TM((uint8_t)p[0] ) {}

///////////////////////////////////////////////////////////////////////////////
void Decagon5TM::print_config() const
{
	SDI12Sensor::print_config();
}

///////////////////////////////////////////////////////////////////////////////
void Decagon5TM::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tTemperature: ", temp);
	LPrintln("\tMoisture: ", dielec_perm);
}

///////////////////////////////////////////////////////////////////////////////
void Decagon5TM::measure()
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
void Decagon5TM::package(JsonObject json)
{
	LMark;
	JsonObject data = get_module_data_object(json, module_name);
	data["moisture"]	= dielec_perm;
	data["temperature"]	= temp;
}

/**
 * Called by the module manager when the feather needs to wake back up
 */ 
void Decagon5TM::power_up(){
	pinMode(pinAddr, OUTPUT);
}


/**
 * Called by the module manager when the feather is sleeping
 */ 
void Decagon5TM::power_down(){
	pinMode(pinAddr, INPUT);
}

///////////////////////////////////////////////////////////////////////////////
void Decagon5TM::parse_results(){
	sdiResponse.toCharArray(buf, sizeof(buf));
	p = buf;

	// Read out the results and parse out each of the data readings and pares them to floats
	strtok_r(p, "+", &p);
	dielec_perm = atof(strtok_r(NULL, "+", &p));
	temp = atof(strtok_r(NULL, "+", &p));
}
///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
