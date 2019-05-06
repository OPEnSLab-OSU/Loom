
#ifndef LOOM_LIS3DH_h
#define LOOM_LIS3DH_h

#include "Loom_I2C_Sensor.h"

#include "SparkFunLIS3DH.h"


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// LIS3DH Accelerometer sensor module
// ###
class Loom_LIS3DH : public LoomI2CSensor
{

protected:

	/// Underlying LIS3DH sensor manager instance
	LIS3DH*		inst_LIS3DH;

	/// Measured acceleration values (x,y,z)(g's)
	float		accel[3];

public:


	/// LIS3DH module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x19> | {0x19} | I2C address
	/// \param[in]	module_name				String | <"LIS3DH"> | null | LIS3DH module name
	Loom_LIS3DH(
			byte			i2c_address		= 0x19,
			const char*		module_name		= "LIS3DH"
		);

	Loom_LIS3DH(JsonVariant p);


	// --- DESTRUCTOR ---
	virtual ~Loom_LIS3DH();

	// Inherited (Overriding) Methods
	void		print_measurements() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;

private:

};


#endif
