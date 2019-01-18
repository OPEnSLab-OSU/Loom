
#include "Loom_SD.h"
#include "Loom_Macros.h"
#include "Loom_Translator.h"

#include <SD.h>
#include <SPI.h>



// --- CONSTRUCTOR ---
Loom_SD::Loom_SD(		char* module_name, bool enable_rate_filter, uint min_filter_delay, byte chip_select, char* default_file)
	: LoomLogPlat( module_name, enable_rate_filter, min_filter_delay )
{
	LOOM_DEBUG_Println("SD Setup");

	this->chip_select  = chip_select;
	this->default_file = default_file;

	// // Remove the #if if you are using a LoRa M0, but don't have LoRa enabled
	// #if is_lora == 1
	// 	digitalWrite(8, HIGH); 	// if using LoRa
	// #endif

	sd_found = SD.begin(chip_select);
	LOOM_DEBUG_Println4("\tInitialize ", module_name, " ", (sd_found) ? "sucessful" : "failed (will continue, but SD functions will be skipped)");


}


// --- DESTRUCTOR ---
Loom_SD::~Loom_SD() {}


// --- PUBLIC METHODS ---

void Loom_SD::print_config() 
{
	LoomLogPlat::print_config();

	LOOM_DEBUG_Println3('\t', "Chip Select Pin     : ", chip_select);
	LOOM_DEBUG_Println3('\t', "SD Found            : ", (sd_found) ? "True" : "False");

	// LOOM_DEBUG_Println3('\t', "SD Version        : ", enum_oled_version_string(version) );
	// if (version == BREAKOUT) {
	// 	LOOM_DEBUG_Println3('\t', "Reset Pin           : ", reset_pin );		
	// }
}

void Loom_SD::set_default_file(char* filename) 
{ 
	default_file = filename; 
}

char* Loom_SD::get_default_file() 
{ 
	return default_file; 
}


void Loom_SD::delete_file(char* file) 
{
	if ( !sd_found ) return;

	SD.remove(file);
}

void Loom_SD::empty_file(char* file)
{
	if ( !sd_found ) return;

	SD.remove(file);
	SDFile = SD.open(file, FILE_WRITE);
	SDFile.close();
}

void Loom_SD::list_files()
{
	if ( !sd_found ) return;

	print_directory(SD.open("/"), 0);
}


bool Loom_SD::dump_file(char* file) 
{
	#if LOOM_DEBUG == 1
		if ( !sd_found ) return false;

		// #if is_lora == 1
		digitalWrite(8, HIGH); 	// if using LoRa
		// #endif

		SD.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup

		SDFile = SD.open(file);
		if (SDFile) {
			LOOM_DEBUG_Println2("Contents of file: ", file);

			// read from the file until there's nothing else in it:
			while (SDFile.available()) 
				Serial.write(SDFile.read());
			Serial.println();
			SDFile.close();
			return true;
		} else {
			// if the file didn't open, print an error:
			LOOM_DEBUG_Println2("Error opening ", file);
			return false;
		}

	#endif
}



template <typename T>
bool Loom_SD::save_elem(char *file, T data, char endchar)
{	
	if ( !sd_found ) return false;

	// #if is_lora == 1
		digitalWrite(8, HIGH); 	// if using LoRa
	// #endif

	SD.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup
	
	SDFile = SD.open(file, FILE_WRITE);

	if (SDFile) {
		LOOM_DEBUG_Println4("Saving ", data, " to SD file: ", file);
		SDFile.print(data);
		SDFile.print(endchar);
		SDFile.close();
		return true;
	} else {
		LOOM_DEBUG_Println2("Error opening: ", file);
		return false;
	}

}

void Loom_SD::log_bundle(OSCBundle* bndl) 
{
	// if ( !sd_found || !check_millis() ) return;


	LOOM_DEBUG_Println("LOGGING");


	save_bundle(bndl, default_file, 3);

}



// --- SD SAVE BUNDLE --- 
//
// Saves a bundle to SD card in 1 of 4 formats
//
// Format options
//  0: Smart Save as comma separated array of data on (if in key-value single/mulit-msg or single message format)
//       First row becomes a header, subsequent rows line up beneath columns
//       (assumes data fields and timestamp format do not change over the course of writing to the file)
//  1: Save as comma separated array of data on (if in key-value single/mulit-msg or single message format)
//  2: Hierarchical output (best for visually understanding bundle)
//  3: Save as OSC bundle translated to string used in LoRa/nRF transmissions 
//
// @param file       The file to save bundle to
// @param bndl       The bundle to be saved
// @param format     How to format the saved bundle in the SD card file
// @param timestamp  Format of timestamp (if any)
bool Loom_SD::save_bundle(OSCBundle *bndl, char * file, int timestamp)
{
	// if ( !sd_found ) return false;
	if ( !sd_found || !check_millis() ) return false;

	LOOM_DEBUG_Println("HERE");

	// #if is_lora == 1
		digitalWrite(8, HIGH); 	// if using LoRa
	// #endif

	SD.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup
	SDFile = SD.open(file, FILE_WRITE);

	if ( SDFile ) {
		LOOM_DEBUG_Println2("Saving bundle to SD file: ", file);

		// Optionally add some form of timestamp
		// #if is_rtc == 1
		// 	if ((format != 0) && (timestamp > 0) && (timestamp <= 4)) {
		// 		sd_write_timestamp(file, timestamp, ',');
		// 	}
		// #endif


		OSCMessage *msg;
		OSCBundle tmpBndl;
		convert_bundle_structure(bndl, &tmpBndl, SINGLEMSG);
		LOOM_DEBUG_Println("Converted Bundle:");
		print_bundle(&tmpBndl);

		if ( SDFile.position() == 0) { // Create Header

			// Timestamp field(s)
			// #if is_rtc == 1
			// 	switch (timestamp) {
			// 		case 1: SDFile.print("Date,");      break;
			// 		case 2: SDFile.print("Time,");      break;
			// 		case 3: SDFile.print("Date,Time,"); break;
			// 		case 4:	SDFile.print("Date_Time,"); break;
			// 		default: break;
			// 	}
			// #endif

			// Address field
			SDFile.print("Address,");

			// Data fields
			msg = tmpBndl.getOSCMessage(0);
			for (int i = 0; i < msg->size(); i+=2) {
				SDFile.print(get_data_value(msg, i));
				SDFile.print( (i <= msg->size()-3) ? ',' : '\n' );
			}
		}

		// #if is_rtc == 1
		// 	if ((timestamp > 0) && (timestamp <= 4)) {
		// 		sd_write_timestamp(file, timestamp, ',');
		// 	}
		// #endif	
		msg = tmpBndl.getOSCMessage(0);
		SDFile.print(get_address_string(msg)+',');
		for (int i = 1; i < msg->size(); i+=2) {
			SDFile.print( get_data_value(msg, i) );
			SDFile.print( (i < msg->size()-1) ? ',' : '\n' );
		}
		// SDFile.println();
	
		SDFile.close();
		return true;

	} else {
		LOOM_DEBUG_Println2("Error opening: ", file);
		return false;
	}


}




// timestamp options:
//   0: no timestamp added
//   1: only date added
//   2: only time added
//   3: both date and time added (two fields)
//   4: both date and time added (combined field)
template <typename T>
bool Loom_SD::save_array(char *file, T data [], int len, char delimiter, int timestamp) 
{
	if ( !sd_found || !check_millis() ) return false;

	// #if is_lora == 1
	digitalWrite(8, HIGH); 	// if using LoRa
	// #endif

	SD.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup
	SDFile = SD.open(file, FILE_WRITE);

	if (SDFile) {
		LOOM_DEBUG_Print3("Saving array to SD file: ", file, " ...");

		// Optionally add some form of timestamp
		// #if is_rtc == 1
		// 	if ((timestamp > 0) && (timestamp <= 4)) {
		// 		sd_write_timestamp(file, timestamp, delimiter);
		// 	}
		// #endif

		for (int i = 0; i < len-1; i++) {
			SDFile.print(data[i]);
			SDFile.print(delimiter);
		}

		SDFile.print(data[len-1]);
		SDFile.println();
		SDFile.close();
		LOOM_DEBUG_Println("Done");
		return true;
	} else {
		LOOM_DEBUG_Println2("Error opening: ", file);
		return false;
	}

}




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


