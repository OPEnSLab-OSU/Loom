
#ifndef LOOM_SD_h
#define LOOM_SD_h

#include "Loom_LogPlat.h"

#include <SD.h>
#include <SPI.h>
#include <OSCBundle.h>


class LoomRTC;


// See if there is any difference to use the SD breakout

// enum SD_Version { FEATHERWING, BREAKOUT };

// Maybe add back the recieved bundle logging? - can probably just be done manually

// Mak sure timestamps work properly

// enum BundleStructure { SINGLEMSG, MULTIMSG };

// Maybe remove timestamp options and just go with #3?

// getters and setters for save/timestamp settings


class Loom_SD : public LoomLogPlat
{

protected:

		// File SDFile;

		byte 	chip_select;
		bool 	sd_found;

		char* 	default_file;

		// SD_Version 		version;
		// byte 			reset_pin;

public:

	// --- CONSTRUCTOR ---
	Loom_SD(	char* 			module_name 		= "SD",

				bool 			enable_rate_filter 	= true,
				uint 			min_filter_delay 	= 1000,

				byte 			chip_select 		= 10,
				char* 			default_file 		= "test.csv"

				// SD_Version 		version 			= FEATHERWING,
				// byte 			reset_pin 			= A2 
			   );

	// --- DESTRUCTOR ---
	virtual ~Loom_SD();



	void print_config();
	void set_default_file(char* filename);
	char* get_default_file();
	void delete_file(char* file);
	void empty_file(char* file);
	void list_files();
	bool dump_file(char* file) ;

	void log_bundle(OSCBundle& bndl);


	// --- SD SAVE BUNDLE --- 
	//
	// @param file       The file to save bundle to
	// @param bndl       The bundle to be saved
	// @param timestamp  Format of timestamp (if any)
	bool save_bundle(OSCBundle& bndl, char* file, int timestamp=3);



	// has_keys : set true if array is alternating keys and values, false otherwise
	// timestamp options:
	//   0: no timestamp added
	//   1: only date added
	//   2: only time added
	//   3: both date and time added (two fields)
	//   4: both date and time added (combined field)
	// Device_id, string identifying device - used if forwarded from save_bundle
	template <typename T>
	bool save_array(char *file, T data [], int len, char delimiter=',', int timestamp=3, bool has_keys=false, char* device_id="");


private:

	void print_directory(File dir, int numTabs);

	template <typename T>
	void SD_print_aux(File SDFile, T data, char delimiter, bool add_space=true)
	{
		SDFile.print(data);
		SDFile.print(delimiter);
		if (add_space) {
			SDFile.print(" ");			
		}
	}
	
};


#endif
