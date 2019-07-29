
#include "Loom_OLED.h"

#include <Adafruit_GFX.h>


///////////////////////////////////////////////////////////////////////////////
const char* Loom_OLED::enum_oled_version_string(Version v)
{
	switch(v) {
		case Version::FEATHERWING : return "FeatherWing";
		case Version::BREAKOUT    : return "Breakout";
	}
}

///////////////////////////////////////////////////////////////////////////////
const char* Loom_OLED::enum_oled_format_string(Format f)
{
	switch(f) {
		case Format::FOUR   : return "OLED 4 Elements";
		case Format::EIGHT  : return "OLED 8 Elements";
		case Format::SCROLL : return "OLED Scroll"; 
	}
}

///////////////////////////////////////////////////////////////////////////////
const char* Loom_OLED::enum_oled_freeze_string(FreezeType f)
{
	switch(f) {
		case FreezeType::DISABLE : return "Freeze Disabled";
		case FreezeType::DATA    : return "Freeze Data";
		case FreezeType::SCROLL  : return "Freeze Scroll";
	}
}

///////////////////////////////////////////////////////////////////////////////
Loom_OLED::Loom_OLED(	
		bool			enable_rate_filter, 
		uint16_t		min_filter_delay, 
		Version			version, 
		byte			reset_pin, 
		Format			display_format, 
		uint16_t		scroll_duration, 
		byte			freeze_pin, 
		FreezeType		freeze_behavior
	)
	: LoomLogPlat( "OLED", Type::OLED, enable_rate_filter, min_filter_delay )
	, version(version)
	, reset_pin(reset_pin)
	, display_format(display_format)
	, scroll_duration(scroll_duration)
	, freeze_behavior(freeze_behavior)
	, freeze_pin(freeze_pin)
	, display( (version == Version::FEATHERWING)
				? Adafruit_SSD1306()
				: Adafruit_SSD1306(reset_pin)
			 )
{
	if (freeze_behavior != FreezeType::DISABLE) {
		pinMode(freeze_pin, INPUT_PULLUP);
	}

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32) cannot be changed
	display.display();
	display.clearDisplay();
}

///////////////////////////////////////////////////////////////////////////////
Loom_OLED::Loom_OLED(JsonArrayConst p)
	: Loom_OLED(p[0], p[1], (Version)(int)p[2], p[3], (Format)(int)p[4], p[5], p[6], (FreezeType)(int)p[7]) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_OLED::print_config() 
{
	LoomLogPlat::print_config();

	LPrintln("\tOLED Version        : ", enum_oled_version_string(version) );
	if (version == Version::BREAKOUT) {
		LPrintln("\tReset Pin           : ", reset_pin );		
	}

	LPrintln("\tDisplay Format      : ", enum_oled_format_string(display_format) );
	if (display_format == Format::SCROLL) {
		LPrintln("\tScroll Duration     : ", scroll_duration );
	}
	LPrintln("\tFreeze Behavior     : ", enum_oled_freeze_string(freeze_behavior) );
	if (freeze_behavior != FreezeType::DISABLE) {
		LPrintln("\tFreeze Pin          : ", freeze_pin );
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_OLED::set_freeze_pin(byte pin) 
{
	freeze_pin = pin;
	if (freeze_behavior != FreezeType::DISABLE) {
		pinMode(freeze_pin, INPUT_PULLUP);
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_OLED::log(JsonObject json) 
{
	if ( !check_millis() ) return;
	
	// If freeze complete, check button if no display update needed
	if (freeze_behavior == FreezeType::DATA) {
		if (digitalRead(freeze_pin) == 0) return; 			
	}

	display.clearDisplay();
	display.setTextColor(WHITE);
	display.setTextSize(1);

	// Structure Json to parse
	flatten_json_data_object(json);

	// Get associated array size
	JsonObject data = json["flatObj"];
	int size = data.size();
	String keys[size], vals[size];

	int i = 0;
	for (auto kv : data) {
		keys[i] = kv.key().c_str();
		vals[i] = kv.value().as<String>();
		i++;
	}

	switch (display_format) {
		case Format::FOUR:

			for (int i = 0; i < 4 && i < size; i++) {
				display.setCursor(0, i*8);
				display.print(keys[i].substring(0,8));
				display.setCursor(64, i*8);
				display.print(vals[i].substring(0,8));
			}
			break;

		case Format::EIGHT:

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
			break;

		case Format::SCROLL:

			unsigned long time;
			if (freeze_behavior == FreezeType::SCROLL) {
				if (digitalRead(freeze_pin) == 0) {
					time = previous_time;
				} else {
					time = millis();
					previous_time = time;
				}		
			} else {
				time = millis();
			}

			int offset = size*( float(time%(scroll_duration)) / (float)(scroll_duration) );

			for (int i = 0; i < 5; i++) {
				display.setCursor(0, i*8);
				display.print(keys[(i+offset)%size].substring(0,8));
				display.setCursor(64, i*8);
				display.print(vals[(i+offset)%size].substring(0,8));
			}

			break;
	}

	// Update display
	display.display();	
}

///////////////////////////////////////////////////////////////////////////////

