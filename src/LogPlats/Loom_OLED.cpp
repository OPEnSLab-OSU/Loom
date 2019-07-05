
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
		const char*		module_name, 
		bool			enable_rate_filter, 
		uint			min_filter_delay, 
		Version			version, 
		byte			reset_pin, 
		Format			display_format, 
		uint			scroll_duration, 
		byte			freeze_pin, 
		FreezeType		freeze_behavior
	)
	: LoomLogPlat( module_name, enable_rate_filter, min_filter_delay )
{
	this->module_type = LoomModule::Type::OLED;

	this->version 	      = version;
	this->reset_pin       = reset_pin;
	this->display_format  = display_format;
	this->scroll_duration = scroll_duration;
	this->freeze_behavior = freeze_behavior;
	this->freeze_pin      = freeze_pin;

	switch(version) {
		case Version::FEATHERWING : display = new Adafruit_SSD1306();
		case Version::BREAKOUT    : display = new Adafruit_SSD1306(reset_pin);
	}

	if (freeze_behavior != FreezeType::DISABLE) {
		pinMode(freeze_pin, INPUT_PULLUP);
	}

	display->begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32) cannot be changed
	display->display();
	display->clearDisplay();
}

///////////////////////////////////////////////////////////////////////////////
Loom_OLED::Loom_OLED(JsonArrayConst p)
	: Loom_OLED(p[0], p[1], p[2], (Version)(int)p[3], p[4], (Format)(int)p[5], p[6], p[7], (FreezeType)(int)p[8]) {}

///////////////////////////////////////////////////////////////////////////////
Loom_OLED::~Loom_OLED() 
{
	delete display;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_OLED::print_config() 
{
	LoomLogPlat::print_config();

	LPrintln('\t', "OLED Version        : ", enum_oled_version_string(this->version) );
	if (this->version == Version::BREAKOUT) {
		LPrintln('\t', "Reset Pin           : ", this->reset_pin );		
	}

	LPrintln('\t', "Display Format      : ", enum_oled_format_string(this->display_format) );
	if (this->display_format == Format::SCROLL) {
		LPrintln('\t', "Scroll Duration     : ", this->scroll_duration );
	}
	LPrintln('\t', "Freeze Behavior     : ", enum_oled_freeze_string(this->freeze_behavior) );
	if (this->freeze_behavior != FreezeType::DISABLE) {
		LPrintln('\t', "Freeze Pin          : ", this->freeze_pin );
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_OLED::set_display_format(Format format) 
{ 
	display_format = format; 
}

///////////////////////////////////////////////////////////////////////////////
Loom_OLED::Format Loom_OLED::get_display_format() 
{ 
	return display_format; 
}

///////////////////////////////////////////////////////////////////////////////
void Loom_OLED::set_scroll_duration(uint duration) 
{ 
	scroll_duration = duration; 
}

///////////////////////////////////////////////////////////////////////////////
uint Loom_OLED::get_scroll_duration() 
{ 
	return scroll_duration; 
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
byte Loom_OLED::get_freeze_pin() 
{ 
	return freeze_pin; 
}

///////////////////////////////////////////////////////////////////////////////
void Loom_OLED::set_freeze_behavior(FreezeType behavior) 
{ 
	freeze_behavior = behavior; 
}

///////////////////////////////////////////////////////////////////////////////
Loom_OLED::FreezeType Loom_OLED::get_freeze_behavior() 
{ 
	return freeze_behavior; 
}

///////////////////////////////////////////////////////////////////////////////
void Loom_OLED::log(JsonObject json) 
{
	if ( !check_millis() ) return;
	
	// If freeze complete, check button if no display update needed
	if (freeze_behavior == FreezeType::DATA) {
		if (digitalRead(freeze_pin) == 0) return; 			
	}

	display->clearDisplay();
	display->setTextColor(WHITE);
	display->setTextSize(1);

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
				display->setCursor(0, i*8);
				display->print(keys[i].substring(0,8));
				display->setCursor(64, i*8);
				display->print(vals[i].substring(0,8));
			}
			break;

		case Format::EIGHT:

			for (int i = 0; i < 4 && i < size; i++) {
				display->setCursor(0, i*8);
				display->print(keys[i].substring(0,4));
				display->setCursor(32, i*8);
				display->print(vals[i].substring(0,4));
			}
			for (int i = 0; i < 4 && i < size; i++) {
				display->setCursor(64, i*8);
				display->print(keys[i+4].substring(0,4));
				display->setCursor(96, i*8);
				display->print(vals[i+4].substring(0,4));
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
				display->setCursor(0, i*8);
				display->print(keys[(i+offset)%size].substring(0,8));
				display->setCursor(64, i*8);
				display->print(vals[(i+offset)%size].substring(0,8));
			}

			break;
	}

	// Update display
	display->display();	
}

///////////////////////////////////////////////////////////////////////////////

