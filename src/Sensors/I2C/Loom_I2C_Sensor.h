
#ifndef LOOM_I2C_SENSOR_h
#define LOOM_I2C_SENSOR_h

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
			char*		module_name, 
			char*		sensor_description, 

			byte		i2c_address 
		);

	// --- DESTRUCTOR ---
	virtual ~LoomI2CSensor();

	// Inherited Methods
	virtual void	print_config() override;
	virtual void	measure() = 0;
	virtual void	package(OSCBundle& bndl, char* suffix="") = 0;

	/// Get the sensor's I2C address
	/// \return	The sensor's I2C address
	byte			get_i2c_address(); 	

private:

};


#endif // of #ifndef LOOM_I2C_SENSOR_h
