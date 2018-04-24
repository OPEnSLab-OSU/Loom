// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Adafruit_NeoPixel.h>

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct state_neopixel_t {
  // Neopixel Data Structures (one per Ishield port)
  Adafruit_NeoPixel pixels[3];
  
  // Based on config.h, enable Neopixel for specified ports
  bool enabled[3] = {NEO_0, NEO_1, NEO_2};
  
  // Store RGB vals for up to 1 pixel per port on Ishield
  int colorVals[3][3] = { {0, 0, 0},
                          {0, 0, 0},
                          {0, 0, 0}};
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct state_neopixel_t state_neopixel;



// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_neopixel();
void setColor(OSCMessage &msg);


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================

// --- NEOPIXEL SETUP ---
// Called by main setup
// Creates an array to store Neopixel data, iterates through to initialize Neopixels
// Arguments: none
// Return:    none
void setup_neopixel() 
{

  
//  state_neopixel.pixels[0] = Adafruit_NeoPixel(1, 14, NEO_GRB + NEO_KHZ800);
//  state_neopixel.pixels[1] = Adafruit_NeoPixel(1, 15, NEO_GRB + NEO_KHZ800);
//  state_neopixel.pixels[2] = Adafruit_NeoPixel(1, 16, NEO_GRB + NEO_KHZ800);

  for(int i = 0; i < 3; i++) {
    if (state_neopixel.enabled[i]) {
//      Serial.println("REACHED C");
      state_neopixel.pixels[i] = Adafruit_NeoPixel(1, 14+i, NEO_GRB + NEO_KHZ800);
//      Serial.println("REACHED D");
      state_neopixel.pixels[i].begin(); // This initializes the NeoPixel library.
//      Serial.println("REACHED E");
      state_neopixel.pixels[i].show();  // Initialize all pixels to 'off'
    }
  }
//  Serial.println("REACHED F");
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================

// --- SET COLOR ---
// Handle OSC messages to set specified Neopixel to a given color
// Arguments: msg (OSC message containing details of which Neopixel and what color)
// Return:    none
void setColor(OSCMessage &msg) 
{
  int port     = msg.getInt(0);
  int pixelNum = msg.getInt(1);
  int color    = msg.getInt(2);
  int val      = msg.getInt(3);

  #if LOOM_DEBUG == 1
    Serial.println("========");
    Serial.print("Port: ");
    Serial.print(port);
    Serial.print("  Num: ");
    Serial.print(pixelNum);
    Serial.print("  Color: ");
    Serial.print(color);
    Serial.print("  Val: ");
    Serial.println(val);
    Serial.println("========\n");
  #endif

  // Update color values stored for specified Neopixel and write them to the Neopixels color
  if (state_neopixel.enabled[port]) {
    state_neopixel.colorVals[port][color] = val;
    state_neopixel.pixels[port].setPixelColor(pixelNum, 
                                                state_neopixel.pixels[port].Color(
                                                  state_neopixel.colorVals[port][0], 
                                                  state_neopixel.colorVals[port][1], 
                                                  state_neopixel.colorVals[port][2]));
  }

  // Update colors displayed by enabled Neopixels
  for (int i = 0; i < 3; i++) {
    if (state_neopixel.enabled[i])
      state_neopixel.pixels[i].show();
  }
}


