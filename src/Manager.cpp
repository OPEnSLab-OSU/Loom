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
  LMark;
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
  LMark;
	snprintf(this->device_name, 20, "%s", device_name);
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
LoomManager::~LoomManager()
{
  LMark;
	free_modules();
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::print_config(const bool print_modules_config)
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
		for (auto module : modules) {
      LMark;
			module->print_config();
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::begin_serial(const bool wait_for_monitor, const bool begin_fault)
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
		for( int i = 0; i < 3; i++){
      LMark;
			digitalWrite(13, HIGH);
      LMark;
			delay(250);
      LMark;
			digitalWrite(13, LOW);
      LMark;
			delay(250);
      LMark;
		}
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
void LoomManager::add_module(LoomModule* module)
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

	LPrintln("Adding Module: ", ((LoomModule*)module)->get_module_name() );
  LMark;

	modules.emplace_back(module);
  LMark;
	module->link_device_manager(this);
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::list_modules() const
{
  LMark;
	print_device_label();
  LMark;
	LPrintln("Modules:");
  LMark;

	auto last_category = LoomModule::Category::Unknown;
  LMark;

	for (auto module : modules) {
    LMark;
		auto category = module->category();
    LMark;
		if ( category != last_category ) {
      LMark;
			LPrintln("\t", LoomModule::enum_category_string(category), "s");//, " (", modules.size(), "):");
      LMark;
			last_category = category;
      LMark;
		}
		if ( module != nullptr ) {
      LMark;
			LPrintln( "\t\t[", module->get_active() ? "+" : "-" , "] ", module->get_module_name() );
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_device_name(const char* device_name)
{
  LMark;
	snprintf(this->device_name, 20, "%s", device_name);
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::get_device_name(char* buf)
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
char* LoomManager::get_device_name()
{
  LMark;
	char name[20];
  LMark;
	get_device_name(name);
  LMark;
	return (char*)name;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_print_verbosity(const Verbosity v, const bool set_modules)
{
  LMark;
	print_verbosity = v;
  LMark;

	if (set_modules) {
    LMark;
		for (auto module : modules) {
      LMark;
			if ( (module != nullptr) && ( ((LoomModule*)module)->get_active() ) ){
        LMark;
				((LoomModule*)module)->set_print_verbosity(v);
        LMark;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_package_verbosity(const Verbosity v, const bool set_modules)
{
  LMark;
	package_verbosity = v;
  LMark;

	if (set_modules) {
    LMark;
		for (auto module : modules) {
      LMark;
			if ( (module != nullptr) && ( ((LoomModule*)module)->get_active() ) ){
        LMark;
				((LoomModule*)module)->set_package_verbosity(v);
        LMark;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::measure()
{
	for (auto module : modules) {
    LMark;
		if ( !module->get_active() ) continue;
    LMark;

		if ( module->category() == LoomModule::Category::Sensor ) {
      LMark;
			((LoomSensor*)module)->measure();
      LMark;
		}
		else if (
      LMark;
			(module->get_module_type() == LoomModule::Type::Multiplexer) ) {
			((Loom_Multiplexer*)module)->measure();
      LMark;
		}
		else if (module->get_module_type() == LoomModule::Type::NTP) {
      LMark;
			((LoomNTPSync*)module)->measure();
      LMark;
		}
		else if (module->get_module_type() == LoomModule::Type::TempSync) {
      LMark;
			((LoomTempSync*)module)->measure();
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::package(JsonObject json)
{
  LMark;
	// Add device identification to json
	add_device_ID_to_json(json);
  LMark;

	// Add a packet number to json
	add_data("Packet", "Number", packet_number++);
  LMark;

	for (auto module : modules) {
    LMark;
		if ( (module != nullptr) && ( ((LoomModule*)module)->get_active() ) ){
      LMark;
			((LoomModule*)module)->package( json );
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
JsonObject LoomManager::package()
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
void LoomManager::diagnose(JsonObject json){
  LMark;
	SdFat sd;
  LMark;
	int countInternet = 0, countSensor = 0;
  LMark;
	bool result;
  LMark;
	JsonObject data;
  LMark;
	// Add device identification to json
	add_device_ID_to_json(json);
  LMark;

	for (auto module : modules) { //Restructure for verbosity ?
    LMark;
		result = false;
    LMark;
		if ( (module != nullptr) && ( ((LoomModule*)module)->get_active() ) ){
      LMark;
			module->diagnose(result);
      LMark;
			if( (module->get_module_type() == LoomModule::Type::InternetPlat) && !result)
				countInternet++; //Check internet platforms are connected
			else if( (module->get_module_type() == LoomModule::Type::Sensor) && !result)
				countSensor++; // Check sensor values that they are in range
			else if ( (module->get_module_type() == LoomModule::Type::Multiplexer) && !result)
				countSensor++; // Check sensor values that they are in range
      LMark;
		}
	}
	data = get_module_data_object(json, "Diagnostics");
  LMark;
	data["All Modules Created"] = (modules.size() == config_count ? true : false);
  LMark;
	data["Sensors Operating"] = (countSensor == 0 ? true : false);
  LMark;
	data["Internet Connected"] = (countInternet == 0 ? true : false);
  LMark;
	data["SD Found"] = (!sd.begin(SD_CS, SD_SCK_MHZ(50))); // Make sure we can communicate with SD
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
JsonObject LoomManager::diagnose(){
  LMark;
	doc.clear();
  LMark;
	doc["type"] = "data"; // Create a new type for easier parsing by Spool?
  LMark;
	JsonObject json = doc.as<JsonObject>();
  LMark;
	diagnose(json);
  LMark;

	if(doc.memoryUsage() > doc.capacity() - 10)
		LPrintln("Warning: JSON document overflowed! Loom does not have enough memory for the number of sensors you are reading, please use less sensors or submit an issue.");
  LMark;

	return json;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::add_device_ID_to_json(JsonObject json)
{
  LMark;
	JsonObject timestamp = json.createNestedObject("id");
  LMark;
	timestamp["name"]		= device_name;
  LMark;
	timestamp["instance"]	= instance;
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
JsonObject LoomManager::internal_json(const bool clear)
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
bool LoomManager::publish_all(const JsonObject json)
{
  LMark;
	bool result = true;
  LMark;
	uint8_t count = 0;
  LMark;
	for (auto module : modules) {
    LMark;
		if ( (module != nullptr) &&
			 (module->category() == LoomModule::Category::PublishPlat) &&
			 (module->get_active())
			) {
      LMark;
			result &= ((LoomPublishPlat*)module)->publish( json );
      LMark;
			count++;
      LMark;
		}
	}
	return (count > 0) && result;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomManager::log_all(const JsonObject json)
{
  LMark;
	bool result = true;
  LMark;
	uint8_t count = 0;
  LMark;
	for(auto module : modules) {
		if( (module!=nullptr) &&
		(module->category() == LoomModule::Category::LogPlat) &&
		(module->get_active())
		){
      LMark;
			result &= ((LoomLogPlat*)module)->log( json );
      LMark;
			count++;
      LMark;
		}
	}
	return (count > 0) && result;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::dispatch(JsonObject json)
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
			for (auto module : modules) {
        LMark;
				if ( (module != nullptr) &&
					 ( ((LoomModule*)module)->get_active() ) &&
					 ( strcmp(cmd["module"].as<const char*>(), module->get_module_name() ) == 0 )
					){
          LMark;
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
void LoomManager::display_data() const
{
  LMark;
	print_device_label();
  LMark;
	LPrintln("Json:");
  LMark;
	serializeJsonPretty(doc, Serial);
  LMark;
	LPrintln();
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::flash_LED(const uint8_t count, const uint8_t time_high, const uint8_t time_low, const bool end_high) const
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
void LoomManager::free_modules()
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
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::power_up()
{
  LMark;
	// Iterate over list of modules powering them on
	for (auto module : modules) {
    LMark;
		if ( module != nullptr ){
      LMark;
			((LoomModule*)module)->power_up();
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::power_down()
{
  LMark;
	// Iterate over list of modules powering them off
	for (auto module : modules) {
    LMark;
		if ( module != nullptr ){
      LMark;
			((LoomModule*)module)->power_down();
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::get_config()
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

	for (auto module : modules) {
    LMark;
		if ( module != nullptr ){
      LMark;
			((LoomModule*)module)->add_config(json);
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
LoomModule*	LoomManager::find_module(const LoomModule::Type type, const uint8_t idx) const
{
  LMark;
	uint8_t current = 0;
  LMark;

	for (auto module : modules) {
    LMark;
		if (type == module->get_module_type()) {
      LMark;
			if (current == idx) {
        LMark;
				return (LoomModule*)module;
			} else {
				current++;
        LMark;
			}
		}
	}
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
LoomModule*	LoomManager::find_module_by_category(const LoomModule::Category category, const uint8_t idx) const
{
  LMark;
	uint8_t current = 0;
  LMark;

	for (auto module : modules) {
    LMark;
		if (category == module->category()) {
      LMark;
			if (current == idx) {
        LMark;
				return (LoomModule*)module;
			} else {
				current++;
        LMark;
			}
		}
	}
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_interval(const uint16_t ms)
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
bool LoomManager::has_module(const LoomModule::Type type) const
{
  LMark;
	for (auto module : modules) {
    LMark;
		if (module->get_module_type() == type) return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomManager::parse_config(const char* json_config)
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
bool LoomManager::parse_config_SD(const char* config_file)
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
bool LoomManager::parse_config_json(JsonObject config)
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

	// Apply LoomManager General Settings
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
    LMark;
	}

	// Call module factory creating each module
	for ( JsonVariant module : config["components"].as<JsonArray>()) {
    LMark;
		if (Factory) {
      LMark;
			add_module(Factory->Create(this, module));
      LMark;
		}
	}
	config_count = config["components"].as<JsonArray>().size();
  LMark;

	// Sort modules by type
	std::sort(modules.begin(), modules.end(), module_sort_comp());
  LMark;

	// Run second stage constructors
	for (auto module : modules) {
    LMark;
		if ( module != nullptr ) {
      LMark;
			module->second_stage_ctor();
      LMark;
		}
	}

	if (print_verbosity == Verbosity::V_HIGH) {
    LMark;
		LPrintln("= = = = = = = = = = = = = = = = =");
    LMark;
		LPrintln();
    LMark;
	}

	if(FeatherFault::DidFault()) {
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
		publish_all(json);
    LMark;
		for(auto module : modules) {
      LMark;
			if( (module!=nullptr) &&
			(module->get_module_type() == LoomModule::Type::SDCARD) &&
			(module->get_active())
			){
        LMark;
				File file;
        LMark;
				file.open("Errors.csv", O_WRONLY | O_CREAT | O_EXCL);
        LMark;
				((Loom_SD*)module)->save_json( json, "Errors.csv" );
        LMark;
			}
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////






///////////////////////////////////////////////////////////////////////////////
bool LoomManager::parse_config_serial()
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
bool LoomManager::check_serial_for_config()
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
