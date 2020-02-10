///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_PCF8523.h
/// @brief		File for Loom_PCF8523 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "RTC.h"


///////////////////////////////////////////////////////////////////////////////
///
/// PCF8523 RTC module.
///
/// OPEnS Lab usually uses the PCF8523 on the Adafruit Adalogger Featherwing
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___p_c_f8523.html)
/// - [Product Page: Adafruit Adalogger Featherwing - RTC + SD](https://www.adafruit.com/product/2922)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_PCF8523 : public LoomRTC
{

protected:

	PCF8523		rtc_inst;		///< Underlying PCF8523 manager instance

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	timezone			Set(TimeZone) | <11> | { 0("WAT"), 1("AT"), 2("ADT"), 3("AST"), 4("EDT"), 5("EST"), 6("CDT"), 7("CST"), 8("MDT"), 9("MST"), 10("PDT"), 11("PST"), 12("ALDT"), 13("ALST"), 14("HST"), 15("SST"), 16("GMT"), 17("BST"), 18("CET"), 19("CEST"), 20("EET"), 21("EEST"), 22("BT"), 23("ZP4"), 24("ZP5"), 25("ZP6"), 26("ZP7"), 27("AWST"), 28("AWDT"), 29("ACST"), 30("ACDT"), 31("AEST"), 32("AEDT")} | Which timezone device is in
	/// @param[in]	use_utc_time		Bool | <false> | {true, false} | True for UTC time, false for local time
	Loom_PCF8523(
			LoomManager* manager,
			const TimeZone		timezone			= TimeZone::PST,
			const bool			use_utc_time		= true
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_PCF8523(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_PCF8523() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	DateTime	now() const override { return rtc_inst.now(); }
	void		set_alarm(DateTime time) override;
	void		clear_alarms() override { rtc_inst.clear_rtc_interrupt_flags(); }

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;

protected:

	bool		_begin() override;
	bool		_initialized() override { rtc_inst.initialized(); }
	void		_adjust(const DateTime time) override { rtc_inst.adjust(time); }

};

				