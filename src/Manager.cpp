///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Manager.cpp
/// @brief		File for LoomManager method implementations.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Module.h"
#include "Manager.h"
#include "Module_Factory.h"
#include "Macros.h"
#include "Interrupt_Manager.h"
#include "Sleep_Manager.h"

#include "Sensors/Sensor.h"
#include "Multiplexer.h"
#include "Actuators/Actuator.h"
#include "CommPlats/CommPlat.h"
#include "InternetPlats/InternetPlat.h"
#include "LogPlats/LogPlat.h"
#include "RTC/RTC.h"
#include "PublishPlats/PublishPlat.h"
#include "NTP_Sync.h"
//#include "I2Cdev.h"


#include <ArduinoJson.h>
#include <SdFat.h>
#include <algorithm>



///////////////////////////////////////////////////////////////////////////////
const char* LoomManager::enum_device_type_string(const DeviceType t)
{
	switch(t) {
		case DeviceType::HUB      : return "Hub";
		case DeviceType::NODE     : return "Node";
		case DeviceType::REPEATER : return "Repeater";
		default : return "";
	}
}

///////////////////////////////////////////////////////////////////////////////
LoomManager::LoomManager(
		const FactoryBase*			factory_ptr,
		const char*				device_name,
		const uint8_t			instance,
		const DeviceType		device_type,
		const Verbosity			print_verbosity,
		const Verbosity			package_verbosity,
		const uint16_t			interval
	)
	: Factory(factory_ptr)
	, instance(instance)
	, print_verbosity(print_verbosity)
	, package_verbosity(package_verbosity)
	, device_type(device_type)
	, interval(interval)
{
	snprintf(this->device_name, 20, "%s", device_name);
}

///////////////////////////////////////////////////////////////////////////////
LoomManager::~LoomManager()
{
	free_modules();
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::print_config(const bool print_modules_config)
{
	print_device_label();
	LPrintln("Config:");
	LPrintln("\tDevice Name         : ", device_name );
	LPrintln("\tInstance Number     : ", instance );
	LPrintln("\tDevice Type         : ", enum_device_type_string(device_type) );
	LPrintln("\tInterval            : ", interval );

	list_modules();

	// Print managed module's configs
	if (print_modules_config) {
		for (auto module : modules) {
			module->print_config();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::begin_serial(const bool wait_for_monitor) const
{
	Serial.begin(SERIAL_BAUD);

	if (wait_for_monitor) {
		unsigned long start = millis();
		while( !Serial && ((millis() - start) < MAX_SERIAL_WAIT) );
		delay(1000);
	}

	LPrintln("Initialized Serial!\n");
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomModule* module)
{

	print_device_label();

	if (module == nullptr) {
		LPrintln("Cannot add null module");
		return;
	} else if (!module->get_active()) {
		LPrintln("Cannot add inactive module");
		delete module;
		return;
	}

	LPrintln("Adding Module: ", ((LoomModule*)module)->get_module_name() );

	modules.emplace_back(module);
	module->link_device_manager(this);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::list_modules() const
{
	print_device_label();
	LPrintln("Modules:");

	auto last_category = LoomModule::Category::Unknown;

	for (auto module : modules) {
		auto category = module->category();
		if ( category != last_category ) {
			LPrintln("\t", LoomModule::enum_category_string(category), "s");//, " (", modules.size(), "):");
			last_category = category;
		}
		if ( module != nullptr ) {
			LPrintln( "\t\t[", module->get_active() ? "+" : "-" , "] ", module->get_module_name() );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_device_name(const char* device_name)
{
	snprintf(this->device_name, 20, "%s", device_name);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::get_device_name(char* buf) const
{
	sprintf(buf, "%s", device_name);
}

///////////////////////////////////////////////////////////////////////////////
const char* LoomManager::get_device_name() const
{
	char name[20];
	get_device_name(name);
	return (const char*)name;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_print_verbosity(const Verbosity v, const bool set_modules)
{
	print_verbosity = v;

	if (set_modules) {
		for (auto module : modules) {
			if ( (module != nullptr) && ( ((LoomModule*)module)->get_active() ) ){
				((LoomModule*)module)->set_print_verbosity(v);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_package_verbosity(const Verbosity v, const bool set_modules)
{
	package_verbosity = v;

	if (set_modules) {
		for (auto module : modules) {
			if ( (module != nullptr) && ( ((LoomModule*)module)->get_active() ) ){
				((LoomModule*)module)->set_package_verbosity(v);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::measure()
{
	for (auto module : modules) {
		if ( !module->get_active() ) continue;

		if ( module->category() == LoomModule::Category::Sensor ) {
			((LoomSensor*)module)->measure();
		}
		else if (
			(module->get_module_type() == LoomModule::Type::Multiplexer) ) {
			((Loom_Multiplexer*)module)->measure();
		}
		else if (module->get_module_type() == LoomModule::Type::NTP) {
			((LoomNTPSync*)module)->measure();
		}
		else if (module->get_module_type() == LoomModule::Type::TempSync) {
			((LoomTempSync*)module)->measure();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::package(JsonObject json)
{
	// Add device identification to json
	add_device_ID_to_json(json);

	// Add a packet number to json
	add_data("Packet", "Number", packet_number++);

	for (auto module : modules) {
		if ( (module != nullptr) && ( ((LoomModule*)module)->get_active() ) ){
			((LoomModule*)module)->package( json );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
JsonObject LoomManager::package()
{
	doc.clear();
	doc["type"] = "data";
	JsonObject json = doc.as<JsonObject>();
	package(json);
	// print an error if the JSON is full
	if (doc.memoryUsage() > doc.capacity() - 10)
		LPrintln("Warning: JSON document overflowed! Loom does not have enough memory for the number of sensors you are reading, please use less sensors or submit an issue.");

	return json;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::add_device_ID_to_json(JsonObject json)
{
	JsonObject timestamp = json.createNestedObject("id");
	timestamp["name"]		= device_name;
	timestamp["instance"]	= instance;
}

///////////////////////////////////////////////////////////////////////////////
JsonObject LoomManager::internal_json(const bool clear)
{
	if (clear) {
		// doc.clear();
		return doc.to<JsonObject>(); // clears in the process
	} else {
		return doc.as<JsonObject>();
	}


	// doc["type"] = "unknown"; // JsonObject::set wont work if there is not anything in object
	// LPrintln("\nDOC MemoryUsage in internal_json: ", doc.memoryUsage());

	// return doc.as<JsonObject>();

}

///////////////////////////////////////////////////////////////////////////////
bool LoomManager::publish_all(const JsonObject json)
{
	bool result = true;
	uint8_t count = 0;
	for (auto module : modules) {
		if ( (module != nullptr) &&
			 (module->category() == LoomModule::Category::PublishPlat) &&
			 (module->get_active())
			) {
			result &= ((LoomPublishPlat*)module)->publish( json );
			count++;
		}
	}
	return (count > 0) && result;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::dispatch(JsonObject json)
{
	print_device_label();
	LPrintln("Processing command");
	serializeJsonPretty(json, Serial);
	LPrintln();
	// return;

	// If is command
	if ( strcmp(json["type"], "command") == 0 )	{

		// For each command
		for ( JsonObject cmd : json["commands"].as<JsonArray>() ) {

			// Check if command is for manager
			if ( strcmp(cmd["module"].as<const char*>(), "Manager" ) == 0) {
				if (dispatch_self(cmd)) break;
			}

			// Otherwise iterate over modules until module to handle command is found
			// LPrintln("Try to dispatch to: ", cmd["module"].as<const char*>() );
			for (auto module : modules) {
				if ( (module != nullptr) &&
					 ( ((LoomModule*)module)->get_active() ) &&
					 ( strcmp(cmd["module"].as<const char*>(), module->get_module_name() ) == 0 )
					){
					// LPrintln("Found module");
					if ( module->dispatch( cmd ) ) break; // move to next command
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
bool LoomManager::dispatch_self(JsonObject json)
{
	JsonArray params = json["params"];
	switch( (char)json["func"] ) {
		case 'i': if (params.size() >= 1) { set_interval( EXPAND_ARRAY(params, 1) ); } return true;
		case 'j': if (params.size() >= 1) { parse_config_SD( EXPAND_ARRAY(params, 1) ); } return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::display_data() const
{
	print_device_label();
	LPrintln("Json:");
	serializeJsonPretty(doc, Serial);
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::flash_LED(const uint8_t count, const uint8_t time_high, const uint8_t time_low, const bool end_high) const
{
	for (int i = 0; i < count; i++) {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(time_high);
		digitalWrite(LED_BUILTIN, LOW);
		delay(time_low);
	}
	if (end_high) {
		digitalWrite(LED_BUILTIN, HIGH);
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::free_modules()
{
	for (auto module : modules) {
		delete module;
	}
	modules.clear();

	rtc_module = nullptr;
	interrupt_manager = nullptr;
	sleep_manager = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::power_up()
{
	// Iterate over list of modules powering them on
	for (auto module : modules) {
		if ( module != nullptr ){
			((LoomModule*)module)->power_up();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::power_down()
{
	// Iterate over list of modules powering them off
	for (auto module : modules) {
		if ( module != nullptr ){
			((LoomModule*)module)->power_down();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::get_config()
{
	doc.clear();
	doc["type"] = "config";
	JsonObject json = doc.as<JsonObject>();

	// Add general device identification
	JsonObject general_info 	= json.createNestedObject("general");
	general_info["name"]		= device_name;
	general_info["instance"]	= instance;

	// Start array for modules to add config objects to
	JsonArray components = json.createNestedArray("components");

	for (auto module : modules) {
		if ( module != nullptr ){
			((LoomModule*)module)->add_config(json);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
LoomModule*	LoomManager::find_module(const LoomModule::Type type, const uint8_t idx) const
{
	uint8_t current = 0;

	for (auto module : modules) {
		if (type == module->get_module_type()) {
			if (current == idx) {
				return (LoomModule*)module;
			} else {
				current++;
			}
		}
	}
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
LoomModule*	LoomManager::find_module_by_category(const LoomModule::Category category, const uint8_t idx) const
{
	uint8_t current = 0;

	for (auto module : modules) {
		if (category == module->category()) {
			if (current == idx) {
				return (LoomModule*)module;
			} else {
				current++;
			}
		}
	}
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_interval(const uint16_t ms)
{
	interval = ms;
	print_device_label();
	LPrintln("Set interval to: ", interval);
}

///////////////////////////////////////////////////////////////////////////////
bool LoomManager::has_module(const LoomModule::Type type) const
{
	for (auto module : modules) {
		if (module->get_module_type() == type) return true;
	}
	return false;
}

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
	SdFat sd;	// File system object

	print_device_label();
	LPrintln("Read config from file: '", config_file, "'");

	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
	delay(25);
	// if (!SD.begin(SD_CS)) {	// Make sure we can communicate with SD
	if ( !sd.begin(SD_CS, SD_SCK_MHZ(50)) ) {	// Make sure we can communicate with SD
		print_device_label();
		LPrintln("SD failed to begin");
		return false;
	}

	File file = sd.open(config_file);
	if (!file) {	// Make sure file exists
		print_device_label();
		LPrintln("Failed to open '", config_file, "'");
		return false;
	}

	DynamicJsonDocument doc(2048);
	DeserializationError error = deserializeJson(doc, file);

	// Test if parsing succeeds.
	if (error) { // Make sure json was valid
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

	if (general.containsKey("name")) {
		snprintf(this->device_name, 20, "%s", general["name"].as<const char*>());
	}
	if (general.containsKey("instance")) {
		const JsonVariantConst inst = general["instance"];
		if (inst.is<int>())
			this->instance = general["instance"];
		else if (inst.is<const char*>())
			this->instance = atoi(inst.as<const char*>());
	}
	if (general.containsKey("interval")) {
		this->interval = general["interval"];
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

	// Call module factory creating each module
	for ( JsonVariant module : config["components"].as<JsonArray>()) {
		if (Factory) {
			add_module(Factory->Create(this, module));
		}
	}

	// Sort modules by type
	std::sort(modules.begin(), modules.end(), module_sort_comp());

	// Run second stage constructors
	for (auto module : modules) {
		if ( module != nullptr ) {
			module->second_stage_ctor();
		}
	}

	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln("= = = = = = = = = = = = = = = = =");
		LPrintln();
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////////////////////
bool LoomManager::parse_config_serial()
{
	flash_LED(4, 200, 100, true);

	LPrintln("Waiting for config over Serial");

	while(!Serial.available());



	// Might need to be even larger
	DynamicJsonDocument doc(2048);
	DeserializationError error = deserializeJson(doc, Serial);

	// Test if parsing succeeds.
	if (error) {
		print_device_label();
		LPrintln("deserializeJson() failed: ", error.c_str());
		return false;
	}

	bool status = parse_config_json( doc.as<JsonObject>() );
	doc.clear();

	flash_LED(12, 50, 25, false);
	// flash_LED(12, 50, 25, true);
	// delay(3000);
	// digitalWrite(LED_BUILTIN, LOW);
	// LPrintln("Received config");

	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomManager::check_serial_for_config()
{
	if (Serial.available()) {
		// return parse_config_serial();

		DynamicJsonDocument doc(2048);
		DeserializationError error = deserializeJson(doc, Serial);

		// Test if parsing succeeds.
		if (error) {
			print_device_label();
			LPrintln("deserializeJson() failed: ", error.c_str());
			return false;
		}

		bool status = parse_config_json( doc.as<JsonObject>() );
		doc.clear();

		flash_LED(12, 50, 25, false);
		return status;
	}
	return false;
}
