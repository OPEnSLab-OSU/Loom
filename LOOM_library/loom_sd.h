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
//
// @return True if no error
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

// --- READ OSCBUNDLE FROM FILE
bool read_OSCBundle_from_file(char *file, OSCBundle *bndl)
{

}

// --- READ OSCBUNDLE FROM FILE
bool write_OSCBundle_to_file(char *file, OSCBundle *bndl)
{

}


