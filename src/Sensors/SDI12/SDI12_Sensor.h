///////////////////////////////////////////////////////////////////////////////
///
/// @file		SDI12_Sensor.h
/// @brief		File for SDI12Sensor definition. Incomplete.
/// @author
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "../Sensor.h"
#include <SDI12.h>

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
	SDI12 mySDI12; // SDI12 Object

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

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	module_name		Name of the module (provided by derived classes)
	/// @param[in]	module_type		Type of the module (provided by derived classes)
	/// @param[in]	num_samples		The number of samples to take and average
	SDI12Sensor(
			const uint8_t			sdiPin,
			const char*				module_name,
			const uint8_t			num_samples = 1
		);

	/// Destructor
	virtual ~SDI12Sensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================
	String sendCommand(char addr, String command);	// Returns just the next command in the buffer
	String sendCommand_allBuffer(char addr, String command);	// Returns the entire buffer
	char* getTaken();	// Returns a char array of all the taken addresses
//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

private:

};

///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS
