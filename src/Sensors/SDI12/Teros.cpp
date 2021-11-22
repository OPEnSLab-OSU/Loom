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
Teros::Teros(SDI12& sdiInterface, char addr, String moduleName, const uint8_t terosVersion)
	: SDI12Sensor(sdiInterface, moduleName.c_str())
	, sensorAddr(addr)
	, terosVersion(terosVersion) {

		LPrintln("\t- Teros version: ", terosVersion, " Initialized at address: ", addr);
	}

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
	LPrintln("\tTemperature: ", temp);
	LPrintln("\tMoisture: ", moisture);
	if(terosVersion > 11)
		LPrintln("\tConductivity: ", elec_cond);
}

///////////////////////////////////////////////////////////////////////////////
void Teros::measure()
{
	LMark;
	
	
	// Measure the data from the sensor
	sendCommand(sensorAddr, "M!");
	LMark;

	// Poll data from the sensor
	sdiResponse = sendCommand(sensorAddr, "D0!");
	
	LMark;
	parse_results();
	
	LMark;
}


///////////////////////////////////////////////////////////////////////////////
void Teros::package(JsonObject json)
{
	LMark;
	JsonObject data = get_module_data_object(json, module_name);

	data["moisture"]			= moisture;
	data["temperature"]			= temp;
	if(terosVersion > 11)
		data["conductivity"] 	= elec_cond;
	
}

///////////////////////////////////////////////////////////////////////////////
void Teros::parse_results(){
	sdiResponse.toCharArray(buf, sizeof(buf));
	p = buf;

	// Read out the results and parse out each of the data readings and pares them to floats
	strtok_r(p, "+", &p);
	moisture = atof(strtok_r(NULL, "+", &p));
	temp = atof(strtok_r(NULL, "+", &p));
	if (terosVersion > 11)
		elec_cond = atof(strtok_r(NULL, "+", &p));
	
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
