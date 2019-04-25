
#ifndef LOOM_PCF8523_h
#define LOOM_PCF8523_h

#include "Loom_RTC.h"

// ### (LoomRTC) | dependencies: [] | conflicts: []
/// PCF8523 RTC module
// ###
class Loom_PCF8523 : public LoomRTC
{

protected:

	/// Underlying PCF8523 manager instance
	PCF8523*		rtc_inst;
	// RTC_PCF8523*  rtc_inst;

public:

	/// PCF8523 module constructor
	///
	/// \param[in]	module_name					String | <"PCF8523"> | null | PCF8523 module name
	/// \param[in]	timezone						Set(TimeZone) | <11> | { 0("WAT"), 1("AT"), 2("ADT"), 3("AST"), 4("EDT"), 5("EST"), 6("CDT"), 7("CST"), 8("MDT"), 9("MST"), 10("PDT"), 11("PST"), 12("ALDT"), 13("ALST"), 14("HST"), 15("SST"), 16("GMT"), 17("BST"), 18("CET"), 19("CEST"), 20("EET"), 21("EEST"), 22("BT"), 23("ZP4"), 24("ZP5"), 25("ZP6"), 26("ZP7"), 27("AWST"), 28("AWDT"), 29("ACST"), 30("ACDT"), 31("AEST"), 32("AEDT")} | Which timezone device is in
	/// \param[in]	use_utc_time				Bool | <false> | {true, false} | True for UTC time, false for local time
	/// \param[in]	get_internet_time		Bool | <false> | {true, false} | Whether or not to try to get time via internet
	/// \param[in]	int_pin							Set(Int) | <6> | {5, 6, 9, 10, 11, 12} | Which pin the interrupt is connected to
	Loom_PCF8523(
			const char*		module_name			= "PCF8523",
			TimeZone		timezone			= TimeZone::PST,
			bool			use_utc_time		= false,
			bool			get_internet_time	= false,
			byte			int_pin				= 6
		);


	// --- DESTRUCTOR ---
	virtual ~Loom_PCF8523();

	// Inherited (overriding) Methods
	void		print_config() override;
	bool		message_route(OSCMessage& msg, int address_offset) {};
	DateTime	now();

// proper alarms not really working yet
	void		set_alarm(DateTime time);
	void		set_alarm(TimeSpan duration);
	void		clear_alarms();

protected:

	bool		_begin();
	bool		_initialized();
	void		time_adjust(DateTime time);

};


#endif // of #ifndef LOOM_PCF8523_h
