///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Manager_Module_Access.cpp
/// @brief		File for LoomManager module access method implementations.
/// @details	This is given its own File for sake of separating the section
///				of code that will change with module additions from the rest of
///				the implementation of Loom_Manager.
///				This implementation is not great and should be changed
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Module.h"
#include "Manager.h"

#include "Components/WarmUp/WarmUp_Manager.h"
#include "Interrupt_Manager.h"
#include "Sleep_Manager.h"
#include "RTC/RTC.h"

///////////////////////////////////////////////////////////////////////////////
// --- General ---

Loom_WarmUp_Manager& LoomManager::WarmUpManager(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = warmup_manager;
  LMark;
	return (*(Loom_WarmUp_Manager*)( (tmp) ? tmp : nullptr ));
}

Loom_Interrupt_Manager& LoomManager::InterruptManager(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = interrupt_manager;
  LMark;
	return (*(Loom_Interrupt_Manager*)( (tmp) ? tmp : nullptr ));
}

Loom_Sleep_Manager& LoomManager::SleepManager(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = sleep_manager;
  LMark;
	return (*(Loom_Sleep_Manager*)( (tmp) ? tmp : nullptr ));
}


Loom_Multiplexer& LoomManager::Multiplexer(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::Multiplexer, idx);
  LMark;
	return (*(Loom_Multiplexer*)( (tmp) ? tmp : nullptr ));
}

///////////////////////////////////////////////////////////////////////////////
// --- Actuators ---

Loom_Neopixel& LoomManager::Neopixel(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::Neopixel, idx);
  LMark;
	return (*(Loom_Neopixel*)( (tmp) ? tmp : nullptr ));
}

Loom_Relay& LoomManager::Relay(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::Relay, idx);
  LMark;
	return (*(Loom_Relay*)( (tmp) ? tmp : nullptr ));
}

Loom_Servo& LoomManager::Servo(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::Servo, idx);
  LMark;
	return (*(Loom_Servo*)( (tmp) ? tmp : nullptr ));
}

Loom_Stepper& LoomManager::Stepper(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::Stepper, idx);
  LMark;
	return (*(Loom_Stepper*)( (tmp) ? tmp : nullptr ));
}

///////////////////////////////////////////////////////////////////////////////
// --- CommPlats ---

Loom_LoRa& LoomManager::LoRa(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::LoRa, idx);
  LMark;
	return (*(Loom_LoRa*)( (tmp) ? tmp : nullptr ));
}

Loom_nRF& LoomManager::nRF(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::nRF, idx);
  LMark;
	return (*(Loom_nRF*)( (tmp) ? tmp : nullptr ));
}

Loom_Bluetooth& LoomManager::Bluetooth(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::Bluetooth, idx);
  LMark;
	return (*(Loom_Bluetooth*)( (tmp) ? tmp : nullptr ));
}

///////////////////////////////////////////////////////////////////////////////
// --- InternetPlats ---

// LoomInternetPlat& LoomManager::InternetPlat(const const uint8_t idx)
// {
// 	// if (idx >= modules.size()) return *(LoomInternetPlat*)(nullptr);
// 	// else return *(modules[idx]);
// 	return *(LoomInternetPlat*)(nullptr);
// }

Loom_Ethernet& LoomManager::Ethernet(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::Ethernet, idx);
  LMark;
	return (*(Loom_Ethernet*)( (tmp) ? tmp : nullptr ));
}

Loom_WiFi& LoomManager::WiFi(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::WiFi, idx);
  LMark;
	return (*(Loom_WiFi*)( (tmp) ? tmp : nullptr ));
}

Loom_LTE& LoomManager::LTE(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::LTE, idx);
  LMark;
	return (*(Loom_LTE*)( (tmp) ? tmp : nullptr ));
}


///////////////////////////////////////////////////////////////////////////////
// --- PublishPlats ---

// LoomPublishPlat& LoomManager::PublishPlat(const const uint8_t idx)
// {
// 	// if (idx >= modules.size()) return *(LoomPublishPlat*)(nullptr);
// 	// else return *(modules[idx]);
// 	return *(LoomPublishPlat*)(nullptr);
// }

Loom_GoogleSheets& LoomManager::GoogleSheets(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::GoogleSheets, idx);
  LMark;
	return (*(Loom_GoogleSheets*)( (tmp) ? tmp : nullptr ));
}

Loom_SpoolPublish& LoomManager::Spool(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::SpoolPub, idx);
  LMark;
	return (*(Loom_SpoolPublish*)( (tmp) ? tmp : nullptr ));
}

Loom_MaxPub& LoomManager::MaxPub(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::MaxPub, idx);
  LMark;
	return (*(Loom_MaxPub*)( (tmp) ? tmp : nullptr ));
}


///////////////////////////////////////////////////////////////////////////////
// --- SubscribePlats ---

Loom_MaxSub& LoomManager::MaxSub(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::MaxSub, idx);
  LMark;
	return (*(Loom_MaxSub*)( (tmp) ? tmp : nullptr ));
}


///////////////////////////////////////////////////////////////////////////////
// --- LogPlats ---

Loom_OLED& LoomManager::OLED(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::OLED, idx);
  LMark;
	return (*(Loom_OLED*)( (tmp) ? tmp : nullptr ));
}

Loom_SD& LoomManager::SDCARD(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::SDCARD, idx);
  LMark;
	return (*(Loom_SD*)( (tmp) ? tmp : nullptr ));
}

Loom_BatchSD& LoomManager::BATCHSD(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::BATCHSD, idx);
  LMark;
	return (*(Loom_BatchSD*)( (tmp) ? tmp : nullptr ));
}

///////////////////////////////////////////////////////////////////////////////
// --- RTC ---

Loom_DS3231& LoomManager::DS3231(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = rtc_module;
  LMark;
	return (*(Loom_DS3231*)( (tmp && tmp->get_module_type() == LoomModule::Type::DS3231) ? tmp : nullptr ));
}

Loom_PCF8523& LoomManager::PCF8523(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = rtc_module;
  LMark;
	// return (*(Loom_PCF8523*)( (tmp) ? tmp : nullptr ));
	return (*(Loom_PCF8523*)( (tmp && tmp->get_module_type() == LoomModule::Type::PCF8523) ? tmp : nullptr ));
}

///////////////////////////////////////////////////////////////////////////////
// --- Sensors ---

Loom_Analog& LoomManager::Analog(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::Analog, idx);
  LMark;
	return (*(Loom_Analog*)( (tmp) ? tmp : nullptr ));
}

Loom_Digital& LoomManager::Digital(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::Digital, idx);
  LMark;
	return (*(Loom_Digital*)( (tmp) ? tmp : nullptr ));
}

///////////////////////////////////////////////////////////////////////////////
//    --- I2C Sensors ---
Loom_ADS1115& LoomManager::ADS1115(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::ADS1115, idx);
  LMark;
	return (*(Loom_ADS1115*)( (tmp) ? tmp : nullptr ));

}
Loom_AS7262& LoomManager::AS7262(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::AS7262, idx);
  LMark;
	return (*(Loom_AS7262*)( (tmp) ? tmp : nullptr ));
}

Loom_AS7263& LoomManager::AS7263(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::AS7263, idx);
  LMark;
	return (*(Loom_AS7263*)( (tmp) ? tmp : nullptr ));
}

Loom_AS7265X& LoomManager::AS7265X(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::AS7265X, idx);
  LMark;
	return (*(Loom_AS7265X*)( (tmp) ? tmp : nullptr ));
}

Loom_FXAS21002& LoomManager::FXAS21002(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::FXAS21002, idx);
  LMark;
	return (*(Loom_FXAS21002*)( (tmp) ? tmp : nullptr ));
}

Loom_FXOS8700& LoomManager::FXOS8700(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::FXOS8700, idx);
  LMark;
	return (*(Loom_FXOS8700*)( (tmp) ? tmp : nullptr ));
}

Loom_LIS3DH& LoomManager::LIS3DH(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::LIS3DH, idx);
  LMark;
	return (*(Loom_LIS3DH*)( (tmp) ? tmp : nullptr ));
}

Loom_MB1232& LoomManager::MB1232(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::MB1232, idx);
  LMark;
	return (*(Loom_MB1232*)( (tmp) ? tmp : nullptr ));
}

Loom_MMA8451& LoomManager::MMA8451(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::MMA8451, idx);
  LMark;
	return (*(Loom_MMA8451*)( (tmp) ? tmp : nullptr ));
}

Loom_MPU6050& LoomManager::MPU6050(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::MPU6050, idx);
  LMark;
	return (*(Loom_MPU6050*)( (tmp) ? tmp : nullptr ));
}

Loom_MS5803& LoomManager::MS5803(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::MS5803, idx);
  LMark;
	return (*(Loom_MS5803*)( (tmp) ? tmp : nullptr ));
}

Loom_SHT31D& LoomManager::SHT31D(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::SHT31D, idx);
  LMark;
	return (*(Loom_SHT31D*)( (tmp) ? tmp : nullptr ));
}

Loom_TMP007& LoomManager::TMP007(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::TMP007, idx);
  LMark;
	return (*(Loom_TMP007*)( (tmp) ? tmp : nullptr ));
}

Loom_TSL2561& LoomManager::TSL2561(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::TSL2561, idx);
  LMark;
	return (*(Loom_TSL2561*)( (tmp) ? tmp : nullptr ));
}

Loom_TSL2591& LoomManager::TSL2591(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::TSL2591, idx);
  LMark;
	return (*(Loom_TSL2591*)( (tmp) ? tmp : nullptr ));
}

Loom_ZXGesture& LoomManager::ZXGesture(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::ZXGesture, idx);
  LMark;
	return (*(Loom_ZXGesture*)( (tmp) ? tmp : nullptr ));
}

Loom_STEMMA& LoomManager::STEMMA(const uint8_t idx)
{
    LMark;
    LoomModule* tmp = find_module(LoomModule::Type::STEMMA, idx);
    LMark;
    return (*(Loom_STEMMA*)( (tmp) ? tmp : nullptr ));
}

///////////////////////////////////////////////////////////////////////////////
//    --- SDI12 Sensors ---

Loom_Decagon5TM& LoomManager::Decagon5TM(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::Decagon5TM, idx);
  LMark;
	return (*(Loom_Decagon5TM*)( (tmp) ? tmp : nullptr ));
}

Loom_DecagonGS3& LoomManager::DecagonGS3(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::DecagonGS3, idx);
  LMark;
	return (*(Loom_DecagonGS3*)( (tmp) ? tmp : nullptr ));
}

///////////////////////////////////////////////////////////////////////////////
//    --- SPI Sensors ---

Loom_MAX31855& LoomManager::MAX31855(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::MAX31855, idx);
  LMark;
	return (*(Loom_MAX31855*)( (tmp) ? tmp : nullptr ));
}

Loom_MAX31856& LoomManager::MAX31856(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::MAX31856, idx);
  LMark;
	return (*(Loom_MAX31856*)( (tmp) ? tmp : nullptr ));
}


///////////////////////////////////////////////////////////////////////////////
//    --- SERIAL Sensors ---

Loom_K30& LoomManager::K30(const uint8_t idx)
{
    LMark;
    LoomModule* tmp = find_module(LoomModule::Type::K30, idx);
    LMark;
    return (*(Loom_K30*)( (tmp) ? tmp : nullptr ));
}


///////////////////////////////////////////////////////////////////////////////
//    --- Other ---

LoomNTPSync& LoomManager::NTP(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::NTP, idx);
  LMark;
	return (*(LoomNTPSync*)( (tmp) ? tmp : nullptr ));
}

LoomTempSync& LoomManager::TempSync(const uint8_t idx)
{
  LMark;
	LoomModule* tmp = find_module(LoomModule::Type::TempSync, idx);
  LMark;
	return (*(LoomTempSync*)( (tmp) ? tmp : nullptr ));
}
