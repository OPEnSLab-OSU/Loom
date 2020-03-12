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
	uint16_t			absolute_reads[4];
	int16_t				diff_reads[2];

public:
	enum class Gain : uint32_t {
		GAIN_TWOTHIRDS    = ADS1015_REG_CONFIG_PGA_6_144V,
		GAIN_ONE          = ADS1015_REG_CONFIG_PGA_4_096V,
		GAIN_TWO          = ADS1015_REG_CONFIG_PGA_2_048V,
		GAIN_FOUR         = ADS1015_REG_CONFIG_PGA_1_024V,
		GAIN_EIGHT        = ADS1015_REG_CONFIG_PGA_0_512V,
		GAIN_SIXTEEN      = ADS1015_REG_CONFIG_PGA_0_256V
	}

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	Loom_ADS1115(
		LoomManager* manager,
		const 	byte i2c_address = ADS1015_ADDRESS, 
		const	uint8_t 		mux_port = 255,
		const 	bool 			analog_0_enabled = false,
		const 	bool 			analog_1_enabled = false,
		const 	bool 			analog_2_enabled = false,
		const 	bool 			analog_3_enabled = false,
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
//	void		print_state()const override;
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




