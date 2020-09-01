///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_AS7262.h
/// @brief		File for Loom_ADS1115 definition.
/// @author		Noah Koontz
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "I2C_Sensor.h"
#include <Adafruit_ADS1015.h>

///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
///
/// ADS1115 External ADC sensor module.
///
/// @par Resources
/// - [Module Documentation](TODO)
/// - [Product Page: ADS1115 16-Bit ADC](https://www.adafruit.com/product/1085)
/// - [Dependency: Adafruit ADS1X15 Arduino Library](https://github.com/adafruit/Adafruit_ADS1X15)
/// - [Hookup Guide: ADS1115 Assembly and Wiring](https://learn.adafruit.com/adafruit-4-channel-adc-breakouts/assembly-and-wiring)
/// - [Datasheet: ADS1115](https://cdn-shop.adafruit.com/datasheets/ads1115.pdf)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#ads1115-16-bit-adc)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_ADS1115 : public LoomI2CSensor
{
protected:
	///< Underlying ADS1115 sensor manager instance

	// Protected members here (are inherited)
	Adafruit_ADS1115 	ads1115;
	const bool			analog_enabled[4];
	const bool			diff_enabled[2];
	// NOTE: There is an error in the adafruit driver, as the single-ended inputs return unsigned numbers
	// all outputs in the ADS1115 are signed, so we use a signed number here.
	// See: https://github.com/adafruit/Adafruit_ADS1X15/pull/9
	int16_t				analog_reads[4];
	int16_t				diff_reads[2];

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
	/// @param[in]	i2c_address				Set(Int) | <0x10> | {0x10, 0x11} | I2C address
	/// @param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	/// @param[in]	analog_0_enabled		Bool | <true> | {true, false} | Enable A0 for reading
	/// @param[in]	analog_1_enabled		Bool | <true> | {true, false} | Enable A1 for reading
	/// @param[in]	analog_2_enabled		Bool | <true> | {true, false} | Enable A2 for reading
	/// @param[in]	analog_3_enabled		Bool | <true> | {true, false} | Enable A3 for reading
	/// @param[in]	diff_0_enabled			Bool | <false> | {true, false} | Enable a differential measurement between A0 and A1
	/// @param[in]	diff_1_enabled			Bool | <false> | {true, false} | Enable a differential measurement between A2 and A3
	/// @param[in]	Fain					Set(Gain) | <0> | {0x0000("GAIN_TWOTHIRDS"), 0x0200("GAIN_ONE"), 0x0400("GAIN_TWO" ), 0x0600("GAIN_FOUR"), 0x0800("GAIN_EIGHT"), 0x0A00("GAIN_SIXTEEN")} | The gain setting for the ADS1115. Please do not change this number unless you are familiar with the ADS1115, as an incorrect setting will destroy the ADC!.
	Loom_ADS1115(
		LoomManager* manager,
		const 	byte i2c_address = ADS1015_ADDRESS, 
		const	uint8_t 		mux_port = 255,
		const 	bool 			analog_0_enabled = true,
		const 	bool 			analog_1_enabled = true,
		const 	bool 			analog_2_enabled = true,
		const 	bool 			analog_3_enabled = true,
		const 	bool 			diff_0_enabled = false,
		const 	bool 			diff_1_enabled = false,
		const	Gain			gain = Gain::GAIN_TWOTHIRDS 
	);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_ADS1115(LoomManager* manager, JsonArrayConst p);
	
	/// Destructor
	virtual ~Loom_ADS1115() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;

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




