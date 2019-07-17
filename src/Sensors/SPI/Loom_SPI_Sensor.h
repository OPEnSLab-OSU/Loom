#pragma once

#include "../Loom_Sensor.h"


/// Abstract base class for SPI sensor modules 
class LoomSPISensor : public LoomSensor
{

protected:


public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	LoomSPISensor(	
			const char*		module_name			= "SPI_Sensor",
			uint8_t			num_samples			= 1 
		);

	/// Destructor
	virtual ~LoomSPISensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	virtual void	calibrate() override {};
	virtual void	measure() override = 0;
	virtual void 	package(JsonObject json) override = 0;
	virtual bool	cmd_route(JsonObject) override {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_measurements() override = 0;

private:

};


