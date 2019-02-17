
#include "Loom_Module.h"
#include "Loom_Manager.h"

#include "Loom_Macros.h"

#include "Loom_Sensor.h"
#include "Loom_Actuator.h"
#include "Loom_CommPlat.h"
#include "Loom_LogPlat.h"
#include "Loom_RTC.h"





char* LoomManager::enum_device_type_string(DeviceType t)
{
	switch(t) {
		case HUB      : return "Hub";
		case NODE     : return "Node";
		case REPEATER : return "Repeater";
	}
}



// --- CONSTRUCTOR ---
LoomManager::LoomManager( char* device_name, char* family, uint family_num, uint instance, DeviceType device_type, char* module_name)
{
	this->device_name 	= device_name;
	this->family 		= family;
	this->family_num 	= family_num;
	this->instance 		= instance;
	this->module_count 	= 0;

	// Initialize module counts
	this->other_module_count = 0;
	this->sensor_count       = 0;
	this->actuator_count     = 0;
	this->rtc_count          = 0;
	this->comm_count         = 0;
	this->log_count          = 0;

}

// --- DESTRUCTOR ---
LoomManager::~LoomManager() 
{
	// Impelement this
}



// --- PUBLIC METHODS ---



void LoomManager::print_device_label()
{
	LOOM_DEBUG_Print3("[", device_name, "] ");
}


void LoomManager::print_config() 
{
	print_device_label();
	LOOM_DEBUG_Println("Config:");
	LOOM_DEBUG_Println3('\t', "Device Name         : ", device_name );
	LOOM_DEBUG_Println3('\t', "Family              : ", family );
	LOOM_DEBUG_Println3('\t', "Family Number       : ", family_num );
	LOOM_DEBUG_Println3('\t', "Instance Number     : ", instance );
	LOOM_DEBUG_Println3('\t', "Device Type         : ", enum_device_type_string(device_type) );

	LOOM_DEBUG_Println3('\t', "Instance Number     : ", instance );

	// LOOM_DEBUG_Println4('\t', "Modules (", module_count, "):");
	list_modules();
}


void LoomManager::add_module(LoomModule* module) 
{
	// // If module array is not dynamic, add check to make sure there is room in the array
	// print_device_label();
	// LOOM_DEBUG_Println2("Adding module ", LM->get_module_name() );
	// modules[ module_count++ ] = LM;

	// LM->link_device_manager(this);

	print_device_label();

	// If module array is not dynamic, add check to make sure there is room in the array
	if ( other_module_count >= MAX_OTHER_MODULES ) {
		LOOM_DEBUG_Println2("Cannot add ", module->get_module_name() );
		return;
	}

	LOOM_DEBUG_Println2("Adding Module: ", module->get_module_name() );
	other_modules[ other_module_count++ ] = module;

	module->link_device_manager(this);
}

void LoomManager::add_module(LoomSensor* sensor) 
{
	print_device_label();

	// If module array is not dynamic, add check to make sure there is room in the array
	if ( sensor_count >= MAX_SENSORS ) {
		LOOM_DEBUG_Println2("Cannot add ", sensor->get_module_name() );
		return;
	}

	LOOM_DEBUG_Println2("Adding Sensor: ", sensor->get_module_name() );
	sensor_modules[ sensor_count++ ] = sensor;

	sensor->link_device_manager(this);
}

void LoomManager::add_module(LoomActuator* actuator) 
{
	print_device_label();

	// If module array is not dynamic, add check to make sure there is room in the array
	if ( actuator_count >= MAX_ACTUATORS ) {
		LOOM_DEBUG_Println2("Cannot add ", actuator->get_module_name() );
		return;
	}

	LOOM_DEBUG_Println2("Adding Actuator: ", actuator->get_module_name() );
	actuator_modules[ actuator_count++ ] = actuator;

	actuator->link_device_manager(this);
}

void LoomManager::add_module(LoomRTC* rtc) 
{
	print_device_label();

	// If module array is not dynamic, add check to make sure there is room in the array
	if ( rtc_count >= MAX_RTCS ) {
		LOOM_DEBUG_Println2("Cannot add ", rtc->get_module_name() );
		return;
	}

	LOOM_DEBUG_Println2("Adding RTC: ", rtc->get_module_name() );
	rtc_modules[ rtc_count++ ] = rtc;

	rtc->link_device_manager(this);
}

void LoomManager::add_module(LoomCommPlat* comm_plat) 
{
	print_device_label();

	// If module array is not dynamic, add check to make sure there is room in the array
	if ( comm_count >= MAX_COMMS ) {
		LOOM_DEBUG_Println2("Cannot add ", comm_plat->get_module_name() );
		return;
	}

	LOOM_DEBUG_Println2("Adding Communication Platform: ", comm_plat->get_module_name() );
	comm_modules[ comm_count++ ] = comm_plat;

	comm_plat->link_device_manager(this);
}

void LoomManager::add_module(LoomLogPlat* log_plat) 
{
	print_device_label();

	// If module array is not dynamic, add check to make sure there is room in the array
	if ( log_count >= MAX_LOGS ) {
		LOOM_DEBUG_Println2("Cannot add ", log_plat->get_module_name() );
		return;
	}

	LOOM_DEBUG_Println2("Adding Logging Platform: ", log_plat->get_module_name() );
	log_modules[ log_count++ ] = log_plat;

	log_plat->link_device_manager(this);
}









// void module_enable(LoomModule* LM, bool e) ?


void LoomManager::list_modules()
{
	print_device_label();
	LOOM_DEBUG_Println("Modules:");
	// LOOM_DEBUG_Println("Modules");
	// for (int i = 0; i < module_count; i++) {
	// 	LOOM_DEBUG_Println4( "\t[", (modules[i]->get_active()) ? "+" : "-" , "] ", modules[i]->get_module_name() );
	// }	

	LOOM_DEBUG_Println4("\t", "Other Modules (", other_module_count, "):");
	for (int i = 0; i < other_module_count; i++) {
		LOOM_DEBUG_Println4( "\t\t[", (other_modules[i]->get_active()) ? "+" : "-" , "] ", other_modules[i]->get_module_name() );
	}

	LOOM_DEBUG_Println4("\t", "Sensors (", sensor_count, "):");
	for (int i = 0; i < sensor_count; i++) {
		LOOM_DEBUG_Println4( "\t\t[", (sensor_modules[i]->get_active()) ? "+" : "-" , "] ", sensor_modules[i]->get_module_name() );
	}

	LOOM_DEBUG_Println4("\t", "Actuators (", actuator_count, "):");
	for (int i = 0; i < actuator_count; i++) {
		LOOM_DEBUG_Println4( "\t\t[", (actuator_modules[i]->get_active()) ? "+" : "-" , "] ", actuator_modules[i]->get_module_name() );
	}

	LOOM_DEBUG_Println4("\t", "RTC Modules (", rtc_count, "):");
	for (int i = 0; i < rtc_count; i++) {
		LOOM_DEBUG_Println4( "\t\t[", (rtc_modules[i]->get_active()) ? "+" : "-" , "] ", rtc_modules[i]->get_module_name() );
	}

	LOOM_DEBUG_Println4("\t", "Communication Platforms (", comm_count, "):");
	for (int i = 0; i < comm_count; i++) {
		LOOM_DEBUG_Println4( "\t\t[", (comm_modules[i]->get_active()) ? "+" : "-" , "] ", comm_modules[i]->get_module_name() );
	}

	LOOM_DEBUG_Println4("\t", "Logging Platforms (", log_count, "):");
	for (int i = 0; i < log_count; i++) {
		LOOM_DEBUG_Println4( "\t\t[", (log_modules[i]->get_active()) ? "+" : "-" , "] ", log_modules[i]->get_module_name() );
	}	
}



void LoomManager::set_device_name(char* device_name)
{
	this->device_name = device_name;
}

void LoomManager::get_device_name(char* buf)
{
	sprintf(buf, "%s", device_name); 
}

char* LoomManager::get_device_name()
{
	char name[50];
	get_device_name(name);
	return (char*)name;
}



void LoomManager::packet_header_family(char* buf)
{ 
	sprintf(buf, "/F/%s", family); 
}


char* LoomManager::packet_header_family() 
{
	char result[50];
	packet_header_family(result);
	return (char*)result;
}


void LoomManager::packet_header_subnet(char* buf)
{ 
	sprintf(buf, "/S/%s/%d", family, family_num); 
}


char* LoomManager::packet_header_subnet() 
{
	char result[50];
	packet_header_subnet(result);
	return (char*)result;
}


void LoomManager::packet_header_device(char* buf)
{ 
	sprintf(buf, "/D/%s/%d/%s/%d", family, family_num, device_name, instance); 
}


char* LoomManager::packet_header_device() 
{
	char result[50];
	packet_header_device(result);
	return (char*)result;
}


char* LoomManager::get_family() 
{ 
	return family; 
}


void  LoomManager::set_family(char* f) 
{ 
	family = f; 
}

int  LoomManager::get_family_num() 
{ 
	return family_num; 
}


void LoomManager::set_family_num(int n) 
{ 
	family_num = n; 
}


int  LoomManager::get_instance_num() 
{ 
	return instance; 
}


void LoomManager::set_instance_num(int n) 
{ 
	instance = n; 
}


DeviceType LoomManager::get_device_type() 
{ 
	return device_type; 
}



// void set_device_type(DeviceType t) {device_type = t; }





// ** MAYBE IMPLEMENT THESE HERE **
// might not be needed becaused of CommPlat and other

//but probably put here, because measure and package aren't managed elsewhere

	// void measure();
	// void package();
	// void receive(); // not srue if this should take arg to specify platform
	// void send();
	// void log(Enum );
	// void sleep(); // could have default sleep behavior?

// void current_bundle(OSCBundle* bndl) ? return a stored bundle
// void print_data


void LoomManager::measure()
{
	for (int i = 0; i < module_count; i++) {
		if ( modules[i]->get_active() ) {
			modules[i]->measure();
		}
	}	
}


void LoomManager::package() 
{
	bundle.empty();

	for (int i = 0; i < module_count; i++) {
		if ( modules[i]->get_active() ) {
			// modules[i]->package( &bundle, packet_header_device() );
			modules[i]->package( bundle );

		}
	}

}

void LoomManager::package(OSCBundle& bndl) 
{
	package();
	deep_copy_bundle(bundle, bndl);
}


void LoomManager::print_current_bundle() 
{
	LOOM_DEBUG_Println("Member Bundle:");
	print_bundle(bundle);
}








