
#include "Loom_PCF8523.h"


///////////////////////////////////////////////////////////////////////////////
Loom_PCF8523::Loom_PCF8523(	
		const char*		module_name,
	 
		TimeZone		timezone,
		bool			use_utc_time,
		bool			get_internet_time,
		byte			int_pin
	) 
	: LoomRTC( module_name, timezone, use_utc_time, get_internet_time, int_pin )
{
	this->module_type = ModuleType::PCF8523;

	// rtc_inst = new RTC_PCF8523();
	rtc_inst = new PCF8523();
	init();

	clear_alarms();
}

///////////////////////////////////////////////////////////////////////////////
Loom_PCF8523::Loom_PCF8523(JsonArrayConst p)
	: Loom_PCF8523(p[0], (TimeZone)(int)p[1], p[2], p[3], p[4]) {}

///////////////////////////////////////////////////////////////////////////////
Loom_PCF8523::~Loom_PCF8523() 
{
	delete rtc_inst;
}	

///////////////////////////////////////////////////////////////////////////////
bool Loom_PCF8523::_begin()
{
	return rtc_inst->begin();
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_PCF8523::_initialized()
{
	rtc_inst->initialized();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_PCF8523::print_config()
{
	LoomRTC::print_config();

	// will print out alarm info
}

///////////////////////////////////////////////////////////////////////////////
DateTime Loom_PCF8523::now()
{
	return rtc_inst->now();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_PCF8523::time_adjust(DateTime time)
{
	rtc_inst->adjust(time);

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Adjusted time to: "); 
		print_DateTime(time);		
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_PCF8523::set_alarm(DateTime time)
{

	// Example: PCF8523.set_alarm(10,5,45)
	// Set alarm at day = 5, 5:45 a.m.
	// rtc_inst->set_alarm(uint8_t day_alarm, uint8_t hour_alarm,uint8_t minute_alarm ) {

	// rtc_inst->set_alarm(time.hour(), time.minute() );

	// rtc_inst->start_counter_1(time.totalseconds());

	// Repeats by default
	// rtc_inst->start_counter_1((time - now()).totalseconds());
	set_alarm(time - now());

}

///////////////////////////////////////////////////////////////////////////////
void Loom_PCF8523::set_alarm(TimeSpan duration)
{
	rtc_inst->start_counter_1(duration.totalseconds());
}

///////////////////////////////////////////////////////////////////////////////
void Loom_PCF8523::clear_alarms()
{
	rtc_inst->clear_rtc_interrupt_flags();
}

///////////////////////////////////////////////////////////////////////////////

