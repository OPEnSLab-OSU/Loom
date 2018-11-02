// ================================================================ 
// ===                          NOTES                           === 
// ================================================================


// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
Adafruit_SSD1306 display(oled_reset_pin);


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void oled_display_bundle(OSCBundle* log_bndl);


// ================================================================
// ===                          SETUP                           ===
// ================================================================


// --- SETUP OLED ---
//
// Handles configuration of reading device configuration settings from flash (if config is saved)
// else uses config.h specified settings, and then writes these to flash
// 
void setup_oled()
{
	LOOM_DEBUG_Print("Initializing OLED...");

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
	display.display();
	// delay(1000);
	display.clearDisplay();

	LOOM_DEBUG_Println("done.");
}



// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================

// add option to interface to select type of display
//  - first 8 args
//  - scroll


void oled_display_bundle(OSCBundle* bndl)
{
	if (bundle_empty(bndl)) return;

	String keys[16], vals[16];

	// int size = bndl->getOSCMessage(0)->size();

	int size = 10;

	// OSCBundle tmpBndl;
	// deep_copy_bundle(bndl, &tmpBndl);
	// convert_bundle_structure(&tmpBndl, SINGLEMSG);

	convert_bundle_to_arrays_assoc(bndl, keys, vals, 16);

	// print_message(tmpBndl.getOSCMessage(0));

	// int size = tmpBndl.getOSCMessage(0)->size();
	// int size = tmpBndl.size();
	LOOM_DEBUG_Println2("Size: ", size);


	display.clearDisplay();
	display.setTextColor(WHITE);
	display.setTextSize(1);

	unsigned long time = millis();
	LOOM_DEBUG_Println2("Time: ", time);


	int rate = 4;
	int full_loop = 15000;
	// or time per full loop

	int total_offset = ( float(time%full_loop)/(float)full_loop )*(8)*(size);
	LOOM_DEBUG_Println2("Total Offset: ", total_offset);
	// range from 0 to 16 [-4 as 4 will be shown] (or number of data points)

	// ideally smooth scroll
	//  shift the y coordinate back
	//  shift array access up

	// have some sort of scroll rate

	// for (int i = 0; i < 5; i++) {
	// 	display.setCursor(0, i*8);
	// 	display.print(keys[i].substring(0,5));
	// 	display.setCursor(32, i*8);
	// 	display.print(vals[i].substring(0,4));
	// }

	for (int i = 0; i < 16; i++) {
		display.setCursor(0, i*8-total_offset);
		display.print(keys[i].substring(0,5));
		display.setCursor(64, i*8-total_offset);
		display.print(vals[i].substring(0,5));
	}

	// for (int i = 4; i < 8; i++) {
	// 	display.setCursor(64, (i-4)*8);
	// 	display.print(keys[i].substring(0,4));
	// 	display.setCursor(96, (i-4)*8);
	// 	display.print(vals[i].substring(0,4));
	// }


  // display.startscrollleft(0x00, 0xFF);
  // delay(4000);
  // display.stopscroll();

	// display.print(millis());
	display.display();	
}
