#pragma once

#include "Loom_I2C_Sensor.h"

#include <MS5803_02.h>


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// MS5803 Atmospheric Pressure / Temperature sensor module
// ###
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

	/// MS5803 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x76> | {0x76, 0x77} | I2C address (0x76 if CSB (pin 3) is High, 0x77 if CSB is Low)
	/// \param[in]	module_name				String | <"MS5803"> | null | MS5803 module name
	Loom_MS5803(
			byte			i2c_address		= 0x76,
			const char*		module_name		= "MS5803"
		);

	Loom_MS5803(JsonVariant p);


	// --- DESTRUCTOR ---
	virtual ~Loom_MS5803();

	// Inherited (Overriding) Methods
	void		print_measurements() override;
	void		measure() override;
	void 		package(JsonObject json);

private:

};



