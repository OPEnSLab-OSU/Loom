
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
	Print3("[", device_name, "] ");
}

/////////////////////////////////////////////////////////////////////
void LoomManager::print_config() 
{
	print_device_label();
	Println("Config:");
	Println3('\t', "Device Name         : ", device_name );
	Println3('\t', "Family              : ", family );
	Println3('\t', "Family Number       : ", family_num );
	Println3('\t', "Instance Number     : ", instance );
	Println3('\t', "Device Type         : ", enum_device_type_string(device_type) );
	Println3('\t', "Instance Number     : ", instance );

	list_modules();
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module_aux(LoomModule** modules, LoomModule* module, uint& len, const int max_len)
{
	print_device_label();

	if (module == nullptr) {
		Println("Cannot add null module");
		return;
	}

	// If module array is not dynamic, add check to make sure there is room in the array
	if ( len >= max_len ) {
		Println2("Cannot add ", module->get_module_name() );
		return;
	}

	Println2("Adding Module: ", module->get_module_name() );

	modules[ len++ ] = module;
	module->link_device_manager(this);
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module(Loom_Interrupt_Manager* interrupt_manager) 
{
	print_device_label();
	if (!interrupt_manager) {
		Println2("Cannot add ", interrupt_manager->get_module_name() );
		return;
	}
	Println2("Adding Module: ", interrupt_manager->get_module_name() );
	this->interrupt_manager = interrupt_manager; 

	interrupt_manager->link_device_manager(this);

	if (rtc_module != nullptr) {
		this->interrupt_manager->set_RTC_module(rtc_module);
	}
}

/////////////////////////////////////////////////////////////////////
void LoomManager::add_module(Loom_Sleep_Manager* sleep_manager) 
{
	print_device_label();
	if (!sleep_manager) {
		Println2("Cannot add ", sleep_manager->get_module_name() );
		return;
	}
	Println2("Adding Module: ", sleep_manager->get_module_name() );
	this->sleep_manager = sleep_manager; 

	sleep_manager->link_device_manager(this);
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
		Println("Cannot add null module");
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
LoomModule* LoomManager::get_other_module(int idx)
{
	return (idx < other_module_count) ? other_modules[idx] : nullptr;
}

/////////////////////////////////////////////////////////////////////
LoomSensor* LoomManager::get_sensor_module(int idx)
{
	return (idx < sensor_count) ? sensor_modules[idx] : nullptr;
}

/////////////////////////////////////////////////////////////////////
LoomActuator* LoomManager::get_actuator_module(int idx)
{
	return (idx < actuator_count) ? actuator_modules[idx] : nullptr;
}

/////////////////////////////////////////////////////////////////////
LoomCommPlat* LoomManager::get_comm_plat_module(int idx)
{
	return (idx < comm_count) ? comm_modules[idx] : nullptr;
}

/////////////////////////////////////////////////////////////////////
LoomInternetPlat* LoomManager::get_internet_plat_module(int idx)
{
	return (idx < internet_count) ? internet_modules[idx] : nullptr;
}

/////////////////////////////////////////////////////////////////////
LoomLogPlat* LoomManager::get_log_plat_module(int idx)
{
	return (idx < log_count) ? log_modules[idx] : nullptr;
}

/////////////////////////////////////////////////////////////////////
// void module_enable(LoomModule* LM, bool e) ?


/////////////////////////////////////////////////////////////////////
void LoomManager::list_modules_aux(LoomModule** modules, uint len, char* module_type)
{
	Println5("\t", module_type, " (", len, "):");
	for (int i = 0; i < len; i++) {
		if ( (modules[i] != nullptr) && (modules[i]->get_active()) ) {
			Println4( "\t\t[", (modules[i]->get_active()) ? "+" : "-" , "] ", modules[i]->get_module_name() );
		}
	}	
}

/////////////////////////////////////////////////////////////////////
void LoomManager::list_modules()
{
	print_device_label();
	Println("Modules:");

	Println5("\t", "RTC modules", " (", (rtc_module != nullptr), "):");
	if ( (rtc_module != nullptr) && (rtc_module->get_active()) ) {
		Println4( "\t\t[", (rtc_module->get_active()) ? "+" : "-" , "] ", rtc_module->get_module_name() );
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
void LoomManager::packet_header_family(char* buf)
{ 
	sprintf(buf, "/F/%s", family); 
}

/////////////////////////////////////////////////////////////////////
const char* LoomManager::packet_header_family() 
{
	char result[50];
	packet_header_family(result);
	return (const char*)result;
}

/////////////////////////////////////////////////////////////////////
void LoomManager::packet_header_subnet(char* buf)
{ 
	sprintf(buf, "/S/%s/%d", family, family_num); 
}

/////////////////////////////////////////////////////////////////////
const char* LoomManager::packet_header_subnet() 
{
	char result[50];
	packet_header_subnet(result);
	return (const char*)result;
}

/////////////////////////////////////////////////////////////////////
void LoomManager::packet_header_device(char* buf)
{ 
	sprintf(buf, "/D/%s/%d/%s/%d", family, family_num, device_name, instance); 
}

/////////////////////////////////////////////////////////////////////
const char* LoomManager::packet_header_device() 
{
	char result[50];
	packet_header_device(result);
	return (const char*)result;
}

/////////////////////////////////////////////////////////////////////
const char* LoomManager::get_family() 
{ 
	return family; 
}

/////////////////////////////////////////////////////////////////////
void  LoomManager::set_family(const char* f) 
{ 
	// strcpy(family, f);
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
void LoomManager::package_aux(LoomModule** modules, uint len)
{
	for (int i = 0; i < len; i++) {
		if ( (modules[i] != nullptr) && ( modules[i]->get_active() ) ){
			modules[i]->package( bundle );
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
void LoomManager::package() 
{
	bundle.empty();

	package_aux( (LoomModule**)other_modules    , other_module_count ); 
	package_aux( (LoomModule**)sensor_modules   , sensor_count ); 

	if (package_verbosity == Verbosity::V_HIGH) {
		package_aux( (LoomModule**)actuator_modules , actuator_count ); 
		package_aux( (LoomModule**)rtc_module      , 1 ); 
		package_aux( (LoomModule**)comm_modules     , comm_count ); 
		package_aux( (LoomModule**)log_modules      , log_count );		
	}
}

/////////////////////////////////////////////////////////////////////
void LoomManager::package(OSCBundle& bndl) 
{
	package();
	deep_copy_bundle(bundle, bndl);
}

/////////////////////////////////////////////////////////////////////
void LoomManager::print_current_bundle() 
{
	Println("Member Bundle:");
	print_bundle(bundle);
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

