
#ifndef LOOM_NEOPIXEL_h
#define LOOM_NEOPIXEL_h


#include "Loom_Actuator.h"

#include <Adafruit_NeoPixel.h>



class Loom_Neopixel : public LoomActuator
{

protected:

	/// Array of Neopixel instances to manage up to 3 Neopixels
	Adafruit_NeoPixel* pixels[3];

	/// Whether pins A0-A2 are enabled
	bool    pin_enabled[3];

	/// The associated color values (RGB) for each of the 3 Neopixels
	uint8_t color_vals[3][3];

public:
	
	// --- CONSTRUCTOR ---
	Loom_Neopixel( 	char* 	module_name 	= "Neopixel",

					bool 	enableA0		= false,
					bool 	enableA1		= false,
					bool 	enableA2		= true
				) ;

	// --- DESTRUCTOR ---
	virtual ~Loom_Neopixel();

	// General
	void print_config();
	void print_state();
	void package(OSCBundle& bndl, char* suffix="");
	bool message_route(OSCMessage& msg, int address_offset);

	// Actuator Control
	void enable_pin(uint8_t port, bool state);

	void set_color(OSCMessage& msg);
	void set_color(uint8_t port, uint8_t chain_num, uint8_t red, uint8_t green, uint8_t blue);


private:


};


#endif // of LOOM_NEOPIXEL_h
