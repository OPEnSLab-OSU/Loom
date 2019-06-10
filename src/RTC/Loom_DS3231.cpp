
#include "Loom_DS3231.h"

// #define EI_NOTEXTERNAL
// #include <EnableInterrupt.h>


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_DS3231::Loom_DS3231(
		const char*		module_name,

		TimeZone		timezone,
		bool			use_utc_time,
		bool			get_internet_time,
		byte			int_pin
	)
	: LoomRTC( module_name, timezone, use_utc_time, get_internet_time, int_pin )
{
	this->module_type = ModuleType::DS3231;

	rtc_inst = new RTC_DS3231();
	init();

	// Set SQW pin to OFF (in my case it was set by default to 1Hz)
	// The output of the DS3231 INT pin is connected to this pin
	rtc_inst->writeSqwPinMode(DS3231_OFF);

	clear_alarms();
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_DS3231::Loom_DS3231(JsonVariant p)
	: Loom_DS3231(p[0], (TimeZone)(int)p[1], p[2], p[3], p[4])
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_DS3231::~Loom_DS3231()
{
	delete rtc_inst;
}

/////////////////////////////////////////////////////////////////////
bool Loom_DS3231::_begin()
{
	return rtc_inst->begin();
}

/////////////////////////////////////////////////////////////////////
bool Loom_DS3231::_initialized()
{
	return !rtc_inst->lostPower();
}

/////////////////////////////////////////////////////////////////////
void Loom_DS3231::print_config()
{
	LoomRTC::print_config();

	// will print out alarm info
}

/////////////////////////////////////////////////////////////////////
DateTime Loom_DS3231::now()
{
	return rtc_inst->now();
}

/////////////////////////////////////////////////////////////////////
void Loom_DS3231::time_adjust(DateTime time)
{
	rtc_inst->adjust(time);

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_DS3231::set_alarm(DateTime time)
{
	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();

	}

	// Set alarm 1
	rtc_inst->setAlarm(ALM1_MATCH_HOURS, time.second(), time.minute(), time.hour(), 0);
	rtc_inst->alarmInterrupt(1, true);
}

/////////////////////////////////////////////////////////////////////
void Loom_DS3231::set_alarm(TimeSpan duration)
{
	set_alarm(now()+duration);
}

/////////////////////////////////////////////////////////////////////
void Loom_DS3231::clear_alarms()
{
	LPrintln("\n\nClearing DS3231 alarms");
	rtc_inst->armAlarm(1, false);
	rtc_inst->clearAlarm(1);
	rtc_inst->alarmInterrupt(1, false);
	rtc_inst->armAlarm(2, false);
	rtc_inst->clearAlarm(2);
	rtc_inst->alarmInterrupt(2, false);

	// Not sure if this goes here
	// rtc_inst->writeSqwPinMode(DS3231_OFF);

}