///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_DS3231.cpp
/// @brief		File for Loom_DS3231 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "DS3231.h"


///////////////////////////////////////////////////////////////////////////////
Loom_DS3231::Loom_DS3231(
		LoomManager* manager,
		TimeZone		timezone,
		const bool			use_local_time,
		const bool			custom_time
	)
	: LoomRTC(manager, "DS3231", Type::DS3231, timezone, use_local_time, custom_time)
{
	init();

	// Set SQW pin to OFF (in my case it was set by default to 1Hz)
	// The output of the DS3231 INT pin is connected to this pin
	rtc_inst.writeSqwPinMode(DS3231_OFF);

	clear_alarms();
}

///////////////////////////////////////////////////////////////////////////////
Loom_DS3231::Loom_DS3231(LoomManager* manager, JsonArrayConst p)
	: Loom_DS3231(manager, (TimeZone)(int)p[0], p[1], p[2] ) {}

///////////////////////////////////////////////////////////////////////////////
bool Loom_DS3231::_begin()
{
	return rtc_inst.begin();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_DS3231::print_config() const
{
	LoomRTC::print_config();
	// will print out alarm info
}

///////////////////////////////////////////////////////////////////////////////
void Loom_DS3231::set_alarm(DateTime time)
{
	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Setting alarm for: ");
		print_DateTime(time);
	}

	// Set alarm 1
	rtc_inst.setAlarm(ALM1_MATCH_HOURS, time.second(), time.minute(), time.hour(), 0);
	rtc_inst.alarmInterrupt(1, true);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_DS3231::clear_alarms()
{
	print_module_label();
	LPrintln("Clearing DS3231 alarms");
	rtc_inst.armAlarm(1, false);
	rtc_inst.clearAlarm(1);
	rtc_inst.alarmInterrupt(1, false);
	rtc_inst.armAlarm(2, false);
	rtc_inst.clearAlarm(2);
	rtc_inst.alarmInterrupt(2, false);
}

///////////////////////////////////////////////////////////////////////////////
