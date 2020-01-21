///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_NTP_Sync.cpp
/// @brief		File for LoomNTPSync implementation.
/// @author		Noah Koontz
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "NTP_Sync.h"
#include "Manager.h"

///////////////////////////////////////////////////////////////////////////////
LoomNTPSync::LoomNTPSync(
		LoomManager* 	manager,
		const uint          sync_interval_hours
	) 
	: LoomModule(manager, "NTP", Type::NTP )
	, m_sync_interval( sync_interval_hours )
	, m_internet( nullptr )
	, m_rtc( nullptr )
	, m_next_sync( 1 )
	, m_last_error( LoomNTPSync::Error::NON_START ) 
	{}

///////////////////////////////////////////////////////////////////////////////
LoomNTPSync::LoomNTPSync(LoomManager* manager, JsonArrayConst p)
	: LoomNTPSync(manager, (uint)p[0] ) {}

///////////////////////////////////////////////////////////////////////////////
void LoomNTPSync::second_stage_ctor() 
{
	// check to see if we have a device manager
	if (device_manager == nullptr) { m_last_error = Error::INVAL_DEVICE_MANAGE; return; }
	// check if internet platform exist

	// Try to get internet platform from manager
	LoomModule* temp = device_manager->find_module_by_category(LoomModule::Category::InternetPlat, 0);
	if (temp != nullptr) m_internet = (LoomInternetPlat*)temp;
	else {
		m_last_error = Error::INVAL_INTERNET;
		print_module_label();
		LPrint("Unable to find internet platform");
		return;
	}
	// same for RTC
	LoomRTC* rtc_temp = device_manager->get_rtc_module();
	if (rtc_temp != nullptr) {
		m_rtc = rtc_temp;
		print_module_label();
		LPrint("Running NTP...\n");
		measure();
	} else {
		m_last_error = Error::INVAL_RTC;
		print_module_label();
		LPrint("Unable to find RTC\n");
		return;
	}
	// made it here, guess we're good to go!
	print_config();
	m_last_error = Error::OK;
}

///////////////////////////////////////////////////////////////////////////////
void LoomNTPSync::print_config() const
{
	print_module_label();
	if (m_sync_interval == 0) LPrintln("\tNTPSync set to synchronize once.");
	else LPrint("\tNTPSync set to synchronize every ", m_sync_interval, " hours\n");
}

///////////////////////////////////////////////////////////////////////////////
void LoomNTPSync::print_state() const 
{
	print_module_label();
	if (m_last_error != Error::OK) LPrint("\tNTPSync in error state: ", static_cast<uint8_t>(m_last_error), "\n");
	else if (m_next_sync.unixtime() == 0) LPrint("\tNTPSync done synchronizing.\n");
	else LPrint("\tNTPSync synchronizing next at: ", m_next_sync.unixtime(), "\n");
}

///////////////////////////////////////////////////////////////////////////////
void LoomNTPSync::measure() 
{
	// if a sync is requested
	if (m_next_sync.unixtime() != 0 && m_rtc->now().secondstime() > m_next_sync.secondstime()) {
		// if the engine is operating correctly
		if ((m_last_error == Error::OK || m_last_error == Error::NON_START) && m_internet->is_connected()) {
			// synchronize the RTC
			DateTime timeNow;
			int attempt_count = 0;
			// repeat synchronize if this is the first power on
			do {
				timeNow = m_sync_rtc();
				if (timeNow.unixtime() != 0) m_next_sync = DateTime(0);
				else delay(100);
			} while (m_next_sync.unixtime() == 1 && ++attempt_count < 10);
			if (attempt_count == 10) m_last_error = Error::NO_CONNECTION;
			// set the next sync time
			else if (m_sync_interval != 0) {
				// to n hours from now
				m_next_sync = DateTime(timeNow) + TimeSpan(0, m_sync_interval, 0, 0);
			}
		}
		// else log errors

		if (m_last_error != Error::OK) {
			print_module_label();
			LPrint("Could not synchronize RTC due to error ", static_cast<uint8_t>(m_last_error), "\n");
		}
		else if (!(m_internet->is_connected())) {
			print_module_label();
			LPrintln("Could not synchronize RTC due to lack of internet");
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
DateTime LoomNTPSync::m_sync_rtc() 
{
	// it is presumed that the objects this function needs are in working order
	// get the current time from the internet
	const unsigned long epoch = m_internet->get_time();
	if (epoch == 0 || epoch > 4131551103UL) {
		// invalid time, ignore
		print_module_label();
		LPrint("Failed to fetch time for RTC! Will try again. \n");
		return DateTime(0);
	}
	// send it to the rtc
	const DateTime time(epoch);
	m_rtc->time_adjust(time);
	m_last_error = Error::OK;
	// log boi
	print_module_label();
	LPrint("Synchronized RTC to ", time.unixtime(), "\n");
	return time;
}

///////////////////////////////////////////////////////////////////////////////
