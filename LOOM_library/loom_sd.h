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

struct config_sd_t {
}; 

struct state_sd_t {
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
// struct config_sd_t configuration;
// struct state_sd_t state_sd;
File sdFile;
// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_sd();

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
	#if LOOM_DEBUG == 1
		Serial.print("Initializing SD card...");
	#endif

	if (!SD.begin(chipSelect)) {
		#if LOOM_DEBUG == 1
			Serial.println("initialization failed!");
		#endif
	}
	#if LOOM_DEBUG == 1
		Serial.println("initialization done.");
	#endif
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
bool read_from_file(char* file) 
{
	sdFile = SD.open(file);
	if (sdFile) {
		#if LOOM_DEBUG == 1
			Serial.print(file); 
			Serial.println(": ");
		#endif

		// read from the file until there's nothing else in it:
		while (sdFile.available()) 
			Serial.write(sdFile.read());
		
		// close the file:
		sdFile.close();

	} else {
		// if the file didn't open, print an error:
		#if LOOM_DEBUG == 1
			Serial.print("Error opening ");
			Serial.println(file);
		#endif
	}
}


// --- SD WRITE TO FILE --- 
//
// Attempts to write text to a file on the SD card
//
// @param file  The name of the file to write to
//
// @return True if no error
//
bool write_to_file(char* file, char* text) 
{
	Serial.print("writing to file :");
	Serial.println(text);
	sdFile = SD.open(file, FILE_WRITE);
	if (sdFile) {

		#if LOOM_DEBUG == 1
			Serial.print("Writing to ");
			Serial.print(file);
			Serial.print("...");
		#endif

		sdFile.println(text);

		#if LOOM_DEBUG == 1
			Serial.println("done");
		#endif
		// close the file:
		sdFile.close();

	} else {
		// if the file didn't open, print an error:
		#if LOOM_DEBUG == 1
			Serial.print("Error writing to ");
			Serial.println(file);
		#endif
	}
}





// --- SD READ BUNDLE
bool sd_read_bundle(char *file, OSCBundle *bndl)
{

}

// --- SD SAVE BUNDLE
bool sd_save_bundle(char *file, OSCBundle *bndl)
{
	Serial.println("sd save bundle");
	char osc_str[255];
	memset(osc_str, '\0', sizeof(osc_str));
	translate_OSC_to_string(bndl, osc_str);
	#if LOOM_DEBUG == 1
		Serial.println(osc_str);
		Serial.print("osc_str length: ");
		Serial.println(strlen(osc_str));
	#endif

	write_to_file(file, osc_str);
}
















