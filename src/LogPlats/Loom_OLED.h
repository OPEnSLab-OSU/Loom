
#ifndef LOOM_OLED_h
#define LOOM_OLED_h

#include "Loom_LogPlat.h"

#include <Adafruit_SSD1306.h>


// Maybe move these enums into class

/// Different forms of the OLED display
enum class OLED_Version { 
	FEATHERWING,	///< FeatherWing OLED
	BREAKOUT		///< Breakout board 
};

/// Different formats to display information in
enum class OLED_Format { 
	FOUR,		///< 4 Key values
	EIGHT,		///< 8 Key values
	SCROLL		///< Scrolling
};

/// Differnt freeze behaviors
enum class OLED_Freeze { 
	DISABLE, 	///< Freeze disabled
	DATA, 		///< Screen freezes
	SCROLL 		///< Scroll freezes, data updates
};


/// OLED logging platform module
class Loom_OLED : public LoomLogPlat
{

protected:

		/// Underlying OLED controller
		Adafruit_SSD1306*	display;

		/// What type the OLED is (FeatherWing or breakout)
		OLED_Version		version;

		/// The reset pin (only applies to breakout version)
		byte				reset_pin;

		/// How to display the data on OLED
		OLED_Format			display_format;

		/// The duration to complete a full cycle through a bundle of data (milliseconds)(non-blocking)
		uint				scroll_duration; 

		/// Which pin to check if display should freeze
		byte				freeze_pin;

		/// What 'freezing' behavior should be followed
		OLED_Freeze			freeze_behavior;

public:

	const static char* enum_oled_version_string(OLED_Version v);
	const static char* enum_oled_format(OLED_Format f);
	const static char* enum_oled_freeze(OLED_Freeze f);


	// --- CONSTRUCTOR ---
	Loom_OLED(	
			char*				module_name				=  "OLED",     // OLED_Module_Name,

			bool				enable_rate_filter		=  true,     // OLED_Enable_Rate_Filter,
			uint				min_filter_delay		=  300,     // OLED_Min_Filter_Delay,

			OLED_Version		type					=  OLED_Version::FEATHERWING,     // OLED_Type,
			byte				reset_pin				=  A2,     // OLED_Reset_Pin,
			OLED_Format			display_format			=  OLED_Format::SCROLL,     // OLED_Display_Format,
			uint 				scroll_duration			=  6000,     // OLED_Scroll_Duration,
			byte				freeze_pin				=  10,					// Common pins might be: 10 (Ishield); 9(A), 6(B), 5(C) (OLED Featherwing buttons)     // OLED_Freeze_Pin,
			OLED_Freeze			freeze_behavior			= OLED_Freeze::SCROLL     // OLED_Freeze_Behavior 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_OLED();


	void			print_config();

	void			log_bundle(OSCBundle& bndl);
	void			log_bundle(OSCBundle& bndl, OLED_Format format);



	void			set_display_format(OLED_Format format);
	OLED_Format		get_display_format();

	void			set_scroll_duration(uint duration);
	uint			get_scroll_duration();

	void			set_freeze_pin(byte pin);
	byte			get_freeze_pin();

	void			set_freeze_behavior(OLED_Freeze behavior);
	OLED_Freeze		get_freeze_behavior();



private:
	
	/// Last display update time.
	/// Used to manage scrolling
	unsigned long	previous_time; 

};



#endif
