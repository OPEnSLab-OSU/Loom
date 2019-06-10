
#include "Loom_Neopixel.h"

#include <Adafruit_NeoPixel.h>


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_Neopixel::Loom_Neopixel(
		const char*		module_name, 
		bool			enableA0, 
		bool			enableA1, 
		bool			enableA2
	) 
	: LoomActuator(module_name)
{
	this->module_type = ModuleType::Neopixel;

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
	LPrintln("Setup");
} 

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_Neopixel::Loom_Neopixel(JsonVariant p)
	: Loom_Neopixel( EXPAND_ARRAY(p, 4) )
{

}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_Neopixel::~Loom_Neopixel() 
{
	for (int i = 0; i < 3; i++) {
		if (pixels[i] != NULL) {
			delete pixels[i];
		}
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Neopixel::print_config() 
{
	LoomActuator::print_config();

	for (int i = 0; i < 3; i++) {
		LPrintln("\tNeopixel ", i, "          : ", (pin_enabled[i]) ? "Enabled" : "Disabled" );
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Neopixel::print_state()
{
	LPrintln(module_name, " State:");
	for (int i = 0; i < 3; i++) {
		if (pin_enabled[i]) {
			LPrint("\tNeo", i, ": ");
			LPrint( color_vals[i][0], ", ");
			LPrint( color_vals[i][1], ", ");
			LPrintln(color_vals[i][2]);
		}
	}
}

/////////////////////////////////////////////////////////////////////
bool Loom_Neopixel::cmd_route(JsonObject json)
{
	if ( strcmp(json["module"], module_name) == 0 ) {
		JsonArray params = json["params"];
		return functionRoute(
			json["func"],
			"set_color", [this, params]() { if (params.size() >= 5) { set_color( EXPAND_ARRAY(params, 5) ); } else { LPrintln("Not enough parameters"); } } 
		);
	} else {
		return false;
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Neopixel::enable_pin(uint8_t port, bool state)
{
	pin_enabled[port] = state;
	if (state) {
		pinMode(14+port, OUTPUT);
	}

	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln("Neopixel ", (state) ? "enabled" : "disabled", " on port ", port);
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Neopixel::set_color( uint8_t port, uint8_t chain_num, uint8_t red, uint8_t green, uint8_t blue)
{
	if ( pin_enabled[port] ) {
		
		// Update color vars
		color_vals[port][0] = (red > 0)   ? ( (red < 255)   ? red   : 255 ) : 0;
		color_vals[port][1] = (green > 0) ? ( (green < 255) ? green : 255 ) : 0;
		color_vals[port][2] = (blue > 0)  ? ( (blue < 255)  ? blue  : 255 ) : 0;

		// Apply color
		pixels[port]->setPixelColor(chain_num, pixels[port]->Color(color_vals[port][0], color_vals[port][1], color_vals[port][2]));

		// Update colors displayed by Neopixel
		pixels[port]->show();

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