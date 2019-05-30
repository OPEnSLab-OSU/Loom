#pragma once

#include "Loom_LogPlat.h"

#include <SD.h>

// Forward declare existence of RTC class
class LoomRTC;


// See if there is any difference to use the SD breakout

// enum SD_Version { FEATHERWING, BREAKOUT };

// Maybe add back the recieved bundle logging? - can probably just be done manually

// Mak sure timestamps work properly

// enum BundleStructure { SINGLEMSG, MULTIMSG };

// Maybe remove timestamp options and just go with #3?

// getters and setters for save/timestamp settings



// ### (LoomLogPlat) | dependencies: [] | conflicts: []
/// SD logging platform module
// ###
class Loom_SD : public LoomLogPlat
{

protected:

		// File SDFile;

		/// Chip select pin
		byte		chip_select;

		/// Whether or not the SD hardware was found
		bool		sd_found;

		/// String of file to write to if not filename explicitly provided
		char		default_file[16];

		/// Pointer to an RTC object for timestamps
		LoomRTC*	RTC_Inst;


		// SD_Version 		version;
		// byte 			reset_pin;

public:

	/// SD Module Constructor
	///
	/// \param[in]	module_name					String | <"SD"> | null | OLED module name
	/// \param[in]	enable_rate_filter	Bool | <true> | {true, false} | Whether or not to impose maximum update rate
	/// \param[in]	min_filter_delay		Int | <1000> | [100-5000] | Minimum update delay, if enable_rate_filter enabled
	/// \param[in]	chip_select					Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin to use for chip select
	/// \param[in]	default_file				String | <"test.csv"> | null | File to write to if none explicity provided (should be < 8 characters w/out extension)
	Loom_SD(
			const char*		module_name			= "SD",
			bool			enable_rate_filter	= true,
			uint			min_filter_delay	= 1000,
			byte			chip_select			= 10,
			const char*		default_file		= "test.csv"

			// SD_Version 		version 			= FEATHERWING,
			// byte 			reset_pin 			= A2
		);

	Loom_SD(JsonVariant p);


	// --- DESTRUCTOR ---
	virtual ~Loom_SD();


	void		print_config() override;


	/// Add pointer back to device manager.
	/// Generally only called when device manager links module
	/// to provide pointer both directions
	/// \param[in]	LM	LoomManager to point to
	void		link_device_manager(LoomManager* LM);



	// Save bundle to SD card
	// \param[in]	file		The file to save bundle to
	// \param[in]	bndl		The bundle to be saved
	// \param[in]	timestamp	Format of timestamp (if any)
	bool		save_json(JsonObject json, const char* file, int timestamp=3);


	/// Set the RTC module to use for timers
	/// \param[in]	RTC_Inst	Pointer to the RTC object
	void		set_RTC_module(LoomRTC* RTC_Inst);
	/// Return pointer to the currently linked RTC object
	/// \return		Current RTC object
	LoomRTC*	get_RTC_module();


	void		set_default_file(char* filename);
	char*		get_default_file();
	void		delete_file(char* file);
	void		empty_file(char* file);
	void		list_files();
	bool		dump_file(char* file);

	void		log(JsonObject json);



	// has_keys : set true if array is alternating keys and values, false otherwise
	// timestamp options:
	//   0: no timestamp added
	//   1: only date added
	//   2: only time added
	//   3: both date and time added (two fields)
	//   4: both date and time added (combined field)
	// Device_id, string identifying device - used if forwarded from save_bundle


// Fix this as it is currently in .cpp

	/// Save array to SD card.
	/// Takes array of generic type
	// \param[in]	file		The file to save array to
	// \param[in]	data		The array of data to save
	// \param[in]	len			Length of the incoming array
	// \param[in]	delimiter	Delimiter to use
	// \param[in]	timestamp	Timestamp setting to use
	// \param[in]	has_keys	True if data is assumed to have keys (alternating key-values)
	// \param[in]	device_id	Device ID to label row with
	template <typename T>
	bool		save_array(char *file, T data [], int len, char delimiter=',', int timestamp=3, bool has_keys=false, char* device_id="");
	// {
	// 	if ( !sd_found || !check_millis() ) return false;

	// 	// #if is_lora == 1
	// 	digitalWrite(8, HIGH); 	// if using LoRa
	// 	// #endif

	// 	bool got_timestamp = false;

	// 	SD.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup
	// 	File SDFile = SD.open(file, FILE_WRITE);

	// 	// If file successfully opened
	// 	if (SDFile) {
	// 		LPrint("Saving array to SD file: '", file, "' ...");

	// 		char time_key[30], time_val[30];

	// 		if (timestamp) {
	// 			if (device_manager != NULL) {
	// 				// LoomRTC* rtc = device_manager->get_rtc_module(0);
	// 				if (RTC_Inst != NULL) {
	// 					LPrintln("RTC Object: ", RTC_Inst->get_module_name() );
	// 					RTC_Inst->get_timestamp(time_key, time_val, delimiter, timestamp);
	// 					got_timestamp = true;
	// 				}
	// 			}
	// 		}

	// 		// Array is assumed to have alternating keys and values
	// 		if (has_keys) {

	// 			// Check if at first row (create header)
	// 			if ( SDFile.position() == 0) {

	// 				// Add timestamp header
	// 				if (got_timestamp) {
	// 					SDFile.print(time_key);
	// 				}

	// 				// Add address header if address was provided
	// 				if (strlen(device_id) > 0) {
	// 					SD_print_aux(SDFile, "Device", delimiter);
	// 				}

	// 				// LPrint keys
	// 				for (int i = 0; i < len-2; i+=2) {
	// 					SD_print_aux(SDFile, data[i], delimiter);
	// 				}
	// 				SDFile.println(data[len-2]);
	// 			}

	// 			// Add timestamp
	// 			if (got_timestamp) {
	// 				SDFile.print(time_val);
	// 			}

	// 			// Add device ID if provided
	// 			if (strlen(device_id) > 0) {
	// 				SD_print_aux(SDFile, device_id, delimiter);
	// 			}

	// 			// LPrint values
	// 			for (int i = 1; i < len-2; i+=2) {
	// 				SD_print_aux(SDFile, data[i], delimiter);
	// 			}
	// 			SDFile.println(data[len-1]);
	// 		}

	// 		// Array is assume to only have values
	// 		else {

	// 			if (got_timestamp) {
	// 				SDFile.print(time_val);
	// 			}

	// 			// Add device ID if provided
	// 			if (strlen(device_id) > 0) {
	// 				SD_print_aux(SDFile, device_id, delimiter);
	// 			}

	// 			for (int i = 0; i < len-1; i++) {
	// 				SD_print_aux(SDFile, data[i], delimiter);
	// 			}
	// 			SDFile.println(data[len-1]);
	// 		}

	// 		SDFile.close();
	// 		LPrintln("Done");
	// 		return true;
	// 	}

	// 	// If file could not be opened
	// 	else {
	// 		LPrintln("Error opening: ", file);
	// 		return false;
	// 	}
	// }

private:

	/// LPrint the files on the SD card.
	/// Recursive for nested folders
	/// \param[in]	dir			File to treat at root
	/// \param[in]	numTabs		Number of tabs to prepend to properly show nesting
	void		print_directory(File dir, int numTabs);

	/// Auxiliary funcntion to print data element, delimiter, and (optionally) space
	/// \param[in]	SDFile		File to write to
	/// \param[in]	data		Data element to write
	/// \param[in]	delimiter	Delimiter to use
	/// \param[in]	add_space	True to add space after delimiter
	template <typename T>
	void		SD_print_aux(File SDFile, T data, char delimiter, bool add_space=true)
	{
		SDFile.print(data);
		SDFile.print(delimiter);
		if (add_space) {
			SDFile.print(" ");
		}
	}

};


