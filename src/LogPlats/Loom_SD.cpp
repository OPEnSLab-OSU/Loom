
#include "Loom_SD.h"

#include <SPI.h>

#include "../RTC/Loom_RTC.h"



/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_SD::Loom_SD(		
		const char*		module_name, 
		bool			enable_rate_filter, 
		uint			min_filter_delay, 
		byte			chip_select, 
		const char*		default_file
	)
	: LoomLogPlat( module_name, enable_rate_filter, min_filter_delay )
{
	this->module_type = ModuleType::SDCARD;

	Println("SD Setup");

	this->chip_select  = chip_select;
	strcpy(this->default_file, default_file);

	// // Remove the #if if you are using a LoRa M0, but don't have LoRa enabled
	// #if is_lora == 1
	// 	digitalWrite(8, HIGH); 	// if using LoRa
	// #endif

	sd_found = SD.begin(chip_select);
	print_module_label();
	Println2("Initialize ", (sd_found) ? "sucessful" : "failed (will continue, but SD functions will be skipped)");


}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_SD::Loom_SD(JsonVariant p)
	: Loom_SD(p[0], p[1], p[2], p[3], p[4])
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_SD::~Loom_SD() {}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void Loom_SD::print_config() 
{
	LoomLogPlat::print_config();

	Println3('\t', "Chip Select Pin     : ", chip_select);
	Println3('\t', "SD Found            : ", (sd_found) ? "True" : "False");

	// Println3('\t', "SD Version        : ", enum_oled_version_string(version) );
	// if (version == BREAKOUT) {
	// 	Println3('\t', "Reset Pin           : ", reset_pin );		
	// }
}

/////////////////////////////////////////////////////////////////////
void Loom_SD::link_device_manager(LoomManager* LM)
{
	LoomModule::link_device_manager(LM);

	// If no currently linked RTC object, try to get one from Manager
	if ( (RTC_Inst == NULL) && (LM != NULL) ){
		RTC_Inst = LM->get_rtc_module();
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_SD::set_RTC_module(LoomRTC* RTC_Inst)
{
	this->RTC_Inst = RTC_Inst;
}

/////////////////////////////////////////////////////////////////////
LoomRTC* Loom_SD::get_RTC_module()
{
	return RTC_Inst;
}

/////////////////////////////////////////////////////////////////////
void Loom_SD::set_default_file(char* filename) 
{ 
	default_file = filename; 
}

/////////////////////////////////////////////////////////////////////
char* Loom_SD::get_default_file() 
{ 
	return default_file; 
}

/////////////////////////////////////////////////////////////////////
void Loom_SD::delete_file(char* file) 
{
	if ( !sd_found ) return;

	SD.remove(file);
}

/////////////////////////////////////////////////////////////////////
void Loom_SD::empty_file(char* file)
{
	if ( !sd_found ) return;

	SD.remove(file);
	File SDFile = SD.open(file, FILE_WRITE);
	SDFile.close();
}

/////////////////////////////////////////////////////////////////////
void Loom_SD::list_files()
{
	if ( !sd_found ) return;

	print_directory(SD.open("/"), 0);
}

/////////////////////////////////////////////////////////////////////
bool Loom_SD::dump_file(char* file) 
{
	if ( !sd_found ) return false;

	#if LOOM_DEBUG == 1

		// #if is_lora == 1
		digitalWrite(8, HIGH); 	// if using LoRa
		// #endif

		SD.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup

		File SDFile = SD.open(file);

		if (SDFile) {
			Println2("Contents of file: ", file);

			// read from the file until there's nothing else in it:
			while (SDFile.available()) 
				Serial.write(SDFile.read());
			Serial.println();
			SDFile.close();
			return true;
		} else {
			// if the file didn't open, print an error:
			Println2("Error opening ", file);
			return false;
		}

	#endif
}

/////////////////////////////////////////////////////////////////////
void Loom_SD::log_bundle(OSCBundle& bndl) 
{
	save_bundle(bndl, default_file, 3);
}


/////////////////////////////////////////////////////////////////////
bool Loom_SD::save_bundle(OSCBundle& bndl, char* file, int timestamp)
{
	// Get device ID
	char device_id[30];
	osc_extract_header_to_section(bndl.getOSCMessage(0), 5, device_id);

	// Convert bundle to 
	int len = bundle_num_data_pairs(bndl)*2;
	String key_values[len];
	convert_bundle_to_array_key_value(bndl, key_values, len);

	bool status = save_array(file, key_values, len, ',', timestamp, true, device_id);

	return status;
}

/////////////////////////////////////////////////////////////////////

// Might need to move to header file

template <typename T>
bool Loom_SD::save_array(char *file, T data [], int len, char delimiter, int timestamp, bool has_keys, char* device_id) 
{
	if ( !sd_found || !check_millis() ) return false;

	// #if is_lora == 1
	digitalWrite(8, HIGH); 	// if using LoRa
	// #endif

	bool got_timestamp = false;

	SD.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup
	File SDFile = SD.open(file, FILE_WRITE);

	// If file successfully opened
	if (SDFile) {
		Print3("Saving array to SD file: '", file, "' ...");

		char time_key[30], time_val[30];

		if (timestamp) {
			if (device_manager != NULL) {
				// LoomRTC* rtc = device_manager->get_rtc_module(0);
				if (RTC_Inst != NULL) {
					Println2("RTC Object: ", RTC_Inst->get_module_name() );
					RTC_Inst->get_timestamp(time_key, time_val, delimiter, timestamp);
					got_timestamp = true; 
				}
			}
		}

		// Array is assumed to have alternating keys and values
		if (has_keys) {

			// Check if at first row (create header)
			if ( SDFile.position() == 0) {

				// Add timestamp header
				if (got_timestamp) {
					SDFile.print(time_key);
				}

				// Add address header if address was provided
				if (strlen(device_id) > 0) {
					SD_print_aux(SDFile, "Device", delimiter);
				}

				// Print keys
				for (int i = 0; i < len-2; i+=2) {
					SD_print_aux(SDFile, data[i], delimiter);
				}
				SDFile.println(data[len-2]);
			}

			// Add timestamp
			if (got_timestamp) {
				SDFile.print(time_val);
			}

			// Add device ID if provided
			if (strlen(device_id) > 0) {
				SD_print_aux(SDFile, device_id, delimiter);
			}

			// Print values 
			for (int i = 1; i < len-2; i+=2) {
				SD_print_aux(SDFile, data[i], delimiter);
			}
			SDFile.println(data[len-1]);
		} 

		// Array is assume to only have values
		else {

			if (got_timestamp) {
				SDFile.print(time_val);
			}

			// Add device ID if provided
			if (strlen(device_id) > 0) {
				SD_print_aux(SDFile, device_id, delimiter);
			}


			for (int i = 0; i < len-1; i++) {
				SD_print_aux(SDFile, data[i], delimiter);
			}
			SDFile.println(data[len-1]);
		}

		SDFile.close();
		Println("Done");
		
		return true;
	} 
	
	// If file could not be opened
	else {
		Println2("Error opening: ", file);
		
		return false;
	}

}

/////////////////////////////////////////////////////////////////////
void Loom_SD::print_directory(File dir, int numTabs) 
{
	if ( !sd_found ) return;

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


