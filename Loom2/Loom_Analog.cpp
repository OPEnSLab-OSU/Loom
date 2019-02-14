
// #define VBATPIN A7


// typedef struct {
// 	byte checksum;  
// 	// bool valid;  
// 	char string[50];
// 	int  number;
// } AnalogConfig;

#include "Loom_Analog.h"



// FlashStorage(analog_flash_config, AnalogConfig);




	// --- CONSTRUCTOR ---
																	// The default parameters will be controlled by config file

	// Loom_Analog::Loom_Analog(	char*   module_name 			= "Loom_Analog", 
	// 				char*   sensor_description 		= "Analog Values",
	// 				uint8_t num_samples 			= 8, 

	// 				uint8_t analog_read_resolution 	= 12,
	// 				bool    enableA0 				= true,
	// 				bool    enableA1 				= true,
	// 				bool    enableA2 				= false,
	// 				bool    enableA3 				= true,
	// 				bool    enableA4 				= true,
	// 				bool    enableA5 				= true

	// 			) : LoomSensor( module_name, sensor_description, num_samples )
	// {
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
Loom_Analog::Loom_Analog(	char*   module_name 	,
					char*   sensor_description 		,
					uint8_t num_samples 			,

					uint8_t analog_read_resolution 	,
					bool    enableA0 				,
					bool    enableA1 				,
					bool    enableA2 				,
					bool    enableA3 				,
					bool    enableA4 				,
					bool    enableA5 				

				) : LoomSensor( module_name, sensor_description, num_samples )
{
	// LOOM_DEBUG_Println("Loom_Analog Constructor");
	// LOOM_DEBUG_Println("Loom_Analog Setup");


	// Set Analog Read Resolution
	this->analog_read_resolution = analog_read_resolution;
	analogReadResolution(this->analog_read_resolution);

	// Zero out array of measurements 
	for (int i = 0; i < 6; i++) { analog_vals[i] = 0; }

	battery = 0.;

	// Set enabled pins
	pin_enabled[0] = enableA0;
	pin_enabled[1] = enableA1;
	pin_enabled[2] = enableA2;
	pin_enabled[3] = enableA3;
	pin_enabled[4] = enableA4;
	pin_enabled[5] = enableA5;

	// print_config_struct();
	// load_config();

}

// --- DESTRUCTOR ---
Loom_Analog::~Loom_Analog() {}


// --- PUBLIC METHODS ---

void Loom_Analog::print_config()
{
	LoomSensor::print_config();

	LOOM_DEBUG_Println2("\tAnalog Resolution   : ", analog_read_resolution);
	LOOM_DEBUG_Print("\tEnabled Pins        : ");
	for (int i = 0; i < 6; i++) { 
		if (pin_enabled[i]) {
			LOOM_DEBUG_Print3("A", i, ", ");
		}
	}
	LOOM_DEBUG_Println();

	// print_config_struct();

}


void Loom_Analog::print_measurements()
{
	print_module_label();
	LOOM_DEBUG_Println("Measurements:");
	for (int i = 0; i < 6; i++) {
		if (pin_enabled[i]) {
			LOOM_DEBUG_Println4("\tA", i, " = ", analog_vals[i]);
		}
	}
	LOOM_DEBUG_Println2("\tBattery = ", battery);
}


void Loom_Analog::measure()
{
	for (int i = 0; i < 6; i++) {
		if (pin_enabled[i]) {
			analog_vals[i] = read_analog(i);
		}
	}

	battery = read_analog(VBATPIN) * 2 * 3.3 / (float)pow(2, analog_read_resolution);
}


// This might be where analog conversions are applied
void Loom_Analog::package(OSCBundle* bndl, char* suffix)
{	
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	// Add Battery Data
	append_to_bundle(bndl, id_prefix, "VBat", battery, NEW_MSG);


	char buf[10];
	// maybe make the analog array into a block message
	for (int i = 0; i < 6; i++) {
		if (pin_enabled[i]) {
			sprintf(buf, "%s%d", "A", i);
			// append_to_bundle_key_value(bndl, id_prefix, buf, analog_vals[i]);
			// append_to_bundle_msg_key_value(bndl, "buf", analog_vals[i]);
			append_to_bundle(bndl, id_prefix, buf, analog_vals[i]);

		}
	}

	// append_to_bundle_key_value(bndl, id_prefix, "Battery", battery);


	

}


// Save a FlashStorage struct
void Loom_Analog::save_config() 
{
	// Probably set valid to true here
	LOOM_DEBUG_Println("Save Analog Config");
// analog_flash_config.write(configuration);
}


// Load a FlashStorage struct, true if valid
bool Loom_Analog::load_config() 
{
	LOOM_DEBUG_Println("Load Analog Config");
// configuration = analog_flash_config.read();


// 		LOOM_DEBUG_Print("B: ");
// 		print_config_struct();

// 		if (configuration.checksum != 42) {
// 			// configuration.valid = true;
// 			configuration.checksum = 42;
// 			sprintf( configuration.string, "%s", "Data" );
// 			configuration.number = 1;	
// 		}

// 		LOOM_DEBUG_Print("C: ");
// 		print_config_struct();

// 		LOOM_DEBUG_Println("Incrementing number");
// 		configuration.number++;

// 		LOOM_DEBUG_Print("D: ");
// 		print_config_struct();
}

void Loom_Analog::print_config_struct() 
{
	LOOM_DEBUG_Println("Analog Flash Struct:");
	// LOOM_DEBUG_Println2("\tValid : ", configuration.valid);
	LOOM_DEBUG_Println2("\tChecksum : ", configuration.checksum);

	LOOM_DEBUG_Println2("\tString   : ", configuration.string);
	LOOM_DEBUG_Println2("\tNumber   : ", configuration.number);

}




bool Loom_Analog::message_route(OSCMessage* msg, int address_offset) 
{
	// Enable or disable individual pins
}


void Loom_Analog::set_analog_resolution(uint8_t res) 
{ 
	analogReadResolution(analog_read_resolution = res); 
}


uint8_t Loom_Analog::get_analog_resolution() 
{ 
	return analog_read_resolution; 
}


int Loom_Analog::get_analog_val(int i) 
{
	return ( (i >= 0) && (i < 6) ) ? analog_vals[i] : -1;
}


int Loom_Analog::read_analog(uint8_t chnl)
{
	int i = num_samples;
	int reading = 0;

	while (i--) {
		reading += analogRead(chnl); 
	}

	switch (num_samples) {
		case 1 : return (reading);
		case 2 : return (reading >> 1); // Divide by 2
		case 4 : return (reading >> 2); // Divide by 4
		case 8 : return (reading >> 3); // Divide by 8
		case 16: return (reading >> 4); // Divide by 16
		default: return (reading);
	}
} 



