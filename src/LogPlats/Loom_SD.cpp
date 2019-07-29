
#include "Loom_SD.h"
#include "Loom_Manager.h"

#include <SPI.h>


///////////////////////////////////////////////////////////////////////////////
Loom_SD::Loom_SD(		
		bool			enable_rate_filter, 
		uint16_t		min_filter_delay, 
		byte			chip_select, 
		const char*		default_file
	)
	: LoomLogPlat( "SD", Type::SDCARD, enable_rate_filter, min_filter_delay )
	, chip_select(chip_select)
{
	snprintf(this->default_file, 16, "%s", default_file);

	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

	sd_found = SD.begin(chip_select);

	print_module_label();	
	LPrintln("Initialize ", (sd_found) ? "sucessful" : "failed (will continue, but SD functions will be skipped)");
}

///////////////////////////////////////////////////////////////////////////////
Loom_SD::Loom_SD(JsonArrayConst p)
	: Loom_SD( EXPAND_ARRAY(p, 4) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::print_config() 
{
	LoomLogPlat::print_config();

	LPrintln("\tChip Select Pin     : ", chip_select);
	LPrintln("\tSD Found            : ", (sd_found) ? "True" : "False");

	// LPrintln("\tSD Version        : ", enum_oled_version_string(version) );
	// if (version == BREAKOUT) {
	// 	LPrintln("\tReset Pin           : ", reset_pin );		
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
void Loom_SD::set_default_file(const char* filename) 
{ 
	snprintf(this->default_file, 16, "%s", default_file); 
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::empty_file(const char* file)
{
	if ( !sd_found ) return;

	SD.remove(file);
	File sdFile = SD.open(file, FILE_WRITE);
	sdFile.close();
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_SD::dump_file(const char* file) 
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
void Loom_SD::log(const char* filename)
{
	if (device_manager != nullptr) {
		JsonObject tmp = device_manager->internal_json();
		if (strcmp(tmp["type"], "data") == 0 ) {
			save_json(tmp, filename);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

// Note that timestamp implementation may change with blocks of data in json
// Format:
// Identification Date Time ModuleA key1 val1 key2 val2 ... ModuleB key1 val1 ...   

bool Loom_SD::save_json(JsonObject json, const char* file)
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

	JsonObject dev_id    = json["id"];
	JsonObject timestamp = json["timestamp"];
	JsonArray  contents  = json["contents"];
	
	// Don't log if no data
	if (contents.isNull()) return false;

	// Create Header Rows
	if ( sdFile.position() == 0) {

		// Create Header Row 1 (Categories)
		_write_json_header_part1(sdFile, dev_id, timestamp, contents);

		// Create Header Row 2 (Column names)
		_write_json_header_part2(sdFile, dev_id, timestamp, contents);
	}

	// Write data values
	_write_json_data(sdFile, dev_id, timestamp, contents);

	sdFile.close();
	print_module_label();
	LPrintln("Done writing to SD");
	return true;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::_write_json_header_part1(File sdFile, JsonObject dev_id, JsonObject timestamp, JsonArray contents)
{
	// Print device indentifcation headers
	if (!dev_id.isNull()) { 
		sdFile.print("ID");
		for (JsonPair dataPoint : dev_id) {
			sdFile.print(',');
		}
	}

	// Print timestamp headers
	if (!timestamp.isNull()) { 
		sdFile.print("Timestamp");
		for (JsonPair dataPoint : timestamp) {
			sdFile.print(',');
		}
	}

	// Print module data headers
	for (JsonObject module : contents) {
		sdFile.print(module["module"].as<const char*>());
		
		JsonObject data = module["data"];
		if (data.isNull()) continue;

		for (JsonPair dataPoint : data) {
			sdFile.print(',');
		}
	}

	sdFile.println();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::_write_json_header_part2(File sdFile, JsonObject dev_id, JsonObject timestamp, JsonArray contents)
{
	// Print device indentifcation headers
	if (!dev_id.isNull()) { 
		for (JsonPair dataPoint : dev_id) {
			sdFile.print(dataPoint.key().c_str());
			sdFile.print(',');
		}
	}

	// Print timestamp headers
	if (!timestamp.isNull()) { 
		for (JsonPair dataPoint : timestamp) {
			sdFile.print(dataPoint.key().c_str());
			sdFile.print(',');
		}
	}

	// Print module data headers
	for (JsonObject module : contents) {
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

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::_write_json_data(File sdFile, JsonObject dev_id, JsonObject timestamp, JsonArray contents)
{
	if (!dev_id.isNull()) { 
		for (JsonPair dataPoint : dev_id) {
			JsonVariant val = dataPoint.value();	
			if (val.is<int>()) {
				sdFile.print(dataPoint.value().as<int>());
			} else if (val.is<char*>() || val.is<const char*>() ) {
				sdFile.print(dataPoint.value().as<const char*>());
			} 
			sdFile.print(',');
		}
	}

	if (!timestamp.isNull()) { 
		for (JsonPair dataPoint : timestamp) {
			JsonVariant val = dataPoint.value();				
			if (val.is<char*>() || val.is<const char*>() ) {
				sdFile.print(dataPoint.value().as<const char*>());
			} 
			sdFile.print(',');
		}
	}
	
	for (JsonObject module : contents) {
		JsonObject data = module["data"];
		if (data.isNull()) continue;

		for (JsonPair dataPoint : data) {
			JsonVariant val = dataPoint.value();
			if (val.is<int>()) {
				sdFile.print(dataPoint.value().as<int>());
			} else if (val.is<bool>()) {
				sdFile.print(dataPoint.value().as<bool>());								
			} else if (val.is<float>()) {
				sdFile.print(dataPoint.value().as<float>());				
			} else if (val.is<char*>() || val.is<const char*>() ) {
				sdFile.print(dataPoint.value().as<const char*>());
			} 
			sdFile.print(",");		
		}
	}

	sdFile.println();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::print_directory(File dir, uint8_t numTabs) 
{
	if ( !sd_found ) return;

	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

	while (true) {
		File entry =  dir.openNextFile();
		
		if ( !entry ) break;	// no more files

		for (auto i = 0; i < numTabs; i++) {
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






