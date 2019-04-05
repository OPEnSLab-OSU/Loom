
#ifndef LOOM_LIS3DH_h
#define LOOM_LIS3DH_h

#include "Loom_I2C_Sensor.h"

#include "SparkFunLIS3DH.h"


/// LIS3DH Accelerometer sensor module
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
			byte		i2c_address				=  0x19,  // LIS3DH_I2C_Address,
			
			char*		module_name				=  "LIS3DH",  // LIS3DH_Module_Name,
			char*		sensor_description		= "Accelerometer"   // LIS3DH_Sensor_Description 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_LIS3DH();

	// Inherited (Overriding) Methods
	void		print_measurements() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;

private:

};


#endif
