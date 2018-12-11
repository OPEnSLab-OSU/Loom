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
#if oled_form_factor == 1 	// FeatherWing
	Adafruit_SSD1306 display;
#else 						// Breakout
	Adafruit_SSD1306 display(oled_reset_pin);
#endif



unsigned long oled_time = 0;
#if (is_button == 1) && (oled_button_freeze == 2)
	unsigned long previous_oled_time; 
#endif

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

	#if (oled_display_format == 3) && (oled_button_freeze > 0)
		pinMode(oled_freeze_pin, INPUT_PULLUP);
	#endif

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32) cannot be changed
	display.display();
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

	// If button press set to freeze display, don't do anything
	// #if (is_button == 1) && (oled_button_freeze == 1)
	// 	if (button_state == 0) return; 
	// #endif
	#if oled_button_freeze == 1
		if (digitalRead(oled_freeze_pin) == 0) return; 
	#endif

	// LOOM_DEBUG_Println2("Button: ", digitalRead(oled_freeze_pin));


	display.clearDisplay();
	display.setTextColor(WHITE);
	display.setTextSize(1);

	String keys[16], vals[16];

	int size = bundle_num_data_pairs(bndl);
	convert_bundle_to_arrays_assoc(bndl, keys, vals, 16);



	// Remove leading sections of address from multiplexer sections
	for (int i = 0; i < size; i++) {
		int count = osc_address_section_count(keys[i]);
		if (count > 0)  keys[i] = String( nth_strchr(keys[i].c_str(), '/', count)+1 );
	}


	// Display first 4 elements
	#if oled_display_format == 1
		for (int i = 0; i < 4 && i < size; i++) {
			display.setCursor(0, i*8);
			display.print(keys[i].substring(0,8));
			display.setCursor(64, i*8);
			display.print(vals[i].substring(0,8));
		}

	// Display first 8 elements
	#elif oled_display_format == 2
		for (int i = 0; i < 4 && i < size; i++) {
			display.setCursor(0, i*8);
			display.print(keys[i].substring(0,4));
			display.setCursor(32, i*8);
			display.print(vals[i].substring(0,4));
		}
		for (int i = 0; i < 4 && i < size; i++) {
			display.setCursor(64, i*8);
			display.print(keys[i+4].substring(0,4));
			display.setCursor(96, i*8);
			display.print(vals[i+4].substring(0,4));
		}

	// Display all elements with scrolling
	#elif oled_display_format == 3

		#if oled_button_freeze == 2
			if (digitalRead(oled_freeze_pin) == 0) {
				oled_time = previous_oled_time;
			} else {
				oled_time = millis();
				previous_oled_time = oled_time;
			}
		#else
			oled_time = millis();
		#endif


		int offset = size*( float(oled_time%(oled_scroll_duration*1000)) / (float)(oled_scroll_duration*1000) );

		for (int i = 0; i < 5; i++) {
			display.setCursor(0, i*8);
			display.print(keys[(i+offset)%size].substring(0,6));
			display.setCursor(64, i*8);
			display.print(vals[(i+offset)%size].substring(0,6));
		}
	#endif

	display.display();	
}
