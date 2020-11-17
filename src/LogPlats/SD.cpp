///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SD.cpp
/// @brief		File for Loom_SD implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "SD.h"
#include "Manager.h"

#include <SPI.h>


///////////////////////////////////////////////////////////////////////////////
Loom_SD::Loom_SD(
		LoomManager* manager,
		const bool			enable_rate_filter,
		const uint16_t		min_filter_delay,
		const byte			chip_select,
		const char*			default_file,
		const bool			number_files
	)
	: LoomLogPlat(manager, "SD", Type::SDCARD, enable_rate_filter, min_filter_delay )
	, chip_select(chip_select)
{
  LMark;
	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  LMark;

	// bool sd_found = sd.begin(chip_select);
	bool sd_found = sd.begin(chip_select, SD_SCK_MHZ(50));
  LMark;

	if (sd_found) {
   	LMark;
		// Setup file name
		update_filename(default_file, number_files);
  	LMark;
	} else {
		active = false;
  	LMark;
	}

	print_module_label();
  LMark;
	LPrintln("Initialize ", (sd_found) ? "sucessful" : "failed");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_SD::Loom_SD(LoomManager* manager, JsonArrayConst p)
	: Loom_SD(manager, EXPAND_ARRAY(p, 5) ) {}

///////////////////////////////////////////////////////////////////////////////
bool Loom_SD::update_filename(const char* default_file, const bool number_files)
{
  LMark;
	snprintf(filename, 12, "%s", default_file);	// file before potential modifcation
  LMark;
	File file;				// file representation
  LMark;

	if (number_files) {	// Use numbers
    LMark;

		if ( strlen(filename) > 6) {
    	LMark;
			print_module_label();
    	LMark;
			LPrintln("filename too long, truncating");
    	LMark;
			filename[6] = '\0';
   		LMark;
		}

		const uint8_t base_len = strlen(filename);
   	LMark;
		snprintf(filename, 13, "%s00.csv", filename);
  	LMark;

		// Determine next file number
		while (sd.exists(filename)) {
    	LMark;
			if (filename[base_len + 1] != '9') {
     		LMark;
				filename[base_len + 1]++;
    		LMark;
			} else if (filename[base_len] != '9') {
				filename[base_len + 1] = '0';
     		LMark;
				filename[base_len]++;
    		LMark;
			} else {
				print_module_label();
     		LMark;
				LPrintln("Can't create file");
     		LMark;
				active = false;
     		LMark;
				file.close();
     		LMark;
				return false;
			}
		}

	} else {			// Don't use numbers
		if ( strlen(filename) > 8) {
    	LMark;
			print_module_label();
    	LMark;
			LPrintln("filename too long, truncating");
    	LMark;
			filename[8] = '\0';
   		LMark;
		}
		snprintf(filename, 13, "%s.csv", filename);
  	LMark;
	}

	if (!file.open(filename, O_WRONLY | O_CREAT | O_EXCL)) {
  	LMark;
	// if ( !file.open(filename) ) {
		print_module_label();
   	LMark;
		LPrintln("Could not open file");
   	LMark;
		active = false;
   	LMark;
		file.close();
   	LMark;
		return false;
	}

	print_module_label();
  LMark;
	LPrintln("Using filename: ", filename);
  LMark;

	file.close();
  LMark;

	return true;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::print_config() const
{
  LMark;
	LoomLogPlat::print_config();
  LMark;

	// LPrintln("\tChip Select Pin     : ", chip_select);

	// LPrintln("\tSD Version        : ", enum_oled_version_string(version) );
	// if (version == BREAKOUT) {
	// 	LPrintln("\tReset Pin           : ", reset_pin );
	// }
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::link_device_manager(LoomManager* LM)
{
  LMark;
	LoomModule::link_device_manager(LM);
  LMark;

	// If no currently linked RTC object, try to get one from Manager
	if ( (RTC_Inst == NULL) && (LM != NULL) ){
   	LMark;
		RTC_Inst = LM->get_rtc_module();
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::set_filename(const char* name)
{
  LMark;
	snprintf(this->filename, 13, "%s", name);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::empty_file(const char* name)
{
  LMark;
	sd.remove(name);
  LMark;
	File file = sd.open(name, O_WRITE);
  LMark;
	file.close();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_SD::dump_file(const char* name)
{
  LMark;
	#if LOOM_DEBUG == 1
  	LMark;

		digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  	LMark;

		sd.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup
  	LMark;

		File file = sd.open(name, O_READ);
  	LMark;

		if (file) {
    	LMark;
			LPrintln("Contents of file: ", name);
  		LMark;

			// read from the file until there's nothing else in it:
			while (file.available())
				Serial.write(file.read());
    	LMark;
			Serial.println();
    	LMark;
			file.close();
    	LMark;
			return true;
		} else {
			// if the file didn't open, print an error:
			LPrintln("Error opening ", name);
    	LMark;
			return false;
		}

	#endif
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_SD::log(const char* name)
{
  LMark;
	if (device_manager != nullptr) {
   	LMark;
		JsonObject tmp = device_manager->internal_json();
   	LMark;
		if (strcmp(tmp["type"], "data") == 0 ) {
    	LMark;
			return save_json(tmp, name);
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_SD::save_json(JsonObject json, const char* name)
{
  LMark;
	if ( !check_millis() ) return false;
  LMark;

	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  LMark;

	// sd.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup
	File file;
  LMark;

	// if ( !file ) {
	if ( !file.open(name, O_WRITE | O_AT_END) ) {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("Error opening: ", name);
   	LMark;
		return false;
	} else {
		print_module_label();
   	LMark;
		LPrintln("Writing to: ", name);
  	LMark;
	}

	JsonObject dev_id    = json["id"];
  LMark;
	JsonObject timestamp = json["timestamp"];
  LMark;
	JsonArray  contents  = json["contents"];
  LMark;

	// Don't log if no data
	if (contents.isNull()) return false;
  LMark;


	// Create Header Rows
	if ( file.curPosition() == 0) {
  	LMark;

		// Create Header Row 1 (Categories)
		_write_json_header_part1(file, dev_id, timestamp, contents);
  	LMark;

		// Create Header Row 2 (Column names)
		_write_json_header_part2(file, dev_id, timestamp, contents);
  	LMark;

	}

	// Write data values
	_write_json_data(file, dev_id, timestamp, contents);
  LMark;

	// Force data to SD and update the directory entry to avoid data loss.
	if (!file.sync() || file.getWriteError()) {
   	LMark;
		file.close();
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("write error");
   	LMark;
		return false;
	}

	file.close();
  LMark;

	print_module_label();
  LMark;
	LPrintln("Done writing to SD");
  LMark;
	return true;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::_write_json_header_part1(File& file, JsonObject dev_id, JsonObject timestamp, JsonArray contents) const
{
  LMark;
	// Print device indentifcation headers
	if (!dev_id.isNull()) {
   	LMark;
		file.print("ID");
   	LMark;
		for (JsonPair dataPoint : dev_id) {
    	LMark;
			file.print(',');
   		LMark;
		}
	}

	// Print timestamp headers
	if (!timestamp.isNull()) {
   	LMark;
		file.print("Timestamp");
   	LMark;
		for (JsonPair dataPoint : timestamp) {
    	LMark;
			file.print(',');
   		LMark;
		}
	}

	// Print module data headers
	for (JsonObject module : contents) {
   	LMark;
		file.print(module["module"].as<const char*>());
    LMark;

		JsonObject data = module["data"];
   	LMark;
		if (data.isNull()) continue;
  	LMark;

		for (JsonPair dataPoint : data) {
    	LMark;
			file.print(',');
   		LMark;
		}
	}

	file.println();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::_write_json_header_part2(File& file, JsonObject dev_id, JsonObject timestamp, JsonArray contents) const
{
  LMark;
	// Print device indentifcation headers
	if (!dev_id.isNull()) {
   LMark;
		for (JsonPair dataPoint : dev_id) {
    	LMark;
			file.print(dataPoint.key().c_str());
    	LMark;
			file.print(',');
   		LMark;
		}
	}

	// Print timestamp headers
	if (!timestamp.isNull()) {
   	LMark;
		for (JsonPair dataPoint : timestamp) {
    	LMark;
			file.print(dataPoint.key().c_str());
    	LMark;
			file.print(',');
   		LMark;
		}
	}

	// Print module data headers
	for (JsonObject module : contents) {
   	LMark;
		JsonObject data = module["data"];
   	LMark;
		if (data.isNull()) continue;
  	LMark;

		for (JsonPair dataPoint : data) {
    	LMark;
			file.print(dataPoint.key().c_str());
    	LMark;
			file.print(',');
   		LMark;
		}
	}
	// LPrintln();
	file.println();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::_write_json_data(File& file, JsonObject dev_id, JsonObject timestamp, JsonArray contents) const
{
  LMark;
	if (!dev_id.isNull()) {
   	LMark;
		for (JsonPair dataPoint : dev_id) {
    	LMark;
			JsonVariant val = dataPoint.value();
    	LMark;
			if (val.is<int>()) {
     		LMark;
				file.print(dataPoint.value().as<int>());
    		LMark;
			} else if (val.is<char*>() || val.is<const char*>() ) {
				file.print(dataPoint.value().as<const char*>());
    		LMark;
			}
			file.print(',');
   		LMark;
		}
	}

	if (!timestamp.isNull()) {
   	LMark;
		for (JsonPair dataPoint : timestamp) {
    	LMark;
			JsonVariant val = dataPoint.value();
    	LMark;
			if (val.is<char*>() || val.is<const char*>() ) {
     		LMark;
				file.print(dataPoint.value().as<const char*>());
    		LMark;
			}
			file.print(',');
   		LMark;
		}
	}

	for (JsonObject module : contents) {
   	LMark;
		JsonObject data = module["data"];
   	LMark;
		if (data.isNull()) continue;
  	LMark;

		for (JsonPair dataPoint : data) {
    	LMark;
			JsonVariant val = dataPoint.value();
    	LMark;
			if (val.is<int>()) {
     		LMark;
				file.print(dataPoint.value().as<int>());
    		LMark;
			} else if (val.is<bool>()) {
				file.print(dataPoint.value().as<bool>());
    		LMark;
			} else if (val.is<float>()) {
				file.print(dataPoint.value().as<float>());
    		LMark;
			} else if (val.is<char*>() || val.is<const char*>() ) {
				file.print(dataPoint.value().as<const char*>());
    		LMark;
			}
			file.print(",");
   		LMark;
		}
	}

	file.println();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::power_up() {
  LMark;
	digitalWrite(8, HIGH); // LoRa fix
  LMark;
	// re-begin SD
	sd.begin(chip_select, SD_SCK_MHZ(50));
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_SD::power_down() {
	// do nothing
}

// ///////////////////////////////////////////////////////////////////////////////
// void Loom_SD::print_directory(File dir, const uint8_t numTabs) const
// {
// 	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

// 	while (true) {
// 		File entry =  dir.openNextFile();

// 		if ( !entry ) break;	// no more files

// 		for (auto i = 0; i < numTabs; i++) {
// 			Serial.print('\t');
// 		}

// 		Serial.print(entry.name());
// 		if (entry.isDirectory()) {
// 			Serial.println("/");
// 			print_directory(entry, numTabs + 1);
// 		} else {
// 			// files have sizes, directories do not
// 			Serial.print("\t\t");
// 			Serial.println(entry.size(), DEC);
// 		}
// 		entry.close();
// 	}
// }

// ///////////////////////////////////////////////////////////////////////////////
