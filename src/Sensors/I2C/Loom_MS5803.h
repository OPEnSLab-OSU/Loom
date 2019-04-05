
#ifndef LOOM_MS5803_h
#define LOOM_MS5803_h

#include "Loom_I2C_Sensor.h"

#include <MS5803_02.h> 


/// MS5803 Atmospheric Pressure / Temperature sensor module
class Loom_MS5803 : public LoomI2CSensor
{

protected:

	/// Underlying MS5803 sensor manager instance
	MS_5803*	inst_MS5803;

	/// Read calibration data stored on sensor
	uint16_t	MS_PROM[8];

	/// Measure pressure (mbar)
	float		pressure;

	/// Measured temperature (C)
	float		temp;

public:

	// --- CONSTRUCTOR ---
	Loom_MS5803(	
			byte		i2c_address				=  0x77, // 0x76 if CSB (pin 3) is High, 0x77 if CSB is Low // MS5803_I2C_Address,
			
			char*		module_name				=  "MS5803", // MS5803_Module_Name,
			char*		sensor_description		= "Atmospheric Pressure / Temperature"  // MS5803_Sensor_Description 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_MS5803();

	// Inherited (Overriding) Methods
	void		print_measurements() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;

private:

};


#endif
