///////////////////////////////////////////////////////////////////////////////
///
/// @file		Neopixel.cpp
/// @brief		File for Neopixel implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_ACTUATORS

#include "Neopixel.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
Neopixel::Neopixel(
		const bool					enableA0,
		const bool					enableA1,
		const bool					enableA2
	)
	: Actuator("Neopixel")
	, pin_enabled( {enableA0, enableA1, enableA2} )
	, pixels( { Adafruit_NeoPixel(1, 14, NEO_GRB + NEO_KHZ800),
				Adafruit_NeoPixel(1, 15, NEO_GRB + NEO_KHZ800),
				Adafruit_NeoPixel(1, 16, NEO_GRB + NEO_KHZ800)
			} )
	, color_vals{}
{
  LMark;
	// Set pin mode on enabled pins (pins A0-A5 = 14-19)
	for (auto i = 0; i < 3; i++) {
   	LMark;
		if (pin_enabled[i]) pinMode(14+i, OUTPUT);
  	LMark;
	}

	// Initialize Neopixels
	for (auto i = 0; i < 3; i++) {
   	LMark;
		if (pin_enabled[i]) {
    	LMark;
			pixels[i].begin(); // This initializes the NeoPixel library.
    	LMark;
			pixels[i].show();  // Initialize all pixels to 'off'
   		LMark;
		}
	}

	print_module_label();
  LMark;
	LPrintln("Setup");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Neopixel::Neopixel(JsonArrayConst p)
	: Neopixel(EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void Neopixel::print_config() const
{
	LMark;
	Actuator::print_config();
  LMark;

	for (auto i = 0; i < 3; i++) {
   	LMark;
		LPrintln("\tNeopixel ", i, ": ", (pin_enabled[i]) ? "Enabled" : "Disabled" );
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Neopixel::print_state() const
{
  LMark;
	LPrintln(module_name, " State:");
  LMark;
	for (auto i = 0; i < 3; i++) {
   	LMark;
		if (pin_enabled[i]) {
    	LMark;
			LPrint("\tNeo", i, ": ");
    	LMark;
			LPrint( color_vals[i][0], ", ");
    	LMark;
			LPrint( color_vals[i][1], ", ");
    	LMark;
			LPrintln(color_vals[i][2]);
   		LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Neopixel::dispatch(JsonObject json)
{
  LMark;
	JsonArray params = json["params"];
  LMark;
	switch( (char)json["func"] ) {
		case 's': if (params.size() >= 5) { set_color( EXPAND_ARRAY(params, 5) ); } return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void Neopixel::enable_pin(const uint8_t port, const bool state)
{
  LMark;
	pin_enabled[port] = state;
  LMark;
	if (state) {
   	LMark;
		pinMode(14+port, OUTPUT);
  	LMark;
	}

	if (print_verbosity == Verbosity::V_HIGH) {
   	LMark;
		LPrintln("Neopixel ", (state) ? "enabled" : "disabled", " on port ", port);
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Neopixel::set_color(const uint8_t port, const uint8_t chain_num, const uint8_t red, const uint8_t green, const uint8_t blue)
{
  LMark;
	if ( pin_enabled[port] ) {
    LMark;

		// Update color vars
		color_vals[port][0] = (red > 0)   ? ( (red < 255)   ? red   : 255 ) : 0;
   	LMark;
		color_vals[port][1] = (green > 0) ? ( (green < 255) ? green : 255 ) : 0;
   	LMark;
		color_vals[port][2] = (blue > 0)  ? ( (blue < 255)  ? blue  : 255 ) : 0;
  	LMark;

		// Apply color
		pixels[port].setPixelColor(chain_num, pixels[port].Color(color_vals[port][0], color_vals[port][1], color_vals[port][2]));
  	LMark;

		// Update colors displayed by Neopixel
		pixels[port].show();
  	LMark;

		if (print_verbosity == Verbosity::V_HIGH) {
    	LMark;
			LPrint("Set Neopixel on Port: ", port, ", Chain #: ", chain_num);
    	LMark;
			LPrint(" to R: ", color_vals[port][0]);
    	LMark;
			LPrint(  ", G: ", color_vals[port][1]);
    	LMark;
			LPrintln(", B: ", color_vals[port][2]);
   		LMark;
		}
	} else {
		if (print_verbosity == Verbosity::V_HIGH) {
    	LMark;
			LPrintln("Neopixel not enabled on port ", port);
   		LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_ACTUATORS
