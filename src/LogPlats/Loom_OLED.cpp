
#include "Loom_OLED.h"

#include <Adafruit_GFX.h>




const char* Loom_OLED::enum_oled_version_string(OLED_Version v)
{
	switch(v) {
		case FEATHERWING : return "FeatherWing";
		case BREAKOUT    : return "Breakout";
	}
}


const char* Loom_OLED::enum_oled_format(OLED_Format f)
{
	switch(f) {
		case OLED_4      : return "OLED 4 Elements";
		case OLED_8      : return "OLED 8 Elements";
		case OLED_SCROLL : return "OLED Scroll"; 
	}
}

const char* Loom_OLED::enum_oled_freeze(OLED_Freeze f)
{
	switch(f) {
		case FREEZE_DISABLE : return "Freeze Disabled";
		case FREEZE_DATA    : return "Freeze Data";
		case FREEZE_SCROLL  : return "Freeze Scroll";
	}
}





// --- CONSTRUCTOR ---
Loom_OLED::Loom_OLED(	char* module_name, 
						bool enable_rate_filter, 
						uint min_filter_delay, 
						OLED_Version version, 
						byte reset_pin, 
						OLED_Format display_format, 
						uint scroll_duration, 
						byte freeze_pin, 
						OLED_Freeze freeze_behavior)
	: LoomLogPlat( module_name, enable_rate_filter, min_filter_delay )
{
	this->version 	      = version;
	this->reset_pin       = reset_pin;
	this->display_format  = display_format;
	this->scroll_duration = scroll_duration;
	this->freeze_behavior = freeze_behavior;
	this->freeze_pin      = freeze_pin;

	switch(version) {
		case FEATHERWING : display = new Adafruit_SSD1306();
		case BREAKOUT    : display = new Adafruit_SSD1306(reset_pin);
	}

	if (freeze_behavior != FREEZE_DISABLE) {
		pinMode(freeze_pin, INPUT_PULLUP);
	}

	display->begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32) cannot be changed
	display->display();
	display->clearDisplay();
}


// --- DESTRUCTOR ---
Loom_OLED::~Loom_OLED() 
{
	delete display;
}


// --- PUBLIC METHODS ---


void Loom_OLED::print_config() 
{
	LoomLogPlat::print_config();

	Println3('\t', "OLED Version        : ", enum_oled_version_string(this->version) );
	if (this->version == BREAKOUT) {
		Println3('\t', "Reset Pin           : ", this->reset_pin );		
	}

	Println3('\t', "Display Format      : ", enum_oled_format(this->display_format) );
	if (this->display_format == OLED_SCROLL) {
		Println3('\t', "Scroll Duration     : ", this->scroll_duration );
	}
	Println3('\t', "Freeze Behavior     : ", enum_oled_freeze(this->freeze_behavior) );
	if (this->freeze_behavior != FREEZE_DISABLE) {
		Println3('\t', "Freeze Pin          : ", this->freeze_pin );
	}
}



void Loom_OLED::set_display_format(OLED_Format format) 
{ 
	display_format = format; 
}


OLED_Format Loom_OLED::get_display_format() 
{ 
	return display_format; 
}


void Loom_OLED::set_scroll_duration(uint duration) 
{ 
	scroll_duration = duration; 
}


uint Loom_OLED::get_scroll_duration() 
{ 
	return scroll_duration; 
}


void Loom_OLED::set_freeze_pin(byte pin) 
{
	freeze_pin = pin;
	if (freeze_behavior != FREEZE_DISABLE) {
		pinMode(freeze_pin, INPUT_PULLUP);
	}
}


byte Loom_OLED::get_freeze_pin() 
{ 
	return freeze_pin; 
}


void Loom_OLED::set_freeze_behavior(OLED_Freeze behavior) 
{ 
	freeze_behavior = behavior; 
}


OLED_Freeze Loom_OLED::get_freeze_behavior() 
{ 
	return freeze_behavior; 
}


void Loom_OLED::log_bundle(OSCBundle& bndl) 
{
	log_bundle(bndl, display_format);
}


void Loom_OLED::log_bundle(OSCBundle& bndl, OLED_Format format) 
{
	if ( !check_millis() ) return;
	
	// If freeze complete, check button if no display update needed
	if (freeze_behavior == FREEZE_DATA) {
		if (digitalRead(freeze_pin) == 0) return; 			
	}

	display->clearDisplay();
	display->setTextColor(WHITE);
	display->setTextSize(1);

	String keys[16], vals[16];

	int size = bundle_num_data_pairs(bndl);
	convert_bundle_to_arrays_assoc(bndl, keys, vals, 16);

	// Remove leading sections of address from multiplexer sections
	for (int i = 0; i < size; i++) {
		int count = osc_address_section_count(keys[i]);
		if (count > 0)  keys[i] = String( nth_strchr(keys[i].c_str(), '/', count)+1 );
	}

	switch (format) {
		case OLED_4:

			for (int i = 0; i < 4 && i < size; i++) {
				display->setCursor(0, i*8);
				display->print(keys[i].substring(0,8));
				display->setCursor(64, i*8);
				display->print(vals[i].substring(0,8));
			}
			break;

		case OLED_8:

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

		case OLED_SCROLL:

			unsigned long time;
			if (freeze_behavior == FREEZE_SCROLL) {
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

