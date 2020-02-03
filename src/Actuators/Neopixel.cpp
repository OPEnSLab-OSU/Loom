///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Neopixel.cpp
/// @brief		File for Loom_Neopixel implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "Neopixel.h"


///////////////////////////////////////////////////////////////////////////////
Loom_Neopixel::Loom_Neopixel(
		LoomManager* 	manager,
		const bool					enableA0, 
		const bool					enableA1, 
		const bool					enableA2
	) 
	: LoomActuator(manager, "Neopixel", Type::Neopixel)
	, pin_enabled( {enableA0, enableA1, enableA2} )
	, pixels( { Adafruit_NeoPixel(1, 14, NEO_GRB + NEO_KHZ800),
				Adafruit_NeoPixel(1, 15, NEO_GRB + NEO_KHZ800),
				Adafruit_NeoPixel(1, 16, NEO_GRB + NEO_KHZ800)
			} ) 
	, color_vals{}
{
	// Set pin mode on enabled pins (pins A0-A5 = 14-19)
	for (auto i = 0; i < 3; i++) { 
		if (pin_enabled[i]) pinMode(14+i, OUTPUT);
	}

	// Initialize Neopixels
	for (auto i = 0; i < 3; i++) {
		if (pin_enabled[i]) {
			pixels[i].begin(); // This initializes the NeoPixel library.
			pixels[i].show();  // Initialize all pixels to 'off'
		}
	}		

	print_module_label();
	LPrintln("Setup");
} 

///////////////////////////////////////////////////////////////////////////////
Loom_Neopixel::Loom_Neopixel(LoomManager* manager, JsonArrayConst p)
	: Loom_Neopixel(manager, EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Neopixel::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name,
	// 	pin_enabled[0], pin_enabled[1], pin_enabled[2]
	// );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Neopixel::print_config() const
{
	LoomActuator::print_config();

	for (auto i = 0; i < 3; i++) {
		LPrintln("\tNeopixel ", i, ": ", (pin_enabled[i]) ? "Enabled" : "Disabled" );
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Neopixel::print_state() const
{
	LPrintln(module_name, " State:");
	for (auto i = 0; i < 3; i++) {
		if (pin_enabled[i]) {
			LPrint("\tNeo", i, ": ");
			LPrint( color_vals[i][0], ", ");
			LPrint( color_vals[i][1], ", ");
			LPrintln(color_vals[i][2]);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Neopixel::dispatch(JsonObject json)
{
	JsonArray params = json["params"];
	switch( (char)json["func"] ) {
		case 's': if (params.size() >= 5) { set_color( EXPAND_ARRAY(params, 5) ); } return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Neopixel::enable_pin(const uint8_t port, const bool state)
{
	pin_enabled[port] = state;
	if (state) {
		pinMode(14+port, OUTPUT);
	}

	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln("Neopixel ", (state) ? "enabled" : "disabled", " on port ", port);
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Neopixel::set_color(const uint8_t port, const uint8_t chain_num, const uint8_t red, const uint8_t green, const uint8_t blue)
{
	if ( pin_enabled[port] ) {
		
		// Update color vars
		color_vals[port][0] = (red > 0)   ? ( (red < 255)   ? red   : 255 ) : 0;
		color_vals[port][1] = (green > 0) ? ( (green < 255) ? green : 255 ) : 0;
		color_vals[port][2] = (blue > 0)  ? ( (blue < 255)  ? blue  : 255 ) : 0;

		// Apply color
		pixels[port].setPixelColor(chain_num, pixels[port].Color(color_vals[port][0], color_vals[port][1], color_vals[port][2]));

		// Update colors displayed by Neopixel
		pixels[port].show();

		if (print_verbosity == Verbosity::V_HIGH) {
			LPrint("Set Neopixel on Port: ", port, ", Chain #: ", chain_num);
			LPrint(" to R: ", color_vals[port][0]);
			LPrint(  ", G: ", color_vals[port][1]);
			LPrintln(", B: ", color_vals[port][2]);			
		}
	} else {
		if (print_verbosity == Verbosity::V_HIGH) {
			LPrintln("Neopixel not enabled on port ", port);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

