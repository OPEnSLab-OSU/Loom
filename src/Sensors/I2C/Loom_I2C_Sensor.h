#pragma once

#include "../Loom_Sensor.h"
#include <Wire.h>


///////////////////////////////////////////////////////////////////////////////


/// Abstract root for I2C sensor modules
class LoomI2CSensor : public LoomSensor
{

protected:

	byte	i2c_address;	/// The sensor's I2C address
	uint8_t	port_num;		/// Used with multiplexer, keep track of port it is on

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomI2CSensor( 	
			const char*		module_name, 
			byte			i2c_address,
			uint8_t			mux_port = -1	// Default to -1 (not on mux)		 
		);

	/// Destructor
	virtual ~LoomI2CSensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	virtual void	measure() = 0;
	virtual void 	package(JsonObject json) override = 0;
	virtual void	print_measurements() = 0; 

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the sensor's I2C address
	/// \return	The sensor's I2C address
	byte			get_i2c_address(); 	

private:

	// Allow Multiplexer to access private methods of I2C sensor modules
	friend class Loom_Multiplexer;
};


