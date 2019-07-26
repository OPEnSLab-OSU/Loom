
#include "Loom_DS3231.h"


///////////////////////////////////////////////////////////////////////////////
Loom_DS3231::Loom_DS3231(
		const char*		module_name,
		TimeZone		timezone,
		bool			use_utc_time,
		bool			get_internet_time
	)
	: LoomRTC( module_name, Type::DS3231, timezone, use_utc_time, get_internet_time )
{
	init();

	// Set SQW pin to OFF (in my case it was set by default to 1Hz)
	// The output of the DS3231 INT pin is connected to this pin
	rtc_inst.writeSqwPinMode(DS3231_OFF);

	clear_alarms();
}

///////////////////////////////////////////////////////////////////////////////
Loom_DS3231::Loom_DS3231(JsonArrayConst p)
	: Loom_DS3231(p[0], (TimeZone)(int)p[1], p[2], p[3]) {}

///////////////////////////////////////////////////////////////////////////////
bool Loom_DS3231::_begin()
{
	return rtc_inst.begin();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_DS3231::print_config()
{
	LoomRTC::print_config();

	// will print out alarm info
}

///////////////////////////////////////////////////////////////////////////////
void Loom_DS3231::time_adjust(DateTime time)
{
	rtc_inst.adjust(time);

	print_module_label();
	LPrint("Adjusted time to: "); 
	print_DateTime(time);	
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
