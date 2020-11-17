///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_OLED.cpp
/// @brief		File for Loom_OLED implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "OLED.h"

#include <Adafruit_GFX.h>


///////////////////////////////////////////////////////////////////////////////
const char* Loom_OLED::enum_oled_version_string(const Version v)
{
  LMark;
	switch(v) {
		case Version::FEATHERWING : return "FeatherWing";
		case Version::BREAKOUT    : return "Breakout";
	}
}

///////////////////////////////////////////////////////////////////////////////
const char* Loom_OLED::enum_oled_format_string(const Format f)
{
  LMark;
	switch(f) {
		case Format::FOUR   : return "OLED 4 Elements";
		case Format::EIGHT  : return "OLED 8 Elements";
		case Format::SCROLL : return "OLED Scroll";
	}
}

///////////////////////////////////////////////////////////////////////////////
const char* Loom_OLED::enum_oled_freeze_string(const FreezeType f)
{
  LMark;
	switch(f) {
		case FreezeType::DISABLE : return "Freeze Disabled";
		case FreezeType::DATA    : return "Freeze Data";
		case FreezeType::SCROLL  : return "Freeze Scroll";
	}
}

///////////////////////////////////////////////////////////////////////////////
Loom_OLED::Loom_OLED(
		LoomManager* manager,
		const bool			enable_rate_filter,
		const uint16_t		min_filter_delay,
		const Version		version,
		const byte			reset_pin,
		const Format		display_format,
		const uint16_t		scroll_duration,
		const byte			freeze_pin,
		const FreezeType	freeze_behavior
	)
	: LoomLogPlat(manager, "OLED", Type::OLED, enable_rate_filter, min_filter_delay )
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
  LMark;
	if (freeze_behavior != FreezeType::DISABLE) {
    LMark;
		pinMode(freeze_pin, INPUT_PULLUP);
    LMark;
	}

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32) cannot be changed
  LMark;
	display.display();
  LMark;
	display.clearDisplay();
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_OLED::Loom_OLED(LoomManager* manager, JsonArrayConst p)
	: Loom_OLED(manager, p[0], p[1], (Version)(int)p[2], p[3], (Format)(int)p[4], p[5], p[6], (FreezeType)(int)p[7]) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_OLED::print_config() const
{
  LMark;
	LoomLogPlat::print_config();
  LMark;

	LPrintln("\tOLED Version        : ", enum_oled_version_string(version) );
  LMark;
	if (version == Version::BREAKOUT) {
    LMark;
		LPrintln("\tReset Pin           : ", reset_pin );
    LMark;
	}

	LPrintln("\tDisplay Format      : ", enum_oled_format_string(display_format) );
  LMark;
	if (display_format == Format::SCROLL) {
    LMark;
		LPrintln("\tScroll Duration     : ", scroll_duration );
    LMark;
	}
	LPrintln("\tFreeze Behavior     : ", enum_oled_freeze_string(freeze_behavior) );
  LMark;
	if (freeze_behavior != FreezeType::DISABLE) {
    LMark;
		LPrintln("\tFreeze Pin          : ", freeze_pin );
    LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_OLED::set_freeze_pin(const byte pin)
{
  LMark;
	freeze_pin = pin;
  LMark;
	if (freeze_behavior != FreezeType::DISABLE) {
    LMark;
		pinMode(freeze_pin, INPUT_PULLUP);
    LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_OLED::log(JsonObject json)
{
  LMark;
	if ( !check_millis() )
		return false;
  LMark;

	// If freeze complete, check button if no display update needed
	if (freeze_behavior == FreezeType::DATA) {
    LMark;
		if (digitalRead(freeze_pin) == 0)
			return false;
    LMark;
	}

  LMark;
	display.clearDisplay();
  LMark;
	display.setTextColor(WHITE);
  LMark;
	display.setTextSize(1);
  LMark;

	// Structure Json to parse
	flatten_json_data_object(json);
  LMark;

	// Get associated array size
	JsonObject data = json["flatObj"];
  LMark;
	int size = data.size();
  LMark;
	String keys[size], vals[size];
  LMark;

	int i = 0;
  LMark;
	for (auto kv : data) {
    LMark;
		keys[i] = kv.key().c_str();
    LMark;
		vals[i] = kv.value().as<String>();
    LMark;
		i++;
    LMark;
	}

	switch (display_format) {
		case Format::FOUR:

			for (int i = 0; i < 4 && i < size; i++) {
        LMark;
				display.setCursor(0, i*8);
        LMark;
				display.print(keys[i].substring(0,8));
        LMark;
				display.setCursor(64, i*8);
        LMark;
				display.print(vals[i].substring(0,8));
        LMark;
			}
			break;

		case Format::EIGHT:

			for (int i = 0; i < 4 && i < size; i++) {
        LMark;
				display.setCursor(0, i*8);
        LMark;
				display.print(keys[i].substring(0,4));
        LMark;
				display.setCursor(32, i*8);
        LMark;
				display.print(vals[i].substring(0,4));
        LMark;
			}
			for (int i = 0; i < 4 && i < size; i++) {
        LMark;
				display.setCursor(64, i*8);
        LMark;
				display.print(keys[i+4].substring(0,4));
        LMark;
				display.setCursor(96, i*8);
        LMark;
				display.print(vals[i+4].substring(0,4));
        LMark;
			}
			break;

		case Format::SCROLL:

			unsigned long time;
      LMark;
			if (freeze_behavior == FreezeType::SCROLL) {
        LMark;
				if (digitalRead(freeze_pin) == 0) {
          LMark;
					time = previous_time;
          LMark;
				} else {
					time = millis();
          LMark;
					previous_time = time;
          LMark;
				}
			} else {
				time = millis();
        LMark;
			}

			int offset = size*( float(time%(scroll_duration)) / (float)(scroll_duration) );
      LMark;

			for (int i = 0; i < 5; i++) {
        LMark;
				display.setCursor(0, i*8);
        LMark;
				display.print(keys[(i+offset)%size].substring(0,8));
        LMark;
				display.setCursor(64, i*8);
        LMark;
				display.print(vals[(i+offset)%size].substring(0,8));
        LMark;
			}

			break;
	}

	// Update display
	display.display();
  LMark;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
