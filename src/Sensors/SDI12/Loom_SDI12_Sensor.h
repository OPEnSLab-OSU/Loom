#pragma once

#include "../Loom_Sensor.h"


/// Abstract base class for SDI12 sensor modules 
class LoomSDI12Sensor : public LoomSensor
{

protected:


public:

	LoomSDI12Sensor(	
			const char*		module_name		= "SDI12_Sensor",
			uint8_t			num_samples		= 1 
		);


	/// Destructor
	virtual ~LoomSDI12Sensor();

	// Inherited Methods
	virtual void	print_config() override;
	virtual void	print_measurements() = 0;
	virtual void	calibrate() {};
	virtual void	measure() = 0;
	virtual void 	package(JsonObject json) = 0;
	virtual bool	cmd_route(JsonObject) {}


private:


};


