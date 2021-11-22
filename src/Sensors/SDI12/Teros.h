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
#include <map>

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

	char			sensorAddr;		// Address of the sensor
	const uint8_t 	terosVersion;	// Version of the Teros sensor

	float			moisture;	// Moisture reading
	float			temp;		// Temperature reading
	float			elec_cond;	// Electrical conductivity
	
	String			sdiResponse = "";
	String			myCommand = "";
	char			buf[20];
	char*			p;

	// Parse the measurement results into their own variables
	void parse_results();

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Decagon GS3 module constructor
	///
	/// @param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	Teros(
			SDI12& 			sdiInterface, 
			char 			addr, 
			String 			moduleName,
			const uint8_t 	terosVersion
		);

	/// Destructor
	~Teros() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_measurements() const override;

private:

};

///////////////////////////////////////////////////////////////////////////////
//REGISTER(Module, Teros, "Teros");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS


