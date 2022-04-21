///////////////////////////////////////////////////////////////////////////////
///
/// @file		SDI_Manager.cpp
/// @brief		File for SDI_Manager implementation.
/// @author		Will Richards
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "SDI_Manager.h"
#include "Module_Factory.h"
#include <Arduino.h>

// SDI12 Sensors
#include "Decagon_GS3.h"
#include "Teros.h"
#include "Decagon_5TM.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
SDI_Manager::SDI_Manager(const uint8_t sdiPin)
	: SDI12Sensor(*(new SDI12(sdiPin)), "SDIManager")
	, pinAddr(sdiPin){

		// Set the SDI pin to output so we can communicate with device
		pinMode(sdiPin, OUTPUT);

		// Start the interface and wait for the sensor to initialize
		get_SDI12_interface().begin();
		delay(100);

		// Scan the address space
		scanAddressSpace();

		// List of occupied addresses
		std::vector<char> takenAddrs = getTaken();

		// If no addresses were found, inform the user and deactivate the module
		if(takenAddrs.size() <= 0){
			LPrintln("=== No SDI12 Devices Found ===");
			active = false;
		}
		else{
			LPrintln("=== SDI12 Devices Have Been Found ===");
		}

		// Loop over all active addresses, poll the sensor name and add them all to a map
		for (char addr : takenAddrs){
			sensorsInfo.insert(std::pair<char, String>(addr, get_sensor_type(addr)));
		}

		// Construct the actual sensor objects
		construct_sensors();
	}

///////////////////////////////////////////////////////////////////////////////
SDI_Manager::SDI_Manager(JsonArrayConst p)
	: SDI_Manager((uint8_t)p[0] ) {}

///////////////////////////////////////////////////////////////////////////////

void SDI_Manager::print_config() const
{
	SDI12Sensor::print_config();
}

///////////////////////////////////////////////////////////////////////////////

void SDI_Manager::print_measurements() const
{
	// For each constructed sensor call their respective package methods
	for(SDI12Sensor* sensor : sensors){
		sensor->print_measurements();
	}
	
}

///////////////////////////////////////////////////////////////////////////////

void SDI_Manager::measure()
{
	// For each constructed sensor call their respective measure
	for(SDI12Sensor* sensor : sensors){
		sensor->measure();
	}
}

///////////////////////////////////////////////////////////////////////////////

void SDI_Manager::package(JsonObject json)
{
	// For each constructed sensor call their respective package methods
	for(SDI12Sensor* sensor : sensors){
		sensor->package(json);
	}
}

///////////////////////////////////////////////////////////////////////////////

void SDI_Manager::power_up(){
	pinMode(pinAddr, OUTPUT);
}

///////////////////////////////////////////////////////////////////////////////

void SDI_Manager::power_down(){
	pinMode(pinAddr, INPUT);
}

///////////////////////////////////////////////////////////////////////////////

void SDI_Manager::construct_sensors(){

	// Loop over the collected sensors
	for (auto sensor : sensorsInfo){
		// If the sensor is a Decagon_GS3, construct a new sensor and add it to the vector contain all active SDI sensors
		if (sensor.second.indexOf("GS3") != -1){
			sensors.push_back(new DecagonGS3(get_SDI12_interface(), sensor.first, (String("GS3_") + sensor.first) ) );
		}

		// Teros 11
		else if(sensor.second.indexOf("TER11") != -1){
			sensors.push_back(new Teros(get_SDI12_interface(), sensor.first, (String("Teros11_") + sensor.first), 11) );
		}

		// Teros 12
		else if(sensor.second.indexOf("TER12") != -1){
			sensors.push_back(new Teros(get_SDI12_interface(), sensor.first, (String("Teros12_") + sensor.first), 12) );
		}

		// Decagon 5TM
		else if(sensor.second.indexOf("5TM") != -1){
			sensors.push_back(new Decagon5TM(get_SDI12_interface(), sensor.first, (String("5TM_") + sensor.first ) ) );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
