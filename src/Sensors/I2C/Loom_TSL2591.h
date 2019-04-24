
#ifndef LOOM_TSL2591_h
#define LOOM_TSL2591_h

#include "Loom_I2C_Sensor.h"

#include <Adafruit_TSL2591.h>


// # (LoomI2CSensor) | dependencies: [] | conflicts: []
/// TSL2591 Lux sensor module
// #
class Loom_TSL2591 : public LoomI2CSensor
{

protected:

	/// Underlying TSL2591 sensor manager instance
	Adafruit_TSL2591	inst_tsl2591;

	/// Measured visable lux
	uint16_t			vis;
	/// Measured infra-red lux
	uint16_t			ir;
	/// Measured full spectrum lux
	uint16_t			full;

	/// Sensor gain level setting to use
	uint8_t				gain_level;
	/// Sensor integration time setting
	uint8_t				timing_level;

public:

	/// TSL2591 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x29> | {0x29} | I2C address
	/// \param[in]	module_name				String | <"TSL2591"> | null | TSL2591 module name
	/// \param[in]	gain							Set(Int) | <1> | { 1 } | Gain level
	/// \param[in]	timing_level			Set(Int) | <0> | { 0 } | Timing level
	Loom_TSL2591(
			byte			i2c_address			= 0x29,
			const char*		module_name			= "TSL2591",
			uint8_t			gain_level			= 1,
			uint8_t			timing_level		= 0
		);


	// --- DESTRUCTOR ---
	virtual ~Loom_TSL2591();

	// Inherited (Overriding) Methods
	void		print_measurements() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;

private:

};


#endif
