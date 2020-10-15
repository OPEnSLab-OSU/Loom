///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_DS3231.h
/// @brief		File for Loom_DS3231 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "RTC.h"


///////////////////////////////////////////////////////////////////////////////
///
/// DS3231 RTC module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___d_s3231.html)
/// - [Product Page: Adafruit DS3231 Precision RTC FeatherWing](https://www.adafruit.com/product/3028)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#ds3231-rtc)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_DS3231 : public LoomRTC
{

protected:

	RTC_DS3231	rtc_inst;	///< Underlying DS3231 manager instance

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	timezone			Set(TimeZone) | <11> | { 0("WAT"), 1("AT"), 2("ADT"), 3("AST"), 4("EDT"), 5("EST"), 6("CDT"), 7("CST"), 8("MDT"), 9("MST"), 10("PDT"), 11("PST"), 12("AKDT"), 13("AKST"), 14("HST"), 15("SST"), 16("GMT"), 17("BST"), 18("CET"), 19("EET"), 20("EEST"), 21("BRT"), 22("ZP4"), 23("ZP5"), 24("ZP6"), 25("ZP7"), 26("AWST"), 27("ACST"), 28("AEST")} | Which timezone device is in
	/// @param[in]	use_local_time		Bool | <false> | {true, false} | True for local time, false for UTC time
	/// @param[in]	customize_start_time Bool | <false>| {true, false} | True for customize UTC time, false for complie time
	/// @param[in]	new_year		uint16_t | <2000> | The current year | The new year for the customize time
	/// @param[in]	new_month		uint8_t | <1> | The current month | The new month for the customize time
	/// @param[in]	new_day			uint8_t | <1> | The current date | The new day for the customize time
	/// @param[in] 	new_hour		uint8_t | <0> | {0 ,1 ,2 ,3 ,4 ,5 ,6 ,7 ,8 ,9 ,10 ,11 ,12 ,13(1PM) ,14(2PM) ,15(3PM) ,16(4PM) ,17(5PM) ,18(6PM) ,19(7PM) ,20(8PM) ,21(9PM) ,22(10PM) ,23(11PM) }, The current hour | The new hour for the customize time
	/// @param[in]	new_min			uint8_t | <0> | The current minute | The new minutes for the customize time
	Loom_DS3231(
			LoomManager* manager,
			TimeZone		timezone			= TimeZone::PST,
			const bool			use_local_time		= false,
			const bool				customize_start_time = false,
			const uint16_t			new_year = 2000,
			const uint8_t			new_month = 1,
			const uint8_t			new_day = 1,
			const uint8_t			new_hour = 0,
			const uint8_t			new_min = 0
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_DS3231(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_DS3231() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	DateTime	now() const override { return rtc_inst.now(); }
	void		set_alarm(DateTime time) override;
	void		clear_alarms() override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;

protected:

	bool		_begin() override;
	bool		_initialized() override { return !rtc_inst.lostPower(); }
	void		_adjust(const DateTime time) override { rtc_inst.adjust(time); }

};

