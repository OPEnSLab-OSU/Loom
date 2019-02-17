
#include "Loom_Analog.h"


// FlashStorage(analog_flash_config, AnalogConfig);



// Make a static class for analog conversions?
// Is this going to be a member of the Loom_Analog class?
	// this might contain the information about which conversion is being use - selecting conversion should be dynamic
// Point is to contain location of updates adding new conversions

// Should support taking an extra float for use in conversion
	// Like temperature for electrical conductivity 


float Loom_Analog::convert(uint8_t pin, int analog)
{
	switch(conversion[pin]) {
		// case NO_CONVERT 		: return (float)analog;
		case THERMISTOR_CONVERT	:
		case PH_CONVERT 		:
		case TURBIDITY_CONVERT	:
		case EC_CONVERT			:
		default					: return (float)analog;  
	}
}





// --- CONSTRUCTOR ---
Loom_Analog::Loom_Analog(	char*   module_name 	,
						char*   sensor_description 		,
						uint8_t num_samples 			,

						uint8_t read_resolution 		,
						bool    enableA0 				,
						bool    enableA1 				,
						bool    enableA2 				,
						bool    enableA3 				,
						bool    enableA4 				,
						bool    enableA5 				

				) : LoomSensor( module_name, sensor_description, num_samples )
{
	// LOOM_DEBUG_Println("Loom_Analog Constructor");


	// Set Analog Read Resolution
	this->read_resolution = read_resolution;
	analogReadResolution(this->read_resolution);

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

	LOOM_DEBUG_Println2("\tAnalog Resolution   : ", read_resolution);
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
	LOOM_DEBUG_Println2("\tBattery = ", battery);
	for (int i = 0; i < 6; i++) {
		if (pin_enabled[i]) {
			LOOM_DEBUG_Println4("\tA", i, " = ", analog_vals[i]);
		}
	}
}


void Loom_Analog::measure()
{
	battery = read_analog(VBATPIN) * 2 * 3.3 / (float)pow(2, read_resolution);

	for (int i = 0; i < 6; i++) {
		if (pin_enabled[i]) {
			analog_vals[i] = read_analog(i);
		}

		// Put conversions here

	}
}


// This might be where analog conversions are applied
void Loom_Analog::package(OSCBundle* bndl, char* suffix)
{	
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	// Add Battery Data
	append_to_bundle(bndl, id_prefix, "VBat", battery, NEW_MSG);

	char buf[10];
	for (int i = 0; i < 6; i++) {
		if (pin_enabled[i]) {
			sprintf(buf, "%s%d", "A", i);
			append_to_bundle(bndl, id_prefix, buf, analog_vals[i]);
		}

		// Put conversions here

	}	
}








bool Loom_Analog::message_route(OSCMessage* msg, int address_offset) 
{
	// Enable or disable individual pins
}


void Loom_Analog::set_analog_resolution(uint8_t res) 
{ 
	analogReadResolution(read_resolution = res); 
}


uint8_t Loom_Analog::get_analog_resolution() 
{ 
	return read_resolution; 
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








