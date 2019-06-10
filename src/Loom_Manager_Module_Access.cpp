
// This is given its own file for the sake of separating the section of code that
// will change with module additions from the rest of the implementation of Loom_Manager

#include "Loom_Module.h"
#include "Loom_Manager.h"
#include "Loom_Module_Stub.h"

// Global stub to reference if user tries to access 
// object that does not exist
Loom_Module_Stub global_stub;




LoomModule*	LoomManager::find_module(ModuleType type, int idx, LoomModule** modules, int count)
{
	int current = 0;

	for (int i = 0; i < count; i++) {
		if (type == modules[i]->get_module_type()) {
			if (current == idx) {
				return modules[i];
			} else {
				current++;
			}
		}
	}
	return nullptr;
}

// --- General ---

// maybe this not templatized
Loom_Interrupt_Manager& LoomManager::InterruptManager(int idx) 
{
	// LoomModule* tmp = find_module(ModuleType::Interrupt_Manager, idx, (LoomModule**)interrupt_manager, 1);
	LoomModule* tmp = interrupt_manager;
	return (*(Loom_Interrupt_Manager*)( (tmp) ? tmp : &global_stub ));
}

// maybe this not templatized
Loom_Sleep_Manager& LoomManager::SleepManager(int idx) 
{
	// LoomModule* tmp = find_module(ModuleType::Sleep_Manager, idx, (LoomModule**)sleep_manager, 1);
	LoomModule* tmp = sleep_manager;
	return (*(Loom_Sleep_Manager*)( (tmp) ? tmp : &global_stub ));
}


Loom_Multiplexer& LoomManager::Multiplexer(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Multiplexer, idx, (LoomModule**)other_modules, 1);
	return (*(Loom_Multiplexer*)( (tmp) ? tmp : &global_stub ));
}

// --- Actuators ---

Loom_Neopixel& LoomManager::Neopixel(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Neopixel, idx, (LoomModule**)actuator_modules, actuator_count);
	return (*(Loom_Neopixel*)( (tmp) ? tmp : &global_stub ));
}

Loom_Relay& LoomManager::Relay(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Relay, idx, (LoomModule**)actuator_modules, actuator_count);
	return (*(Loom_Relay*)( (tmp) ? tmp : &global_stub ));

	// return find_module(ModuleType::Relay, idx, (LoomModule**)actuator_modules, actuator_count);
}

Loom_Servo& LoomManager::Servo(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Servo, idx, (LoomModule**)actuator_modules, actuator_count);
	return (*(Loom_Servo*)( (tmp) ? tmp : &global_stub ));
}

Loom_Stepper& LoomManager::Stepper(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Stepper, idx, (LoomModule**)actuator_modules, actuator_count);
	return (*(Loom_Stepper*)( (tmp) ? tmp : &global_stub ));
}



// --- CommPlats --- 


Loom_LoRa& LoomManager::LoRa(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::LoRa, idx, (LoomModule**)comm_modules, comm_count);
	return (*(Loom_LoRa*)( (tmp) ? tmp : &global_stub ));
}

Loom_nRF& LoomManager::nRF(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::nRF, idx, (LoomModule**)comm_modules, comm_count);
	return (*(Loom_nRF*)( (tmp) ? tmp : &global_stub ));
}

Loom_Bluetooth& LoomManager::Bluetooth(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Bluetooth, idx, (LoomModule**)comm_modules, comm_count);
	return (*(Loom_Bluetooth*)( (tmp) ? tmp : &global_stub ));
}



// --- InternetPlats --- 
LoomInternetPlat& LoomManager::InternetPlat(const int idx)
{
	if (idx >= internet_count) return *(LoomInternetPlat*)(&global_stub);
	else return *(internet_modules[idx]);
}


// --- LogPlats ---

Loom_OLED& LoomManager::OLED(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::OLED, idx, (LoomModule**)log_modules, log_count);
	return (*(Loom_OLED*)( (tmp) ? tmp : &global_stub ));
}

Loom_SD& LoomManager::SDCARD(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::SDCARD, idx, (LoomModule**)log_modules, log_count);
	return (*(Loom_SD*)( (tmp) ? tmp : &global_stub ));
}


// --- RTC --- 

Loom_DS3231& LoomManager::DS3231(int idx) 
{
	LoomModule* tmp =rtc_module;
	return (*(Loom_DS3231*)( (tmp) ? tmp : &global_stub ));
}

Loom_PCF8523& LoomManager::PCF8523(int idx) 
{
	// LoomModule* tmp = find_module(ModuleType::PCF8523, idx, (LoomModule**)rtc_module, 1);
	LoomModule* tmp =rtc_module;
	return (*(Loom_PCF8523*)( (tmp) ? tmp : &global_stub ));
}



// --- Sensors ---

Loom_Analog& LoomManager::Analog(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Analog, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_Analog*)( (tmp) ? tmp : &global_stub ));
}

Loom_Digital& LoomManager::Digital(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Digital, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_Digital*)( (tmp) ? tmp : &global_stub ));
}


//    --- I2C Sensors ---

Loom_AS7262& LoomManager::AS7262(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::AS7262, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_AS7262*)( (tmp) ? tmp : &global_stub ));
}

Loom_AS7263& LoomManager::AS7263(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::AS7263, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_AS7263*)( (tmp) ? tmp : &global_stub ));
}

Loom_AS7265X& LoomManager::AS7265X(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::AS7265X, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_AS7265X*)( (tmp) ? tmp : &global_stub ));
}

Loom_FXAS21002& LoomManager::FXAS21002(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::FXAS21002, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_FXAS21002*)( (tmp) ? tmp : &global_stub ));
}

Loom_FXOS8700& LoomManager::FXOS8700(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::FXOS8700, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_FXOS8700*)( (tmp) ? tmp : &global_stub ));
}

Loom_LIS3DH& LoomManager::LIS3DH(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::LIS3DH, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_LIS3DH*)( (tmp) ? tmp : &global_stub ));
}

Loom_MB1232& LoomManager::MB1232(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::MB1232, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_MB1232*)( (tmp) ? tmp : &global_stub ));
}

Loom_MS5803& LoomManager::MS5803(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::MS5803, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_MS5803*)( (tmp) ? tmp : &global_stub ));
}

Loom_SHT31D& LoomManager::SHT31D(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::SHT31D, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_SHT31D*)( (tmp) ? tmp : &global_stub ));
}

Loom_TSL2561& LoomManager::TSL2561(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::TSL2561, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_TSL2561*)( (tmp) ? tmp : &global_stub ));
}

Loom_TSL2591& LoomManager::TSL2591(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::TSL2591, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_TSL2591*)( (tmp) ? tmp : &global_stub ));
}

Loom_ZXGesture& LoomManager::ZXGesture(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::ZXGesture, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_ZXGesture*)( (tmp) ? tmp : &global_stub ));
}


//    --- SDI12 Sensors ---

Loom_Decagon5TM& LoomManager::Decagon5TM(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Decagon5TM, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_Decagon5TM*)( (tmp) ? tmp : &global_stub ));
}

Loom_DecagonGS3& LoomManager::DecagonGS3(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::DecagonGS3, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_DecagonGS3*)( (tmp) ? tmp : &global_stub ));
}


//    --- SPI Sensors ---

Loom_MAX31856& LoomManager::MAX31856(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::MAX31856, idx, (LoomModule**)sensor_modules, sensor_count);
	return (*(Loom_MAX31856*)( (tmp) ? tmp : &global_stub ));
}







