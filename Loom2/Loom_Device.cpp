
#include "Loom_Module.h"
#include "Loom_Device.h"

#include "Loom_Macros.h"

#include "Loom_Sensor.h"
#include "Loom_Actuator.h"

#include "Loom_CommPlat.h"
#include "Loom_LogPlat.h"

#include "Loom_RTC.h"





char* LoomDevice::enum_device_type_string(DeviceType t)
{
	switch(t) {
		case HUB      : return "Hub";
		case NODE     : return "Node";
		case REPEATER : return "Repeater";
	}
}



// --- CONSTRUCTOR ---
LoomDevice::LoomDevice( char* device_name, char* family, uint family_num, uint instance, DeviceType device_type, char* module_name)
	// : LoomModule(module_name)
{
	this->device_name 	= device_name;
	this->family 		= family;
	this->family_num 	= family_num;
	this->instance 		= instance;
	this->module_count 	= 0;
}

// // --- CONSTRUCTOR ---
// LoomDevice::LoomDevice( char* device_name, char* family, uint family_num, uint instance, DeviceType device_type)
// {
// 	this->device_name 	= device_name;
// 	this->family 		= family;
// 	this->family_num 	= family_num;
// 	this->instance 		= instance;
// 	this->module_count 	= 0;
// }

// --- DESTRUCTOR ---
LoomDevice::~LoomDevice() {}


// --- PUBLIC METHODS ---

void LoomDevice::print_device_label()
{
	LOOM_DEBUG_Print3("[", device_name, "] ");
}

void LoomDevice::print_config() 
{
	print_device_label();
	LOOM_DEBUG_Println("Config:");
	LOOM_DEBUG_Println3('\t', "Device Name         : ", device_name );
	LOOM_DEBUG_Println3('\t', "Family              : ", family );
	LOOM_DEBUG_Println3('\t', "Family Number       : ", family_num );
	LOOM_DEBUG_Println3('\t', "Instance Number     : ", instance );
	LOOM_DEBUG_Println3('\t', "Device Type         : ", enum_device_type_string(device_type) );

	LOOM_DEBUG_Println3('\t', "Instance Number     : ", instance );

	LOOM_DEBUG_Println4('\t', "Modules (", module_count, "):");
	list_modules();
}


void LoomDevice::add_module(LoomModule* LM) 
{
	// If module array is not dynamic, add check to make sure there is room in the array
	print_device_label();
	LOOM_DEBUG_Println2("Adding module ", LM->get_module_name() );
	modules[ module_count++ ] = LM;

	LM->link_parent_device(this);

}

// void module_enable(LoomModule* LM, bool e) ?


void LoomDevice::list_modules()
{
	print_device_label();
	LOOM_DEBUG_Println("Modules");
	for (int i = 0; i < module_count; i++) {
		LOOM_DEBUG_Println4( "\t[", (modules[i]->get_active()) ? "+" : "-" , "] ", modules[i]->get_module_name() );
	}	
}



void LoomDevice::set_device_name(char* device_name)
{
	this->device_name = device_name;
}

void LoomDevice::get_device_name(char* buf)
{
	sprintf(buf, "%s", device_name); 
}

char* LoomDevice::get_device_name()
{
	char name[50];
	get_device_name(name);
	return (char*)name;
}



void LoomDevice::packet_header_family(char* buf)
{ 
	sprintf(buf, "/F/%s", family); 
}


char* LoomDevice::packet_header_family() 
{
	char result[50];
	packet_header_family(result);
	return (char*)result;
}


void LoomDevice::packet_header_subnet(char* buf)
{ 
	sprintf(buf, "/S/%s/%d", family, family_num); 
}


char* LoomDevice::packet_header_subnet() 
{
	char result[50];
	packet_header_subnet(result);
	return (char*)result;
}


void LoomDevice::packet_header_device(char* buf)
{ 
	sprintf(buf, "/D/%s/%d/%s/%d", family, family_num, device_name, instance); 
}


char* LoomDevice::packet_header_device() 
{
	char result[50];
	packet_header_device(result);
	return (char*)result;
}


char* LoomDevice::get_family() 
{ 
	return family; 
}


void  LoomDevice::set_family(char* f) 
{ 
	family = f; 
}

int  LoomDevice::get_family_num() 
{ 
	return family_num; 
}


void LoomDevice::set_family_num(int n) 
{ 
	family_num = n; 
}


int  LoomDevice::get_instance_num() 
{ 
	return instance; 
}


void LoomDevice::set_instance_num(int n) 
{ 
	instance = n; 
}


DeviceType LoomDevice::get_device_type() 
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


void LoomDevice::measure()
{
	for (int i = 0; i < module_count; i++) {
		if ( modules[i]->get_active() ) {
			modules[i]->measure();
		}
	}	
}


void LoomDevice::package() 
{
	bundle.empty();

	for (int i = 0; i < module_count; i++) {
		if ( modules[i]->get_active() ) {
			// modules[i]->package( &bundle, packet_header_device() );
			modules[i]->package( &bundle );

		}
	}

}

void LoomDevice::package(OSCBundle* bndl) 
{
	package();
	deep_copy_bundle(&bundle, bndl);
}


void LoomDevice::print_current_bundle() 
{
	LOOM_DEBUG_Println("Member Bundle:");
	print_bundle(&bundle);
}




