///////////////////////////////////////////////////////////////////////////////
///
/// @file		Teros.h
/// @brief		File for Teros definition.
/// @author		Will Richards
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "SDI12_Sensor.h"
#include <SDI12.h>
#include <vector>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// Teros soil moisture sensor
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___decagon_g_s3.html)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sdi-12-sensors)
///
///////////////////////////////////////////////////////////////////////////////
class Teros : public SDI12Sensor
{

protected:

	uint8_t		pinAddr;		// Address of the sensor

	std::vector<float>		moisture;	// Moisture reading
	std::vector<float>		temp;		// Temperature reading
	
	String		sdiResponse = "";
	String		myCommand = "";
	char		buf[20];
	char*		p;
	std::vector<char>		sensorAddresses;

	void parse_results();
	void clear_vectors();

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Decagon GS3 module constructor
	///
	/// @param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	Teros(
			const uint8_t	pinAddr = 11,
			const uint8_t	num_samples	= 1
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Teros(JsonArrayConst p);

	/// Destructor
	~Teros() = default;

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
REGISTER(Module, Teros, "Teros");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS


