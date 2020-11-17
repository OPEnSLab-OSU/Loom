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
  LMark;
	// check to see if we have a device manager
	if (device_manager == nullptr) { m_last_error = Error::INVAL_DEVICE_MANAGE; return; }
	// check if internet platform exist

	// Try to get internet platform from manager
	LoomModule* temp = device_manager->find_module_by_category(LoomModule::Category::InternetPlat, 0);
  LMark;
	if (temp != nullptr) m_internet = (LoomInternetPlat*)temp;
	else {
   	LMark;
		m_last_error = Error::INVAL_INTERNET;
   	LMark;
		print_module_label();
   	LMark;
		LPrint("Unable to find internet platform");
   	LMark;
		return;
	}
	// same for RTC
	LoomRTC* rtc_temp = device_manager->get_rtc_module();
  LMark;
	if (rtc_temp != nullptr) {
   	LMark;
		m_rtc = rtc_temp;
   	LMark;
		print_module_label();
   	LMark;
		LPrint("Running NTP...\n");
   	LMark;
		measure();
  	LMark;
	} else {
		m_last_error = Error::INVAL_RTC;
   	LMark;
		print_module_label();
   	LMark;
		LPrint("Unable to find RTC\n");
   	LMark;
		return;
	}
	// made it here, guess we're good to go!
	print_config();
  LMark;
	m_last_error = Error::OK;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomNTPSync::print_config() const
{
  LMark;
	print_module_label();
  LMark;
	if (m_sync_interval == 0) LPrintln("\tNTPSync set to synchronize once.");
	else LPrint("\tNTPSync set to synchronize every ", m_sync_interval, " hours\n");
}

///////////////////////////////////////////////////////////////////////////////
void LoomNTPSync::print_state() const
{
  LMark;
	print_module_label();
  LMark;
	if (m_last_error != Error::OK) LPrint("\tNTPSync in error state: ", static_cast<uint8_t>(m_last_error), "\n");
	else if (m_next_sync.unixtime() == 0) LPrint("\tNTPSync done synchronizing.\n");
	else LPrint("\tNTPSync synchronizing next at: ", m_next_sync.unixtime(), "\n");
}

///////////////////////////////////////////////////////////////////////////////
void LoomNTPSync::measure()
{
  LMark;
	// if a sync is requested
	if (m_next_sync.unixtime() != 0 && m_rtc->now().secondstime() > m_next_sync.secondstime()) {
   	LMark;
		// if the engine is operating correctly
		if ((m_last_error == Error::OK || m_last_error == Error::NON_START) && m_internet->is_connected()) {
    	LMark;
			// synchronize the RTC
			DateTime timeNow;
    	LMark;
			int attempt_count = 0;
    	LMark;
			// repeat synchronize if this is the first power on
			do {
     		LMark;
				timeNow = m_sync_rtc();
     		LMark;
				if (timeNow.unixtime() != 0) m_next_sync = DateTime(0);
				else delay(100);
			} while (m_next_sync.unixtime() == 1 && ++attempt_count < 10);
			if (attempt_count == 10) m_last_error = Error::NO_CONNECTION;
			// set the next sync time
			else if (m_sync_interval != 0) {
     		LMark;
				// to n hours from now
				m_next_sync = DateTime(timeNow) + TimeSpan(0, m_sync_interval, 0, 0);
    		LMark;
			}
		}
		// else log errors

		if (m_last_error != Error::OK) {
    	LMark;
			print_module_label();
    	LMark;
			LPrint("Could not synchronize RTC due to error ", static_cast<uint8_t>(m_last_error), "\n");
   		LMark;
		}
		else if (!(m_internet->is_connected())) {
    	LMark;
			print_module_label();
    	LMark;
			LPrintln("Could not synchronize RTC due to lack of internet");
   		LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
DateTime LoomNTPSync::m_sync_rtc()
{
  LMark;
	// it is presumed that the objects this function needs are in working order
	// get the current time from the internet
	const unsigned long epoch = m_internet->get_time();
  LMark;
	if (epoch == 0 || epoch > 4131551103UL) {
   	LMark;
		// invalid time, ignore
		print_module_label();
   	LMark;
		LPrint("Failed to fetch time for RTC! Will try again. \n");
   	LMark;
		return DateTime(0);
	}
	// send it to the rtc
	const DateTime time(epoch);
  LMark;
	m_rtc->time_adjust(time);
  LMark;
	m_last_error = Error::OK;
  LMark;
	// log boi
	print_module_label();
  LMark;
	LPrint("Synchronized RTC to ", time.unixtime(), "\n");
  LMark;
	return time;
}

///////////////////////////////////////////////////////////////////////////////
