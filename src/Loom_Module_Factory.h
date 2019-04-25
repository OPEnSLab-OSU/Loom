

#ifndef LOOM_MODULE_FACTORY_h
#define LOOM_MODULE_FACTORY_h



#include "Loom_Module.h"

#include <ArduinoJson.h>


// Should overload to just take 'const char*' in which use code provided defaults
// multiplexer might make use of this



void module_factory_aux(
		JsonVariant					module, 
		Loom_Interrupt_Manager*&	interrupt_manager, 
		Loom_Sleep_Manager*&		sleep_manager,
		LoomRTC*&					rtc,
		LoomModule*&				other_module, 
		LoomSensor*&				sensor,
		LoomActuator*&				actuator,
		LoomCommPlat*&				comm_plat,
		LoomInternetPlat*&			internet_plat,
		LoomLogPlat*&				log_plat
	); 

// LoomModule* module_factory_aux(JsonVariant module);


#endif // of LOOM_MODULE_FACTORY_h