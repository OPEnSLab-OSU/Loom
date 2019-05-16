
#ifndef LOOM_TSL2561_h
#define LOOM_TSL2561_h


#include "Loom_I2C_Sensor.h"

#include <Adafruit_TSL2561_U.h>


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// TSL2561 Luminosity sensor module
// ###
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

	/// TSL2561 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x29> | {0x29, 0x39, 0x49} | I2C address
	/// \param[in]	module_name				String | <"TSL2561"> | null | TSL2561 module name
	/// \param[in]	gain							Set(Int) | <1> | {1, 16} | Gain level
	/// \param[in]	resolution				Set(Int) | <3> | { 1("Low"), 2("Med"), 3("High") } | Resolution
	Loom_TSL2561(
			byte			i2c_address		= 0x29,
			const char*		module_name		= "TSL2561",
			int				gain			= 1,
			int				resolution		= 3
		);

	Loom_TSL2561(JsonVariant p);


	// --- DESTRUCTOR ---
	virtual ~Loom_TSL2561();

	// Inherited (Overriding) Methods
	void		print_measurements() override;
	void		measure() override;
	void 		package(JsonObject json);

private:

};


#endif
