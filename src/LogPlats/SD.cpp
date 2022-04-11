///////////////////////////////////////////////////////////////////////////////
///
/// @file		SD.cpp
/// @brief		File for SD implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "SD.h"
#include "Manager.h"
#include "Module_Factory.h"

#include <SPI.h>

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
SD::SD(
		const bool			enable_rate_filter,
		const uint16_t		min_filter_delay,
		const byte			chip_select,
		const char*			default_file,
		const bool			number_files
	)
	: LogPlat("SD", enable_rate_filter, min_filter_delay )
	, chip_select(chip_select)
{
	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  LMark;

	// bool sd_found = sd.begin(chip_select);
	bool sd_found = sd.begin(chip_select, SD_SCK_MHZ(50));

	if (sd_found) {
   	LMark;
		// Setup file name
		update_filename(default_file, number_files);
	} else {
		active = false;
	}

	print_module_label();
	LPrintln("Initialize ", (sd_found) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
SD::SD(JsonArrayConst p)
	: SD(EXPAND_ARRAY(p, 5) ) {}

///////////////////////////////////////////////////////////////////////////////
bool SD::update_filename(const char* default_file, const bool number_files)
{
  LMark;
	snprintf(filename, 12, "%s", default_file);	// file before potential modifcation
	File file;				// file representation

	if (number_files) {	// Use numbers

		if ( strlen(filename) > 6) {
			print_module_label();
			LPrintln("filename too long, truncating");
			filename[6] = '\0';
		}

		const uint8_t base_len = strlen(filename);
		snprintf(filename, 13, "%s000.csv", filename);

		// Determine next file number
		while (sd.exists(filename)) {
			if (filename[base_len + 1] != '9') {
				filename[base_len + 1]++;
			} else if (filename[base_len] != '9') {
				filename[base_len + 1] = '0';
				filename[base_len]++;
			} else {
				print_module_label();
				LPrintln("Can't create file");
				active = false;
				file.close();
				return false;
			}
		}

	} else {			// Don't use numbers
		if ( strlen(filename) > 8) {
			print_module_label();
			LPrintln("filename too long, truncating");
			filename[8] = '\0';
		}
		snprintf(filename, 13, "%s.csv", filename);
	}

	if (!file.open(filename, O_WRONLY | O_CREAT | O_EXCL)) {
	// if ( !file.open(filename) ) {
		print_module_label();
		LPrintln("Could not open file");
		active = false;
		file.close();
		return false;
	}

	print_module_label();
	LPrintln("Using filename: ", filename);

	file.close();

	return true;
}

///////////////////////////////////////////////////////////////////////////////
void SD::print_config() const
{
	LogPlat::print_config();

	// LPrintln("\tChip Select Pin     : ", chip_select);

	// LPrintln("\tSD Version        : ", enum_oled_version_string(version) );
	// if (version == BREAKOUT) {
	// 	LPrintln("\tReset Pin           : ", reset_pin );
	// }
}

///////////////////////////////////////////////////////////////////////////////
void SD::link_device_manager(Manager* LM)
{
	Module::link_device_manager(LM);

	// If no currently linked RTC object, try to get one from Manager
	if ( (RTC_Inst == NULL) && (LM != NULL) ){
   	LMark;
		RTC_Inst = LM->get_rtc_module();
	}
}

///////////////////////////////////////////////////////////////////////////////
void SD::set_filename(const char* name)
{
	snprintf(this->filename, 13, "%s", name);
}

///////////////////////////////////////////////////////////////////////////////
void SD::empty_file(const char* name)
{
  LMark;
	sd.remove(name);
	File file = sd.open(name, O_WRITE);
	file.close();
}

///////////////////////////////////////////////////////////////////////////////
bool SD::dump_file(const char* name)
{
	#if LOOM_DEBUG == 1

		digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  	LMark;

		sd.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup

		File file = sd.open(name, O_READ);

		if (file) {
			LPrintln("Contents of file: ", name);

			// read from the file until there's nothing else in it:
			while (file.available())
				Serial.write(file.read());
			Serial.println();
    	LMark;
			file.close();
			return true;
		} else {
			// if the file didn't open, print an error:
			LPrintln("Error opening ", name);
			return false;
		}

	#endif
}

///////////////////////////////////////////////////////////////////////////////
bool SD::log(const char* name)
{
	if (device_manager != nullptr) {
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
bool SD::save_json(JsonObject json, const char* name)
{
	if ( !check_millis() ) return false;

	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

	// sd.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup
	File file;

	// if ( !file ) {
	if ( !file.open(name, O_WRITE | O_AT_END) ) {
		print_module_label();
		LPrintln("Error opening: ", name);
		return false;
	} else {
		print_module_label();
		LPrintln("Writing to: ", name);
	}

	JsonObject dev_id    = json["id"];
	JsonObject timestamp = json["timestamp"];
	JsonArray  contents  = json["contents"];

	// Don't log if no data
	if (contents.isNull()) return false;

	// Print the header rows if they have not already been printed
	static bool headers_printed = false;
	if (!headers_printed) {
		// Create Header Row 1 (Categories)
		_write_json_header_part1(file, dev_id, timestamp, contents);

		// Create Header Row 2 (Column names)
		_write_json_header_part2(file, dev_id, timestamp, contents);

		headers_printed = true;
	}

	// Write data values
	_write_json_data(file, dev_id, timestamp, contents);

	// Force data to SD and update the directory entry to avoid data loss.
	if (!file.sync() || file.getWriteError()) {
		file.close();
		print_module_label();
		LPrintln("write error");
		return false;
	}

	file.close();

	print_module_label();
	LPrintln("Done writing to SD");
	return true;
}

///////////////////////////////////////////////////////////////////////////////
void SD::_write_json_header_part1(File& file, JsonObject dev_id, JsonObject timestamp, JsonArray contents) const
{
	// Print device indentifcation headers
	if (!dev_id.isNull()) {
		file.print("ID");
		for (JsonPair dataPoint : dev_id) {
			file.print(',');
		}
	}

	// Print timestamp headers
	if (!timestamp.isNull()) {
		file.print("Timestamp");
		for (JsonPair dataPoint : timestamp) {
			file.print(',');
		}
	}

	// Print module data headers
	for (JsonObject module : contents) {
   	LMark;
		file.print(module["module"].as<const char*>());

		JsonObject data = module["data"];
		if (data.isNull()) continue;

		for (JsonPair dataPoint : data) {
			file.print(',');
		}
	}

	file.println();
}

///////////////////////////////////////////////////////////////////////////////
void SD::_write_json_header_part2(File& file, JsonObject dev_id, JsonObject timestamp, JsonArray contents) const
{
	// Print device indentifcation headers
	if (!dev_id.isNull()) {
		for (JsonPair dataPoint : dev_id) {
    	LMark;
			file.print(dataPoint.key().c_str());
			file.print(',');
		}
	}

	// Print timestamp headers
	if (!timestamp.isNull()) {
		for (JsonPair dataPoint : timestamp) {
    	LMark;
			file.print(dataPoint.key().c_str());
			file.print(',');
		}
	}

	// Print module data headers
	for (JsonObject module : contents) {
		JsonObject data = module["data"];
		if (data.isNull()) continue;

		for (JsonPair dataPoint : data) {
    	LMark;
			file.print(dataPoint.key().c_str());
			file.print(',');
		}
	}
	// LPrintln();
	file.println();
}

///////////////////////////////////////////////////////////////////////////////
void SD::_write_json_data(File& file, JsonObject dev_id, JsonObject timestamp, JsonArray contents) const
{
	if (!dev_id.isNull()) {
		for (JsonPair dataPoint : dev_id) {
    	LMark;
			JsonVariant val = dataPoint.value();
			if (val.is<int>()) {
     		LMark;
				file.print(dataPoint.value().as<int>());
			} else if (val.is<char*>() || val.is<const char*>() ) {
				file.print(dataPoint.value().as<const char*>());
			}
			file.print(',');
		}
	}

	if (!timestamp.isNull()) {
   	LMark;
		for (JsonPair dataPoint : timestamp) {
    	LMark;
			JsonVariant val = dataPoint.value();
			if (val.is<char*>() || val.is<const char*>() ) {
     		LMark;
				file.print(dataPoint.value().as<const char*>());
			}
			file.print(',');
		}
	}

	for (JsonObject module : contents) {
		JsonObject data = module["data"];
		if (data.isNull()) continue;

		for (JsonPair dataPoint : data) {
    	LMark;
			JsonVariant val = dataPoint.value();
			if (val.is<int>()) {
				file.print(dataPoint.value().as<int>());
			} else if (val.is<bool>()) {
				file.print(dataPoint.value().as<bool>());
			} else if (val.is<float>()) {
				file.print(dataPoint.value().as<float>());
			} else if (val.is<char*>() || val.is<const char*>() ) {
				file.print(dataPoint.value().as<const char*>());
			}
			file.print(",");
		}
	}

	file.println();
}

///////////////////////////////////////////////////////////////////////////////
void SD::power_up() {
	digitalWrite(8, HIGH); // LoRa fix
	// re-begin SD
	sd.begin(chip_select, SD_SCK_MHZ(50));
}

///////////////////////////////////////////////////////////////////////////////
void SD::power_down() {
	// do nothing
}

// ///////////////////////////////////////////////////////////////////////////////
// void SD::print_directory(File dir, const uint8_t numTabs) const
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
