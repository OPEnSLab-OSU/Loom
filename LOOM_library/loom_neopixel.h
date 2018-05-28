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
	int colorVals[3][3] = { {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
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
// Creates an array to store Neopixel data, iterates through to initialize Neopixels
// 
void setup_neopixel() 
{
	for(int i = 0; i < 3; i++) {
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
// Each message only holds one of R, G, B values
//
// @param msg  OSC message containing details of which Neopixel and what color)
//
void set_color(OSCMessage &msg) 
{
	int port     = msg.getInt(0);
	int pixelNum = msg.getInt(1);
	int red      = msg.getInt(2);
	int green    = msg.getInt(3);
	int blue     = msg.getInt(4);

	#if LOOM_DEBUG == 1
		Serial.println("========");
		Serial.print("Port: ");  Serial.print(port);
		Serial.print("  Num: "); Serial.print(pixelNum);
		Serial.print("  R: ");   Serial.print(red);
		Serial.print("  G: ");   Serial.print(green);
		Serial.print("  B: ");   Serial.println(blue);
		Serial.println("========\n");
	#endif

	// Update color values stored for specified Neopixel and write them to the Neopixels color
	if (state_neopixel.enabled[port]) {
		state_neopixel.colorVals[port][0] = red;
		state_neopixel.colorVals[port][1] = green;
		state_neopixel.colorVals[port][2] = blue;
		state_neopixel.pixels[port]->setPixelColor(pixelNum, 
										state_neopixel.pixels[port]->Color(
											state_neopixel.colorVals[port][0], 
											state_neopixel.colorVals[port][1], 
											state_neopixel.colorVals[port][2]));
	}

	// Update colors displayed by any enabled Neopixels
	for (int i = 0; i < 3; i++) {
		if (state_neopixel.enabled[i])
			state_neopixel.pixels[i]->show();
	}
}


