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
DecagonGS3::DecagonGS3(SDI12& sdiInterface, char addr, String moduleName)
	: SDI12Sensor(sdiInterface, moduleName.c_str()), sensorAddr(addr)
{	
	LPrintln("\t- GS3 Initialized at address: ", addr);
}

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
	LPrintln("\tTemperature: ", temp);
	LPrintln("\tMoisture: ", dielec_perm);
	LPrintln("\tConductivity: ", elec_cond);
	
}

///////////////////////////////////////////////////////////////////////////////
void DecagonGS3::measure()
{
	
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
void DecagonGS3::package(JsonObject json)
{
	LMark;
	JsonObject data = get_module_data_object(json, module_name);

	data["moisture"] = dielec_perm;
	data["temperature"] = temp;
	data["conductivity"] = elec_cond;
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
