#pragma once

#include "Loom_I2C_Sensor.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// MB1232 Sonar sensor module
// ###
class Loom_MB1232 : public LoomI2CSensor
{

protected:

	int range;		/// Measure distance (cm)

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// MB1232 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x70> | {0x70} | I2C address
	/// \param[in]	module_name				String | <"MB1232"> | null | MB1232 module name
	Loom_MB1232(
			byte			i2c_address		= 0x70,
			const char*		module_name		= "MB1232"
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_MB1232(JsonVariant p);

	/// Destructor
	~Loom_MB1232() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_measurements() override;

private:

};



