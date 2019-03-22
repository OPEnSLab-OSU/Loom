
#ifndef LOOM_SD_h
#define LOOM_SD_h

#include "Loom_LogPlat.h"

#include <SD.h>


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

		/// Chip select pin
		byte		chip_select;

		/// Whether or not the SD hardware was found
		bool		sd_found;

		/// String of file to write to if not filename explicitly provided
		char*		default_file;

		/// Pointer to an RTC object for timestamps
		LoomRTC*	RTC_Inst;


		// SD_Version 		version;
		// byte 			reset_pin;

public:

	// --- CONSTRUCTOR ---
	Loom_SD(	
			char*		module_name			=  "SD", // SD_Module_Name,

			bool		enable_rate_filter	=  true, // SD_Enable_Rate_Filter,
			uint		min_filter_delay	=  1000, // SD_Min_Filter_Delay,

			byte		chip_select			=  10, // SD_Chip_Select,
			char*		default_file		= "test.csv" // SD_Default_File 

			// SD_Version 		version 			= FEATHERWING,
			// byte 			reset_pin 			= A2 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_SD();


	void		print_config();


	/// Add pointer back to device manager.
	/// Generally only called when device manager links module
	/// to provide pointer both directions
	/// \param[in]	LM	LoomManager to point to
	void		link_device_manager(LoomManager* LM);



	// Save bundle to SD card
	// \param[in]	file		The file to save bundle to
	// \param[in]	bndl		The bundle to be saved
	// \param[in]	timestamp	Format of timestamp (if any)
	bool		save_bundle(OSCBundle& bndl, char* file, int timestamp=3);


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
	bool		dump_file(char* file) ;

	void		log_bundle(OSCBundle& bndl);



	// has_keys : set true if array is alternating keys and values, false otherwise
	// timestamp options:
	//   0: no timestamp added
	//   1: only date added
	//   2: only time added
	//   3: both date and time added (two fields)
	//   4: both date and time added (combined field)
	// Device_id, string identifying device - used if forwarded from save_bundle


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


private:

	/// Print the files on the SD card.
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


#endif
