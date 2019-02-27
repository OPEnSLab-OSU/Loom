
#ifndef LOOM_MAX31856_h
#define LOOM_MAX31856_h

#include "Loom_SPI_Sensor.h"

#include <Adafruit_MAX31856.h>



class Loom_MAX31856 : public LoomSPISensor
{

protected:

	Adafruit_MAX31856* inst_max;

	// float voltage;		// Raw value
	float temperature;
	float cj_temp;

public:

	Loom_MAX31856(	char* 	module_name			= "MAX31856",
					char* 	sensor_description 	= "Thermocouple", 
					uint8_t	num_samples 		= 1,

					byte	CS_pin				= 10

					// type 
			  );


	// --- DESTRUCTOR ---
	virtual ~Loom_MAX31856();

	void print_config();
	void print_measurements();
	void calibrate() {}
	void measure();
	void package(OSCBundle& bndl, char* suffix="");
	bool message_route(OSCMessage& msg, int address_offset) {}

private:


};



#endif // of #ifndef LOOM_MAX31856_h
