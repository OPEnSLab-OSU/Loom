///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_MB1232.h
/// @brief		File for Loom_MB1232 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "I2C_Sensor.h"


///////////////////////////////////////////////////////////////////////////////
///
/// MB1232 Sonar sensor module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___m_b1232.html)
/// - [Product Page: MB1232 I2CXL-MaxSonar-EZ3](https://www.maxbotix.com/Ultrasonic_Sensors/MB1232.htm)
/// - [Datasheet: I2CXL-MaxSonar- EZTM Series](https://www.maxbotix.com/documents/I2CXL-MaxSonar-EZ_Datasheet.pdf)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_MB1232 : public LoomI2CSensor
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
	Loom_MB1232(
LoomManager* manager,
const byte i2c_address		= 0x70,
			const uint8_t		mux_port		= 255
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_MB1232(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_MB1232() = default;

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



