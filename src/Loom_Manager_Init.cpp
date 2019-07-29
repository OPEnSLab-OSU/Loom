
// This is given its own file for the sake of separating the section of code that
// will change with module additions from the rest of the implementation of Loom_Manager

#include "Loom_Module.h"
#include "Loom_Manager.h"
#include "Loom_Module_Factory.h"

#include <ArduinoJson.h>
#include <SD.h>


Factory LoomFactory;


///////////////////////////////////////////////////////////////////////////////
bool LoomManager::parse_config(const char* json_config)
{
	// Might need to be even larger
	DynamicJsonDocument doc(2048);
	DeserializationError error = deserializeJson(doc, json_config);

	// Test if parsing succeeds.
	if (error) {
		print_device_label();
		LPrintln("deserializeJson() failed: ", error.c_str());
		return false;
	}

	bool status = parse_config_json( doc.as<JsonObject>() );
	doc.clear();
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomManager::parse_config_SD(const char* config_file)
{
	print_device_label();
	LPrintln("Read config from file: '", config_file, "'");

	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
	if (!SD.begin(SD_CS)) {
		print_device_label();
		LPrintln("SD failed to begin");
		return false;
	}

	File file = SD.open(config_file);
	if (!file) {
		print_device_label();
		LPrintln("Failed to open '", config_file, "'");
		return false;
	}

	DynamicJsonDocument doc(2048);
	DeserializationError error = deserializeJson(doc, file);
	
	// Test if parsing succeeds.
	if (error) {
		print_device_label();
		LPrintln("deserializeJson() failed: ", error.c_str());
		return false;
	}

	bool status = parse_config_json( doc.as<JsonObject>() );
	doc.clear();
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomManager::parse_config_json(JsonObject config)
{
	// Remove current modules
	free_modules();

	if (print_verbosity == Verbosity::V_HIGH) {

		LPrintln("\n= = = = = Parse Config = = = = =");
		LPrintln("\nConfig Pretty Version:");
		serializeJsonPretty(config, Serial);
		LPrintln("\nSIZE: ", config.memoryUsage());
	}

	// Apply LoomManager General Settings
	JsonObject general = config["general"];

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

	// Generate Module Objects
	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln("= = = = = Generate Objects = = = = =\n");
	}

	// Call module factory
	for ( JsonVariant module : config["components"].as<JsonArray>()) {
		
		Loom_Interrupt_Manager*	interrupt_manager	= nullptr; 
		Loom_Sleep_Manager*		sleep_manager		= nullptr;
		LoomRTC* 				rtc					= nullptr;
		LoomModule*				other_module		= nullptr; 
		LoomSensor*				sensor				= nullptr;
		LoomActuator*			actuator			= nullptr;
		LoomCommPlat*			comm_plat			= nullptr;
		LoomInternetPlat*		internet_plat		= nullptr;
		LoomPublishPlat*		publish_plat		= nullptr;
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
			publish_plat,
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
		if (publish_plat != nullptr) {
			add_module(publish_plat); 
		} 
		if (log_plat != nullptr) {
			add_module(log_plat); 
		}

	}

	// call second stage construction
	// other modules must go last, as they are most likely to do weird things 
	second_stage_ctor_aux( sensor_modules   ); 
	second_stage_ctor_aux( actuator_modules ); 
	second_stage_ctor_aux( comm_modules     ); 
	second_stage_ctor_aux( internet_modules ); 
	second_stage_ctor_aux( publish_modules  ); 
	second_stage_ctor_aux( log_modules      );
	second_stage_ctor_aux( other_modules    ); 


	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln("= = = = = = = = = = = = = = = = =");
		LPrintln();
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////









