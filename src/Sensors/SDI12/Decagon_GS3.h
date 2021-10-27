///////////////////////////////////////////////////////////////////////////////
///
/// @file		Decagon_GS3.h
/// @brief		File for DecagonGS3 definition.
/// @author		Will Richards
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "SDI12_Sensor.h"
#include <SDI12.h>

#define DATAPIN 11

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// Decagon GS3 soil moisture sensor module. Incomplete.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___decagon_g_s3.html)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sdi-12-sensors)
///
///////////////////////////////////////////////////////////////////////////////
class DecagonGS3 : public SDI12Sensor
{

protected:
	SDI12 inst_GS3;

	String sdiResponse;
	String command;
	// address is a char as per the SDI-12 protocol
	// you'll have to cast it to int if you want to display it properly
	char address;

	char buf[20];
	char* p;

	uint8_t		pinAddr;

	float		dielec_perm = 0;	///< Measured dielectric permativity
	float		temp = 0;			///< Measured temperature
	float		elec_cond = 0;		///< Measure electrical conductivity

	String		sdiResponse = "";
	String		myCommand = "";
	char		buf[20];
	char*		p;
	char		sensorAddress = -1;

	void parse_results();


public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Decagon GS3 module constructor
	///
	/// @param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	DecagonGS3(
			const uint8_t	pinAddr = 11,
			const uint8_t	num_samples	= 1
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	DecagonGS3(JsonArrayConst p);

	/// Destructor
	~DecagonGS3() = default;

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


	// tokenize the Decagon sdiResponse into the three meaningful float data measurements
	void parse_results();

	// reads the entire available Decagon buffer (may read multiple messages)
	void read_buffer();

	// reads just the next message in the Decagon buffer, stopping at CRLF
	void read_next_message();

	// broadcast query to SDI12 sensors, set sensor_address to the first response
	void get_address();
};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, DecagonGS3, "DecagonGS3");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS
