#include "Loom_NTP_Sync.h"

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
LoomNTPSync::LoomNTPSync(	
		const char*			module_name,
        const uint          internet_module_index,
        const uint          sync_interval_hours
    ) 
    : LoomModule( module_name )
    , m_internet_module_index( internet_module_index )
    , m_sync_interval( sync_interval_hours )
    , m_internet( nullptr )
    , m_rtc( nullptr )
    , m_next_sync( 1 )
    , m_last_error( LoomNTPSync::Error::NON_START ) {
        module_type = ModuleType::Unknown;
    }

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
LoomNTPSync::LoomNTPSync(JsonVariant p)
	: LoomNTPSync( EXPAND_ARRAY(p, 2) ) {}

/////////////////////////////////////////////////////////////////////
void LoomNTPSync::second_stage_ctor() {
    // check to see if we have a device manager
    if (device_manager == nullptr) { m_last_error = Error::INVAL_DEVICE_MANAGE; return; }
    // check if internet platform exist
    LoomModule* temp = (LoomModule*)&(device_manager->InternetPlat(m_internet_module_index));
    if (temp->get_module_type() == ModuleType::Internet) m_internet = (LoomInternetPlat*)temp;
    else {
        m_last_error = Error::INVAL_INTERNET;
        print_module_label();
        LPrint("Unable to find internet platform, intstead got: ", (int)(temp->get_module_type()), " using index ", m_internet_module_index, "\n");
        return;
    }
    // same for RTC
    Loom_Interrupt_Manager* inter_temp = &(device_manager->InterruptManager());
    if (inter_temp->get_module_type() == ModuleType::Interrupt_Manager 
        && inter_temp->get_RTC_module() != nullptr) m_rtc = inter_temp->get_RTC_module();
    else {
        m_last_error = Error::INVAL_RTC;
        print_module_label();
        LPrint("Unable to find RTC\n");
        return;
    }
    // made it here, guess we're good to go!
    print_module_label();
    LPrint("Ready\n");
    m_last_error = Error::OK;
}

/////////////////////////////////////////////////////////////////////
void LoomNTPSync::print_config() {
	print_module_label();
    if (m_next_sync.unixtime() == 0) LPrint("\tNTPSync set to synchronize once.");
    else LPrint("\tNTPSync set to synchronize every ", m_sync_interval, " hours\n");
}

/////////////////////////////////////////////////////////////////////
void LoomNTPSync::print_state() {
    print_module_label();
    if (m_last_error != Error::OK) LPrint("\tNTPSync in error state: ", static_cast<uint8_t>(m_last_error), "\n");
    else if (m_next_sync.unixtime() == 0) LPrint("\tNTPSync done synchronizing.\n");
    else LPrint("\tNTPSync synchronizing next at: ", m_next_sync.unixtime(), "\n");
}

/////////////////////////////////////////////////////////////////////
void LoomNTPSync::measure() {
    // if periodic sync is requested and the sync time has passed
    if (m_last_error == Error::OK 
        && m_next_sync.unixtime() != 0 
        && (m_rtc->now() - m_next_sync).totalseconds() >= 0) {
        // synchronize the RTC
        const DateTime& timeNow = m_sync_rtc();
        if (timeNow.unixtime() == 0) {
            // try again at the next cycle
            return;
        }
        // set the next sync time
        if (m_sync_interval != 0) {
            // to n hours from now
            m_next_sync = DateTime(timeNow) + TimeSpan(0, m_sync_interval, 0, 0);
        }
        // to done syncing
        else m_next_sync = DateTime(0);
    }
    else if (m_last_error != Error::OK) {
        print_module_label();
        LPrint("Could not synchronize RTC due to error ", static_cast<uint8_t>(m_last_error), "\n");
    }
}

/////////////////////////////////////////////////////////////////////
DateTime LoomNTPSync::m_sync_rtc() {
    // it is presumed that the objects this function needs are in working order
    // get the current time from the internet
    const DateTime time = DateTime(m_internet->get_time());
    if (time.unixtime() == 0) {
        // invalid time, ignore
        print_module_label();
        LPrint("Failed to fetch time for RTC! Will try again. \n");
        return time;
    }
    // send it to the rtc
    m_rtc->time_adjust(time);
    // log boi
    print_module_label();
    LPrint("Synchronized RTC to ", time.unixtime(), "\n");
    return time;
}

