///////////////////////////////////////////////////////////////////////////////
///
/// @file		Relay.h
/// @brief		File for Relay definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_ACTUATORS
#pragma once

#include "Actuator.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// Relay actuator module
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___relay.html)
/// - [Product Page: Adafruit Latching Relay Featherwing](https://www.adafruit.com/product/3191)
/// - [Relay Datasheet](https://cdn-shop.adafruit.com/product-files/3191/G5LE-14-DC3-Omron-datasheet-10841140.pdf)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#relay)
///
///////////////////////////////////////////////////////////////////////////////
class Relay : public Actuator
{

protected:

	byte 	pin;	///< The digital pin the relay is connect to
	bool	on;		///< Whether the relay is on or off

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor.
	///
	/// @param[in]	pin		Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used to control the relay
	Relay(
			const byte					pin				= 10
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Relay(JsonArrayConst p);

	/// Destructor
	~Relay() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		package(JsonObject json) override;
	bool		dispatch(JsonObject json) override;

	/// Set relay state
	/// @param[in]	state	The state to set relay to (True=HIGH, False=LOW)
	void		set(const bool state);

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_state() const override;

private:

};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, Relay, "Relay");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_ACTUATORS
