///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Manager.cpp
/// @brief		File for Manager method implementations.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Manager.h"
#include "Module_Factory.h"
#include "Macros.h"
#include "InterruptManager.h"
#include "SleepManager.h"
#include "LogPlats/SD.h"

#include "Sensors/Sensor.h"
#include "Multiplexer.h"
#include "Actuators/Actuator.h"
#include "CommPlats/CommPlat.h"
#include "InternetPlats/InternetPlat.h"
#include "LogPlats/LogPlat.h"
#include "RTC/RTC.h"
#include "PublishPlats/PublishPlat.h"
#include "NTPSync.h"
#include "TemperatureSync.h"
// #include "I2Cdev.h"

#include <ArduinoJson.h>
#include <SdFat.h>
#include <algorithm>
#include <ranges>

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

auto module_exists = [](Module* module) { return module != nullptr; };
auto module_active = [](Module* module) { return module->get_active(); };

namespace views = std::views;

///////////////////////////////////////////////////////////////////////////////
const char* Manager::enum_device_type_string(const DeviceType t)
{
  LMark;
	switch(t) {
		case DeviceType::HUB      : return "Hub";
		case DeviceType::NODE     : return "Node";
		case DeviceType::REPEATER : return "Repeater";
		default : return "";
	}
}

///////////////////////////////////////////////////////////////////////////////
Manager::Manager(
		const char*				device_name,
		const uint8_t			instance,
		const DeviceType		device_type,
		const Verbosity			print_verbosity,
		const Verbosity			package_verbosity,
		const uint16_t			interval
	)
	: instance(instance)
	, print_verbosity(print_verbosity)
	, package_verbosity(package_verbosity)
	, device_type(device_type)
	, interval(interval)
{
  LMark;
	snprintf(this->device_name, 20, "%s", device_name);
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
Manager::~Manager()
{
  LMark;
	free_modules();
}

///////////////////////////////////////////////////////////////////////////////
void Manager::print_config(const bool print_modules_config)
{
  LMark;
	print_device_label();
  LMark;
	LPrintln("Config:");
  LMark;
	LPrintln("\tDevice Name         : ", device_name );
  LMark;
	LPrintln("\tInstance Number     : ", instance );
  LMark;
	LPrintln("\tDevice Type         : ", enum_device_type_string(device_type) );
  LMark;
	LPrintln("\tInterval            : ", interval );
  LMark;

	list_modules();
  LMark;

	// Print managed module's configs
	if (print_modules_config) {
    LMark;
    for (auto module : modules | views::filter(module_exists)) {
      LMark;
			module->print_config();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Manager::begin_serial(const bool wait_for_monitor, const bool begin_fault)
{
  LMark;
	Serial.begin(SERIAL_BAUD);
  LMark;

	if (wait_for_monitor) {
    LMark;
		unsigned long start = millis();
    LMark;
		while( !Serial && ((millis() - start) < MAX_SERIAL_WAIT) );
    LMark;
		delay(1000);
    LMark;
	}

	if(FeatherFault::DidFault()){
    LMark;
		pinMode(13, OUTPUT);
    LMark;
    flash_LED(3, 1000, 1000, false);
		FeatherFault::PrintFault(Serial);
    LMark;
		delay(3000);
    LMark;
	}
	if(begin_fault) start_fault();
  LMark;

	LPrintln("Initialized Serial!\n");
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Manager::add_module(Module* module)
{
  LMark;

	print_device_label();
  LMark;

	if (module == nullptr) {
    LMark;
		LPrintln("Cannot add null module");
    LMark;
		return;
    LMark;
	} else if (!module->get_active()) {
		LPrintln("Cannot add inactive module");
    LMark;
		delete module;
    LMark;
		return;
	}

	LPrintln("Adding Module: ", module->get_module_name() );
  LMark;

	modules.emplace_back(module);
  LMark;
	module->link_device_manager(this);
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Manager::list_modules() const
{
  LMark;
	print_device_label();
  LMark;
	LPrintln("Modules:");
  LMark;

  for (auto module : modules | views::filter(module_exists) ) {
    LMark;
		LPrintln( "\t\t[", module->get_active() ? "+" : "-" , "] ", module->get_module_name() );
	}
}

///////////////////////////////////////////////////////////////////////////////
void Manager::set_device_name(const char* device_name)
{
  LMark;
	snprintf(this->device_name, 20, "%s", device_name);
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Manager::get_device_name(char* buf)
{
  LMark;
	if(temp_device_name[0] == '\0') sprintf(buf, "%s", device_name);
	else {
    LMark;
		sprintf(buf, "%s", temp_device_name);
    LMark;
		strcpy(temp_device_name, "");
    LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
char* Manager::get_device_name()
{
  LMark;
	char name[20];
  LMark;
	get_device_name(name);
  LMark;
	return (char*)name;
}

///////////////////////////////////////////////////////////////////////////////
void Manager::set_print_verbosity(const Verbosity v, const bool set_modules)
{
  LMark;
	print_verbosity = v;
  LMark;

	if (set_modules) {
    LMark;
    for (auto module : modules | views::filter(module_exists) | views::filter(module_active)) {
      LMark;
      module->set_print_verbosity(v);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Manager::set_package_verbosity(const Verbosity v, const bool set_modules)
{
  LMark;
	package_verbosity = v;
  LMark;

	if (set_modules) {
    LMark;
    for (auto module : modules | views::filter(module_exists) | views::filter(module_active)) {
      LMark;
      module->set_package_verbosity(v);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Manager::measure()
{
  for (auto module : modules | views::filter(module_exists) | views::filter(module_active)) {
		// Not within LOOM_INCLUDE_SENSORS as Analog and Digital are always enabled
    LMark;
    if (dynamic_cast<Loom::Sensor*>(module)) {
      LMark;
			((Sensor*)module)->measure();
		}

#ifdef LOOM_INCLUDE_SENSORS
		else if (dynamic_cast<Loom::Multiplexer*>(module) ) {
      LMark;
			((Multiplexer*)module)->measure();
		}
		// else if (dynamic_cast<Loom::TempSync*>(module)) {
		// 	((TempSync*)module)->measure();
		// }
#endif // ifdef LOOM_INCLUDE_SENSORS

#if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
		else if (dynamic_cast<Loom::NTPSync*>(module)) {
      LMark;
			((NTPSync*)module)->measure();
		}
#endif // if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
	}
}

///////////////////////////////////////////////////////////////////////////////
void Manager::package(JsonObject json)
{
  LMark;
	// Add device identification to json
	add_device_ID_to_json(json);
  LMark;

	// Add a packet number to json
	add_data("Packet", "Number", packet_number++);
  LMark;

  for (auto module : modules | views::filter(module_exists) | views::filter(module_active)) {
    LMark;
    module->package(json);
	}
}

///////////////////////////////////////////////////////////////////////////////
JsonObject Manager::package()
{
  LMark;
	doc.clear();
  LMark;
	doc["type"] = "data";
  LMark;
	JsonObject json = doc.as<JsonObject>();
  LMark;
	package(json);
  LMark;
	// print an error if the JSON is full
	if (doc.memoryUsage() > doc.capacity() - 10)
		LPrintln("Warning: JSON document overflowed! Loom does not have enough memory for the number of sensors you are reading, please use less sensors or submit an issue.");
  LMark;

	return json;
}

///////////////////////////////////////////////////////////////////////////////
void Manager::add_device_ID_to_json(JsonObject json)
{
  LMark;
	JsonObject timestamp = json.createNestedObject("id");
  LMark;
	timestamp["name"]		= device_name;
  LMark;
	timestamp["instance"]	= instance;
}

///////////////////////////////////////////////////////////////////////////////
JsonObject Manager::internal_json(const bool clear)
{
  LMark;
	if (clear) {
    LMark;
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
#if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))

bool Manager::publish_all(const JsonObject json)
{
  LMark;
	auto is_publish_plat = [](Module *module) { return dynamic_cast<Loom::PublishPlat*>(module) != nullptr; };

  LMark;
	bool result = true;
	uint8_t count = 0;
  LMark;
	for (auto module : modules | views::filter(module_exists) | views::filter(module_active) | views::filter(is_publish_plat) ) {
    LMark;
    result &= ((PublishPlat*)module)->publish( json );
		count++;
	}
	return (count > 0) && result;
}

#endif // if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))


///////////////////////////////////////////////////////////////////////////////
bool Manager::log_all(const JsonObject json)
{
  LMark;
	auto is_log_plat = [](Module *module) { return dynamic_cast<Loom::LogPlat*>(module) != nullptr; };

	bool result = true;
	uint8_t count = 0;
  LMark;
	for (auto module : modules | views::filter(module_exists) | views::filter(module_active) | views::filter(is_log_plat)) {
    LMark;
    result &= ((LogPlat*)module)->log( json );
		count++;
	}
	return (count > 0) && result;
}

///////////////////////////////////////////////////////////////////////////////
void Manager::dispatch(JsonObject json)
{
  LMark;
	print_device_label();
  LMark;
	LPrintln("Processing command");
  LMark;
	serializeJsonPretty(json, Serial);
  LMark;
	LPrintln();
  LMark;
	// return;

	// If is command
	if ( strcmp(json["type"], "command") == 0 )	{
    LMark;

		// For each command
		for ( JsonObject cmd : json["commands"].as<JsonArray>() ) {
      LMark;

			// Check if command is for manager
			if ( strcmp(cmd["module"].as<const char*>(), "Manager" ) == 0) {
        LMark;
				if (dispatch_self(cmd)) break;
			}

			// Otherwise iterate over modules until module to handle command is found
			// LPrintln("Try to dispatch to: ", cmd["module"].as<const char*>() );
      for (auto module : modules | views::filter(module_exists) | views::filter(module_active)) {
				if ( strcmp(cmd["module"].as<const char*>(), module->get_module_name() ) == 0 ) {
					// LPrintln("Found module");
					if ( module->dispatch( cmd ) ) break; // move to next command
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Manager::dispatch_self(JsonObject json)
{
  LMark;
	JsonArray params = json["params"];
  LMark;
	switch( (char)json["func"] ) {
    LMark;
		case 'i': if (params.size() >= 1) { set_interval( EXPAND_ARRAY(params, 1) ); } return true;
		case 'j': if (params.size() >= 1) { parse_config_SD( EXPAND_ARRAY(params, 1) ); } return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
void Manager::display_data() const
{
  LMark;
	print_device_label();
  LMark;
	LPrintln("Json:");
  LMark;
	serializeJsonPretty(doc, Serial);
  LMark;
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////
void Manager::flash_LED(const uint8_t count, const uint8_t time_high, const uint8_t time_low, const bool end_high) const
{
  LMark;
	for (int i = 0; i < count; i++) {
    LMark;
		digitalWrite(LED_BUILTIN, HIGH);
    LMark;
		delay(time_high);
    LMark;
		digitalWrite(LED_BUILTIN, LOW);
    LMark;
		delay(time_low);
    LMark;
	}
	if (end_high) {
    LMark;
		digitalWrite(LED_BUILTIN, HIGH);
    LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Manager::free_modules()
{
  LMark;
	for (auto module : modules) {
    LMark;
		delete module;
	}
	modules.clear();
  LMark;

	rtc_module = nullptr;
  LMark;
	interrupt_manager = nullptr;
  LMark;
	sleep_manager = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
void Manager::power_up()
{
  LMark;
	// Iterate over list of modules powering them on
	for (auto module : modules | views::filter(module_exists)) {
    LMark;
    module->power_up();
	}
}

///////////////////////////////////////////////////////////////////////////////
void Manager::power_down()
{
  LMark;
	// Iterate over list of modules powering them off
  for (auto module : modules | views::filter(module_exists)) {
    LMark;
    module->power_down();
  }
}

///////////////////////////////////////////////////////////////////////////////
void Manager::get_config()
{
  LMark;
	doc.clear();
  LMark;
	doc["type"] = "config";
  LMark;
	JsonObject json = doc.as<JsonObject>();
  LMark;

	// Add general device identification
	JsonObject general_info 	= json.createNestedObject("general");
  LMark;
	general_info["name"]		= device_name;
  LMark;
	general_info["instance"]	= instance;
  LMark;

	// Start array for modules to add config objects to
	JsonArray components = json.createNestedArray("components");
  LMark;

  for (auto module : modules | views::filter(module_exists)) {
    LMark;
    module->add_config(json);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Module*	Manager::find_module(const Module::Type type, const uint8_t idx) const
// {
//   LMark;
// 	uint8_t current = 0;
//   LMark;
//
// 	for (auto module : modules) {
//     LMark;
// 		if (type == module->get_module_type()) {
//       LMark;
// 			if (current == idx) {
//         LMark;
// 				return (Module*)module;
// 			} else {
// 				current++;
//         LMark;
// 			}
// 		}
// 	}
// 	return nullptr;
// }
//
// ///////////////////////////////////////////////////////////////////////////////
// Module*	Manager::find_module_by_category(const Module::Category category, const uint8_t idx) const
// {
//   LMark;
// 	uint8_t current = 0;
//   LMark;
//
// 	for (auto module : modules) {
//     LMark;
// 		if (category == module->category()) {
//       LMark;
// 			if (current == idx) {
//         LMark;
// 				return (Module*)module;
// 			} else {
// 				current++;
//         LMark;
// 			}
// 		}
// 	}
// 	return nullptr;
// }

///////////////////////////////////////////////////////////////////////////////
void Manager::set_interval(const uint16_t ms)
{
  LMark;
	interval = ms;
  LMark;
	print_device_label();
  LMark;
	LPrintln("Set interval to: ", interval);
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
// bool Manager::has_module(const Module::Type type) const
// {
//   LMark;
// 	for (auto module : modules) {
//     LMark;
// 		if (module->get_module_type() == type) return true;
// 	}
// 	return false;
// }

///////////////////////////////////////////////////////////////////////////////
bool Manager::parse_config(const char* json_config)
{
  LMark;
	// Might need to be even larger
	DynamicJsonDocument doc(2048);
  LMark;
	DeserializationError error = deserializeJson(doc, json_config);
  LMark;

	// Test if parsing succeeds.
	if (error) {
    LMark;
		print_device_label();
    LMark;
		LPrintln("deserializeJson() failed: ", error.c_str());
    LMark;
		return false;
	}

	bool status = parse_config_json( doc.as<JsonObject>() );
  LMark;
	doc.clear();
  LMark;
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool Manager::parse_config_SD(const char* config_file)
{
  LMark;
	SdFat sd;	// File system object
  LMark;

	print_device_label();
  LMark;
	LPrintln("Read config from file: '", config_file, "'");
  LMark;

	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI
  LMark;
	delay(25);
  LMark;
	// if (!SD.begin(SD_CS)) {	// Make sure we can communicate with SD
	if ( !sd.begin(SD_CS, SD_SCK_MHZ(50)) ) {	// Make sure we can communicate with SD
    LMark;
		print_device_label();
    LMark;
		LPrintln("SD failed to begin");
    LMark;
		return false;
	}

	File file = sd.open(config_file);
  LMark;
	if (!file) {	// Make sure file exists
    LMark;
		print_device_label();
    LMark;
		LPrintln("Failed to open '", config_file, "'");
    LMark;
		return false;
	}

	DynamicJsonDocument doc(2048);
  LMark;
	DeserializationError error = deserializeJson(doc, file);
  LMark;

	// Test if parsing succeeds.
	if (error) { // Make sure json was valid
    LMark;
		print_device_label();
    LMark;
		LPrintln("deserializeJson() failed: ", error.c_str());
    LMark;
		return false;
	}

	bool status = parse_config_json( doc.as<JsonObject>() );
  LMark;
	doc.clear();
  LMark;
	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool Manager::parse_config_json(JsonObject config)
{
  LMark;
	// Remove current modules
	free_modules();
  LMark;

	if (print_verbosity == Verbosity::V_HIGH) {
    LMark;

		LPrintln("\n= = = = = Parse Config = = = = =");
    LMark;
		LPrintln("\nConfig Pretty Version:");
    LMark;
		serializeJsonPretty(config, Serial);
    LMark;
		LPrintln("\nSIZE: ", config.memoryUsage());
    LMark;
	}

	// Apply Manager General Settings
	JsonObject general = config["general"];
  LMark;

	if (general.containsKey("name")) {
    LMark;
		snprintf(this->device_name, 20, "%s", general["name"].as<const char*>());
    LMark;
	}
	if (general.containsKey("instance")) {
    LMark;
		const JsonVariantConst inst = general["instance"];
    LMark;
		if (inst.is<int>())
			this->instance = general["instance"];
		else if (inst.is<const char*>())
      LMark;
			this->instance = atoi(inst.as<const char*>());
      LMark;
	}
	if (general.containsKey("interval")) {
    LMark;
		this->interval = general["interval"];
    LMark;
	}
	if (general.containsKey("device_type")) {
    LMark;
		this->device_type = (DeviceType)(int)general["device_type"];
    LMark;
	}
	if (general.containsKey("print_verbosity")) {
    LMark;
		this->print_verbosity = (Verbosity)(int)general["print_verbosity"];
    LMark;
	}
	if (general.containsKey("package_verbosity")) {
    LMark;
		this->package_verbosity = (Verbosity)(int)general["package_verbosity"];
    LMark;
	}

	// Generate Module Objects
	if (print_verbosity == Verbosity::V_HIGH) {
    LMark;
		LPrintln("= = = = = Generate Objects = = = = =\n");
	}

	// Call module factory creating each module
  for ( JsonVariant module : config["components"].as<JsonArray>()) {
    LMark;
    add_module(Registry<Module>::create(module));
	}
	config_count = config["components"].as<JsonArray>().size();
  LMark;

	// Sort modules by type
	// std::sort(modules.begin(), modules.end(), module_sort_comp());

	// Run second stage constructors
  for (auto module : modules) {
		if ( module != nullptr ) {
			module->second_stage_ctor();
		}
	}

	if (print_verbosity == Verbosity::V_HIGH) {
    LMark;
		LPrintln("= = = = = = = = = = = = = = = = =");
    LMark;
		LPrintln();
    LMark;
	}

  package_fault();

	return true;
}

///////////////////////////////////////////////////////////////////////////////
void Manager::package_fault(){
  if(FeatherFault::DidFault()) {
    auto is_log_plat = [](Module *module) { return dynamic_cast<Loom::LogPlat*>(module) != nullptr; };
    LMark;
    doc.clear();
    LMark;
    doc["type"] = "data";
    LMark;
    JsonObject json = doc.as<JsonObject>();
    LMark;

    add_device_ID_to_json(json);
    LMark;

    strcpy(temp_device_name, "Errors");
    LMark;

    LPrintln("Packaging Fault");
    LMark;
    FeatherFault::FaultData error = FeatherFault::GetFault();
    LMark;
    JsonObject data = get_module_data_object(json, "Error");
    LMark;
    if(error.cause == 1) data["Problem"] = "Hanging";
    else if(error.cause == 2) data["Problem"] = "Hard_Fault";
    else if(error.cause == 3) data["Problem"] = "Out_of_Memory";
    else data["Problem"] = "Unknown_Trigger";
    LMark;
    data["Corrupted"] = (error.is_corrupted == 1 ? true : false);
    LMark;
    data["Failures"] = error.failnum;
    LMark;
    data["File_Source"] = error.file;
    LMark;
    data["Line_Number"] = error.line;
    LMark;
    serializeJsonPretty(json, Serial);
    LMark;
    #if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
    publish_all(json);
    #endif // if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
    LMark;
    for (auto module : modules | views::filter(module_exists) | views::filter(module_active) | views::filter(is_log_plat)) {
      LMark;
      if(dynamic_cast<Loom::SD*>(module)){
        LMark;
        File file;
        LMark;
        file.open("Errors.csv", O_WRONLY | O_CREAT | O_EXCL);
        LMark;
        ((SD*)module)->save_json( json, "Errors.csv" );
        LMark;
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
bool Manager::parse_config_serial()
{
  LMark;
	flash_LED(4, 200, 100, true);
  LMark;

	LPrintln("Waiting for config over Serial");
  LMark;

	while(!Serial.available());
  LMark;



	// Might need to be even larger
	DynamicJsonDocument doc(2048);
  LMark;
	DeserializationError error = deserializeJson(doc, Serial);
  LMark;

	// Test if parsing succeeds.
	if (error) {
    LMark;
		print_device_label();
    LMark;
		LPrintln("deserializeJson() failed: ", error.c_str());
    LMark;
		return false;
	}

	bool status = parse_config_json( doc.as<JsonObject>() );
  LMark;
	doc.clear();
  LMark;

	flash_LED(12, 50, 25, false);
  LMark;
	// flash_LED(12, 50, 25, true);
	// delay(3000);
	// digitalWrite(LED_BUILTIN, LOW);
	// LPrintln("Received config");

	return status;
}

///////////////////////////////////////////////////////////////////////////////
bool Manager::check_serial_for_config()
{
  LMark;
	if (Serial.available()) {
    LMark;
		// return parse_config_serial();

		DynamicJsonDocument doc(2048);
    LMark;
		DeserializationError error = deserializeJson(doc, Serial);
    LMark;

		// Test if parsing succeeds.
		if (error) {
      LMark;
			print_device_label();
      LMark;
			LPrintln("deserializeJson() failed: ", error.c_str());
      LMark;
			return false;
		}

		bool status = parse_config_json( doc.as<JsonObject>() );
    LMark;
		doc.clear();
    LMark;

		flash_LED(12, 50, 25, false);
    LMark;
		return status;
	}
	return false;
}
