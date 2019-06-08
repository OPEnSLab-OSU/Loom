
// This is given its own file for the sake of separating the section of code that
// will change with module additions from the rest of the implementation of Loom_Manager

#include "Loom_Module.h"
#include "Loom_Manager.h"
#include "Loom_Module_Factory.h"

#include <ArduinoJson.h>



Factory LoomFactory;



/// Parses a JSON configuration and instantiates the specified 
/// LoomModules.
/// Adds created LoomModules to the arrays of LoomManager
void LoomManager::parse_config(const char* json_config)
{
	// Parse Json config

	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln();
		LPrintln("= = = = = Parse Config = = = = =");
	}

// Might need to be even larger
	DynamicJsonDocument doc(2048);
	DeserializationError error = deserializeJson(doc, json_config);

	// Test if parsing succeeds.
	if (error) {
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.c_str());
		return;
	}



	if (print_verbosity == Verbosity::V_HIGH) {

		Serial.println("\n\nConfig Pretty Version:");
		serializeJsonPretty(doc, Serial);
		LPrintln("\nSIZE: ", doc.memoryUsage());

		// LPrintln("measureJson: ", measureJson(doc));

		// Serial.println("\n\nGeneral Settings:");
		// serializeJsonPretty(doc["general"], Serial);

		// Serial.print("\n\nNum modules: ");
		// Serial.println(doc["components"].size()); 
		// LPrintln();


		// Apply LoomManager General Settings
		LPrintln("= = = = = LoomManager Settings = = = = =");
		LPrintln();
	}

	JsonObject general = doc["general"];


	if (general.containsKey("device_name")) {
		snprintf(this->device_name, 20, "%s", general["device_name"].as<const char*>());
	}
	if (general.containsKey("family")) {
		snprintf(this->family, 20, "%s", general["family"].as<const char*>());
	}
	if (general.containsKey("family_num")) {
		this->family_num = general["family_num"];
	}
	if (general.containsKey("instance")) {
		this->instance = general["instance"];
	}
	if (general.containsKey("device_type")) {
		this->device_type = (DeviceType)(int)general["device_type"];
	}
	if (general.containsKey("print_verbosity")) {
		this->print_verbosity = (Verbosity)(int)general["print_verbosity"];
	}
	if (general.containsKey("package_verbosity")) {
		this->package_verbosity = (Verbosity)(int)general["package_verbosity"];
	}

	// print_config();


	// Generate Module Objects

	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln("= = = = = Generate Objects = = = = =");
		LPrintln();
	}


	// Call module factory
	for ( JsonVariant module : doc["components"].as<JsonArray>()) {
		
		Loom_Interrupt_Manager*	interrupt_manager	= nullptr; 
		Loom_Sleep_Manager*		sleep_manager		= nullptr;
		LoomRTC* 				rtc					= nullptr;
		LoomModule*				other_module		= nullptr; 
		LoomSensor*				sensor				= nullptr;
		LoomActuator*			actuator			= nullptr;
		LoomCommPlat*			comm_plat			= nullptr;
		LoomInternetPlat*		internet_plat		= nullptr;
		LoomLogPlat*			log_plat			= nullptr;

		LoomFactory.CreateAndSort(
			module,
			interrupt_manager,
			sleep_manager,
			rtc,
			other_module,
			sensor,
			actuator,
			comm_plat,
			internet_plat,
			log_plat
		);


		if (interrupt_manager != nullptr) {
			add_module(interrupt_manager); 
		} 
		if (sleep_manager != nullptr) {
			add_module(sleep_manager); 
		} 
		if (rtc != nullptr) {
			add_module(rtc);
		} 
		if (other_module != nullptr) {
			add_module(other_module); 
		} 
		if (sensor != nullptr) {
			add_module(sensor); 
		} 
		if (actuator != nullptr) {
			add_module(actuator); 
		} 
		if (comm_plat != nullptr) {
			add_module(comm_plat); 
		} 
		if (internet_plat != nullptr) {
			add_module(internet_plat); 
		} 
		if (log_plat != nullptr) {
			add_module(log_plat); 
		}

	}

	// Empty
	doc.clear();

	/// call second stage construction
	/// other modules must go last, as they are most likely to do weird things 
	second_stage_ctor_aux( (LoomModule**)sensor_modules   , sensor_count       ); 
	second_stage_ctor_aux( (LoomModule**)actuator_modules , actuator_count     ); 
	second_stage_ctor_aux( (LoomModule**)comm_modules     , comm_count         ); 
	second_stage_ctor_aux( (LoomModule**)internet_modules , internet_count     ); 
	second_stage_ctor_aux( (LoomModule**)log_modules      , log_count          );
	second_stage_ctor_aux( (LoomModule**)other_modules    , other_module_count ); 

	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln("= = = = = = = = = = = = = = = = =");
		LPrintln();
	}

}

