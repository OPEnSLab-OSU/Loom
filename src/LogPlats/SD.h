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

#include "LogPlat.h"

#include <SdFat.h>
#include "../RTC/RTC.h"


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
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#sd-card)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_SD : public LoomLogPlat
{

protected:

		SdFat sd;						///< File system object
		// File file;

		const byte	chip_select;		///< Chip select pin
		char		filename[13];		///< String of file to write to if not filename explicitly provided (should not exceed 6 characters)
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
	/// @param[in]	default_file_base			String | <"test"> | null | File to write to if none explicity provided (should be <= 6 characters, don't add extension)
	/// @param[in]	number_files				Bool | <true> | {true, false} | True to number files with run number, false to not.
	Loom_SD(
			LoomManager* manager,
			const bool			enable_rate_filter	= true,
			const uint16_t		min_filter_delay	= 1000,
			const byte			chip_select			= 10,
			const char*			default_file		= "test",
			const bool			number_files		= true

			// SD_Version 		version 			= FEATHERWING,
			// byte 			reset_pin 			= A2
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_SD(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_SD() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================


	// void		log() override;  

	bool		log(JsonObject json) override { return save_json(json, filename); }

	/// Version of logging for use with LoomManager.
	/// Accesses Json from LoomManager
	/// @param[in]	filename	Name of file to write to
	bool		log(const char* name);

	// manually expose superclass version of log() that gets json from
	// linked LoomManager, calling this classes implementation of 
	// 'log(JsonObject json)', which is pure virtual in superclass
	using LoomLogPlat::log; 

	/// Save data to SD card in CSV format.
	/// Format:
	/// Identification Date Time ModuleA key1 val1 key2 val2 ... ModuleB key1 val1 ...   
	/// @param[in]	json		The data to be saved
	/// @param[in]	filename	The file to save to
	bool		save_json(JsonObject json, const char* name);

	/// Delete a file
	/// @param[in]	filename	Name of file to delete
	void		delete_file(const char* name) { sd.remove(name); }

	/// Clear a file (remove contents but not file itself)
	/// @param[in]	filename	Name of file to empty
	void		empty_file(const char* name);
	
	void		power_up() override;
	void 		power_down() override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;

	/// List current files on the SD card
	// void		list_files() { print_directory(sd.open("/"), 0); }

	/// Print the contents of a particular file
	/// @param[in]	filename 	Name of file to print
	bool		dump_file(const char* name);

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Return pointer to the currently linked RTC object
	/// @return		Current RTC object
	LoomRTC*	get_RTC_module() const { return RTC_Inst; }

	/// Get the current default file to write to
	/// @return Default file
	const char*	get_default_file() const { return filename; }

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
	void		set_filename(const char* name);

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================


private:

	/// Print the files on the SD card.
	/// Recursive for nested folders
	/// @param[in]	dir			File to treat at root
	/// @param[in]	numTabs		Number of tabs to prepend to properly show nesting
	// void		print_directory(File dir, const uint8_t numTabs) const;

	/// Auxiliary funcntion to print data element, delimiter, and (optionally) space
	/// @param[in]	file		File to write to
	/// @param[in]	data		Data element to write
	/// @param[in]	delimiter	Delimiter to use
	/// @param[in]	add_space	True to add space after delimiter
	template <typename T>
	void		SD_print_aux(File file, T data, const char delimiter, const bool add_space=true) const
	{
		file.print(data);
		file.print(delimiter);
		if (add_space) {
			file.print(" ");
		}
	}

	/// Aux method to create header row 1 (Categories)
	void _write_json_header_part1(File& file, JsonObject dev_id, JsonObject timestamp, JsonArray contents) const;
	/// Aux method to create Header row 2 (Column names)
	void _write_json_header_part2(File& file, JsonObject dev_id, JsonObject timestamp, JsonArray contents) const;
	/// Aux method to write data values
	void _write_json_data(File& file, JsonObject dev_id, JsonObject timestamp, JsonArray contents) const;

	/// When instantiating the SD module, you provide a file name base, to which a number is appended.
	/// This number represents the run number, starting at 0. 
	/// For example, if you use the file name base 'data', the first run will generate a file named: 'data00.csv'.
	/// The next run of the program will produce 'data01.csv' and so on
	/// @param[in]	default_file	File name base if using file numbers, or complete file name if not using numbers
	/// @param[in]	number_files	True to use numbers and increment number each run. False otherwise.
	/// @return True is generating filename and opening it works, false otherwise.
	bool update_filename(const char* default_file, const bool number_files);

};


