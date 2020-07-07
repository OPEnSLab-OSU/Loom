///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Digital.h
/// @brief		File for Loom_Digital definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Sensor.h"

/// Number of digital pins
#define DIGITAL_COUNT 12


///////////////////////////////////////////////////////////////////////////////
///
/// Digital pin manager module.
///
/// Available digital pins 5, 6, 9, 10, 11, 12, A0(14), A1(15), A2(16), A3(17), A4(18), A5(19)
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___digital.html)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#spi-sensors)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_Digital : public LoomSensor
{

protected:

	bool		pin_enabled[DIGITAL_COUNT];		///< Array of which pins are enabled
	bool		digital_vals[DIGITAL_COUNT];	///< Array of last read digital values

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Digital manager module constructor
	///
	/// @param[in]	enable5				Bool | <true> | {true, false} | Enable pin 5 for managing
	/// @param[in]	enable6				Bool | <true> | {true, false} | Enable pin 6 for managing
	/// @param[in]	enable9				Bool | <false> | {true, false} | Enable pin 9 for managing
	/// @param[in]	enable10			Bool | <false> | {true, false} | Enable pin 10 for managing
	/// @param[in]	enable11			Bool | <false> | {true, false} | Enable pin 11 for managing
	/// @param[in]	enable12			Bool | <false> | {true, false} | Enable pin 12 for managing
	/// @param[in]	enableA0			Bool | <false> | {true, false} | Enable pin A0 for managing
	/// @param[in]	enableA1			Bool | <false> | {true, false} | Enable pin A1 for managing
	/// @param[in]	enableA2			Bool | <false> | {true, false} | Enable pin A2 for managing
	/// @param[in]	enableA3			Bool | <false> | {true, false} | Enable pin A3 for managing
	/// @param[in]	enableA4			Bool | <false> | {true, false} | Enable pin A4 for managing
	/// @param[in]	enableA5			Bool | <false> | {true, false} | Enable pin A5 for managing
	Loom_Digital(
			LoomManager* manager,
			const bool		enable5			= true,
			const bool		enable6			= true,
			const bool		enable9			= false,
			const bool		enable10		= false,
			const bool		enable11		= false,
			const bool		enable12		= false,
			const bool		enableA0		= false,
			const bool		enableA1		= false,
			const bool		enableA2		= false,
			const bool		enableA3		= false,
			const bool		enableA4		= false,
			const bool		enableA5		= false
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_Digital(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_Digital() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;
	void		add_config(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_measurements() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get value on digital pin
	/// @return		True if HIGH, false if LOW
	bool		get_digital_val(const uint8_t pin) const;

	/// Get if pin is enabled in manager
	/// @return 	Enable state of pin
	bool		get_pin_enabled(const uint8_t pin) const;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set digital value of pin
	/// @param[in]	pin		Pin to set value of
	/// @param[in]	state	State to set pin to
	void		set_digital_val(const uint8_t pin, const bool state);

	/// Set pin enable state in manager
	/// @param[in]	pin		Pin to set enable state of
	/// @param[in]	e		Enable state to set to
	void		set_pin_enabled(const uint8_t pin, const bool e);

private:

	/// Used to convert pin number to index in member arrays
	const static uint8_t pin_nums[DIGITAL_COUNT];

	/// Convert pin number to index in manager arrays
	/// @param[in]	pin		Pin to convert to index
	/// @return Index
	uint8_t		pin_to_index(const uint8_t pin) const;

};

