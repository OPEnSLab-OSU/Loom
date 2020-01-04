#pragma once

#include "Loom_I2C_Sensor.h"

#include <Adafruit_ADS1015.h>





///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// Class ADS1115
// ###
class Loom_ADS1115 : public LoomI2CSensor
{

protected:
	 ///< Underlying ADS1115 sensor manager instance

	// Protected members here (are inherited)

public:
			Adafruit_ADS1015 ads1115;
			byte				i2c_address;
			uint8_t 			mux_port;
			char*				module_name;
			 int				analog0;
			 int				analog1;
			 int				analog2;
			 int				analog3;
			 int				diff0;
			 int				diff1;
			 bool 				analog_0_enabled;
			 bool 				analog_1_enabled;
			 bool 				analog_2_enabled;
			 bool 				analog_3_enabled;
			 bool 				diff_0_enabled;
			 bool 				diff_1_enabled;
			 

		 


//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	Loom_ADS1115(
		const byte				i2c_address = 0x2A, 
		const uint8_t 			mux_port = 255,
		const char*				module_name="ADS1115",
		const int				analog0=0,
		const 	int				analog1=0,
		const 	int				analog2=0,
		const 	int				analog3=0,
		const 	int				diff0=0,
		const 	int				diff1=0,
		const 	bool 			analog_0_enabled=false,
		const 	bool 			analog_1_enabled=false,
		const 	bool 			analog_2_enabled=false,
		const 	bool 			analog_3_enabled=false,
		const 	bool 			diff_0_enabled=false,
		const 	bool 			diff_1_enabled=false
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
	//bool		dispatch(JsonObject json) override;
	//void		calibrate() override;
	//void		power_down() override {} 
	//void		power_up() override {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config()const override;
//	void		print_state()const override;
	void		print_measurements()const override;

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




