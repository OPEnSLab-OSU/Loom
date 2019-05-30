#pragma once

#include "Loom_RTC.h"

// ### (LoomRTC) | dependencies: [] | conflicts: []
/// DS3231 RTC module
// ###
class Loom_DS3231 : public LoomRTC
{

protected:

	/// Underlying DS3231 manager instance
	RTC_DS3231*  rtc_inst;

public:

	/// DS3231 module constructor
	///
	/// \param[in]	module_name					String | <"DS3231"> | null | DS3231 module name
	/// \param[in]	timezone					Set(TimeZone) | <11> | { 0("WAT"), 1("AT"), 2("ADT"), 3("AST"), 4("EDT"), 5("EST"), 6("CDT"), 7("CST"), 8("MDT"), 9("MST"), 10("PDT"), 11("PST"), 12("ALDT"), 13("ALST"), 14("HST"), 15("SST"), 16("GMT"), 17("BST"), 18("CET"), 19("CEST"), 20("EET"), 21("EEST"), 22("BT"), 23("ZP4"), 24("ZP5"), 25("ZP6"), 26("ZP7"), 27("AWST"), 28("AWDT"), 29("ACST"), 30("ACDT"), 31("AEST"), 32("AEDT")} | Which timezone device is in
	/// \param[in]	use_utc_time				Bool | <false> | {true, false} | True for UTC time, false for local time
	/// \param[in]	get_internet_time			Bool | <false> | {true, false} | Whether or not to try to get time via internet
	/// \param[in]	int_pin						Set(Int) | <6> | {5, 6, 9, 10, 11, 12} | Which pin the interrupt is connected to
	Loom_DS3231(
			const char*		module_name			= "DS3231",
			TimeZone		timezone			= TimeZone::PST,
			bool			use_utc_time		= false,
			bool			get_internet_time	= false,
			byte			int_pin				= 6
		);

	Loom_DS3231(JsonVariant p);

	// --- DESTRUCTOR ---
	virtual ~Loom_DS3231();

	// Inherited (overriding) Methods
	void		print_config() override;
	bool		message_route(OSCMessage& msg, int address_offset) {};
	DateTime	now();
	void		set_alarm(DateTime time);
	void		set_alarm(TimeSpan duration);
	void		clear_alarms();


protected:

	bool		_begin();
	bool		_initialized();
	void		time_adjust(DateTime time);

};

