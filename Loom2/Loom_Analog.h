
#ifndef LOOM_ANALOG_h
#define LOOM_ANALOG_h


// #include "Sensors/Loom_Sensor.h"
#include "Loom_Sensor.h"


#include <OSCBundle.h>




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





class Loom_Analog : public LoomSensor
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

	Loom_Analog(	char*   module_name 			= "Loom_Analog", 
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
	


// 	Loom_Analog(	char*   module_name 			= "Loom_Analog", 
// 					char*   sensor_description 		= "Analog Values",
// 					uint8_t num_samples 			= 8, 

// 					uint8_t analog_read_resolution 	= 12,
// 					bool    enableA0 				= true,
// 					bool    enableA1 				= true,
// 					bool    enableA2 				= false,
// 					bool    enableA3 				= true,
// 					bool    enableA4 				= true,
// 					bool    enableA5 				= true

// 				) : LoomSensor( module_name, sensor_description, num_samples )
// 	{
// 	// LOOM_DEBUG_Println("Loom_Analog Constructor");
// 	LOOM_DEBUG_Println("Loom_Analog Setup");


// 	// Set Analog Read Resolution
// 	this->analog_read_resolution = analog_read_resolution;
// 	analogReadResolution(this->analog_read_resolution);

// 	// Zero out array of measurements 
// 	for (int i = 0; i < 6; i++) { analog_vals[i] = 0; }

// 	battery = 0.;

// 	// Set enabled pins
// 	pin_enabled[0] = enableA0;
// 	pin_enabled[1] = enableA1;
// 	pin_enabled[2] = enableA2;
// 	pin_enabled[3] = enableA3;
// 	pin_enabled[4] = enableA4;
// 	pin_enabled[5] = enableA5;

// 	// print_config_struct();
// 	// load_config();

// }



	// --- DESTRUCTOR ---
	~Loom_Analog();


	// --- PUBLIC METHODS ---

	void print_config();


	void print_measurements();


	void measure();


// This might be where analog conversions are applied
	void package(OSCBundle* bndl);
	// void package(OSCBundle* bndl) { LoomModule::package(bndl); }

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