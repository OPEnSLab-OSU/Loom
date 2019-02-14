
#ifndef LOOM_SPI_SENSOR_h
#define LOOM_SPI_SENSOR_h


#include "Loom_Sensor.h"


class LoomSPISensor : public LoomSensor
{

protected:


public:

	LoomSPISensor(	char* 	module_name			= "SPI_Sensor",
					char* 	sensor_description 	= "SPI Sensor", 
					uint8_t	num_samples 		= 1 

			  );


	// --- DESTRUCTOR ---
	virtual ~LoomSPISensor();

	virtual void print_config();
	virtual void print_measurements() = 0;
	virtual void calibrate() {};
	virtual void measure() = 0;
	virtual void package(OSCBundle* bndl, char* suffix="") = 0;
	virtual bool message_route(OSCMessage* msg, int address_offset) {};

private:


};



#endif // of #ifndef LOOM_SPI_SENSOR_h
