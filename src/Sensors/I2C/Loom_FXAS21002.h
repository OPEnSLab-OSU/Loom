
#ifndef LOOM_FXAS21002_h
#define LOOM_FXAS21002_h

#include "Loom_I2C_Sensor.h"

#include <Adafruit_FXAS21002C.h>


// # (LoomI2CSensor) | dependencies: [] | conflicts: []
/// FXAS21002 3-Axis Gyroscope snesor module
// #
class Loom_FXAS21002 : public LoomI2CSensor
{

protected:

	/// Underlying FXAS21002 sensor manager instance
	Adafruit_FXAS21002C*	inst_FXAS21002;

	/// Measured gyro values (x,y,z)
	float		gyro[3];

public:


	/// FXAS21002 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x20> | {0x20, 0x21} | I2C address
	/// \param[in]	module_name				String | <"FXAS21002"> | FXAS21002 module name					
	Loom_FXAS21002(	
			byte		i2c_address				= 0x20,
			char*		module_name				= "FXAS21002"
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
