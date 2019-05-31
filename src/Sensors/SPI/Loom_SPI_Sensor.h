#pragma once

#include "../Loom_Sensor.h"


/// Abstract base class for SPI sensor modules 
class LoomSPISensor : public LoomSensor
{

protected:


public:

	LoomSPISensor(	
			const char*		module_name			= "SPI_Sensor",
			uint8_t			num_samples			= 1 
		);


	// --- DESTRUCTOR ---
	virtual ~LoomSPISensor();

	// Inherited Methods
	virtual void	print_config() override;
	virtual void	print_measurements() = 0;
	virtual void	calibrate() {};
	virtual void	measure() = 0;
	virtual void 	package(JsonObject json) = 0;
	virtual bool	cmd_route(JsonObject) {}


private:


};


