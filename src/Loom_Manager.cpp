
#include "Loom_Module.h"
#include "Loom_Manager.h"

#include "Loom_Macros.h"

#include "Loom_Interrupt_Manager.h"
#include "Loom_Sleep_Manager.h"

#include "Sensors/Loom_Sensor.h"
#include "Loom_Multiplexer.h"
#include "Actuators/Loom_Actuator.h"
#include "CommPlats/Loom_CommPlat.h"
#include "InternetPlats/Loom_InternetPlat.h"
#include "LogPlats/Loom_LogPlat.h"
#include "RTC/Loom_RTC.h"
#include "PublishPlats/Loom_PublishPlat.h"

#include <Adafruit_SleepyDog.h>


///////////////////////////////////////////////////////////////////////////////
const char* LoomManager::enum_device_type_string(DeviceType t)
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

///////////////////////////////////////////////////////////////////////////////
LoomManager::~LoomManager() 
{
	free_modules();
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::print_device_label()
{
	LPrint("[", device_name, "] ");
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::print_config(bool print_modules_config) 
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

	// Print managed module's configs
	if (print_modules_config) {
		if (interrupt_manager != nullptr) {
			interrupt_manager->print_config();
		}
		if (sleep_manager != nullptr) {
			interrupt_manager->print_config();
		}
		if (rtc_module != nullptr) {
			interrupt_manager->print_config();
		}

		print_config_aux( other_modules); 
		print_config_aux( sensor_modules); 
		print_config_aux( actuator_modules); 
		print_config_aux( comm_modules); 
		print_config_aux( internet_modules); 
		print_config_aux( publish_modules); 
		print_config_aux( log_modules );  
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::begin_LED()
{
	pinMode(LED_BUILTIN, OUTPUT);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::begin_serial(bool wait_for_monitor)
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

///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
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

///////////////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomModule* module) 
{
	add_module_aux(other_modules, module);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomSensor* sensor) 
{
	add_module_aux(sensor_modules, sensor);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomActuator* actuator) 
{
	add_module_aux(actuator_modules, actuator);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomCommPlat* comm_plat) 
{
	add_module_aux(comm_modules, comm_plat);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomInternetPlat* internet_module) 
{
	add_module_aux(internet_modules, internet_module);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomPublishPlat* publish_module) 
{
	add_module_aux(publish_modules, publish_module);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::add_module(LoomLogPlat* log_plat) 
{	
	add_module_aux(log_modules, log_plat);
}

///////////////////////////////////////////////////////////////////////////////
Loom_Interrupt_Manager* LoomManager::get_interrupt_manager()
{
	return interrupt_manager;
}

///////////////////////////////////////////////////////////////////////////////
Loom_Sleep_Manager* LoomManager::get_sleep_manager()
{
	return sleep_manager;
}

///////////////////////////////////////////////////////////////////////////////
LoomRTC* LoomManager::get_rtc_module()
{
	return rtc_module;
}

///////////////////////////////////////////////////////////////////////////////
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

	list_modules_aux( other_modules, "Other Modules"); 
	list_modules_aux( sensor_modules, "Sensors"); 
	list_modules_aux( actuator_modules, "Actuators"); 
	list_modules_aux( comm_modules, "Communication Platforms"); 
	list_modules_aux( internet_modules, "Internet Platforms"); 
	list_modules_aux( publish_modules, "Publish Platforms"); 
	list_modules_aux( log_modules, "Logging Platforms" ); 
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_device_name(const char* device_name)
{
	snprintf(this->device_name, 20, "%s", device_name);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::get_device_name(char* buf)
{
	sprintf(buf, "%s", device_name); 
}

///////////////////////////////////////////////////////////////////////////////
const char* LoomManager::get_device_name()
{
	char name[50];
	get_device_name(name);
	return (const char*)name;
}

///////////////////////////////////////////////////////////////////////////////
const char* LoomManager::get_family() 
{ 
	return family; 
}

///////////////////////////////////////////////////////////////////////////////
void  LoomManager::set_family(const char* family) 
{ 
	snprintf(this->family, 20, "%s", family); 
}

///////////////////////////////////////////////////////////////////////////////
int  LoomManager::get_family_num() 
{ 
	return family_num; 
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_family_num(int n) 
{ 
	family_num = n; 
}

///////////////////////////////////////////////////////////////////////////////
int  LoomManager::get_instance_num() 
{ 
	return instance; 
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_instance_num(int n) 
{ 
	instance = n; 
}

///////////////////////////////////////////////////////////////////////////////
LoomManager::DeviceType LoomManager::get_device_type() 
{ 
	return device_type; 
}

///////////////////////////////////////////////////////////////////////////////
// void set_device_type(DeviceType t) 
// {
// 	device_type = t; 
// }

///////////////////////////////////////////////////////////////////////////////
Verbosity LoomManager::get_print_verbosity()
{
	return print_verbosity;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_print_verbosity(Verbosity v, bool set_modules)
{
	print_verbosity = v;

	if (set_modules) {
		if (rtc_module != nullptr) { rtc_module->set_print_verbosity(v); }
		if (interrupt_manager != nullptr) { interrupt_manager->set_print_verbosity(v); }
		if (sleep_manager != nullptr) { sleep_manager->set_print_verbosity(v); }

		// Iterate over lists of modules set verbosity for each
		set_print_verbosity_aux(v, other_modules);
		set_print_verbosity_aux(v, sensor_modules);
		set_print_verbosity_aux(v, actuator_modules);
		set_print_verbosity_aux(v, comm_modules);
		set_print_verbosity_aux(v, internet_modules);
		set_print_verbosity_aux(v, publish_modules);
		set_print_verbosity_aux(v, log_modules);
	}
}

///////////////////////////////////////////////////////////////////////////////
Verbosity LoomManager::get_package_verbosity()
{
	return package_verbosity;
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::set_package_verbosity(Verbosity v, bool set_modules)
{
	package_verbosity = v;

	if (set_modules) {
		if (rtc_module != nullptr) { rtc_module->set_package_verbosity(v); }
		if (interrupt_manager != nullptr) { interrupt_manager->set_package_verbosity(v); }
		if (sleep_manager != nullptr) { sleep_manager->set_package_verbosity(v); }

		// Iterate over lists of modules set verbosity for each
		set_package_verbosity_aux(v, other_modules);
		set_package_verbosity_aux(v, sensor_modules);
		set_package_verbosity_aux(v, actuator_modules);
		set_package_verbosity_aux(v, comm_modules);
		set_package_verbosity_aux(v, internet_modules);
		set_package_verbosity_aux(v, publish_modules);
		set_package_verbosity_aux(v, log_modules);
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::measure()
{
	for (auto sensor : sensor_modules) {
		if ( (sensor != nullptr) && (sensor->get_active()) ) {
			sensor->measure();
		}
	}
	
	for (auto module : other_modules) {	
		if (module->get_module_type() == LoomModule::Type::Multiplexer) {
			((Loom_Multiplexer*)module)->measure();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::package_aux(JsonObject json, LoomModule* module)
{
	if ( (module != nullptr) && ( module->get_active() ) ){
		module->package( json );
	}
}


///////////////////////////////////////////////////////////////////////////////
void LoomManager::package(JsonObject json) 
{
	// Add device identification to json
	add_device_ID_to_json(json);
	
	// Iterate over lists of modules getting data from each
	package_aux( json, (LoomModule*)rtc_module ); 
	package_aux( json, (LoomModule*)sleep_manager ); 
	package_aux( json, (LoomModule*)interrupt_manager ); 
	package_aux( json, other_modules); 
	package_aux( json, sensor_modules); 

	// If high verbosity, also check these lists
	if (package_verbosity == Verbosity::V_HIGH) {
		package_aux( json, actuator_modules); 
		package_aux( json, comm_modules); 
		package_aux( json, log_modules);		
	}
}

///////////////////////////////////////////////////////////////////////////////
JsonObject LoomManager::package()
{
	doc.clear();
	doc["type"] = "data";
	JsonObject json = doc.as<JsonObject>();
	package(json);
	return json;
}


///////////////////////////////////////////////////////////////////////////////
void LoomManager::add_device_ID_to_json(JsonObject json)
{
	JsonObject timestamp = json.createNestedObject("id");
	timestamp["name"]		= device_name;
	timestamp["instance"]	= instance;
	timestamp["family"]		= family;
	timestamp["family_num"]	= family_num;
}

///////////////////////////////////////////////////////////////////////////////
JsonObject LoomManager::internal_json(bool clear)
{
	if (clear) {
		doc.clear();
		return doc.to<JsonObject>();

	} 
	else {
		return doc.as<JsonObject>();
	}
	// LPrintln("\nDOC MemoryUsage in internal_json: ", doc.memoryUsage());

	// doc["type"] = "unknown";
	// return doc.as<JsonObject>();

}

bool LoomManager::publish_all(const JsonObject json)
{
	bool result = true;
	for (auto publisher : publish_modules) {
		if ( (publisher != nullptr) && ( publisher->get_active() ) ){
			result &= publisher->publish( json );
		}
	}
	return (publish_modules.size() > 0) && result;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomManager::cmd_route_aux(JsonObject json, LoomModule* module)
{
	if ( (module != nullptr) && ( module->get_active() ) ){
		module->cmd_route( json );
	}

}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::cmd_route(JsonObject json)
{
	// LPrintln("Command object:");
	// serializeJsonPretty(json, Serial);

	if ( strcmp(json["type"], "command") == 0 )	{

		for ( JsonObject cmd : json["commands"].as<JsonArray>() ) {

			// LPrintln("\n\nCMD:");
			// serializeJsonPretty(cmd, Serial);

			if ( cmd_route_aux(cmd, actuator_modules) )			continue;
			
			if ( cmd_route_aux(cmd, (LoomModule*)rtc_module) )		continue;
			if ( cmd_route_aux(cmd, other_modules) )			continue;
			if ( cmd_route_aux(cmd, sensor_modules) )			continue;
			if ( cmd_route_aux(cmd, comm_modules) )				continue;
			if ( cmd_route_aux(cmd, log_modules) )				continue;
		}
	}
}

// ///////////////////////////////////////////////////////////////////////////////
void LoomManager::print_internal_json()
{
	print_device_label();
	LPrintln("Json:");
	serializeJsonPretty(doc, Serial);
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::flash_LED(uint count, uint time_high, uint time_low, bool end_high)
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
void LoomManager::flash_LED(uint sequence[3])
{
	flash_LED(sequence[0], sequence[1], sequence[2]);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::pause(int ms)
{
	Serial.end();
	USBDevice.detach();

	int sleepMS = Watchdog.sleep(ms);

	USBDevice.attach();
	Serial.begin(SERIAL_BAUD);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::free_modules()
{
	if (rtc_module != nullptr) { delete rtc_module; }
	if (interrupt_manager != nullptr) { delete interrupt_manager; }
	if (sleep_manager != nullptr) { delete sleep_manager; }

	// Iterate over lists of modules freeing each
	free_modules_aux(other_modules);
	free_modules_aux(sensor_modules);
	free_modules_aux(actuator_modules);
	free_modules_aux(comm_modules);
	free_modules_aux(internet_modules);
	free_modules_aux(publish_modules);
	free_modules_aux(log_modules);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::power_up()
{
	// Iterate over lists of modules powering active one on
	power_up_aux(other_modules);
	power_up_aux(sensor_modules);
	power_up_aux(actuator_modules);
	power_up_aux(comm_modules);
	power_up_aux(internet_modules);
	power_up_aux(publish_modules);
	power_up_aux(log_modules);
}

///////////////////////////////////////////////////////////////////////////////
void LoomManager::power_down()
{
	// Iterate over lists of modules powering them off
	power_down_aux(other_modules);
	power_down_aux(sensor_modules);
	power_down_aux(actuator_modules);
	power_down_aux(comm_modules);
	power_down_aux(internet_modules);
	power_down_aux(publish_modules);
	power_down_aux(log_modules);
}

///////////////////////////////////////////////////////////////////////////////






