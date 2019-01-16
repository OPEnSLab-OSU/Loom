

#ifndef LOOM_SENSOR_ANALOG_h
#define LOOM_SENSOR_ANALOG_h


#include <OSCBundle.h>

#include "loom_sensor_class.h"



// #include "loom_misc.h"

// #include "loom_translator.h"

#define VBATPIN A7


typedef struct {
	byte checksum;  
	// bool valid;  
	char string[50];
	int  number;
} AnalogConfig;




// FlashStorage(analog_flash_config, AnalogConfig);





class AnalogManager : public LoomSensor
{

protected:
	uint8_t  analog_read_resolution;

	// Pins A0-A5
	bool     pin_enabled[6];
	uint16_t analog_vals[6];

	float    battery;  


	AnalogConfig configuration;


public:
	// --- CONSTRUCTOR ---
																	// The default parameters will be controlled by config file

	AnalogManager(	char*   module_name 			= "AnalogManager", 
					char*   sensor_description 		= "Analog Values",
					uint8_t num_samples 			= 8, 

					uint8_t analog_read_resolution 	= 12,
					bool    enableA0 				= true,
					bool    enableA1 				= true,
					bool    enableA2 				= false,
					bool    enableA3 				= true,
					bool    enableA4 				= true,
					bool    enableA5 				= true

				);// : LoomSensor( module_name, sensor_description, num_samples );
	// --- DESTRUCTOR ---
	~AnalogManager();


	// --- PUBLIC METHODS ---

	void print_config();


	void print_measurements();


	void measure();


// This might be where analog conversions are applied
	void package(OSCBundle* bndl, char* id_prefix);

	// Save a FlashStorage struct
	void save_config();


	// Load a FlashStorage struct, true if valid
	bool load_config();

	void print_config_struct();




	bool message_route(OSCMessage* msg, int address_offset);


	void set_analog_resolution(uint8_t res);

	uint8_t get_analog_resolution();



	int get_analog_val(int i);

private:

	int read_analog(uint8_t chnl);



};



#endif