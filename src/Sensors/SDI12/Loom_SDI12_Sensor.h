#pragma once

#include "../Loom_Sensor.h"


/// Abstract base class for SDI12 sensor modules 
class LoomSDI12Sensor : public LoomSensor
{

protected:


public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	LoomSDI12Sensor(	
			const char*		module_name		= "SDI12_Sensor",
			uint8_t			num_samples		= 1 
		);

	/// Destructor
	virtual ~LoomSDI12Sensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	virtual void	calibrate() override {};
	virtual void	measure() override = 0;
	virtual void 	package(JsonObject json) override = 0;
	virtual bool	dispatch(JsonObject) override {}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() override;
	virtual void	print_measurements() override = 0;

private:

};


