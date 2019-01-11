
#ifndef LOOM_NEOPIXEL_h
#define LOOM_NEOPIXEL_h


#include "loom_actuator_class.h"

#include <Adafruit_NeoPixel.h>
#include <OSCBundle.h>



// void set_color(OSCMessage& msg)
// // // void set_color( uint8_t port, uint8_t chain_num, uint8_t red, uint8_t green, uint8_t blue)
// {
	
// }
// 

// 	bool message_route(OSCMessage& msg, int address_offset) 
// 	{
// 		// Set color
// 		if ( msg.dispatch("/SetNeopixel", 	set_color, 	address_offset) ) return true;
		
// 		// Enable/Disable individual Neopixel


// 		return false;
// 	}





class Neopixel : public LoomActuator
{

protected:
	// --- PROTECTED MEMBERS ---
	
	Adafruit_NeoPixel* pixels[3];

	// Pins A0-A2
	bool    pin_enabled[3];
	uint8_t color_vals[3][3]; 	// Store RGB vals for up to 1 pixel per port on Ishield


public:
	// --- CONSTRUCTOR ---
	Neopixel( 	char* 	module_name 	= "Neopixel",

				bool 	enableA0		= false,
				bool 	enableA1		= false,
				bool 	enableA2		= true

			) ;

	// --- DESTRUCTOR ---
	~Neopixel();


	// --- PUBLIC METHODS ---

	void print_config();

	void print_state();


	void package(OSCBundle* bndl, char* id_prefix);

	bool message_route(OSCMessage* msg, int address_offset);

	void set_color(OSCMessage* msg);

	void set_color( uint8_t port, uint8_t chain_num, uint8_t red, uint8_t green, uint8_t blue);


private:

	// will be called by msg_route
	// void set_color(OSCMessage &msg) 

};


#endif