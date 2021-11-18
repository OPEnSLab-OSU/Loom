///////////////////////////////////////////////////////////////////////////////
///
/// @file		SDI12_Device.h
/// @brief		File for SDI_Device definition.
/// @author		Will Richards
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "SDI12_Sensor.h"
#include <map>
#include <vector>
#include <string>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// Overall class for handling all interaction with different types of SDI12 Devices
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_s_d_i12_sensor.html)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sdi-12-sensors)
///
///////////////////////////////////////////////////////////////////////////////
class SDI_Device : public SDI12Sensor
{

protected:

	std::vector<SDI12Sensor&> sensorArray;
	
	// Get the sensor name from the I! call
	String get_sensor_type(char addr);

	// Split a string by a specified delimeter and get the "split value" at that index
	String parse_string_by_delimeter(string str, const char* delim, int index);

	// Given a sensor name map the name to the correct sensor, create it and then push it onto the vector
	void parse_sensors(String sensorName, std::vector<SDI12Sensor> sensorArray);

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	module_name		Name of the module (provided by derived classes)
	/// @param[in]	module_type		Type of the module (provided by derived classes)
	/// @param[in]	num_samples		The number of samples to take and average
	SDI_Device(
			const uint8_t			addr = 11
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	SDI_Device(JsonArrayConst p);

	/// Destructor
	~SDI_Device() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;

	void		power_up() override;
	void		power_down() override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_measurements() const override;

private:

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, SDI_Device, "SDIDevice");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS

