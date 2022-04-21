///////////////////////////////////////////////////////////////////////////////
///
/// @file		MB1232.h
/// @brief		File for MB1232 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "I2C_Sensor.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// MB1232 Sonar sensor module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___m_b1232.html)
/// - [Product Page: MB1232 I2CXL-MaxSonar-EZ3](https://www.maxbotix.com/Ultrasonic_Sensors/MB1232.htm)
/// - [Datasheet: I2CXL-MaxSonar- EZTM Series](https://www.maxbotix.com/documents/I2CXL-MaxSonar-EZ_Datasheet.pdf)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#mb1232-sonar)
///
///////////////////////////////////////////////////////////////////////////////
class MB1232 : public I2CSensor
{
protected:

	uint16_t	range;		///< Measure distance. Units: cm.

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address				Set(Int) | <0x70> | {0x70} | I2C address
	/// @param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	MB1232(
			const byte		i2c_address		= 0x70,
			const uint8_t	mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	MB1232(JsonArrayConst p);

	/// Destructor
	~MB1232() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_measurements() const override;

private:

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, MB1232, "MB1232");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS


