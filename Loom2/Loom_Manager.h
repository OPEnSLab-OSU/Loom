
#ifndef LOOM_MANAGER_h
#define LOOM_MANAGER_h


// #include "loom_abstract_module_class.h"

// these might need to be forward declared instead
// e.g. class LoomSensor;
// both h files included in .cpp files
// #include "loom_sensor_class.h"
// #include "loom_actuator_class.h"

#include "Loom_Misc.h"
#include "Loom_Translator.h"



enum DeviceType { HUB, NODE, REPEATER };


// Forward declarations, specify that these classes 
// exist but are defined in their own respective files
class LoomModule;
class LoomSensor;
class LoomActuator;
class LoomCommPlat;
class LoomLogPlat;



class LoomManager
{

protected:
	// --- PROTECTED MEMBERS ---
	char* 		device_name;		// The name of the device

	char* 		family;			// The family the device belongs to
	uint 		family_num;		// The subnet of the family
	uint 		instance;		// The instance / channel ID within the subnet

	DeviceType 	device_type;	// Maybe remove if using Hub, Node, and Repeater become subclasses of LoomManager

	uint8_t		module_count;		//
	LoomModule* modules[10];


// keep counts for these as well? or just make all of these arrays dynamic
// or maybe just use check for null 
	LoomSensor*      sensor_modules[10];
	LoomActuator*    actuator_modules[10];
	// LoomRTC*         rtc_module;
	LoomCommPlat*    comm_modules[3];
	LoomLogPlat*     log_modules[3];


	// Array of pointers to enabled modules
	// Maybe preprocessor defined?
	// LoomModule * modules[10];
	// or
	// LoomModule ** modules;     // probably not - I think only mux and analog should be dynamic
	//   then later
	//  modules = new LoomModule*[10];

	// Maybe separate sensor, actuator, and various platform arrays?

	// would allow for easy log-, send- all functionality 
	// and better calls to measure()

	OSCBundle bundle;	// Not sure if this will always work...

public:


	static char* enum_device_type_string(DeviceType t);



	// --- CONSTRUCTOR ---

								// The parameters to the constructor will be defined in config
	LoomManager( char* device_name 	= "Default",
				char* family 		= "Loom",
				uint  family_num 	= 1,
				uint  instance 		= 1,

				DeviceType device_type	= NODE,

				char* module_name = "LoomManager"
			  );

	// LoomManager( char* device_name,
	// 		char* family,
	// 		uint  family_num,
	// 		uint  instance,

	// 		DeviceType device_typ
	// 	  );

	// --- DESTRUCTOR ---
	~LoomManager();

	// --- PUBLIC METHODS ---

	void print_device_label();

	void print_config();



// // From LoomModule

// 	// Display configuration settings
// 	// void print_config() {}

// 	// Display current state
// 	void print_state() {}

// 	// void measure() {}

// 	// Append to a bundle 
// 	void package(OSCBundle* bndl, char* id_prefix) {}

// 	bool message_route(OSCMessage* msg, int address_offset) {}




// does this check type and sort into arrays accordingly, or is there specific add module methods per type

	void add_module(LoomModule* LM);
	// void module_enable(LoomModule* LM, bool e) ?

	void list_modules();



	void set_device_name(char* device_name);

	void get_device_name(char* buf);
	char* get_device_name();

	


		// Should this return a char* or copy into char array passed by reference?
		// Could overload to do either like String does with .toCharArray() and .c_str()
	// char* packet_header() {}
	// void packet_header(char* buf) {}

	void packet_header_family(char* buf);
	char* packet_header_family();

	void packet_header_subnet(char* buf);
	char* packet_header_subnet();

	void packet_header_device(char* buf);
	char* packet_header_device();


	char* get_family();
	void set_family(char* f);

	int  get_family_num();
	void set_family_num(int n);

	int  get_instance_num();
	void set_instance_num(int n);

	DeviceType get_device_type();
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


	void measure();
	void package();
	void package(OSCBundle* bndl);

	void print_current_bundle();

	// Methods to set package and print verbosities all at once

private:

// --- PRIVATE METHODS --- 

};


#endif



















// #ifndef LOOM_DEVICE_h
// #define LOOM_DEVICE_h


// #include "loom_abstract_module_class.h"

// #include "loom_misc.h"
// #include "loom_translator.h"



// enum DeviceType { HUB, NODE, REPEATER };



// class LoomManager
// {

// protected:
// 	// --- PROTECTED MEMBERS ---
// 	char* device_name;		// The name of the device

// 	char* 		family;			// The family the device belongs to
// 	uint 		family_num;		// The subnet of the family
// 	uint 		instance;		// The instance / channel ID within the subnet

// 	DeviceType 	device_type;	// Maybe remove if using Hub, Node, and Repeater become subclasses of LoomManager

// 	uint8_t		module_count;		//

// 	LoomModule* modules[10];


// 	// Array of pointers to enabled modules
// 	// Maybe preprocessor defined?
// 	// LoomModule * modules[10];
// 	// or
// 	// LoomModule ** modules;     // probably not - I think only mux and analog should be dynamic
// 	//   then later
// 	//  modules = new LoomModule*[10];

// 	// Maybe separate sensor, actuator, and various platform arrays?

// 	// would allow for easy log-, send- all functionality 
// 	// and better calls to measure()

// 	OSCBundle bundle;	// Not sure if this will always work...

// public:


// 	static char* enum_device_type_string(DeviceType t);



// 	// --- CONSTRUCTOR ---

// 								// The parameters to the constructor will be defined in config
// 	LoomManager( char* device_name 	= "LoomManager",
// 				char* family 		= "Loom",
// 				uint  family_num 	= 1,
// 				uint  instance 		= 1,

// 				DeviceType device_type	= NODE
// 			  );

// 	// LoomManager( char* device_name,
// 	// 		char* family,
// 	// 		uint  family_num,
// 	// 		uint  instance,

// 	// 		DeviceType device_typ
// 	// 	  );

// 	// --- DESTRUCTOR ---
// 	~LoomManager();// {}


// 	// --- PUBLIC METHODS ---
// 	void print_config();


// 	void add_module(LoomModule* LM);
// 	// void module_enable(LoomModule* LM, bool e) ?

// 	void list_modules();


// 		// Should this return a char* or copy into char array passed by reference?
// 		// Could overload to do either like String does with .toCharArray() and .c_str()
// 	// char* packet_header() {}
// 	// void packet_header(char* buf) {}

// 	void packet_header_family(char* buf);

// 	char* packet_header_family();


// 	void packet_header_subnet(char* buf);

// 	char* packet_header_subnet();


// 	void packet_header_device(char* buf);

// 	char* packet_header_device();



// 	char* get_family();
// 	void set_family(char* f);

// 	int  get_family_num();
// 	void set_family_num(int n);

// 	int  get_instance_num();
// 	void set_instance_num(int n);

// 	DeviceType get_device_type();
// 	// void set_device_type(DeviceType t) {device_type = t; }





// // ** MAYBE IMPLEMENT THESE HERE **
// // might not be needed becaused of CommPlat and other

// //but probably put here, because measure and package aren't managed elsewhere

// 	// void measure();
// 	// void package();
// 	// void receive(); // not srue if this should take arg to specify platform
// 	// void send();
// 	// void log(Enum );
// 	// void sleep(); // could have default sleep behavior?

// // void current_bundle(OSCBundle* bndl) ? return a stored bundle
// // void print_data


// 	void measure();


// 	void package();


// 	void package(OSCBundle* bndl);

// 	void print_current_bundle();



// 	// Methods to set package and print verbosities all at once

// private:

// // --- PRIVATE METHODS --- 

// };





// #endif