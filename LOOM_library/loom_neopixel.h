// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Adafruit_NeoPixel.h>


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct state_neopixel_t {
	// Neopixel Data Structures (one per Ishield port)
	Adafruit_NeoPixel * pixels[3];
	
	// Based on config.h, enable Neopixel for specified ports
	bool enabled[3] = {NEO_0, NEO_1, NEO_2};
	
	// Store RGB vals for up to 1 pixel per port on Ishield
	int colorVals[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0} };
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct state_neopixel_t state_neopixel;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_neopixel();
void set_color(OSCMessage &msg);


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Called by main setup
// Creates an array to store Neopixel data,
// iterates through to initialize Neopixels
// 
void setup_neopixel() 
{

	// Plan to set digital output here on relevant pins

	for (int i = 0; i < 3; i++) {
		if (state_neopixel.enabled[i]) {
			state_neopixel.pixels[i] = new Adafruit_NeoPixel(1, 14+i, NEO_GRB + NEO_KHZ800);
			state_neopixel.pixels[i]->begin(); // This initializes the NeoPixel library.
			state_neopixel.pixels[i]->show();  // Initialize all pixels to 'off'
		}
	}
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================

// --- SET COLOR ---
//
// Handle OSC messages to set specified Neopixel to a given color
// Each message holds the port of the Neopixel being targetting,
// which number is in a chain, and R, G, and B values from 0 to 255 
//
// @param msg  OSC message containing details of which Neopixel and what color to set it to
//
void set_color(OSCMessage &msg) 
{
	int port     = msg.getInt(0); // 0-2 corresponding to A0-A2
	int pixelNum = msg.getInt(1); // Which number in a daisy chain, starting at 0
	int red      = msg.getInt(2); // Val 0-255
	int green    = msg.getInt(3); // Val 0-255
	int blue     = msg.getInt(4); // Val 0-255

	// Print received values if debug enabled
	LOOM_DEBUG_Println("========");
	LOOM_DEBUG_Print2(  "Port: " , port);
	LOOM_DEBUG_Print2(  "  Num: ", pixelNum);
	LOOM_DEBUG_Print2(  "  R: "  , red);
	LOOM_DEBUG_Print2(  "  G: "  , green);
	LOOM_DEBUG_Println2("  B:"   , blue);
	LOOM_DEBUG_Println("========\n");

	// Update color values stored for specified Neopixel and write them to the Neopixels color
	if (state_neopixel.enabled[port]) {
		state_neopixel.colorVals[port][0] = red;
		state_neopixel.colorVals[port][1] = green;
		state_neopixel.colorVals[port][2] = blue;

		state_neopixel.pixels[port]->setPixelColor(pixelNum, 
			state_neopixel.pixels[port]->Color(red, green, blue));
	} 

	// Update colors displayed by any enabled Neopixels
	for (int i = 0; i < 3; i++) {
		if (state_neopixel.enabled[i])
			state_neopixel.pixels[i]->show();
	}
}









