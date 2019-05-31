
#include "Loom_OLED.h"

#include <Adafruit_GFX.h>



/////////////////////////////////////////////////////////////////////
const char* Loom_OLED::enum_oled_version_string(OLED_Version v)
{
	switch(v) {
		case OLED_Version::FEATHERWING : return "FeatherWing";
		case OLED_Version::BREAKOUT    : return "Breakout";
	}
}

/////////////////////////////////////////////////////////////////////
const char* Loom_OLED::enum_oled_format_string(OLED_Format f)
{
	switch(f) {
		case OLED_Format::FOUR   : return "OLED 4 Elements";
		case OLED_Format::EIGHT  : return "OLED 8 Elements";
		case OLED_Format::SCROLL : return "OLED Scroll"; 
	}
}

/////////////////////////////////////////////////////////////////////
const char* Loom_OLED::enum_oled_freeze_string(OLED_Freeze f)
{
	switch(f) {
		case OLED_Freeze::DISABLE : return "Freeze Disabled";
		case OLED_Freeze::DATA    : return "Freeze Data";
		case OLED_Freeze::SCROLL  : return "Freeze Scroll";
	}
}


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_OLED::Loom_OLED(	
		const char*			module_name, 
		bool				enable_rate_filter, 
		uint				min_filter_delay, 
		OLED_Version		version, 
		byte				reset_pin, 
		OLED_Format			display_format, 
		uint				scroll_duration, 
		byte				freeze_pin, 
		OLED_Freeze			freeze_behavior
	)
	: LoomLogPlat( module_name, enable_rate_filter, min_filter_delay )
{
	this->module_type = ModuleType::OLED;

	this->version 	      = version;
	this->reset_pin       = reset_pin;
	this->display_format  = display_format;
	this->scroll_duration = scroll_duration;
	this->freeze_behavior = freeze_behavior;
	this->freeze_pin      = freeze_pin;

	switch(version) {
		case OLED_Version::FEATHERWING : display = new Adafruit_SSD1306();
		case OLED_Version::BREAKOUT    : display = new Adafruit_SSD1306(reset_pin);
	}

	if (freeze_behavior != OLED_Freeze::DISABLE) {
		pinMode(freeze_pin, INPUT_PULLUP);
	}

	display->begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32) cannot be changed
	display->display();
	display->clearDisplay();
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_OLED::Loom_OLED(JsonVariant p)
	: Loom_OLED(p[0], p[1], p[2], (OLED_Version)(int)p[3], p[4], (OLED_Format)(int)p[5], p[6], p[7], (OLED_Freeze)(int)p[8])
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_OLED::~Loom_OLED() 
{
	delete display;
}

/////////////////////////////////////////////////////////////////////
void Loom_OLED::print_config() 
{
	LoomLogPlat::print_config();

	LPrintln('\t', "OLED Version        : ", enum_oled_version_string(this->version) );
	if (this->version == OLED_Version::BREAKOUT) {
		LPrintln('\t', "Reset Pin           : ", this->reset_pin );		
	}

	LPrintln('\t', "Display Format      : ", enum_oled_format_string(this->display_format) );
	if (this->display_format == OLED_Format::SCROLL) {
		LPrintln('\t', "Scroll Duration     : ", this->scroll_duration );
	}
	LPrintln('\t', "Freeze Behavior     : ", enum_oled_freeze_string(this->freeze_behavior) );
	if (this->freeze_behavior != OLED_Freeze::DISABLE) {
		LPrintln('\t', "Freeze Pin          : ", this->freeze_pin );
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_OLED::set_display_format(OLED_Format format) 
{ 
	display_format = format; 
}

/////////////////////////////////////////////////////////////////////
OLED_Format Loom_OLED::get_display_format() 
{ 
	return display_format; 
}

/////////////////////////////////////////////////////////////////////
void Loom_OLED::set_scroll_duration(uint duration) 
{ 
	scroll_duration = duration; 
}

/////////////////////////////////////////////////////////////////////
uint Loom_OLED::get_scroll_duration() 
{ 
	return scroll_duration; 
}

/////////////////////////////////////////////////////////////////////
void Loom_OLED::set_freeze_pin(byte pin) 
{
	freeze_pin = pin;
	if (freeze_behavior != OLED_Freeze::DISABLE) {
		pinMode(freeze_pin, INPUT_PULLUP);
	}
}

/////////////////////////////////////////////////////////////////////
byte Loom_OLED::get_freeze_pin() 
{ 
	return freeze_pin; 
}

/////////////////////////////////////////////////////////////////////
void Loom_OLED::set_freeze_behavior(OLED_Freeze behavior) 
{ 
	freeze_behavior = behavior; 
}

/////////////////////////////////////////////////////////////////////
OLED_Freeze Loom_OLED::get_freeze_behavior() 
{ 
	return freeze_behavior; 
}

/////////////////////////////////////////////////////////////////////
void Loom_OLED::log(JsonObject json) 
{
	if ( !check_millis() ) return;
	
	// If freeze complete, check button if no display update needed
	if (freeze_behavior == OLED_Freeze::DATA) {
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
		case OLED_Format::FOUR:

			for (int i = 0; i < 4 && i < size; i++) {
				display->setCursor(0, i*8);
				display->print(keys[i].substring(0,8));
				display->setCursor(64, i*8);
				display->print(vals[i].substring(0,8));
			}
			break;

		case OLED_Format::EIGHT:

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

		case OLED_Format::SCROLL:

			unsigned long time;
			if (freeze_behavior == OLED_Freeze::SCROLL) {
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


