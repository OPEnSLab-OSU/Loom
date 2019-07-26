#pragma once

#include "Loom_LogPlat.h"

#include <SD.h>
#include "../RTC/Loom_RTC.h"

// Forward declare existence of RTC class
// class LoomRTC;

// See if there is any difference to use the SD breakout
// enum SD_Version { FEATHERWING, BREAKOUT };


///////////////////////////////////////////////////////////////////////////////


// ### (LoomLogPlat) | dependencies: [] | conflicts: []
/// SD logging platform module
// ###
class Loom_SD : public LoomLogPlat
{

protected:

		// File sdFile;
		byte		chip_select;		/// Chip select pin
		bool		sd_found;			/// Whether or not the SD hardware was found
		char		default_file[16];	/// String of file to write to if not filename explicitly provided
		LoomRTC*	RTC_Inst;			/// Pointer to an RTC object for timestamps

		// SD_Version 		version;
		// byte 			reset_pin;

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

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
			uint16_t		min_filter_delay	= 1000,
			byte			chip_select			= 10,
			const char*		default_file		= "test.csv"

			// SD_Version 		version 			= FEATHERWING,
			// byte 			reset_pin 			= A2
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_SD(JsonArrayConst p);

	/// Destructor
	~Loom_SD() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		log(JsonObject json) override { save_json(json, default_file); }

	/// Version of logging for use with LoomManager.
	/// Accesses Json from LoomManager
	/// \param[in]	filename	Name of file to write to
	void		log(const char* filename);

	// manually expose superclass version of log() that gets json from
	// linked LoomManager, calling this classes implementation of 
	// 'log(JsonObject json)', which is pure virtual in superclass
	using LoomLogPlat::log; 

	/// Save data to SD card in CSV format
	/// \param[in]	json		The data to be saved
	/// \param[in]	file		The file to save to
	bool		save_json(JsonObject json, const char* file);

	/// Delete a file
	/// \param[in]	file	Name of file to delete
	void		delete_file(const char* file) { if (sd_found) SD.remove(file); }

	/// Clear a file (remove contents but not file itself)
	/// \param[in]	file	Name of file to empty
	void		empty_file(const char* file);

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;

	/// List current files on the SD card
	void		list_files() { if (sd_found) print_directory(SD.open("/"), 0); }

	/// Print the contents of a particular file
	/// \param[in]	file 	Name of file to print
	bool		dump_file(const char* file);

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Return pointer to the currently linked RTC object
	/// \return		Current RTC object
	LoomRTC*	get_RTC_module() { return RTC_Inst; }

	/// Get the current default file to write to
	/// \return Default file
	const char*	get_default_file() { return default_file; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Add pointer back to device manager.
	/// Generally only called when device manager links module
	/// to provide pointer both directions
	/// \param[in]	LM	LoomManager to point to
	void		link_device_manager(LoomManager* LM) override;

	/// Set the RTC module to use for timers
	/// \param[in]	RTC_Inst	Pointer to the RTC object
	void		set_RTC_module(LoomRTC* RTC_Inst) { this->RTC_Inst = RTC_Inst; }

	/// Set default file to write to
	/// \param[in]	filename	New default file (max 8 characters excluding extension)
	void		set_default_file(const char* filename);

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================



	/// Save array to SD card.
	/// Takes array of generic type
	/// \param[in]	file		The file to save array to
	/// \param[in]	data		The array of data to save
	/// \param[in]	len			Length of the incoming array
	/// \param[in]	delimiter	Delimiter to use
	/// \param[in]	timestamp	Timestamp setting to use.
	///							Options:
	///							   0: no timestamp added
	///							   1: only date added
	///							   2: only time added
	///							   3: both date and time added (two fields)
	///							   4: both date and time added (combined field)
	/// \param[in]	has_keys	True if data is assumed to have keys (alternating key-values)
	/// \param[in]	device_id	Device ID to label row with
	// template <typename T>
	// bool save_array(const char *file, T data [], int len, char delimiter, int timestamp, bool has_keys, char* device_id) 
	// {
	// 	if ( !sd_found || !check_millis() ) return false;

	// 	digitalWrite(8, HIGH); 	// if using LoRa

	// 	bool got_timestamp = false;

	// 	SD.begin(chip_select); // It seems that SD card may become 'unsetup' sometimes, so re-setup
	// 	File sdFile = SD.open(file, FILE_WRITE);

	// 	// If file successfully opened
	// 	if (sdFile) {
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
	// 			if ( sdFile.position() == 0) {
	// 				// Add timestamp header
	// 				if (got_timestamp) sdFile.print(time_key); 
	// 				// Add address header if address was provided
	// 				if (strlen(device_id) > 0) SD_print_aux(sdFile, "Device", delimiter); 
	// 				// LPrint keys
	// 				for (int i = 0; i < len-2; i+=2) 
	// 					SD_print_aux(sdFile, data[i], delimiter);
	// 				sdFile.println(data[len-2]);
	// 			}
	// 			// Add timestamp
	// 			if (got_timestamp) sdFile.print(time_val); 
	// 			// Add device ID if provided
	// 			if (strlen(device_id) > 0) SD_print_aux(sdFile, device_id, delimiter); 
	// 			// LPrint values 
	// 			for (int i = 1; i < len-2; i+=2) 
	// 				SD_print_aux(sdFile, data[i], delimiter); 
	// 			sdFile.println(data[len-1]);
	// 		} 
	// 		// Array is assume to only have values
	// 		else {
	// 			if (got_timestamp) sdFile.print(time_val); 
	// 			// Add device ID if provided
	// 			if (strlen(device_id) > 0) SD_print_aux(sdFile, device_id, delimiter); 
	// 			for (int i = 0; i < len-1; i++) 
	// 				SD_print_aux(sdFile, data[i], delimiter);
	// 			sdFile.println(data[len-1]);
	// 		}
	// 		sdFile.close();
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

	/// Print the files on the SD card.
	/// Recursive for nested folders
	/// \param[in]	dir			File to treat at root
	/// \param[in]	numTabs		Number of tabs to prepend to properly show nesting
	void		print_directory(File dir, uint8_t numTabs);

	/// Auxiliary funcntion to print data element, delimiter, and (optionally) space
	/// \param[in]	sdFile		File to write to
	/// \param[in]	data		Data element to write
	/// \param[in]	delimiter	Delimiter to use
	/// \param[in]	add_space	True to add space after delimiter
	template <typename T>
	void		SD_print_aux(File sdFile, T data, char delimiter, bool add_space=true)
	{
		sdFile.print(data);
		sdFile.print(delimiter);
		if (add_space) {
			sdFile.print(" ");
		}
	}

	/// Create Header Row 1 (Categories)
	void _write_json_header_part1(File sdFile, JsonObject dev_id, JsonObject timestamp, JsonArray contents);
	/// Create Header Row 2 (Column names)
	void _write_json_header_part2(File sdFile, JsonObject dev_id, JsonObject timestamp, JsonArray contents);
	/// Write data values
	void _write_json_data(File sdFile, JsonObject dev_id, JsonObject timestamp, JsonArray contents);

};


