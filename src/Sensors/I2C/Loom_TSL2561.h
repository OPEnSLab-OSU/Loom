
#ifndef LOOM_TSL2561_h
#define LOOM_TSL2561_h


#include "Loom_I2C_Sensor.h"

#include <Adafruit_TSL2561_U.h>



class Loom_TSL2561 : public LoomI2CSensor
{

protected:

	/// Underlying TSL2561 sensor manager instance
	Adafruit_TSL2561_Unified* inst_TSL2561;

	/// Gain level
	int 		gain;

	/// Sensor resolution setting
	int 		resolution;

	/// Measured infra-red lux 
	int 		lightIR;
	
	/// Measure full spectrum lux
	int 		lightFull;

public:

	// --- CONSTRUCTOR ---
	Loom_TSL2561(	
			byte	i2c_address				= 0x29, // 0x29, 0x39, 0x49
			
			char*	module_name				= "TSL2561",
			char*	sensor_description		= "Luminosity",

			int		gain					= 1, // 1, 16
			int		resolution				= 3	 // 1, 2, 3
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_TSL2561();

	// Inherited (Overriding) Methods
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");

private:

};


#endif
