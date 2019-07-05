
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

Loom_Interrupt_Manager& LoomManager::InterruptManager(int idx) 
{
	LoomModule* tmp = interrupt_manager;
	return (*(Loom_Interrupt_Manager*)( (tmp) ? tmp : &global_stub ));
}

Loom_Sleep_Manager& LoomManager::SleepManager(int idx) 
{
	LoomModule* tmp = sleep_manager;
	return (*(Loom_Sleep_Manager*)( (tmp) ? tmp : &global_stub ));
}


Loom_Multiplexer& LoomManager::Multiplexer(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Multiplexer, idx, other_modules);
	return (*(Loom_Multiplexer*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
// --- Actuators ---

Loom_Neopixel& LoomManager::Neopixel(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Neopixel, idx, actuator_modules);
	return (*(Loom_Neopixel*)( (tmp) ? tmp : &global_stub ));
}

Loom_Relay& LoomManager::Relay(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Relay, idx, actuator_modules);
	return (*(Loom_Relay*)( (tmp) ? tmp : &global_stub ));
}

Loom_Servo& LoomManager::Servo(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Servo, idx, actuator_modules);
	return (*(Loom_Servo*)( (tmp) ? tmp : &global_stub ));
}

Loom_Stepper& LoomManager::Stepper(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Stepper, idx, actuator_modules);
	return (*(Loom_Stepper*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
// --- CommPlats --- 

Loom_LoRa& LoomManager::LoRa(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::LoRa, idx, comm_modules);
	return (*(Loom_LoRa*)( (tmp) ? tmp : &global_stub ));
}

Loom_nRF& LoomManager::nRF(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::nRF, idx, comm_modules);
	return (*(Loom_nRF*)( (tmp) ? tmp : &global_stub ));
}

Loom_Bluetooth& LoomManager::Bluetooth(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Bluetooth, idx, comm_modules);
	return (*(Loom_Bluetooth*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
// --- InternetPlats --- 

LoomInternetPlat& LoomManager::InternetPlat(const int idx)
{
	if (idx >= internet_modules.size()) return *(LoomInternetPlat*)(&global_stub);
	else return *(internet_modules[idx]);
}

///////////////////////////////////////////////////////////////////////////////
// --- PublishPlats ---

LoomPublishPlat& LoomManager::PublishPlat(const int idx)
{
	if (idx >= publish_modules.size()) return *(LoomPublishPlat*)(&global_stub);
	else return *(publish_modules[idx]);
}

///////////////////////////////////////////////////////////////////////////////
// --- LogPlats ---

Loom_OLED& LoomManager::OLED(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::OLED, idx, log_modules);
	return (*(Loom_OLED*)( (tmp) ? tmp : &global_stub ));
}

Loom_SD& LoomManager::SDCARD(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::SDCARD, idx, log_modules);
	return (*(Loom_SD*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
// --- RTC --- 

Loom_DS3231& LoomManager::DS3231(int idx) 
{
	LoomModule* tmp = rtc_module;
	return (*(Loom_DS3231*)( (tmp && tmp->get_module_type() == LoomModule::Type::DS3231) ? tmp : &global_stub ));
}

Loom_PCF8523& LoomManager::PCF8523(int idx) 
{
	LoomModule* tmp = rtc_module;
	// return (*(Loom_PCF8523*)( (tmp) ? tmp : &global_stub ));
	return (*(Loom_PCF8523*)( (tmp && tmp->get_module_type() == LoomModule::Type::PCF8523) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
// --- Sensors ---

Loom_Analog& LoomManager::Analog(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Analog, idx, sensor_modules);
	return (*(Loom_Analog*)( (tmp) ? tmp : &global_stub ));
}

Loom_Digital& LoomManager::Digital(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Digital, idx, sensor_modules);
	return (*(Loom_Digital*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
//    --- I2C Sensors ---

Loom_AS7262& LoomManager::AS7262(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::AS7262, idx, sensor_modules);
	return (*(Loom_AS7262*)( (tmp) ? tmp : &global_stub ));
}

Loom_AS7263& LoomManager::AS7263(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::AS7263, idx, sensor_modules);
	return (*(Loom_AS7263*)( (tmp) ? tmp : &global_stub ));
}

Loom_AS7265X& LoomManager::AS7265X(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::AS7265X, idx, sensor_modules);
	return (*(Loom_AS7265X*)( (tmp) ? tmp : &global_stub ));
}

Loom_FXAS21002& LoomManager::FXAS21002(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::FXAS21002, idx, sensor_modules);
	return (*(Loom_FXAS21002*)( (tmp) ? tmp : &global_stub ));
}

Loom_FXOS8700& LoomManager::FXOS8700(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::FXOS8700, idx, sensor_modules);
	return (*(Loom_FXOS8700*)( (tmp) ? tmp : &global_stub ));
}

Loom_LIS3DH& LoomManager::LIS3DH(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::LIS3DH, idx, sensor_modules);
	return (*(Loom_LIS3DH*)( (tmp) ? tmp : &global_stub ));
}

Loom_MB1232& LoomManager::MB1232(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::MB1232, idx, sensor_modules);
	return (*(Loom_MB1232*)( (tmp) ? tmp : &global_stub ));
}

Loom_MS5803& LoomManager::MS5803(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::MS5803, idx, sensor_modules);
	return (*(Loom_MS5803*)( (tmp) ? tmp : &global_stub ));
}

Loom_SHT31D& LoomManager::SHT31D(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::SHT31D, idx, sensor_modules);
	return (*(Loom_SHT31D*)( (tmp) ? tmp : &global_stub ));
}

Loom_TSL2561& LoomManager::TSL2561(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::TSL2561, idx, sensor_modules);
	return (*(Loom_TSL2561*)( (tmp) ? tmp : &global_stub ));
}

Loom_TSL2591& LoomManager::TSL2591(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::TSL2591, idx, sensor_modules);
	return (*(Loom_TSL2591*)( (tmp) ? tmp : &global_stub ));
}

Loom_ZXGesture& LoomManager::ZXGesture(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::ZXGesture, idx, sensor_modules);
	return (*(Loom_ZXGesture*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
//    --- SDI12 Sensors ---

Loom_Decagon5TM& LoomManager::Decagon5TM(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::Decagon5TM, idx, sensor_modules);
	return (*(Loom_Decagon5TM*)( (tmp) ? tmp : &global_stub ));
}

Loom_DecagonGS3& LoomManager::DecagonGS3(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::DecagonGS3, idx, sensor_modules);
	return (*(Loom_DecagonGS3*)( (tmp) ? tmp : &global_stub ));
}

///////////////////////////////////////////////////////////////////////////////
//    --- SPI Sensors ---

Loom_MAX31856& LoomManager::MAX31856(int idx) 
{
	LoomModule* tmp = find_module(LoomModule::Type::MAX31856, idx, sensor_modules);
	return (*(Loom_MAX31856*)( (tmp) ? tmp : &global_stub ));
}







