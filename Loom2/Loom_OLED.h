
#ifndef LOOM_OLED_h
#define LOOM_OLED_h


#include "Loom_LogPlat.h"

#include <OSCBundle.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


enum OLED_Version { FEATHERWING, BREAKOUT };
enum OLED_Format  { OLED_4, OLED_8, OLED_SCROLL };
enum OLED_Freeze  { FREEZE_DISABLE, FREEZE_DATA, FREEZE_SCROLL };



class Loom_OLED : public LoomLogPlat
{

protected:

		Adafruit_SSD1306* display;

		OLED_Version 	version;
		byte 			reset_pin;

		OLED_Format 	display_format;
		uint 			scroll_duration; 

		byte 			freeze_pin;
		OLED_Freeze 	freeze_behavior;

		// Something to manage the buttons, or should that be done externally?

public:

	static char* enum_oled_version_string(OLED_Version v);
	static char* enum_oled_format(OLED_Format f);
	static char* enum_oled_freeze(OLED_Freeze f);


	// --- CONSTRUCTOR ---
	Loom_OLED(	char* 			module_name 		= "OLED",

				bool 			enable_rate_filter 	= true,
				uint 			min_filter_delay 	= 300,

				OLED_Version 	version 			= FEATHERWING,
				byte 			reset_pin 			= A2,
				OLED_Format 	display_format 		= OLED_SCROLL,
				uint  			scroll_duration 	= 6000,
				byte 			freeze_pin 			= 10, 				// Common pins might be: 10 (Ishield); 9(A), 6(B), 5(C) (OLED Featherwing buttons)
				OLED_Freeze 	freeze_behavior 	= FREEZE_SCROLL

			   );

	// --- DESTRUCTOR ---
	virtual ~Loom_OLED();

	// --- PUBLIC METHODS ---

	void print_config();

	void set_display_format(OLED_Format format);
	OLED_Format get_display_format();

	void set_scroll_duration(uint duration);
	uint get_scroll_duration();

	void set_freeze_pin(byte pin);
	byte get_freeze_pin();

	void set_freeze_behavior(OLED_Freeze behavior);
	OLED_Freeze get_freeze_behavior();

	void log_bundle(OSCBundle* bndl);
	void log_bundle(OSCBundle* bndl, OLED_Format format);


private:
	
	unsigned long previous_time; // Used to manage scrolling

};



#endif