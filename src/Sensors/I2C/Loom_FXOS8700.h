
#ifndef LOOM_FXOS8700_h
#define LOOM_FXOS8700_h

#include "Loom_I2C_Sensor.h"

#include <Adafruit_FXOS8700.h>


// # (LoomI2CSensor) | dependencies: [] | conflicts: []
/// FXOS8700 3-Axis Acceleromter/Magnetometer sensor module
// #
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


	/// FXOS8700 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x1F> | {0x1C, 0x1D, 0x1E, 0x1F} | I2C address
	/// \param[in]	module_name				String | <"FXOS8700"> | FXOS8700 module name					
	Loom_FXOS8700(	
			byte			i2c_address		= 0x1F,
			const char*		module_name		= "FXOS8700" 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_FXOS8700();

	// Inherited (Overriding) Methods
	void		print_measurements() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;

private:

};


#endif
