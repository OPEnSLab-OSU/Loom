
#include "Loom_Neopixel.h"
#include "Loom_Macros.h"

#include <Adafruit_NeoPixel.h>



// --- CONSTRUCTOR ---
Loom_Neopixel::Loom_Neopixel(char* module_name, bool enableA0, bool enableA1, bool enableA2)
	: LoomActuator(module_name)
{
	// Set enabled pins
	pin_enabled[0] = enableA0;
	pin_enabled[1] = enableA1;
	pin_enabled[2] = enableA2;

	// Set pin mode on enabled pins (pins A0-A5 = 14-19)
	for (int i = 0; i < 3; i++) { 
		if (pin_enabled[i]) pinMode(14+i, OUTPUT); 
	}

	// Initialize Neopixels
	for (int i = 0; i < 3; i++) {
		if (pin_enabled[i]) {
			pixels[i] = new Adafruit_NeoPixel(1, 14+i, NEO_GRB + NEO_KHZ800);
			for (int j = 0; j < 3; j++) {
				color_vals[i][j] = 0;
			}
			pixels[i]->begin(); // This initializes the NeoPixel library.
			pixels[i]->show();  // Initialize all pixels to 'off'
		}
	}		

	print_module_label();
	LOOM_DEBUG_Println("Setup");
} 

// --- DESTRUCTOR ---
Loom_Neopixel::~Loom_Neopixel() 
{
	for (int i = 0; i < 3; i++) {
		if (pixels[i] != NULL) {
			delete pixels[i];
		}
	}
}



void Loom_Neopixel::print_config() 
{
	LoomActuator::print_config();

	for (int i = 0; i < 3; i++) {
		LOOM_DEBUG_Println4("\tNeopixel ", i, "          : ", (pin_enabled[i]) ? "Enabled" : "Disabled" );
	}
}


void Loom_Neopixel::print_state()
{
	LOOM_DEBUG_Println2(module_name, " State:");
	for (int i = 0; i < 3; i++) {
		if (pin_enabled[i]) {
			LOOM_DEBUG_Print3("\tNeo", i, ": ");
			LOOM_DEBUG_Print2( color_vals[i][0], ", ");
			LOOM_DEBUG_Print2( color_vals[i][1], ", ");
			LOOM_DEBUG_Println(color_vals[i][2]);
		}
	}
}


void Loom_Neopixel::package(OSCBundle& bndl, char* suffix) 
{

}


bool Loom_Neopixel::message_route(OSCMessage* msg, int address_offset) 
{
	// Set color
	// if ( msg.dispatch("/SetNeopixel", 	set_color, 	address_offset) ) return true;
	
	if ( msg->fullMatch( "/SetNeopixel" , address_offset) ) {
		set_color(msg);
		return true;
	}

	// Enable/Disable individual Neopixel



	return false;
}


void Loom_Neopixel::set_color(OSCMessage* msg)
{
	set_color( msg->getInt(0), msg->getInt(1), msg->getInt(2), msg->getInt(3), msg->getInt(4) );
}


void Loom_Neopixel::set_color( uint8_t port, uint8_t chain_num, uint8_t red, uint8_t green, uint8_t blue)
{
	if ( pin_enabled[port] ) {
		
		// Update color vars
		color_vals[port][0] = red;
		color_vals[port][1] = green;
		color_vals[port][2] = blue;

		// Apply color
		pixels[port]->setPixelColor(chain_num, pixels[port]->Color(red, green, blue));

		// Update colors displayed by Neopixel
		pixels[port]->show();

		LOOM_DEBUG_Print4("Set Neopixel on Port: ", port, ", Chain #: ", chain_num);
		LOOM_DEBUG_Print2(" to R: ", red);
		LOOM_DEBUG_Print2(  ", G: ", green);
		LOOM_DEBUG_Println2(", B: ", blue);
	} 

}

