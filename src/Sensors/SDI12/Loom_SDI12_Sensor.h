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

	/// Constructor
	/// \param[in]	module_name		Name of the module (provided by derived classes)
	/// \param[in]	module_type		Type of the module (provided by derived classes)
	/// \param[in]	num_samples		The number of samples to take and average
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

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================


private:

};


