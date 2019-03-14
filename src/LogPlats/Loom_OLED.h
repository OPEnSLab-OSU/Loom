
#ifndef LOOM_OLED_h
#define LOOM_OLED_h

#include "Loom_LogPlat.h"

#include <Adafruit_SSD1306.h>


enum class OLED_Version { 
	FEATHERWING,	///< FeatherWing OLED
	BREAKOUT		///< Breakout board 
};

enum class OLED_Format { 
	FOUR,		///< 4 Key values
	EIGHT,		///< 8 Key values
	SCROLL		///< Scrolling
};

enum class OLED_Freeze { 
	DISABLE, 	///< Freeze disabled
	DATA, 		///< Screen freezes
	SCROLL 		///< Scroll freezes, data updates
};



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
			char*				module_name				= "OLED",

			bool				enable_rate_filter		= true,
			uint				min_filter_delay		= 300,

			OLED_Version		version					= OLED_Version::FEATHERWING,
			byte				reset_pin				= A2,
			OLED_Format			display_format			= OLED_Format::SCROLL,
			uint 				scroll_duration			= 6000,
			byte				freeze_pin				= 10,					// Common pins might be: 10 (Ishield); 9(A), 6(B), 5(C) (OLED Featherwing buttons)
			OLED_Freeze			freeze_behavior			= OLED_Freeze::SCROLL
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
