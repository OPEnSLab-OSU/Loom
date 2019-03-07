
#ifndef LOOM_MANAGER_h
#define LOOM_MANAGER_h


// these might need to be forward declared instead
// e.g. class LoomSensor;
// both h files included in .cpp files
// #include "loom_sensor_class.h"
// #include "loom_actuator_class.h"

#include "Loom_Misc.h"
#include "Loom_Translator.h"



#define SERIAL_BAUD 115200



enum class DeviceType { HUB, NODE, REPEATER };
enum class Verbosity { V_OFF, V_LOW, V_HIGH }; 


// Forward declarations, specify that these classes 
// exist but are defined in their own respective files
class Loom_Interrupt_Manager;
class Loom_Sleep_Manager;
class LoomModule;
class LoomSensor;
class LoomActuator;
class LoomRTC;
class LoomCommPlat;
class LoomInternetPlat;
class LoomLogPlat;


#define MAX_OTHER_MODULES 3
#define MAX_SENSORS       20
#define MAX_ACTUATORS     10
#define MAX_RTCS          3
#define MAX_COMMS         3
#define MAX_INTERNETS     3
#define MAX_LOGS          5



class LoomManager
{

protected:

	char* 		device_name;	// The name of the device

	char* 		family;			// The family the device belongs to
	uint 		family_num;		// The subnet of the family
	uint 		instance;		// The instance / channel ID within the subnet

	DeviceType 	device_type;	// Maybe remove if using Hub, Node, and Repeater become subclasses of LoomManager


	// Sub Managers
	Loom_Interrupt_Manager*	interrupt_manager;
	Loom_Sleep_Manager* 	sleep_manager;

	// Arrays of Loom Modules, categorized by type
	LoomModule*			other_modules[MAX_OTHER_MODULES];
	LoomSensor*			sensor_modules[MAX_SENSORS];
	LoomActuator*		actuator_modules[MAX_ACTUATORS];
	LoomRTC*			rtc_modules[MAX_RTCS];
	LoomCommPlat*		comm_modules[MAX_COMMS];
	LoomInternetPlat*	internet_modules[MAX_INTERNETS];
	LoomLogPlat*		log_modules[MAX_LOGS];

	uint other_module_count;
	uint sensor_count;
	uint actuator_count;
	uint rtc_count;
	uint comm_count;
	uint internet_count;
	uint log_count;


	Verbosity print_verbosity;		// Print verbosity
	Verbosity package_verbosity;	// Package verbosity 

	OSCBundle bundle;	// Not sure if this will always work...

public:


	const static char* enum_device_type_string(DeviceType t);


	// --- CONSTRUCTOR ---
								// The parameters to the constructor will be defined in config
	LoomManager(char* device_name 	= "Default",
				char* family 		= "Loom",
				uint  family_num 	= 1,
				uint  instance 		= 1,

				DeviceType device_type	= DeviceType::NODE,

				Verbosity print_verbosity 	= Verbosity::V_HIGH,
				Verbosity package_verbosity	= Verbosity::V_LOW    // Set high to include non sensors in package 
			  );

	// --- DESTRUCTOR ---
	~LoomManager();


	void print_device_label();
	void print_config();



// // From LoomModule

// 	// Display configuration settings
// 	// void print_config() {}

// 	// Display current state
// 	void print_state() {}

// 	// void measure() {}

// 	// Append to a bundle 
// 	void package(OSCBundle& bndl, char* id_prefix) {}

// 	bool message_route(OSCMessage& msg, int address_offset) {}



	// Over loaded as to sort by module type
	void add_module(Loom_Interrupt_Manager* interrupt_manager); 
	void add_module(Loom_Sleep_Manager* sleep_manager);

	void add_module(LoomModule* module);
	void add_module(LoomSensor* sensor); 
	void add_module(LoomActuator* actuator); 
	void add_module(LoomRTC* rtc); 
	void add_module(LoomCommPlat* comm_plat); 
	void add_module(LoomInternetPlat* internet_plat); 
	void add_module(LoomLogPlat* log_plat); 

	Loom_Interrupt_Manager*	get_interrupt_manager();
	Loom_Sleep_Manager*		get_sleep_manager();

	LoomModule*			get_other_module(int idx);
	LoomSensor*			get_sensor_module(int idx);
	LoomActuator*		get_actuator_module(int idx);
	LoomRTC*			get_rtc_module(int idx);
	LoomCommPlat*		get_comm_plat_module(int idx);
	LoomInternetPlat*	get_internet_plat_module(int idx);
	LoomLogPlat*		get_log_plat_module(int idx);




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


	Verbosity get_print_verbosity();
	void set_print_verbosity(Verbosity v);

	Verbosity get_package_verbosity();
	void set_package_verbosity(Verbosity v);


// ** MAYBE IMPLEMENT THESE HERE **
// might not be needed becaused of CommPlat and other

//but probably put here, because measure and package aren't managed elsewhere

	// void measure();
	// void package();
	// void receive(); // not srue if this should take arg to specify platform
	// void send();
	// void log(Enum );   Enum SD, GOOGLE, OLED ... 
	// void sleep(); // could have default sleep behavior?

// void current_bundle(OSCBundle* bndl) ? return a stored bundle
// void print_data



	void measure();  
	void package();
	void package(OSCBundle& bndl);

	void print_current_bundle();


	void flash_LED(uint count, uint time_high, uint time_low);
	void flash_LED(uint sequence[3]);


	// Methods to set package and print verbosities all at once

private:


	void Device_Init();

	void add_module_aux(LoomModule** modules, LoomModule* module, uint& len, const int max_len);
	void list_modules_aux(LoomModule** modules, uint len, char* module_type);
	void measure_aux(LoomModule** modules, uint len);
	void package_aux(LoomModule** modules, uint len);


};


#endif // of LOOM_MANAGER_h


