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
						// This can be changed if also using Ethernet 
						// Featherwing which uses the same CS pin

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================

#if sdSaveFilter == 1 
	unsigned long lastSdMillis, currentSdMillis;  
#endif

File sdFile;
bool SD_logging = true; // 0: off, 1: log bundles received
bool SD_working;

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_sd();
void sd_delete_file(char* file);
void sd_list_files();
bool sd_dump_file(char* file);
void printDirectory(File dir, int numTabs);
void sd_write_timestamp(char* file, int timestamp, char delimiter);

template <typename T>
bool sd_save_elem(char *file, T data, char endchar);
template <typename T>
bool sd_save_array(char *file, T data [], int len, char delimiter, int timestamp);
bool sd_save_bundle(char * file, OSCBundle *bndl, int format, int timestamp);

void set_SD_logging(OSCMessage &msg);
void broadcastSaved();


// ================================================================
// ===                          SETUP                           ===
// ================================================================


// --- SETUP SD ---
//
// Handles configuration of reading device configuration settings from flash (if config is saved)
// else uses config.h specified settings, and then writes these to flash
// 
void setup_sd()
{
	LOOM_DEBUG_Print("Initializing SD card...");

	// Remove the #if if you are using a LoRa M0, but don't have LoRa enabled
	#if is_lora == 1
		digitalWrite(8, HIGH); 	// if using LoRa
	#endif

	if (!SD.begin(chipSelect)) {
		LOOM_DEBUG_Println("SD Initialization failed!");
		LOOM_DEBUG_Println("Will continue anyway, but SD functions will be skipped");
		SD_working = false;
	} else {
		LOOM_DEBUG_Println("done");
		SD_working = true;
	}
}



// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================


// --- SD DELETE FILE ---
//
// Delete a file on SD card if it exists
//
// @param file  The file to be deleted
// 
void sd_delete_file(char* file) 
{
	if ( (!SD_working) || (!SD_logging) ) return;

	SD.remove(file);
}


// --- SD EMPTY FILE ---
//
// Empty a file on SD card
//
// @param file  The file to be emptied
// 
void sd_empty_file(char* file)
{
	if ( (!SD_working) || (!SD_logging) ) return;

	SD.remove(file);
	sdFile = SD.open(file, FILE_WRITE);
	sdFile.close();
}


// --- SD LIST FILES
// 
// Lists all the files on the SD card
// 
void sd_list_files()
{
	if ( (!SD_working) || (!SD_logging) ) return;

	printDirectory(SD.open("/"), 0);
}


// --- SD DUMP FILE --- 
//
// Attempts to read all of the contents of a file
// on the SD card
//
// @param file  The name of the file to read from
//
// @return True if no error
//
bool sd_dump_file(char* file) 
{
	if (!SD_working) return false;

	#if is_lora == 1
		digitalWrite(8, HIGH); 	// if using LoRa
	#endif
	SD.begin(chipSelect); // It seems that SD card may become 'unsetup' sometimes, so re-setup

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
		return false;
	}
	sdFile.close();
	return true;
}




#if is_rtc == 1
// --- 	SD WRITE TIMESTAMP ---
//	
// Writes a timestamp to a currently opened file
//  This is just a helper function,
//  expects file to be open already
// Also requires that RTC be enabled
// 
// @param file       File to write timestamp to
// @param timestamp  Formatting of timestamp
// @delimiter        Delimiter if timestamp is two fields
//
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



template <typename T>
bool sd_save_elem(char *file, T data, char endchar)
{	
	if ( (!SD_working) || (!SD_logging) ) return false;

	#if is_lora == 1
		digitalWrite(8, HIGH); 	// if using LoRa
	#endif
	SD.begin(chipSelect); // It seems that SD card may become 'unsetup' sometimes, so re-setup
	
	sdFile = SD.open(file, FILE_WRITE);

	if (sdFile) {
		LOOM_DEBUG_Println4("Saving ", data, " to SD file: ", file);
		sdFile.print(data);
		sdFile.print(endchar);
	} else {
		LOOM_DEBUG_Println2("Error opening: ", file);
		return false;
	}
	sdFile.close();
	return true;
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
	if ( (!SD_working) || (!SD_logging) ) return false;
 
	// Only save if a minimum time (sdSaveMinDelay seconds) has passed since last save
	#if sdSaveFilter == 1
		currentSdMillis = millis();
		if ( (currentSdMillis - lastSdMillis) < (1000*sdSaveMinDelay) ) {
			return false; // has not been long enough yet, just return
		} else {
			lastSdMillis = currentSdMillis;
		}
	#endif // of pushUploadFilter


	#if is_lora == 1
		digitalWrite(8, HIGH); 	// if using LoRa
	#endif
	SD.begin(chipSelect); // It seems that SD card may become 'unsetup' sometimes, so re-setup
	
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
		return false;
	}
	sdFile.close();
	LOOM_DEBUG_Println("Done");


	#if sdBroadcastSave == 1 
		broadcastSaved();
	#endif
	return true;
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
// @param bndl       The bunlde to be saved
// @param format     How to format the saved bundle in the SD card file
// @param timestamp  Format of timestamp (if any)
bool sd_save_bundle(char * file, OSCBundle *bndl, int format, int timestamp)
{
	if ( (!SD_working) || (!SD_logging) ) return false;

	// Only save if a minimum time (sdSaveMinDelay seconds) has passed since last save
	#if sdSaveFilter == 1
		currentSdMillis = millis();
		if ( (currentSdMillis - lastSdMillis) < (1000*sdSaveMinDelay) ) {
			return false; // has not been long enough yet, just return
		} else {
			lastSdMillis = currentSdMillis;
		}
	#endif // of pushUploadFilter


	#if is_lora == 1
		digitalWrite(8, HIGH); 	// if using LoRa
	#endif
	SD.begin(chipSelect); // It seems that SD card may become 'unsetup' sometimes, so re-setup

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
				// LOOM_DEBUG_Println("Converted Bundle:");
				// print_bundle(&tmpBndl);

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
						// sdFile.println();
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
		return false;
	}

	sdFile.close();

	#if sdBroadcastSave == 1 
		broadcastSaved();
	#endif

	return true;
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
			SD_logging = false;
			LOOM_DEBUG_Println("Disabled SD");
			break;
		case 1:
			SD_logging = true;
			LOOM_DEBUG_Println("Enabled SD");
			break;
		#if LOOM_DEBUG == 1
		default:
			Serial.println("Not a valid SD logging setting");
		#endif
	}
}



// --- PRINT DIRECTORY
//
// Helper function to list files on SD card
//
void printDirectory(File dir, int numTabs) 
{
	if ( (!SD_working) || (!SD_logging) ) return;

	while (true) {
		File entry =  dir.openNextFile();
		if (! entry) {
			// no more files
			break;
		}
		for (uint8_t i = 0; i < numTabs; i++) {
			Serial.print('\t');
		}
		Serial.print(entry.name());
		if (entry.isDirectory()) {
			Serial.println("/");
			printDirectory(entry, numTabs + 1);
		} else {
			// files have sizes, directories do not
			Serial.print("\t\t");
			Serial.println(entry.size(), DEC);
		}
		entry.close();
	}
}


void broadcastSaved() 
{
	OSCBundle out_bndl;
	out_bndl.empty();
	char address_string[80];
	sprintf(address_string, "%s%s", global_packet_header_string, "/SD_save");
	out_bndl.add(address_string);

	#if is_wifi == 1
		wifi_send_bundle(&out_bndl);
		// wifi_send_bundle(&out_bndl, config_wifi->devicePort);
	#endif

	#if is_lora == 1
		lora_send_bundle(&out_bndl);
	#endif

	#if is_nrf == 1
		nrf_send_bundle(&out_bndl);
	#endif

	out_bndl.empty();		// Empty the bundle to free room for a new one

}



