
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
			byte	i2c_address				=  0x29, // 0x29, 0x39, 0x49    // TSL2561_I2C_Address,
			
			char*	module_name				=  "TSL2561",    // TSL2561_Module_Name,
			char*	sensor_description		=  "Luminosity",    // TSL2561_Sensor_Description,

			int		gain					=  1, // 1, 16    // TSL2561_Gain,
			int		resolution				= 3	 // 1, 2, 3    // TSL2561_Resolution 
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
