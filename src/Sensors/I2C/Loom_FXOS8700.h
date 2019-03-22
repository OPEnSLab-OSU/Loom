
#ifndef LOOM_FXOS8700_h
#define LOOM_FXOS8700_h

#include "Loom_I2C_Sensor.h"

#include <Adafruit_FXOS8700.h>



class Loom_FXOS8700 : public LoomI2CSensor
{

protected:

	/// Underlying FXOS8700 sensor manager instance
	Adafruit_FXOS8700*	inst_FXOS8700;

	/// Measured acceleration values (x,y,z) (g's)
	float		accel[3];
	/// Measured magnetic field values (x,y,z) (T)
	float		mag[3];

public:

	// --- CONSTRUCTOR ---
	Loom_FXOS8700(	
			byte		i2c_address				=  0x1F, // 0x1C, 0x1D, 0x1E, 0x1F // FXOS8700_I2C_Address,
			
			char*		module_name				=  "FXOS8700", // FXOS8700_Module_Name,
			char*		sensor_description		= "3-Axis Acceleromter/Magnetometer"  // FXOS8700_Sensor_Description 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_FXOS8700();

	// Inherited (Overriding) Methods
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");

private:

};


#endif
