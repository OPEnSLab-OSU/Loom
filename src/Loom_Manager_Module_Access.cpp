
// This is given its own file for the sake of separating the section of code that
// will change with module additions from the rest of the implementation of Loom_Manager

#include "Loom_Module.h"
#include "Loom_Manager.h"
#include "Loom_Module_Stub.h"

// Global stub to reference if user tries to access 
// object that does not exist
Loom_Module_Stub global_stub;


///////////////////////////////////////////////////////////////////////////////
// --- General ---

Loom_Interrupt_Manager& LoomManager::InterruptManager(uint8_t idx) 
{
	LoomModule* tmp = interrupt_manager;
	return (*(Loom_Interrupt_Manager*)( (tmp) ? tmp : &global_stub ));
}

Loom_Sleep_Manager& LoomManager::SleepManager(uint8_t idx) 
{
	LoomModule* tmp = sleep_manager;
	return (*(Loom_Sleep_Manager*)( (tmp) ? tmp : &global_stub ));
}


Loom_Multiplexer& LoomManager::Multiplexer(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Multiplexer, idx);
	return (*(Loom_Multiplexer*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
// --- Actuators ---

Loom_Neopixel& LoomManager::Neopixel(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Neopixel, idx);
	return (*(Loom_Neopixel*)( (tmp) ? tmp : &global_stub ));
}

Loom_Relay& LoomManager::Relay(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Relay, idx);
	return (*(Loom_Relay*)( (tmp) ? tmp : &global_stub ));
}

Loom_Servo& LoomManager::Servo(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Servo, idx);
	return (*(Loom_Servo*)( (tmp) ? tmp : &global_stub ));
}

Loom_Stepper& LoomManager::Stepper(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Stepper, idx);
	return (*(Loom_Stepper*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
// --- CommPlats --- 

Loom_LoRa& LoomManager::LoRa(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::LoRa, idx);
	return (*(Loom_LoRa*)( (tmp) ? tmp : &global_stub ));
}

Loom_nRF& LoomManager::nRF(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::nRF, idx);
	return (*(Loom_nRF*)( (tmp) ? tmp : &global_stub ));
}

Loom_Bluetooth& LoomManager::Bluetooth(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Bluetooth, idx);
	return (*(Loom_Bluetooth*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
// --- InternetPlats --- 

// LoomInternetPlat& LoomManager::InternetPlat(const uint8_t idx)
// {
// 	// if (idx >= modules.size()) return *(LoomInternetPlat*)(&global_stub);
// 	// else return *(modules[idx]);
// 	return *(LoomInternetPlat*)(&global_stub);
// }

Loom_Ethernet& LoomManager::Ethernet(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Ethernet, idx);
	return (*(Loom_Ethernet*)( (tmp) ? tmp : &global_stub ));
}

Loom_WiFi& LoomManager::WiFi(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::WiFi, idx);
	return (*(Loom_WiFi*)( (tmp) ? tmp : &global_stub ));
}


///////////////////////////////////////////////////////////////////////////////
// --- PublishPlats ---

// LoomPublishPlat& LoomManager::PublishPlat(const uint8_t idx)
// {
// 	// if (idx >= modules.size()) return *(LoomPublishPlat*)(&global_stub);
// 	// else return *(modules[idx]);
// 	return *(LoomPublishPlat*)(&global_stub);
// }

Loom_GoogleSheets& LoomManager::GoogleSheets(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::GoogleSheets, idx);
	return (*(Loom_GoogleSheets*)( (tmp) ? tmp : &global_stub ));
}

Loom_MaxPub& LoomManager::MaxPub(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::MaxPub, idx);
	return (*(Loom_MaxPub*)( (tmp) ? tmp : &global_stub ));
}


///////////////////////////////////////////////////////////////////////////////
// --- SubscribePlats ---

Loom_MaxSub& LoomManager::MaxSub(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::MaxSub, idx);
	return (*(Loom_MaxSub*)( (tmp) ? tmp : &global_stub ));
}


///////////////////////////////////////////////////////////////////////////////
// --- LogPlats ---

Loom_OLED& LoomManager::OLED(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::OLED, idx);
	return (*(Loom_OLED*)( (tmp) ? tmp : &global_stub ));
}

Loom_SD& LoomManager::SDCARD(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::SDCARD, idx);
	return (*(Loom_SD*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
// --- RTC --- 

Loom_DS3231& LoomManager::DS3231(uint8_t idx) 
{
	LoomModule* tmp = rtc_module;
	return (*(Loom_DS3231*)( (tmp && tmp->get_module_type() == LoomModule::Type::DS3231) ? tmp : &global_stub ));
}

Loom_PCF8523& LoomManager::PCF8523(uint8_t idx) 
{
	LoomModule* tmp = rtc_module;
	// return (*(Loom_PCF8523*)( (tmp) ? tmp : &global_stub ));
	return (*(Loom_PCF8523*)( (tmp && tmp->get_module_type() == LoomModule::Type::PCF8523) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
// --- Sensors ---

Loom_Analog& LoomManager::Analog(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Analog, idx);
	return (*(Loom_Analog*)( (tmp) ? tmp : &global_stub ));
}

Loom_Digital& LoomManager::Digital(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Digital, idx);
	return (*(Loom_Digital*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
//    --- I2C Sensors ---

Loom_AS7262& LoomManager::AS7262(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::AS7262, idx);
	return (*(Loom_AS7262*)( (tmp) ? tmp : &global_stub ));
}

Loom_AS7263& LoomManager::AS7263(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::AS7263, idx);
	return (*(Loom_AS7263*)( (tmp) ? tmp : &global_stub ));
}

Loom_AS7265X& LoomManager::AS7265X(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::AS7265X, idx);
	return (*(Loom_AS7265X*)( (tmp) ? tmp : &global_stub ));
}

Loom_FXAS21002& LoomManager::FXAS21002(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::FXAS21002, idx);
	return (*(Loom_FXAS21002*)( (tmp) ? tmp : &global_stub ));
}

Loom_FXOS8700& LoomManager::FXOS8700(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::FXOS8700, idx);
	return (*(Loom_FXOS8700*)( (tmp) ? tmp : &global_stub ));
}

Loom_LIS3DH& LoomManager::LIS3DH(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::LIS3DH, idx);
	return (*(Loom_LIS3DH*)( (tmp) ? tmp : &global_stub ));
}

Loom_MB1232& LoomManager::MB1232(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::MB1232, idx);
	return (*(Loom_MB1232*)( (tmp) ? tmp : &global_stub ));
}

Loom_MMA8451& LoomManager::MMA8451(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::MMA8451, idx);
	return (*(Loom_MMA8451*)( (tmp) ? tmp : &global_stub ));
}

Loom_MPU6050& LoomManager::MPU6050(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::MPU6050, idx);
	return (*(Loom_MPU6050*)( (tmp) ? tmp : &global_stub ));
}

Loom_MS5803& LoomManager::MS5803(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::MS5803, idx);
	return (*(Loom_MS5803*)( (tmp) ? tmp : &global_stub ));
}

Loom_SHT31D& LoomManager::SHT31D(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::SHT31D, idx);
	return (*(Loom_SHT31D*)( (tmp) ? tmp : &global_stub ));
}

Loom_TMP007& LoomManager::TMP007(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::TMP007, idx);
	return (*(Loom_TMP007*)( (tmp) ? tmp : &global_stub ));
}

Loom_TSL2561& LoomManager::TSL2561(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::TSL2561, idx);
	return (*(Loom_TSL2561*)( (tmp) ? tmp : &global_stub ));
}

Loom_TSL2591& LoomManager::TSL2591(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::TSL2591, idx);
	return (*(Loom_TSL2591*)( (tmp) ? tmp : &global_stub ));
}

Loom_ZXGesture& LoomManager::ZXGesture(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::ZXGesture, idx);
	return (*(Loom_ZXGesture*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
//    --- SDI12 Sensors ---

Loom_Decagon5TM& LoomManager::Decagon5TM(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Decagon5TM, idx);
	return (*(Loom_Decagon5TM*)( (tmp) ? tmp : &global_stub ));
}

Loom_DecagonGS3& LoomManager::DecagonGS3(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::DecagonGS3, idx);
	return (*(Loom_DecagonGS3*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
//    --- SPI Sensors ---

Loom_MAX31855& LoomManager::MAX31855(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::MAX31855, idx);
	return (*(Loom_MAX31855*)( (tmp) ? tmp : &global_stub ));
}

Loom_MAX31856& LoomManager::MAX31856(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::MAX31856, idx);
	return (*(Loom_MAX31856*)( (tmp) ? tmp : &global_stub ));
}


///////////////////////////////////////////////////////////////////////////////
//    --- Other ---

LoomNTPSync& LoomManager::NTP(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::NTP, idx);
	return (*(LoomNTPSync*)( (tmp) ? tmp : &global_stub ));
}

LoomTempSync& LoomManager::TempSync(uint8_t idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::TempSync, idx);
	return (*(LoomTempSync*)( (tmp) ? tmp : &global_stub ));
}



