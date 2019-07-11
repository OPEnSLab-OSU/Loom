
#include "Loom_SD.h"
#include "Loom_Manager.h"

#include <SPI.h>


///////////////////////////////////////////////////////////////////////////////
Loom_SD::Loom_SD(		
		const char*		module_name, 
		bool			enable_rate_filter, 
		uint			min_filter_delay, 
		byte			chip_select, 
		const char*		default_file
	)
	: LoomLogPlat( module_name, enable_rate_filter, min_filter_delay )
{
	this->module_type = LoomModule::Type::SDCARD;

	print_module_label();
	LPrintln("SD Setup");

	this->chip_select  = chip_select;
	snprintf(this->default_file, 16, "%s", default_file);

	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

	sd_found = SD.begin(chip_select);

	print_module_label();	
	LPrintln("Initialize ", (sd_found) ? "sucessful" : "failed (will continue, but SD functions will be skipped)");
}

///////////////////////////////////////////////////////////////////////////////
Loom_SD::Loom_SD(JsonArrayConst p)
	: Loom_SD( EXPAND_ARRAY(p, 5) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::print_config() 
{
	LoomLogPlat::print_config();

	LPrintln('\t', "Chip Select Pin     : ", chip_select);
	LPrintln('\t', "SD Found            : ", (sd_found) ? "True" : "False");

	// LPrintln('\t', "SD Version        : ", enum_oled_version_string(version) );
	// if (version == BREAKOUT) {
	// 	LPrintln('\t', "Reset Pin           : ", reset_pin );		
	// }
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::link_device_manager(LoomManager* LM)
{
	LoomModule::link_device_manager(LM);

	// If no currently linked RTC object, try to get one from Manager
	if ( (RTC_Inst == NULL) && (LM != NULL) ){
		RTC_Inst = LM->get_rtc_module();
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::set_RTC_module(LoomRTC* RTC_Inst)
{
	this->RTC_Inst = RTC_Inst;
}

///////////////////////////////////////////////////////////////////////////////
LoomRTC* Loom_SD::get_RTC_module()
{
	return RTC_Inst;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::set_default_file(char* filename) 
{ 
	snprintf(this->default_file, 16, "%s", default_file); 
}

///////////////////////////////////////////////////////////////////////////////
char* Loom_SD::get_default_file() 
{ 
	return default_file; 
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::delete_file(char* file) 
{
	if ( !sd_found ) return;

	SD.remove(file);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::empty_file(char* file)
{
	if ( !sd_found ) return;

	SD.remove(file);
	File sdFile = SD.open(file, FILE_WRITE);
	sdFile.close();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::list_files()
{
	if ( !sd_found ) return;

	print_directory(SD.open("/"), 0);
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_SD::dump_file(char* file) 
{
	if ( !sd_found ) return false;

	#if LOOM_DEBUG == 1

		digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

		SD.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup

		File sdFile = SD.open(file);

		if (sdFile) {
			LPrintln("Contents of file: ", file);

			// read from the file until there's nothing else in it:
			while (sdFile.available()) 
				Serial.write(sdFile.read());
			Serial.println();
			sdFile.close();
			return true;
		} else {
			// if the file didn't open, print an error:
			LPrintln("Error opening ", file);
			return false;
		}

	#endif
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::log(JsonObject json) 
{
	save_json(json, default_file, 3);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::log(const char* filename)
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 ) {
			LPrintln("HERE");
			save_json(tmp, filename, 3);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

// Note that timestamp implementation may change with blocks of data in json

	// Format:
	// Date Time ModuleA key1 val1 key2 val2 ... ModuleB key1 val1 ...   


bool Loom_SD::save_json(JsonObject json, const char* file, int timestamp_format)
{
	if ( !sd_found || !check_millis() ) return false;

	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

	SD.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup
	File sdFile = SD.open(file, FILE_WRITE);

	if (!sdFile) {
		print_module_label();
		LPrintln("Error opening: ", file);
		return false;
	} else {
		print_module_label();
		LPrintln("Writing to: ", file);
	}

	JsonObject timestamp = json["timestamp"];
	JsonArray  contents  = json["contents"];
	
	// Don't log if no data
	if (contents.isNull()) return false;

	// Create Header
	if ( sdFile.position() == 0) {

		if (!timestamp.isNull()) { 
			for (JsonPair dataPoint : timestamp) {
				sdFile.print(dataPoint.key().c_str());
				sdFile.print(',');
			}
		}

		for (JsonObject module : contents) {
			// LPrint(module["module"].as<const char*>());
			// LPrint(",");
			sdFile.print(module["module"].as<const char*>());
			sdFile.print(",");

			JsonObject data = module["data"];
			if (data.isNull()) continue;

			for (JsonPair dataPoint : data) {
				sdFile.print(dataPoint.key().c_str());
				sdFile.print(',');
			}
		}
		// LPrintln();
		sdFile.println();
	}

	// Write data value

	if (!timestamp.isNull()) { 
		for (JsonPair dataPoint : timestamp) {
			JsonVariant val = dataPoint.value();				
			if (val.is<char*>() || val.is<const char*>() ) {
				// LPrint(dataPoint.value().as<const char*>());
				sdFile.print(dataPoint.value().as<const char*>());
			} 
			sdFile.print(',');
		}
	}
	
	for (JsonObject module : contents) {
		// LPrint(",");
		sdFile.print(",");

		JsonObject data = module["data"];
		if (data.isNull()) continue;

		for (JsonPair dataPoint : data) {
			JsonVariant val = dataPoint.value();
			if (val.is<int>()) {
				// LPrint(dataPoint.value().as<int>());
				sdFile.print(dataPoint.value().as<int>());
			} else if (val.is<bool>()) {
				// LPrint(dataPoint.value().as<bool>());								
				sdFile.print(dataPoint.value().as<bool>());								
			} else if (val.is<float>()) {
				// LPrint(dataPoint.value().as<float>());				
				sdFile.print(dataPoint.value().as<float>());				
			} else if (val.is<char*>() || val.is<const char*>() ) {
				// LPrint(dataPoint.value().as<const char*>());
				sdFile.print(dataPoint.value().as<const char*>());
			} 
			// LPrint(",");		
			sdFile.print(",");		
		}
	}

	// LPrintln();
	sdFile.println();

	sdFile.close();
	print_module_label();
	LPrintln("Done writing to SD");
	return true;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::print_directory(File dir, int numTabs) 
{
	if ( !sd_found ) return;

	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

	while (true) {
		File entry =  dir.openNextFile();
		
		if ( !entry ) break;	// no more files

		for (uint8_t i = 0; i < numTabs; i++) {
			Serial.print('\t');
		}

		Serial.print(entry.name());
		if (entry.isDirectory()) {
			Serial.println("/");
			print_directory(entry, numTabs + 1);
		} else {
			// files have sizes, directories do not
			Serial.print("\t\t");
			Serial.println(entry.size(), DEC);
		}
		entry.close();
	}
}

///////////////////////////////////////////////////////////////////////////////


