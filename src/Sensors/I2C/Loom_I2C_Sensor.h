#pragma once

#include "../Loom_Sensor.h"
#include <Wire.h>


/// Abstract root for I2C sensor modules
class LoomI2CSensor : public LoomSensor
{

protected:

	/// The sensor's I2C address
	byte	i2c_address;	

public:

	// --- CONSTRUCTOR ---
	LoomI2CSensor( 	
			const char*		module_name, 
			byte			i2c_address 
		);

	// --- DESTRUCTOR ---
	virtual ~LoomI2CSensor();

	// Inherited Methods
	virtual void	print_config() override;
	virtual void	measure() = 0;
	virtual void 	package(JsonObject json) = 0;

	/// Get the sensor's I2C address
	/// \return	The sensor's I2C address
	byte			get_i2c_address(); 	
	
	void			adjust_module_name_with_port(int port);


private:

};


