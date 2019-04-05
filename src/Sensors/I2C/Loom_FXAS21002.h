
#ifndef LOOM_FXAS21002_h
#define LOOM_FXAS21002_h

#include "Loom_I2C_Sensor.h"

#include <Adafruit_FXAS21002C.h>


/// FXAS21002 3-Axis Gyroscope snesor module
class Loom_FXAS21002 : public LoomI2CSensor
{

protected:

	/// Underlying FXAS21002 sensor manager instance
	Adafruit_FXAS21002C*	inst_FXAS21002;

	/// Measured gyro values (x,y,z)
	float		gyro[3];

public:

	// --- CONSTRUCTOR ---
	Loom_FXAS21002(	
			byte		i2c_address				=   0x20, // 0x20 / 0x21  // FXAS21002_I2C_Address,
					
			char*		module_name				=   "FXAS21002",  // FXAS21002_Module_Name,
			char*		sensor_description		= "3-Axis Gyroscope"   // FXAS21002_Sensor_Description 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_FXAS21002();

	// Inherited (Overriding) Methods
	void		print_measurements() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;

private:

};


#endif
