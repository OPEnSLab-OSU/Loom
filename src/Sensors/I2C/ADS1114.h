///////////////////////////////////////////////////////////////////////////////
///
/// @file		ADS1114.h
/// @brief		File for ADS1114 definition.
/// @author		Catherine Hu and Carter Peene
/// @date		2021
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "I2C_Sensor.h"

// #include <Adafruit_ADS1015.h>
#include <OPEnS_ADS1114.h>


namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// ADS1114 External ADC sensor module.
///
/// @par Resources
/// - [Module Documentation](TODO)
/// - [Product Page: ADS1114 16-Bit ADC](https://www.adafruit.com/product/1085)
/// - [Dependency: Adafruit ADS1X15 Arduino Library](https://github.com/adafruit/Adafruit_ADS1X15)
/// - [Hookup Guide: ADS1114 Assembly and Wiring](https://learn.adafruit.com/adafruit-4-channel-adc-breakouts/assembly-and-wiring)
/// - [Datasheet: ADS1114](https://cdn-shop.adafruit.com/datasheets/ads1114.pdf)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#ads1114-16-bit-adc)
///
///////////////////////////////////////////////////////////////////////////////
class ADS1114 : public I2CSensor
{
protected :
// Protected members here (are inherited)

///< Underlying ADS1114 sensor manager instance
		OPEnS_ADS1114 ads1114;
	const bool			analog_enabled[2];
	const bool			diff_enabled;
	// See: https://github.com/adafruit/Adafruit_ADS1X15/pull/9
	uint16_t				analog_reads[2];
	int16_t				diff_read;

public:
	enum class Gain : uint32_t {
		GAIN_TWOTHIRDS    = ADS1015_REG_CONFIG_PGA_6_144V,
		GAIN_ONE          = ADS1015_REG_CONFIG_PGA_4_096V,
		GAIN_TWO          = ADS1015_REG_CONFIG_PGA_2_048V,
		GAIN_FOUR         = ADS1015_REG_CONFIG_PGA_1_024V,
		GAIN_EIGHT        = ADS1015_REG_CONFIG_PGA_0_512V,
		GAIN_SIXTEEN      = ADS1015_REG_CONFIG_PGA_0_256V
	};

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address				Set(Int) | <0x48> | {0x10, 0x11} | I2C address
	/// @param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	/// @param[in]	analog_0_enabled		Bool | <true> | {true, false} | Enable A0 for reading
	/// @param[in]	analog_1_enabled		Bool | <true> | {true, false} | Enable A1 for reading
	/// @param[in]	analog_2_enabled		Bool | <true> | {true, false} | Enable A2 for reading
	/// @param[in]	analog_3_enabled		Bool | <true> | {true, false} | Enable A3 for reading
	/// @param[in]	diff_enabled			Bool | <false> | {true, false} | Enable a differential measurement between A0 and A1
	/// @param[in]	Gain				 	Set(Gain) | <0> | {0x0000("GAIN_TWOTHIRDS"), 0x0200("GAIN_ONE"), 0x0400("GAIN_TWO" ), 0x0600("GAIN_FOUR"), 0x0800("GAIN_EIGHT"), 0x0A00("GAIN_SIXTEEN")} | The gain setting for the ADS1114. Please do not change this number unless you are familiar with the ADS1114, as an incorrect setting will destroy the ADC!.
	ADS1114(
		const	byte		i2c_address			= ADS1015_ADDRESS,
		const	uint8_t		mux_port			= 255,
		const	bool		analog_0_enabled	= true,
		const	bool		analog_1_enabled	= true,
		const	bool		diff_enabled		= false,
		const	Gain		gain				= Gain::GAIN_TWOTHIRDS
	);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	ADS1114(JsonArrayConst p);

	/// Destructor
	virtual ~ADS1114() = default;

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

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================


private:

	// Private members (are not inherited)

	// Auxiliary functions

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, ADS1114, "ADS1114");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS
