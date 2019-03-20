
#ifndef LOOM_LIS3DH_h
#define LOOM_LIS3DH_h

#include "Loom_I2C_Sensor.h"

#include "SparkFunLIS3DH.h"



class Loom_LIS3DH : public LoomI2CSensor
{

protected:

	/// Underlying LIS3DH sensor manager instance
	LIS3DH*		inst_LIS3DH;

	/// Measured acceleration values (x,y,z)(g's)
	float		accel[3];

public:

	// --- CONSTRUCTOR ---
	Loom_LIS3DH(	
			byte		i2c_address				= LIS3DH_I2C_Address, // 0x19,
			
			char*		module_name				= LIS3DH_Module_Name, // "LIS3DH",
			char*		sensor_description		= LIS3DH_Sensor_Description // "Accelerometer" 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_LIS3DH();

	// Inherited (Overriding) Methods
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");

private:

};


#endif
