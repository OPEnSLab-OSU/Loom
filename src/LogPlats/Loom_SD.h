///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_SD.h
/// @brief		File for Loom_SD definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Loom_LogPlat.h"

#include <SD.h>
#include "../RTC/Loom_RTC.h"

// Forward declare existence of RTC class
// class LoomRTC;

// See if there is any difference to use the SD breakout
// enum SD_Version { FEATHERWING, BREAKOUT };


///////////////////////////////////////////////////////////////////////////////
///
/// SD logging platform module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___s_d.html)
/// - [Product Page: Adafruit Adalogger Featherwing](https://www.adafruit.com/product/2922)
/// - [Product Page: Adafruit SD Breakout](https://www.adafruit.com/product/254)
/// - [Dependency: SD](https://github.com/arduino-libraries/SD)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_SD : public LoomLogPlat
{

protected:

		// File sdFile;
		byte		chip_select;		///< Chip select pin
		bool		sd_found;			///< Whether or not the SD hardware was found
		char		default_file[16];	///< String of file to write to if not filename explicitly provided
		LoomRTC*	RTC_Inst;			///< Pointer to an RTC object for timestamps

		// SD_Version 		version;
		// byte 			reset_pin;

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// SD Module Constructor
	///
	/// @param[in]	enable_rate_filter			Bool | <true> | {true, false} | Whether or not to impose maximum update rate
	/// @param[in]	min_filter_delay			Int | <1000> | [100-5000] | Minimum update delay, if enable_rate_filter enabled
	/// @param[in]	chip_select					Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin to use for chip select
	/// @param[in]	default_file				String | <"test.csv"> | null | File to write to if none explicity provided (should be < 8 characters w/out extension)
	Loom_SD(
			bool			enable_rate_filter	= true,
			uint16_t		min_filter_delay	= 1000,
			byte			chip_select			= 10,
			const char*		default_file		= "test.csv"

			// SD_Version 		version 			= FEATHERWING,
			// byte 			reset_pin 			= A2
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_SD(JsonArrayConst p);

	/// Destructor
	~Loom_SD() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================


	// void		log() override;  

	void		log(JsonObject json) override { save_json(json, default_file); }

	/// Version of logging for use with LoomManager.
	/// Accesses Json from LoomManager
	/// @param[in]	filename	Name of file to write to
	void		log(const char* filename);

	// manually expose superclass version of log() that gets json from
	// linked LoomManager, calling this classes implementation of 
	// 'log(JsonObject json)', which is pure virtual in superclass
	using LoomLogPlat::log; 

	/// Save data to SD card in CSV format
	/// @param[in]	json		The data to be saved
	/// @param[in]	file		The file to save to
	bool		save_json(JsonObject json, const char* file);

	/// Delete a file
	/// @param[in]	file	Name of file to delete
	void		delete_file(const char* file) { if (sd_found) SD.remove(file); }

	/// Clear a file (remove contents but not file itself)
	/// @param[in]	file	Name of file to empty
	void		empty_file(const char* file);

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;

	/// List current files on the SD card
	void		list_files() { if (sd_found) print_directory(SD.open("/"), 0); }

	/// Print the contents of a particular file
	/// @param[in]	file 	Name of file to print
	bool		dump_file(const char* file);

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Return pointer to the currently linked RTC object
	/// @return		Current RTC object
	LoomRTC*	get_RTC_module() { return RTC_Inst; }

	/// Get the current default file to write to
	/// @return Default file
	const char*	get_default_file() { return default_file; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Add pointer back to device manager.
	/// Generally only called when device manager links module
	/// to provide pointer both directions
	/// @param[in]	LM	LoomManager to point to
	void		link_device_manager(LoomManager* LM) override;

	/// Set the RTC module to use for timers
	/// @param[in]	RTC_Inst	Pointer to the RTC object
	void		set_RTC_module(LoomRTC* RTC_Inst) { this->RTC_Inst = RTC_Inst; }

	/// Set default file to write to
	/// @param[in]	filename	New default file (max 8 characters excluding extension)
	void		set_default_file(const char* filename);

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================


private:

	/// Print the files on the SD card.
	/// Recursive for nested folders
	/// @param[in]	dir			File to treat at root
	/// @param[in]	numTabs		Number of tabs to prepend to properly show nesting
	void		print_directory(File dir, uint8_t numTabs);

	/// Auxiliary funcntion to print data element, delimiter, and (optionally) space
	/// @param[in]	sdFile		File to write to
	/// @param[in]	data		Data element to write
	/// @param[in]	delimiter	Delimiter to use
	/// @param[in]	add_space	True to add space after delimiter
	template <typename T>
	void		SD_print_aux(File sdFile, T data, char delimiter, bool add_space=true)
	{
		sdFile.print(data);
		sdFile.print(delimiter);
		if (add_space) {
			sdFile.print(" ");
		}
	}

	/// Aux method to create header row 1 (Categories)
	void _write_json_header_part1(File sdFile, JsonObject dev_id, JsonObject timestamp, JsonArray contents);
	/// Aux method to create Header row 2 (Column names)
	void _write_json_header_part2(File sdFile, JsonObject dev_id, JsonObject timestamp, JsonArray contents);
	/// Aux method to write data values
	void _write_json_data(File sdFile, JsonObject dev_id, JsonObject timestamp, JsonArray contents);

};


