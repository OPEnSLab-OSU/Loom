///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_PCF8523.cpp
/// @brief		File for Loom_PCF8523 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "PCF8523.h"


///////////////////////////////////////////////////////////////////////////////
Loom_PCF8523::Loom_PCF8523(
		LoomManager* manager,
		TimeZone		timezone,
		const bool			use_local_time,
		const bool			custom_time
	)
	: LoomRTC(manager, "PCF8523", Type::PCF8523, timezone, use_local_time, custom_time)
{
	init();
	clear_alarms();
}

///////////////////////////////////////////////////////////////////////////////
Loom_PCF8523::Loom_PCF8523(LoomManager* manager, JsonArrayConst p)
	: Loom_PCF8523(manager, (TimeZone)(int)p[0], p[1], p[2]) {}

///////////////////////////////////////////////////////////////////////////////
bool Loom_PCF8523::_begin()
{
	rtc_inst.begin();
	rtc_inst.stop_32768_clkout();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_PCF8523::print_config() const
{
	LoomRTC::print_config();
	// will print out alarm info
}

///////////////////////////////////////////////////////////////////////////////
void Loom_PCF8523::set_alarm(DateTime time)
{
	// Don't set alarm for current minute
	DateTime tmp = (now().minute() != time.minute()) ? time : time+TimeSpan(60);

	rtc_inst.set_alarm(tmp.day(), tmp.hour(), tmp.minute() );
	rtc_inst.enable_alarm(true);
}

///////////////////////////////////////////////////////////////////////////////