///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Neopixel.h
/// @brief		File for Loom_Neopixel definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Actuator.h"

#include <Adafruit_NeoPixel.h>


///////////////////////////////////////////////////////////////////////////////
///
/// Neopixel tri-color LED actuator module
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___neopixel.html)
/// - [Product Page: Adafruit Neopixel](https://www.adafruit.com/category/168)
/// - [Dependency](https://github.com/adafruit/Adafruit_NeoPixel)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#neopixel)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_Neopixel : public LoomActuator
{

protected:

	Adafruit_NeoPixel	pixels[3];			///< Array of Neopixel instances to manage up to 3 Neopixels

	bool				pin_enabled[3];		///< Whether pins A0-A2 are enabled
	uint8_t				color_vals[3][3];	///< The associated color values (RGB) for each of the 3 Neopixels

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	enableA0		Bool | <false> | {true, false} | Whether or not to use pin A0 for Neopixel
	/// @param[in]	enableA1		Bool | <false> | {true, false} | Whether or not to use pin A1 for Neopixel
	/// @param[in]	enableA2		Bool | <true> | {true, false} | Whether or not to use pin A2 for Neopixel
	Loom_Neopixel(
			LoomManager* 	manager,
			const bool					enableA0		= false,
			const bool					enableA1		= false,
			const bool					enableA2		= true
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_Neopixel(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_Neopixel() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool		dispatch(JsonObject json) override;
	void		add_config(JsonObject json) override;

	/// Set Neopixel color.
	/// @param[in]	port		The port the Neopixel to control is on (0-2 corresponding to A0-A2)
	/// @param[in]	chain_num	The Neopixel number if daisy-chained
	/// @param[in]	red			The red value (0-255)
	/// @param[in]	green		The green value (0-255)
	/// @param[in]	blue		The blue value (0-255)
	void		set_color(const uint8_t port, const uint8_t chain_num, const uint8_t red, const uint8_t green, const uint8_t blue);

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_state() const override;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// enable or disable pin for use with Neopixel
	/// @param[in]	port	Port to configure (0-2)
	/// @param[in]	state	Enable or disable
	void		enable_pin(const uint8_t port, const bool state);

private:


};


