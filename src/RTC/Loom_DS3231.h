#pragma once

#include "Loom_RTC.h"


///////////////////////////////////////////////////////////////////////////////


// ### (LoomRTC) | dependencies: [] | conflicts: []
/// DS3231 RTC module
// ###
class Loom_DS3231 : public LoomRTC
{

protected:

	RTC_DS3231 rtc_inst;		/// Underlying DS3231 manager instance

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// DS3231 module constructor
	///
	/// \param[in]	module_name					String | <"DS3231"> | null | DS3231 module name
	/// \param[in]	timezone					Set(TimeZone) | <11> | { 0("WAT"), 1("AT"), 2("ADT"), 3("AST"), 4("EDT"), 5("EST"), 6("CDT"), 7("CST"), 8("MDT"), 9("MST"), 10("PDT"), 11("PST"), 12("ALDT"), 13("ALST"), 14("HST"), 15("SST"), 16("GMT"), 17("BST"), 18("CET"), 19("CEST"), 20("EET"), 21("EEST"), 22("BT"), 23("ZP4"), 24("ZP5"), 25("ZP6"), 26("ZP7"), 27("AWST"), 28("AWDT"), 29("ACST"), 30("ACDT"), 31("AEST"), 32("AEDT")} | Which timezone device is in
	/// \param[in]	use_utc_time				Bool | <true> | {true, false} | True for UTC time, false for local time
	/// \param[in]	get_internet_time			Bool | <false> | {true, false} | Whether or not to try to get time via internet
	Loom_DS3231(
			const char*		module_name			= "DS3231",
			TimeZone		timezone			= TimeZone::PST,
			bool			use_utc_time		= true,
			bool			get_internet_time	= false
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_DS3231(JsonArrayConst p);

	/// Destructor
	~Loom_DS3231() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	DateTime	now() override { return rtc_inst.now(); }
	void		set_alarm(DateTime time) override;
	void		clear_alarms() override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;

protected:

	bool		_begin() override;
	bool		_initialized() override { return !rtc_inst.lostPower(); }
	void		time_adjust(DateTime time) override;

};

