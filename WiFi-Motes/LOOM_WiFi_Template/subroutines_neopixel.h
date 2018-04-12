

// --- NEOPIXEL SETUP ---
// Called by main setup
// Arguments: none
// Return:    none
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



// --- SET COLOR ---
// Handle OSC messages to set specified Neopixel to a given color
// Arguments:
// Return:
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

