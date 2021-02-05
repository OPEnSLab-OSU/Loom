///////////////////////////////////////////////////////////////////////////////
///
/// @file		NTP_Sync.h
/// @brief		File for NTPSync definition.
/// @author		Noah Koontz
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
#pragma once

#include "Module.h"
#include "./InternetPlats/InternetPlat.h"
#include "./RTC/RTC.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
///
/// Glue code to synchronize an RTC using an InternetPlat.
/// Always synchronizes the RTC from Loom_Interrupt_Manager::get_RTC_module().
///
/// @note	Requires a LoomRTC and LoomInternetPlat module to work.
///
/// @par Resources
/// - [Documentation](https://openslab-osu.github.io/Loom/html/class_loom_n_t_p_sync.html)
///
///////////////////////////////////////////////////////////////////////////////

class NTPSync : public Module
{

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// NTP Sync module constructor.
	///
	/// @param[in]  sync_interval_hours		Int | <0> | [0-999] | What hourly period to sync the RTC, zero for once on startup.
	NTPSync(
		const uint		sync_interval_hours		= 0
	);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	NTPSync(JsonArrayConst p);

	/// Destructor
	~NTPSync() = default;

	/// Sync the RTC using NTP from the internet platform specified
	void	second_stage_ctor() override;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Sync the time if necessary or enabled.
	/// Allows the module to run regularly by emulating a sensor, which have
	/// thier measure methods called regularly.
	void		measure();
	void		package(JsonObject json) override { /* do nothing */ };
	bool		dispatch(JsonObject) override { /* do nothing */}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_state() const override;

private:

	/// The actual synchronization function
	/// @return Time obtained from InternetPlat
	DateTime m_sync_rtc();

	/// enumerate errors
	enum class Error {
		OK,
		INVAL_DEVICE_MANAGE,	///< Failed to receive a Loom Device Manager
		INVAL_INTERNET,			///< Failed to find the correct internet object
		INVAL_TIME,				///< Retrieved a time that was invalid
		INVAL_RTC,				///< Failed to find a RTC
		NON_START,				///< No attempt was made to sync
		NO_CONNECTION,			///< Repeated attempts were made to sync, but there was no response
	};

	/// Store the sync interval, in hours
	const uint			m_sync_interval;

	/// Store the Internet Plat from second stage contsruction
	InternetPlat*	m_internet;

	/// Store the RTC pointer so we can check the time
	RTC*			m_rtc;

	/// Store when next to change the RTC
	DateTime			m_next_sync;

	/// Store if we've successfully accomplished our task
	Error				m_last_error;
};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, NTPSync, "NTPSync");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
