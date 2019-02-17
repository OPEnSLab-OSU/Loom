
#ifndef LOOM_ANALOG_h
#define LOOM_ANALOG_h

#include "Loom_Sensor.h"



#define VBATPIN A7
#define ANALOG_COUNT

typedef struct {
	byte checksum;  
	// bool valid;  
	char string[50];
	int  number;
} AnalogConfig;



enum AnalogConversion { NO_CONVERT, THERMISTOR_CONVERT, PH_CONVERT, TURBIDITY_CONVERT, EC_CONVERT }; 



// FlashStorage(analog_flash_config, AnalogConfig);




class Loom_Analog : public LoomSensor
{

protected:
	uint8_t  read_resolution;

	// Pins A0-A5
	bool     pin_enabled[ANALOG_COUNT];
	uint16_t analog_vals[ANALOG_COUNT];
	AnalogConversion conversion[ANALOG_COUNT];

	float    battery;  



	AnalogConfig configuration;

public:
	// --- CONSTRUCTOR ---
																	// The default parameters will be controlled by config file
	Loom_Analog(	char*   module_name 			= "Analog", 
					char*   sensor_description 		= "Analog Values",
					uint8_t num_samples 			= 8, 

					uint8_t read_resolution 		= 12,
					bool    enableA0 				= true,
					bool    enableA1 				= true,
					bool    enableA2 				= false,
					bool    enableA3 				= true,
					bool    enableA4 				= true,
					bool    enableA5 				= true
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_Analog();

	void print_config();
	void print_measurements();
	void measure();
	void package(OSCBundle* bndl, char* suffix="");		 // This might be where analog conversions are applied
	bool message_route(OSCMessage* msg, int address_offset);

	void 	set_analog_resolution(uint8_t res);
	uint8_t get_analog_resolution();

	int get_analog_val(int i);



	// Save a FlashStorage struct
	void save_config();
	// Load a FlashStorage struct, true if valid
	bool load_config();
	void print_config_struct();


private:

	int read_analog(uint8_t chnl);

	float convert(uint8_t pin, int analog);

};



#endif