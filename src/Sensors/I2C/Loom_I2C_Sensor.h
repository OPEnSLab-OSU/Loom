#pragma once

#include "../Loom_Sensor.h"
#include <Wire.h>


///////////////////////////////////////////////////////////////////////////////


/// Abstract root for I2C sensor modules
class LoomI2CSensor : public LoomSensor
{

protected:

	byte	i2c_address;	/// The sensor's I2C address

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomI2CSensor( 	
			const char*		module_name, 
			byte			i2c_address 
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

	/// Add port number to sensor's name.
	/// Used by multiplexer so that packaged data can 
	/// be distinquished by port
	void			adjust_module_name_with_port(int port);

};


