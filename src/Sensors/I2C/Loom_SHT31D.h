
#ifndef LOOM_SHT31D_h
#define LOOM_SHT31D_h

#include "Loom_I2C_Sensor.h"

#include "Adafruit_SHT31.h"


// # (LoomI2CSensor) | dependencies: [] | conflicts: []
/// SHT31D Temperature / Humidty sensor module
// #
class Loom_SHT31D : public LoomI2CSensor
{

protected:

	/// Underlying SHT31D sensor manager instance
	Adafruit_SHT31 inst_sht31d;

	/// Measured temperature (C)
	float		temp;
	/// Measured humidity (%)
	float		humid;

public:

	/// SHT31D module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x44> | {0x44, 0x45} | I2C address
	/// \param[in]	module_name				String | <"SHT31D"> | SHT31D module name					
	/// \param[in]	sensor_description		String | <"Temperature/Humidty"> | Debugging display description of module
	Loom_SHT31D(	
			byte		i2c_address				= 0x44,
			char*		module_name				= "SHT31D",
			char*		sensor_description		= "Temperature/Humidty"
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_SHT31D();

	// Inherited (Overriding) Methods
	void		print_measurements() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;

private:

};


#endif
