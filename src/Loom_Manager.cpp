
#include "Loom_Module.h"
#include "Loom_Manager.h"

#include "Loom_Macros.h"

#include "Loom_Interrupt_Manager.h"
#include "Loom_Sleep_Manager.h"

#include "Sensors/Loom_Sensor.h"
#include "Actuators/Loom_Actuator.h"
#include "CommPlats/Loom_CommPlat.h"
#include "InternetPlats/Loom_InternetPlat.h"
#include "LogPlats/Loom_LogPlat.h"
#include "RTC/Loom_RTC.h"




/////////////////////////////////////////////////////////////////////
const char* LoomManager::enum_device_type_string(DeviceType t)
{
	switch(t) {
		case DeviceType::HUB      : return "Hub";
		case DeviceType::NODE     : return "Node";
		case DeviceType::REPEATER : return "Repeater";
		default : return "";
	}
}

/////////////////////////////////////////////////////////////////////

// --- CONSTRUCTOR ---
LoomManager::LoomManager( 
		const char*		device_name, 
		const char*		family, 
		uint			family_num,
		uint			instance, 
		DeviceType		device_type, 
		Verbosity		print_verbosity, 
		Verbosity		package_verbosity
	)
{
	snprintf(this->device_name, 20, "%s", device_name);
	snprintf(this->family, 20, "%s", family);

	this->family_num = family_num;
	this->instance = instance;

	this->device_type = device_type;

	// Set verbosity
	this->print_verbosity 	= print_verbosity;
	this->package_verbosity	= package_verbosity;
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
LoomManager::~LoomManager() 
{
	// Impelement this
}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void LoomManager::print_device_label()
{
	LPrint("[", device_name, "] ");
}

/////////////////////////////////////////////////////////////////////
void LoomManager::print_config() 
{
	print_device_label();
	LPrintln("Config:");
	LPrintln('\t', "Device Name         : ", device_name );
	LPrintln('\t', "Family              : ", family );
	LPrintln('\t', "Family Number       : ", family_num );
	LPrintln('\t', "Instance Number     : ", instance );
	LPrintln('\t', "Device Type         : ", enum_device_type_string(device_type) );
	LPrintln('\t', "Instance Number     : ", instance );

	list_modules();
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module_aux(LoomModule** modules, LoomModule* module, uint& len, const int max_len)
{
	print_device_label();

	if (module == nullptr) {
		LPrintln("Cannot add null module");
		return;
	}

	// If module array is not dynamic, add check to make sure there is room in the array
	if ( len >= max_len ) {
		LPrintln("Cannot add ", module->get_module_name() );
		return;
	}

	LPrintln("Adding Module: ", module->get_module_name() );

	modules[ len++ ] = module;
	module->link_device_manager(this);
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module(Loom_Interrupt_Manager* interrupt_manager) 
{
	print_device_label();
	if (!interrupt_manager) {
		LPrintln("Cannot add ", interrupt_manager->get_module_name() );
		return;
	}
	LPrintln("Adding Module: ", interrupt_manager->get_module_name() );
	this->interrupt_manager = interrupt_manager; 

	interrupt_manager->link_device_manager(this);

	// Point new interrupt manager to existing RTC
	if (rtc_module != nullptr) {
		this->interrupt_manager->set_RTC_module(rtc_module);
	}

	// Point existing sleep manager to new interrupt manager
	if (sleep_manager != nullptr) {
		interrupt_manager->link_sleep_manager(sleep_manager);
		sleep_manager->link_interrupt_manager(interrupt_manager);
	}
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module(Loom_Sleep_Manager* sleep_manager) 
{
	print_device_label();
	if (!sleep_manager) {
		LPrintln("Cannot add ", sleep_manager->get_module_name() );
		return;
	}
	LPrintln("Adding Module: ", sleep_manager->get_module_name() );
	this->sleep_manager = sleep_manager; 

	sleep_manager->link_device_manager(this);

	// Point existing sleep manager to new interrupt manager
	if (interrupt_manager != nullptr) {
		sleep_manager->link_interrupt_manager(interrupt_manager);
		interrupt_manager->link_sleep_manager(sleep_manager);
	}
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomRTC* rtc) 
{
	rtc_module = rtc;
	
	if (rtc != nullptr) {
		if (interrupt_manager != nullptr) {
			interrupt_manager->set_RTC_module(rtc);
		}
	} else {
		LPrintln("Cannot add null module");
		return;
	}
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomModule* module) 
{
	add_module_aux(other_modules, module, other_module_count, MAX_OTHER_MODULES);
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomSensor* sensor) 
{
	add_module_aux( (LoomModule**)sensor_modules, (LoomModule*)sensor, sensor_count, MAX_SENSORS );
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomActuator* actuator) 
{
	add_module_aux( (LoomModule**)actuator_modules, (LoomModule*)actuator, actuator_count, MAX_ACTUATORS );
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomCommPlat* comm_plat) 
{
	add_module_aux( (LoomModule**)comm_modules, (LoomModule*)comm_plat, comm_count, MAX_COMMS );
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomInternetPlat* internet_module) 
{
	add_module_aux( (LoomModule**)internet_modules, (LoomModule*)internet_module, internet_count, MAX_INTERNETS );
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomLogPlat* log_plat) 
{	
	add_module_aux( (LoomModule**)log_modules, (LoomModule*)log_plat, log_count, MAX_LOGS );
}

/////////////////////////////////////////////////////////////////////
Loom_Interrupt_Manager* LoomManager::get_interrupt_manager()
{
	return interrupt_manager;
}

/////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager* LoomManager::get_sleep_manager()
{
	return sleep_manager;
}

/////////////////////////////////////////////////////////////////////
LoomRTC* LoomManager::get_rtc_module()
{
	return rtc_module;
}

/////////////////////////////////////////////////////////////////////
void LoomManager::list_modules_aux(LoomModule** modules, uint len, char* module_type)
{
	LPrintln("\t", module_type, " (", len, "):");
	for (int i = 0; i < len; i++) {
		if ( (modules[i] != nullptr) && (modules[i]->get_active()) ) {
			LPrintln( "\t\t[", (modules[i]->get_active()) ? "+" : "-" , "] ", modules[i]->get_module_name() );
		}
	}	
}

/////////////////////////////////////////////////////////////////////
void LoomManager::list_modules()
{
	print_device_label();
	LPrintln("Modules:");

	// Interrupt Manager
	LPrintln("\t", "Interrupt Manager", " (", (interrupt_manager != nullptr), "):");
	if ( (interrupt_manager != nullptr) && (interrupt_manager->get_active()) ) {
		LPrintln( "\t\t[", (interrupt_manager->get_active()) ? "+" : "-" , "] ", interrupt_manager->get_module_name() );
	}

	// Sleep Manager
	LPrintln("\t", "Sleep Manager", " (", (sleep_manager != nullptr), "):");
	if ( (sleep_manager != nullptr) && (sleep_manager->get_active()) ) {
		LPrintln( "\t\t[", (sleep_manager->get_active()) ? "+" : "-" , "] ", sleep_manager->get_module_name() );
	}

	// RTC 
	LPrintln("\t", "RTC module", " (", (rtc_module != nullptr), "):");
	if ( (rtc_module != nullptr) && (rtc_module->get_active()) ) {
		LPrintln( "\t\t[", (rtc_module->get_active()) ? "+" : "-" , "] ", rtc_module->get_module_name() );
	}

	// list_modules_aux( (LoomModule**)rtc_module      , 1   		       , "RTC Modules"); 

	list_modules_aux( (LoomModule**)other_modules    , other_module_count , "Other Modules" ); 
	list_modules_aux( (LoomModule**)sensor_modules   , sensor_count       , "Sensors"); 
	list_modules_aux( (LoomModule**)actuator_modules , actuator_count     , "Actuators"); 
	list_modules_aux( (LoomModule**)comm_modules     , comm_count         , "Communication Platforms"); 
	list_modules_aux( (LoomModule**)internet_modules , internet_count     , "Internet Platforms"); 
	list_modules_aux( (LoomModule**)log_modules      , log_count          , "Logging Platforms" ); 
}

/////////////////////////////////////////////////////////////////////
void LoomManager::set_device_name(char* device_name)
{
	// this->device_name = device_name;
	// strcpy(this->device_name, device_name);
	snprintf(this->device_name, 20, "%s", device_name);
}

/////////////////////////////////////////////////////////////////////
void LoomManager::get_device_name(char* buf)
{
	sprintf(buf, "%s", device_name); 
}

/////////////////////////////////////////////////////////////////////
const char* LoomManager::get_device_name()
{
	char name[50];
	get_device_name(name);
	return (const char*)name;
}

/////////////////////////////////////////////////////////////////////
const char* LoomManager::get_family() 
{ 
	return family; 
}

/////////////////////////////////////////////////////////////////////
void  LoomManager::set_family(const char* f) 
{ 
	snprintf(this->family, 20, "%s", family); 
}

/////////////////////////////////////////////////////////////////////
int  LoomManager::get_family_num() 
{ 
	return family_num; 
}

/////////////////////////////////////////////////////////////////////
void LoomManager::set_family_num(int n) 
{ 
	family_num = n; 
}

/////////////////////////////////////////////////////////////////////
int  LoomManager::get_instance_num() 
{ 
	return instance; 
}

/////////////////////////////////////////////////////////////////////
void LoomManager::set_instance_num(int n) 
{ 
	instance = n; 
}

/////////////////////////////////////////////////////////////////////
DeviceType LoomManager::get_device_type() 
{ 
	return device_type; 
}

/////////////////////////////////////////////////////////////////////
// void set_device_type(DeviceType t) 
// {
// 	device_type = t; 
// }

/////////////////////////////////////////////////////////////////////
Verbosity LoomManager::get_print_verbosity()
{
	return print_verbosity;
}

/////////////////////////////////////////////////////////////////////
void LoomManager::set_print_verbosity(Verbosity v)
{
	print_verbosity = v;
}

/////////////////////////////////////////////////////////////////////
Verbosity LoomManager::get_package_verbosity()
{
	return package_verbosity;
}

/////////////////////////////////////////////////////////////////////
void LoomManager::set_package_verbosity(Verbosity v)
{
	package_verbosity = v;
}

/////////////////////////////////////////////////////////////////////
void LoomManager::measure_aux(LoomModule** modules, uint len)
{
	for (int i = 0; i < len; i++) {
		if ( (modules[i] != nullptr) && (modules[i]->get_active()) ) {
			modules[i]->measure();
		}
	}	
}

/////////////////////////////////////////////////////////////////////
void LoomManager::measure()
{
	measure_aux( (LoomModule**)sensor_modules   , sensor_count ); 
	measure_aux( (LoomModule**)other_modules    , other_module_count ); 
}

/////////////////////////////////////////////////////////////////////
void LoomManager::package_aux(JsonObject json, LoomModule** modules, uint len)
{
	for (int i = 0; i < len; i++) {
		if ( (modules[i] != nullptr) && ( modules[i]->get_active() ) ){
			modules[i]->package( json );
		}
	}	
}

/////////////////////////////////////////////////////////////////////
void LoomManager::package_aux(JsonObject json, LoomModule* module)
{
	if ( (module != nullptr) && ( module->get_active() ) ){
		module->package( json );
	}
}


/////////////////////////////////////////////////////////////////////
void LoomManager::package(JsonObject json) 
{
	package_aux( json, (LoomModule*)rtc_module ); 
	package_aux( json, (LoomModule**)other_modules    , other_module_count ); 
	package_aux( json, (LoomModule**)sensor_modules   , sensor_count ); 

	if (package_verbosity == Verbosity::V_HIGH) {
		package_aux( json, (LoomModule**)actuator_modules , actuator_count ); 
		package_aux( json, (LoomModule**)comm_modules     , comm_count ); 
		package_aux( json, (LoomModule**)log_modules      , log_count );		
	}
}

/////////////////////////////////////////////////////////////////////
JsonObject LoomManager::package()
{
	// LPrintln("\nDOC MemoryUsage before clear: ", doc.memoryUsage());
	doc.clear();
	// LPrintln("\nDOC MemoryUsage after clear: ", doc.memoryUsage());

// This works
	doc["type"] = "data";
	JsonObject json = doc.as<JsonObject>();

	// LPrintln("In JsonObject LoomManager::package()");
	// serializeJsonPretty(json, Serial);
// Test 
	// JsonObject json = doc.to<JsonObject>();

	package(json);
	// LPrintln("In JsonObject LoomManager::package()");
	// serializeJsonPretty(json, Serial);
	// LPrintln("\nSIZE: ", json.memoryUsage());

	return json;
}

JsonObject LoomManager::internalJson(bool clear)
{
	if (clear) {
		doc.clear();
		return doc.to<JsonObject>();

	} 
	else {
		return doc.as<JsonObject>();
	}
	// LPrintln("\nDOC MemoryUsage in internalJson: ", doc.memoryUsage());

	// doc["type"] = "unknown";
	// return doc.as<JsonObject>();

}

/////////////////////////////////////////////////////////////////////

// Have each module check against provided command
bool LoomManager::cmd_route_aux(JsonObject json, LoomModule** modules, uint len)
{
	for (int i = 0; i < len; i++) {
		if ( (modules[i] != nullptr) && ( modules[i]->get_active() ) ){
			if (modules[i]->cmd_route( json ) ) return true;
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////

// Have module check against provided command
bool LoomManager::cmd_route_aux(JsonObject json, LoomModule* module)
{
	if ( (module != nullptr) && ( module->get_active() ) ){
		module->cmd_route( json );
	}

}

/////////////////////////////////////////////////////////////////////

// Iterate over array of commands
void LoomManager::cmd_route(JsonObject json)
{
	// LPrintln("Command object:");
	// serializeJsonPretty(json, Serial);

	if ( strcmp(json["type"], "command") == 0 )	{

		for ( JsonObject cmd : json["commands"].as<JsonArray>() ) {

			// LPrintln("\n\nCMD:");
			// serializeJsonPretty(cmd, Serial);


			if ( cmd_route_aux( cmd, (LoomModule**)actuator_modules , actuator_count ) )		continue;
			
			if ( cmd_route_aux( cmd, (LoomModule*)rtc_module ) )								continue;
			if ( cmd_route_aux( cmd, (LoomModule**)other_modules    , other_module_count ) )	continue;
			if ( cmd_route_aux( cmd, (LoomModule**)sensor_modules   , sensor_count ) )			continue;
			if ( cmd_route_aux( cmd, (LoomModule**)comm_modules     , comm_count ) )			continue;
			if ( cmd_route_aux( cmd, (LoomModule**)log_modules      , log_count ) )				continue;
		}
	}
}

/////////////////////////////////////////////////////////////////////
void LoomManager::second_stage_ctor_aux(LoomModule** modules, uint len)
{
	for (int i = 0; i < len; i++) {
		if ( (modules[i] != nullptr) && ( modules[i]->get_active() ) ){
			modules[i]->second_stage_ctor();
		}
	}	
}

// /////////////////////////////////////////////////////////////////////
void LoomManager::print_internalJson()
{
	print_device_label();
	LPrintln("Internal Json:");
	serializeJsonPretty(doc, Serial);
	LPrintln();
}

/////////////////////////////////////////////////////////////////////
void LoomManager::flash_LED(uint count, uint time_high, uint time_low)
{
	for (int i = 0; i < count; i++) {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(time_high);
		digitalWrite(LED_BUILTIN, LOW);
		delay(time_low);
	} 
}

/////////////////////////////////////////////////////////////////////
void LoomManager::flash_LED(uint sequence[3])
{
	flash_LED(sequence[0], sequence[1], sequence[2]);
}

