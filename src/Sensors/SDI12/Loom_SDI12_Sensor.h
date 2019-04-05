
#ifndef LOOM_SDI12_SENSOR_h
#define LOOM_SDI12_SENSOR_h


#include "../Loom_Sensor.h"


/// Abstract base class for SDI12 sensor modules 
class LoomSDI12Sensor : public LoomSensor
{

protected:


public:

	LoomSDI12Sensor(	
			char*		module_name			= "SDI12_Sensor",
			char*		sensor_description	= "SDI12 Sensor", 
			uint8_t		num_samples			= 1 
		);


	// --- DESTRUCTOR ---
	virtual ~LoomSDI12Sensor();

	// Inherited Methods
	virtual void	print_config();
	virtual void	print_measurements() = 0;
	virtual void	calibrate() {};
	virtual void	measure() = 0;
	virtual void	package(OSCBundle& bndl, char* suffix="") = 0;
	virtual bool	message_route(OSCMessage& msg, int address_offset) {};

private:


};



#endif // of #ifndef LOOM_SDI12_SENSOR_h
