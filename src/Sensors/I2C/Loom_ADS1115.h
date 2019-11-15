#pragma once

#include "Loom_I2C_Sensor.h"

#include <Adafruit_ADS1015.h>

#include <Wire.h>


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// Class ADS1115
// ###
class Loom_ADS1115 : public LoomI2CSensor
{

protected:
    Adafruit_ADS1015 ads1115 ///< Underlying ADS1115 sensor manager instance

	// Protected members here (are inherited)

public:
			byte			i2c_address		= 0x2A,
			const char*		module_name		= "I2CSensorTemplate"
			int				analog0;
			int				analog1;
			int				analog2;
			int				analog3;
			int				diff0;
			int				diff1;
			bool 			analog_0_enabled;
			bool 			analog_1_enabled;
			bool 			analog_2_enabled;
			bool 			analog_3_enabled;
			bool 			diff_0_enabled;
			bool 			diff_1_enabled;

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	Loom_ADS1115(
			bool 			analog_0_enabled= false;
			bool 			analog_1_enabled= false;
			bool 			analog_2_enabled= false;
			bool 			analog_3_enabled= false;
			bool 			diff_0_enabled  = false;
			bool 			diff_1_enabled  = false;

		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_ADS1115(JsonArrayConst p);
	
	/// Destructor
	virtual ~Loom_ADS1115() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;
	bool		dispatch(JsonObject json) override;
	void		calibrate() override;
	void		power_down() override {} 
	void		power_up() override {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;
	void		print_measurements() override;

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




