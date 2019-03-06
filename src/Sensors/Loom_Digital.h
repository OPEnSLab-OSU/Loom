
#ifndef LOOM_DIGITAL_h
#define LOOM_DIGITAL_h

#include "Loom_Sensor.h"


#define DIGITAL_COUNT 6



class Loom_Digital : public LoomSensor
{

protected:

	bool     pin_enabled[DIGITAL_COUNT];
	uint16_t analog_vals[DIGITAL_COUNT];

public:
	// --- CONSTRUCTOR ---
	Loom_Digital(	char*   module_name 			= "Digital", 
					char*   sensor_description 		= "Digital Values"

					// bool    enableA0 				= true,
					// bool    enableA1 				= true,
					// bool    enableA2 				= true,
					// bool    enableA3 				= true,
					// bool    enableA4 				= true,
					// bool    enableA5 				= true
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_Digital();

	void print_config();
	void print_measurements();
	void measure();
	void package(OSCBundle& bndl, char* suffix="");		 // This might be where analog conversions are applied
	bool message_route(OSCMessage& msg, int address_offset) {}


	int get_digital_val(uint8_t pin);

	bool get_pin_enabled(uint8_t pin);
	void set_pin_enabled(uint8_t pin, bool e);

private:

};



#endif
