#pragma once

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

// ### (LoomLogPlat) | dependencies: [] | conflicts: []
/// OLED logging platform module.
// ###
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
	const static char* enum_oled_format_string(OLED_Format f);
	const static char* enum_oled_freeze_string(OLED_Freeze f);


	/// OLED module constructor.
	///
	/// \param[in]	module_name				String | <"OLED"> | null | OLED module name
	/// \param[in]	enable_rate_filter		Bool | <true> | {true, false} | Whether or not to impose maximum update rate
	/// \param[in]	min_filter_delay		Int | <300> | [50-5000] | Minimum update delay, if enable_rate_filter enabled
	/// \param[in]	type					Set(OLED_Version) | <0> | {0("Featherwing"), 1("Breakout")} | Which version of the OLED is being used
	/// \param[in]	reset_pin				Set(Int) | <14> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used for reseting. Only applies to breakout version
	/// \param[in]	display_format			Set(OLED_Format) | <A0> | {0("4 pairs"), 1("8 pairs"), 2("Scrolling")} | How to display the key value pairs of a bundle
	/// \param[in]	scroll_duration			Int | <6000> | [500-30000] | The time (ms) to complete full scroll cycle if display_format is SCROLL
	/// \param[in]	freeze_pin				Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used to pause the display
	/// \param[in]	freeze_behavior			Set(OLED_Freeze_Pin) | <2> | {O("Disable"), 1("Pause Data"), 2("Pause Data and Scroll")} | How freezing the display should behave
	Loom_OLED(
			const char*			module_name				= "OLED",
			bool				enable_rate_filter		= true,
			uint				min_filter_delay		= 300,
			OLED_Version		type					= OLED_Version::FEATHERWING,
			byte				reset_pin				= A2,
			OLED_Format			display_format			= OLED_Format::SCROLL,
			uint 				scroll_duration			= 6000,
			byte				freeze_pin				= 10,
			OLED_Freeze			freeze_behavior			= OLED_Freeze::SCROLL
		);

	Loom_OLED(JsonVariant p);


	// --- DESTRUCTOR ---
	virtual ~Loom_OLED();


	void			print_config() override;

	void			log(JsonObject json);


	/// Set the OLED display organization.
	/// \param[in]	format	The OLED_Format to set to
	void			set_display_format(OLED_Format format);
	/// Get the current OLED display organization.
	/// \return		The current OLED_Format being used
	OLED_Format		get_display_format();

	/// Set the duration of a complete scroll.
	/// \param[in]	duration	The duration to set to
	void			set_scroll_duration(uint duration);
	/// Get the current scroll duration.
	/// \return		The current scroll duration
	uint			get_scroll_duration();

	/// Set the digital pin to use to freeze the display
	/// param[in]	pin		The pin to use
	void			set_freeze_pin(byte pin);
	/// Get the digital pin use to freeze the display
	/// return	The freeze pin
	byte			get_freeze_pin();

	/// Set the freezing behavior of the display
	/// \param[in]	behavior	Which OLED_Freeze type to use
	void			set_freeze_behavior(OLED_Freeze behavior);
	/// Get the current freeze behavior
	/// \return		The current freeze behavior
	OLED_Freeze		get_freeze_behavior();



private:

	/// Last display update time.
	/// Used to manage scrolling
	unsigned long	previous_time;

};


