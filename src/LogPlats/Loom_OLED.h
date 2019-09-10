///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_OLED.h
/// @brief		File for Loom_OLED definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Loom_LogPlat.h"

#include <Adafruit_SSD1306.h>


///////////////////////////////////////////////////////////////////////////////


// ### (LoomLogPlat) | dependencies: [] | conflicts: []
/// OLED logging platform module.
// ###
class Loom_OLED : public LoomLogPlat
{

public:

	/// Different forms of the OLED display
	enum class Version {
		FEATHERWING,	///< FeatherWing OLED
		BREAKOUT		///< Breakout board
	};

	/// Different formats to display information in
	enum class Format {
		FOUR,			///< 4 Key values
		EIGHT,			///< 8 Key values
		SCROLL			///< Scrolling
	};

	/// Differnt freeze behaviors
	enum class FreezeType {
		DISABLE, 		///< Freeze disabled
		DATA, 			///< Screen freezes
		SCROLL 			///< Scroll freezes, data updates
	};

protected:

		Adafruit_SSD1306	display;			///< Underlying OLED controller
		Version				version;			///< What type the OLED is (FeatherWing or breakout)
		byte				reset_pin;			///< The reset pin (only applies to breakout version)
		
		Format				display_format;		///< How to display the data on OLED
		uint				scroll_duration;	///< The duration to complete a full cycle through a bundle of data (milliseconds)(non-blocking)
		byte				freeze_pin;			///< Which pin to check if display should freeze
		FreezeType			freeze_behavior;	///< What 'freezing' behavior should be followed

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// OLED module constructor.
	///
	/// \param[in]	enable_rate_filter		Bool | <true> | {true, false} | Whether or not to impose maximum update rate
	/// \param[in]	min_filter_delay		Int | <300> | [50-5000] | Minimum update delay, if enable_rate_filter enabled
	/// \param[in]	type					Set(Version) | <0> | {0("Featherwing"), 1("Breakout")} | Which version of the OLED is being used
	/// \param[in]	reset_pin				Set(Int) | <14> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used for reseting. Only applies to breakout version
	/// \param[in]	display_format			Set(Format) | <A0> | {0("4 pairs"), 1("8 pairs"), 2("Scrolling")} | How to display the key value pairs of a bundle
	/// \param[in]	scroll_duration			Int | <6000> | [500-30000] | The time (ms) to complete full scroll cycle if display_format is SCROLL
	/// \param[in]	freeze_pin				Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used to pause the display
	/// \param[in]	freeze_behavior			Set(FreezeType) | <2> | {O("Disable"), 1("Pause Data"), 2("Pause Data and Scroll")} | How freezing the display should behave
	Loom_OLED(
			bool			enable_rate_filter		= true,
			uint16_t		min_filter_delay		= 300,
			Version			type					= Version::FEATHERWING,
			byte			reset_pin				= A2,
			Format			display_format			= Format::SCROLL,
			uint16_t		scroll_duration			= 6000,
			byte			freeze_pin				= 10,
			FreezeType		freeze_behavior			= FreezeType::SCROLL
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_OLED(JsonArrayConst p);

	/// Destructor
	~Loom_OLED() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void			log(JsonObject json) override;

	// manually expose superclass version of log() that gets json from
	// linked LoomManager, calling this classes implementation of 
	// 'log(JsonObject json)', which is pure virtual in superclass
	using LoomLogPlat::log; 

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void			print_config() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the current OLED display organization.
	/// \return		The current Format being used
	Format			get_display_format() { return display_format; }

	/// Get the current scroll duration.
	/// \return		The current scroll duration
	uint			get_scroll_duration() { return scroll_duration; }

	/// Get the digital pin use to freeze the display
	/// \return	The freeze pin
	byte			get_freeze_pin() { return freeze_pin; }

	/// Get the current freeze behavior
	/// \return		The current freeze behavior
	FreezeType		get_freeze_behavior() { return freeze_behavior; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set the OLED display organization.
	/// \param[in]	format	The Format to set to
	void			set_display_format(Format format) { display_format = format; }

	/// Set the duration of a complete scroll.
	/// \param[in]	duration	The duration to set to
	void			set_scroll_duration(uint duration) { scroll_duration = duration; }

	/// Set the digital pin to use to freeze the display
	/// \param[in]	pin		The pin to use
	void			set_freeze_pin(byte pin);

	/// Set the freezing behavior of the display
	/// \param[in]	behavior	Which FreezeType type to use
	void			set_freeze_behavior(FreezeType behavior) { freeze_behavior = behavior; }

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Get c-string of name associated with OLED::Version enum
	/// \param[in]	v	OLED::Version value to get string of
	/// \return C-string of OLED version
	const static char* enum_oled_version_string(Version v);
	
	/// Get c-string of name associated with OLED:Format enum
	/// \param[in]	f	OLED::Format value to get string of
	/// \return C-string of OLED format
	const static char* enum_oled_format_string(Format f);

	/// Get c-string of name associated with OLED freeze behavior enum
	/// \param[in]	f	FreezeType to get string of
	/// \return C-string of OLED freeze behavior
	const static char* enum_oled_freeze_string(FreezeType f);

private:

	/// Last display update time.
	/// Used to manage scrolling
	unsigned long	previous_time;

};


