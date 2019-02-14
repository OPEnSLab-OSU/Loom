
#ifndef LOOM_NEOPIXEL_h
#define LOOM_NEOPIXEL_h


#include "Loom_Actuator.h"

#include <Adafruit_NeoPixel.h>
#include <OSCBundle.h>



class Loom_Neopixel : public LoomActuator
{

protected:

	Adafruit_NeoPixel* pixels[3];

	// Pins A0-A2
	bool    pin_enabled[3];
	uint8_t color_vals[3][3]; 	// Store RGB vals for up to 1 pixel per port on Ishield


public:
	
	// --- CONSTRUCTOR ---
	Loom_Neopixel( 	char* 	module_name 	= "Neopixel",

					bool 	enableA0		= false,
					bool 	enableA1		= false,
					bool 	enableA2		= true
			) ;

	// --- DESTRUCTOR ---
	virtual ~Loom_Neopixel();

	void print_config();
	void print_state();


	void package(OSCBundle* bndl, char* suffix="");

	bool message_route(OSCMessage* msg, int address_offset);

	void set_color(OSCMessage* msg);
	void set_color( uint8_t port, uint8_t chain_num, uint8_t red, uint8_t green, uint8_t blue);


private:


};


#endif