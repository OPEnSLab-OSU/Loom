// ================================================================ 
// ===                          NOTES                           === 
// ================================================================

// It appears that file names need to be limited to 8 characters
//  (not including the extension)


// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <SD.h>
#include <SPI.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define chipSelect 10 	// Adafruit Feathers use GPIO 10

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 

// struct config_sd_t {
// }; 

// struct state_sd_t {
// };

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
// struct config_sd_t configuration;
// struct state_sd_t state_sd;
File sdFile;
int SD_logging = 0; // 0: off, 1: log bundles received

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_sd();
void sd_card_info();
void sd_delete_file(char* file);
void sd_list_files();
bool sd_read_all_from_file(char* file);
// void sd_write_string(char* file, char* text);
void sd_write_timestamp(char* file, int timestamp, char delimiter);
template <typename T>
bool sd_save_elem(char *file, T data, char endchar);
template <typename T>
bool sd_save_array(char *file, T data [], int len, char delimiter, int timestamp);
bool sd_save_bundle(char * file, OSCBundle *bndl, int format, int timestamp);
void set_SD_logging(OSCMessage &msg);

// ================================================================
// ===                          SETUP                           ===
// ================================================================

// Select how to read from and write to non volatile memory
// based on which device is bieng used



// --- SETUP SD ---
//
// Handles configuration of reading device configuration settings from flash (if config is saved)
// else uses config.h specified settings, and then writes these to flash
// 
void setup_sd()
{
	LOOM_DEBUG_Println("Initializing SD card...");

	#if is_lora == 1
		digitalWrite(8, HIGH); 	// if using LoRa
	#endif

	if (!SD.begin(chipSelect)) {
		LOOM_DEBUG_Println("initialization failed!");
	}
	LOOM_DEBUG_Println("initialization done.");
}



// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================

void sd_card_info()
{

}

// Delete a file on SD card if it exists
void sd_delete_file(char* file) 
{
	SD.remove(file);
}

// Empty a file on SD card
void sd_empty_file(char* file)
{
	SD.remove(file);
	sdFile = SD.open(file, FILE_WRITE);
	sdFile.close();
}

void sd_list_files()
{

}
// --- SD READ FROM FILE --- 
//
// Attempts to read all of the contents of a file
// on the SD card
//
// @param file  The name of the file to read from
//
// @return True if no error
//
bool sd_read_all_from_file(char* file) 
{
	sdFile = SD.open(file);
	if (sdFile) {
		LOOM_DEBUG_Println2("Contents of file: ", file);

		// read from the file until there's nothing else in it:
		while (sdFile.available()) 
			Serial.write(sdFile.read());
		Serial.println();
	} else {
		// if the file didn't open, print an error:
		LOOM_DEBUG_Println2("Error opening ", file);
	}
	sdFile.close();
}


// --- SD WRITE TO FILE --- 
//
// Attempts to write text to a file on the SD card
//
// @param file  The name of the file to write to
//
// @return True if no error
//
// void sd_write_string(char* file, char* text) 
// {
// 	sdFile = SD.open(file, FILE_WRITE);
// 	if (sdFile) {
// 		LOOM_DEBUG_Print3("Writing to ", file, "...");

// 		sdFile.println(text);

// 		LOOM_DEBUG_Println(" done");
// 		sdFile.close();
// 	} else {
// 		LOOM_DEBUG_Println2("Error opening: ", file);
// 	}
// }

// This is just a helper function,
// expects file to be open already
#if is_rtc == 1
void sd_write_timestamp(char* file, int timestamp, char delimiter)
{
	switch (timestamp) {
		case 1:
			sdFile.print(get_datestring());
			break;
		case 2:
			sdFile.print(get_timestring());
			break;
		case 3:
			sdFile.print(get_datestring());
			sdFile.print(delimiter);
			sdFile.print(get_timestring());
			break;
		case 4:
			sdFile.print(get_datestring());
			sdFile.print('_');
			sdFile.print(get_timestring());
	}
	sdFile.print(delimiter);
}
#endif




// // --- SD READ BUNDLE
// bool sd_read_bundle(char *file, OSCBundle *bndl)
// {

// }

// --- SD SAVE BUNDLE
// bool sd_save(char *file, OSCBundle *bndl)
// {
// 	Serial.println("sd save bundle");
// 	char osc_str[255];
// 	memset(osc_str, '\0', sizeof(osc_str));
// 	convert_OSC_bundle_to_string(bndl, osc_str);

// 	LOOM_DEBUG_Println(osc_str);
// 	LOOM_DEBUG_Println2("osc_str length: ", osc_str);

// 	sd_write_string(file, osc_str);
// }


template <typename T>
bool sd_save_elem(char *file, T data, char endchar)
{
	sdFile = SD.open(file, FILE_WRITE);

	if (sdFile) {
		LOOM_DEBUG_Println4("Saving ", data, " to SD file: ", file);
		sdFile.print(data);
		sdFile.print(endchar);
	} else {
		LOOM_DEBUG_Println2("Error opening: ", file);
	}
	sdFile.close();
}


// Have a macro call this function with correct length?


// timestamp options:
//   0: no timestamp added
//   1: only date added
//   2: only time added
//   3: both date and time added (two fields)
//   4: both date and time added (combined field)
template <typename T>
bool sd_save_array(char *file, T data [], int len, char delimiter, int timestamp) 
{
	sdFile = SD.open(file, FILE_WRITE);

	if (sdFile) {
		LOOM_DEBUG_Print3("Saving array to SD file: ", file, " ...");

		// Optionally add some form of timestamp
		#if is_rtc == 1
			if ((timestamp > 0) && (timestamp <= 4)) {
				sd_write_timestamp(file, timestamp, delimiter);
			}
		#endif

		for (int i = 0; i < len-1; i++) {
			sdFile.print(data[i]);
			sdFile.print(delimiter);
		}
		sdFile.print(data[len-1]);
		sdFile.println();
	} else {		
		LOOM_DEBUG_Println2("Error opening: ", file);
	}
	sdFile.close();
	LOOM_DEBUG_Println("Done");
}


// Could convert to array first
// Print as OSC string
// Could print in hierarchical format like print_bundle

// Format options
//  0: Smart Save as comma separated array of data on (if in key-value single/mulit-msg or single message format)
//       First row becomes a header, subsequent rows line up beneath columns
//       (assumes data fields and timestamp format do not change over the course of writing to the file)
//  1: Save as comma separated array of data on (if in key-value single/mulit-msg or single message format)
//  2: Hierarchical output (best for visually understanding bundle)
//  3: Save as osc translated to string
bool sd_save_bundle(char * file, OSCBundle *bndl, int format, int timestamp)
{
	sdFile = SD.open(file, FILE_WRITE);
	if (sdFile) {
		LOOM_DEBUG_Println2("Saving bundle to SD file: ", file);

		// Optionally add some form of timestamp
		#if is_rtc == 1
			if ((format != 0) && (timestamp > 0) && (timestamp <= 4)) {
				sd_write_timestamp(file, timestamp, ',');
			}
		#endif

		switch(format) {
			case 0: {
				OSCMessage *msg;
				OSCBundle tmpBndl;
				convert_bundle_structure(bndl, &tmpBndl, SINGLEMSG);

				switch(sdFile.position()) {
					case 0 : // Create Header
					{ 
						// Timestamp field(s)
						#if is_rtc == 1
							switch (timestamp) {
								case 1: sdFile.print("Date,");      break;
								case 2: sdFile.print("Time,");      break;
								case 3: sdFile.print("Date,Time,"); break;
								case 4:	sdFile.print("Date_Time,"); break;
								default: break;
							}
						#endif

						// Address field
						sdFile.print("Address,");

						// Data fields
						msg = tmpBndl.getOSCMessage(0);
						for (int i = 0; i < msg->size(); i+=2) {
							sdFile.print(get_data_value(msg, i));
							sdFile.print( (i <= msg->size()-3) ? ',' : '\n' );
						}
					}

					 // No break
					default : 
					{
						#if is_rtc == 1
							if ((timestamp > 0) && (timestamp <= 4)) {
								sd_write_timestamp(file, timestamp, ',');
							}
						#endif	
						msg = tmpBndl.getOSCMessage(0);
						sdFile.print(get_address_string(msg)+',');
						for (int i = 1; i < msg->size(); i+=2) {
							sdFile.print(get_data_value(msg, i));
							sdFile.print( (i < msg->size()-1) ? ',' : '\n' );
						}
					}

				} // of switch sdFile.position()
				break;
			} // of case 0:
			case 1: {
				OSCMessage *msg;
				OSCBundle tmpBndl;
				convert_bundle_structure(bndl, &tmpBndl, SINGLEMSG);

				msg = tmpBndl.getOSCMessage(0);
				sdFile.print(get_address_string(msg)+',');
				for (int i = 0; i < msg->size(); i++) {
					sdFile.print(get_data_value(msg, i));
					sdFile.print( (i < msg->size()-1) ? ',' : '\n' );
				}
				break;
			}
			case 2: {
				char buf[50];
				char data_type;
				sdFile.print("\nBundle Size: ");
				sdFile.println(bndl->size());
				OSCMessage *msg;
				
				for (int i = 0; i < bndl->size(); i++) {
					msg = bndl->getOSCMessage(i);

					msg->getAddress(buf, 0);
					sdFile.print("Address ");
					sdFile.print(i + 1);
					sdFile.print(": ");
					sdFile.println(buf);

					for (int j = 0; j < msg->size(); j++) {
						data_type = msg->getType(j);
						sdFile.print("Value ");
						sdFile.print(j + 1);
						sdFile.print(": ");
						switch(data_type) {
							case 'f':
								sdFile.print("(f) ");
								sdFile.println(msg->getFloat(j));
								break;
							case 'i':
								sdFile.print("(i) ");
								sdFile.println(msg->getInt(j));
								break;
							case 's':
								sdFile.print("(s) ");
								msg->getString(j, buf, 50);
								sdFile.println(buf);
								break;
							default:
								break;
						}
					}
				}
				sdFile.println();
				break;
			}
			case 3: {
				char osc_str[255];
				memset(osc_str, '\0', sizeof(osc_str));
				convert_OSC_bundle_to_string(bndl, osc_str);
				// sd_write_string(file, osc_str);
				sd_save_elem(file, osc_str, '\n');
				break;
			}
			default: {
				LOOM_DEBUG_Println("Invalid bundle printing format");
				break;
			}
		}
	} else {
		LOOM_DEBUG_Println2("Error opening: ", file);
	}
	sdFile.close();
}







// --- ENABLE SD LOGGING ---
//
// Used if message routed from Max instructs
// device to start or stop logging the bundles 
// that it receives
//
// @param msg  OSC messages that had header ending in '/enableSDlogging'
//
void set_SD_logging(OSCMessage &msg)
{
	int setting = msg.getInt(0);

	switch (setting) {
		case 0:
			SD_logging = 0;
			LOOM_DEBUG_Println("Disabled SD logging");
			break;
		case 1:
			SD_logging = 1;
			LOOM_DEBUG_Println("Enabled SD logging of received bundles");
			break;
		#if LOOM_DEBUG == 1
		default:
			Serial.println("Not a valid SD logging setting");
		#endif
	}
}








