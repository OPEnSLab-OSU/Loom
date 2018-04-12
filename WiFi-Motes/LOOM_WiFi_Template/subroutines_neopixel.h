#include <Adafruit_NeoPixel.h>


// Neopixel Data Structures (one per Ishield port)
Adafruit_NeoPixel * pixels[3];

// Based on config.h, enable Neopixel for specified ports
bool pixel_enabled[3] = {NEO_0, NEO_1, NEO_2};

// Store RGB vals for up to 1 pixel per port on Ishield
int colorVals[3][3] = { {0, 0, 0},
                        {0, 0, 0},
                        {0, 0, 0}};



// Function called by setup()
void neopixel_setup() 
{
  for(int i = 0; i < 3; i++) {
    if (pixel_enabled[i]) {
      pixels[i] = new Adafruit_NeoPixel(1, 14+i, NEO_GRB + NEO_KHZ800);
      pixels[i]->begin(); // This initializes the NeoPixel library.
      pixels[i]->show();  // Initialize all pixels to 'off'
    }
  }
}


// Set Color
// Handle OSC messages to set specified Neopixel to a given color
void setColor(OSCMessage &msg) 
{
  int port     = msg.getInt(0);
  int pixelNum = msg.getInt(1);
  int color    = msg.getInt(2);
  int val      = msg.getInt(3);

  #if DEBUG == 1
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
  if (pixel_enabled[port]) {
    colorVals[port][color] = val;
    pixels[port]->setPixelColor(pixelNum, pixels[port]->Color(colorVals[port][0], colorVals[port][1], colorVals[port][2]));
  }

  // Update colors displayed by enabled Neopixels
  for (int i = 0; i < 3; i++) {
    if (pixel_enabled[i])
      pixels[i]->show();
  }
}

