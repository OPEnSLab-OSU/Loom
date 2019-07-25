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
			const char*			module_name,
			LoomModule::Type	module_type,
			uint8_t				num_samples = 1
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


private:

};


