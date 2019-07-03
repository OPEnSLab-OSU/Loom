#pragma once

#include "Loom_I2C_Sensor.h"


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// MB1232 Sonar sensor module
// ###
class Loom_MB1232 : public LoomI2CSensor
{

protected:

	/// Measure distance (cm)
	int range;

public:

	/// MB1232 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x70> | {0x70} | I2C address
	/// \param[in]	module_name				String | <"MB1232"> | null | MB1232 module name
	Loom_MB1232(
			byte			i2c_address		= 0x70,
			const char*		module_name		= "MB1232"
		);

	Loom_MB1232(JsonVariant p);


	/// Destructor
	virtual ~Loom_MB1232();

	// Inherited (Overriding) Methods
	void		print_measurements() override;
	void		measure() override;
	void 		package(JsonObject json);

private:

};



