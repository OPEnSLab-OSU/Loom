///////////////////////////////////////////////////////////////////////////////
///
/// @file		PCF8523.cpp
/// @brief		File for PCF8523 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "PCF8523.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
PCF8523::PCF8523(
		TimeZone		timezone,
		const bool			use_local_time,
		const bool			custom_time
	)
	: RTC("PCF8523", timezone, use_local_time, custom_time)
{
  LMark;
	init();
	clear_alarms();
}

///////////////////////////////////////////////////////////////////////////////
PCF8523::PCF8523(JsonArrayConst p)
	: PCF8523((TimeZone)(int)p[0], p[1], p[2]) {}

///////////////////////////////////////////////////////////////////////////////
bool PCF8523::_begin()
{
  LMark;
	bool success = rtc_inst.begin();
	rtc_inst.stop_32768_clkout();
	return success;
}

///////////////////////////////////////////////////////////////////////////////
void PCF8523::print_config() const
{
	RTC::print_config();
	// will print out alarm info
}

///////////////////////////////////////////////////////////////////////////////
void PCF8523::set_alarm(DateTime time)
{
  LMark;
	// Don't set alarm for current minute
	DateTime tmp = (now().minute() != time.minute()) ? time : time+TimeSpan(60);

	rtc_inst.set_alarm(tmp.day(), tmp.hour(), tmp.minute() );
	rtc_inst.enable_alarm(true);
}

///////////////////////////////////////////////////////////////////////////////
