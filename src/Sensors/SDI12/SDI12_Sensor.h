///////////////////////////////////////////////////////////////////////////////
///
/// @file		SDI12_Sensor.h
/// @brief		File for SDI12Sensor definition. Incomplete.
/// @author		Will Richards
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "../Sensor.h"
#include <SDI12.h>
#include <vector>
#include <string.h>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// Abstract base class for SDI12 sensor modules.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_s_d_i12_sensor.html)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sdi-12-sensors)
///
///////////////////////////////////////////////////////////////////////////////
class SDI12Sensor : public Sensor
{

protected:
	SDI12 sdiInterface; // SDI12 Object

	// Register representing each sensors address connected to the sdi device
	byte addressRegister[8] = {
		0B00000000,
		0B00000000,
		0B00000000,
		0B00000000,
		0B00000000,
		0B00000000,
		0B00000000,
		0B00000000
	};

	// Check if there is a activity at that set address
	bool checkActive(char i);

	// Scan the address space setting active addresses as taken
	void scanAddressSpace();

	// Checks if there is an active sensor on the address
	bool isTaken(byte i);

	// Sets the specified address to taken, returns false if already taken
	bool setTaken(byte i);

	//Convert the ascii characters to numbers between 0 and 61 inclusive to represent the 62 possible address
	byte charToDec(char i);

	String read_next_message(); // Read the next message out of the buffer

	SDI12& get_SDI12_interface(); // Return a reference to the SDI12 class we are using to communicate

	// Poll the sensor at the given address for info (I!) and then parse out the sensor name
	String get_sensor_type(char addr);

	// Given a string split by delimeter and return the requested index
	String parse_string_by_delimeter(String str, const char* delim, int index);

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	module_name		Name of the module (provided by derived classes)
	/// @param[in]	module_type		Type of the module (provided by derived classes)
	/// @param[in]	num_samples		The number of samples to take and average
	SDI12Sensor(
			SDI12&					sdiReference,
			const char*				module_name
		);

	/// Destructor
	virtual ~SDI12Sensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================
	String sendCommand(char addr, String command);	// Returns just the next command in the buffer
	String sendCommand_allBuffer(char addr, String command);	// Returns the entire buffer
	std::vector<char> getTaken();	// Returns a char array of all the taken addresses 
//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

private:

};

///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS

