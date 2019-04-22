
#ifndef LOOM_MANAGER_h
#define LOOM_MANAGER_h


// these might need to be forward declared instead
// e.g. class LoomSensor;
// both h files included in .cpp files
// #include "loom_sensor_class.h"
// #include "loom_actuator_class.h"

#include "Loom_Misc.h"
#include "Loom_Translator.h"


// #include "Loom_Interrupt_Manager.h"
// #include "Loom_Sleep_Manager.h"
// #include "Loom_Module.h"
// #include "Sensors/Loom_Sensor.h"
// #include "Actuators/Loom_Actuator.h"
// #include "RTC/Loom_RTC.h"
// #include "CommPlats/Loom_CommPlat.h"
// #include "InternetPlats/Loom_InternetPlat.h"
// #include "LogPlats/Loom_LogPlat.h"



#define SERIAL_BAUD 115200


/// Different general types of devices 
enum class DeviceType { 
	HUB, 		///< Central device 
	NODE, 		///< Data collecting / actuating node
	REPEATER 	///< Forwards messages between other devices
};

/// Different levels of verbosity (for printing or packaging)
enum class Verbosity { 
	V_OFF,		///< Disable
	V_LOW, 		///< Minimal/Stardard
	V_HIGH 		///< Full details
}; 


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





// switch to using vectors
#define MAX_OTHER_MODULES 3
#define MAX_SENSORS       20
#define MAX_ACTUATORS     10
// #define MAX_RTCS          3
#define MAX_COMMS         3
#define MAX_INTERNETS     3
#define MAX_LOGS          5


// # () | dependencies: [] | conflicts: []
/// Manager class to simplify with enabled modules
// #
class LoomManager
{

protected:

	/// The name of the device
	// char*		device_name;	
	char		device_name[20];	
	/// The family the device belongs to
	char		family[20];			
	/// The subnet of the family
	uint		family_num;
	/// The instance / channel ID within the subnet		
	uint		instance;


	DeviceType	device_type;	// Maybe remove if using Hub, Node, and Repeater become subclasses of LoomManager


	// Sub Managers
	Loom_Interrupt_Manager*	interrupt_manager;
	Loom_Sleep_Manager*		sleep_manager;

	// RTC object
	LoomRTC*			rtc_module;

	// Arrays of Loom Modules, categorized by type
	LoomModule*			other_modules[MAX_OTHER_MODULES];
	LoomSensor*			sensor_modules[MAX_SENSORS];
	LoomActuator*		actuator_modules[MAX_ACTUATORS];
	LoomCommPlat*		comm_modules[MAX_COMMS];
	LoomInternetPlat*	internet_modules[MAX_INTERNETS];
	LoomLogPlat*		log_modules[MAX_LOGS];

	/// Count of miscellaneous modules
	uint		other_module_count;
	/// Count of sensor modules
	uint		sensor_count;
	/// Count of actuator modules
	uint		actuator_count;
	/// Count of RTC modules
	// uint		rtc_count;
	/// Count of communication modules
	uint		comm_count;
	/// Count of internet modules
	uint		internet_count;
	/// Count of logging platform modules
	uint		log_count;

	/// Print detail verbosity
	Verbosity	print_verbosity;		
	/// Package detail verbosity 
	Verbosity	package_verbosity;	


	OSCBundle	bundle;	// Not sure if this will always work...

public:

	const static char* enum_device_type_string(DeviceType t);


	/// Loom Manager constructor.
	/// 
	/// \param[in]	device_name			String | <"Default"> | Manager name
	/// \param[in]	family				String | <"Loom"> | Which family the device belongs to
	/// \param[in]	family_num			Int | <1> | [0-99] | Which family subnet the device belongs to
	/// \param[in]	instance			Int | <1> | [0-99] | Device instance number on its subnet
	/// \param[in]	type				Set(DeviceType) | <1> | {0("Hub"), 1("Node"), 2("Repeater")} | Device's topological type
	/// \param[in]	type				Set(Verbosity) | <1> | {0("Off"), 1("Low"), 2("High")} | How detailed prints to the Serial Monitor should be
	/// \param[in]	type				Set(Verbosity) | <2> | {0("Off"), 1("Low"), 2("High")} | How detailed OSC bundles are
	LoomManager(
			const char*		device_name			= "Default",
			const char*		family				= "Loom",
			uint			family_num			= 1,
			uint			instance			= 1,
			DeviceType		device_type			= DeviceType::NODE,
			Verbosity		print_verbosity		= Verbosity::V_HIGH,
			Verbosity		package_verbosity	= Verbosity::V_LOW
		);

	// --- DESTRUCTOR ---
	virtual ~LoomManager();

	/// Print the device name as '[device_name]'
	void		print_device_label();
	/// Print the devices current configuration.
	/// Also prints configuration of linked modules.
	void		print_config();


	void		parse_config();
	// maybe overload to take JsonVariant or const char* of json?


// // From LoomModule

// 	// Display configuration settings
// 	// void print_config() {}

// 	// Display current state
// 	void print_state() {}

// 	// void measure() {}

// 	// Append to a bundle 
// 	void package(OSCBundle& bndl, char* id_prefix) {}

// 	bool message_route(OSCMessage& msg, int address_offset) {}



	// Overloaded as to sort by module type
	void		add_module(Loom_Interrupt_Manager* interrupt_manager); 
	void		add_module(Loom_Sleep_Manager* sleep_manager);

	void		add_module(LoomRTC* rtc); 

	void		add_module(LoomModule* module);
	void		add_module(LoomSensor* sensor); 
	void		add_module(LoomActuator* actuator); 
	void		add_module(LoomCommPlat* comm_plat); 
	void		add_module(LoomInternetPlat* internet_plat); 
	void		add_module(LoomLogPlat* log_plat); 

	Loom_Interrupt_Manager*	get_interrupt_manager();
	Loom_Sleep_Manager*		get_sleep_manager();

	LoomRTC*				get_rtc_module();

	LoomModule*				get_other_module(int idx);
	LoomSensor*				get_sensor_module(int idx);
	LoomActuator*			get_actuator_module(int idx);
	LoomCommPlat*			get_comm_plat_module(int idx);
	LoomInternetPlat*		get_internet_plat_module(int idx);
	LoomLogPlat*			get_log_plat_module(int idx);




	// void module_enable(LoomModule* LM, bool e) ?

	/// Print the linked modules
	void 		list_modules();


	/// Set the device name.
	void 		set_device_name(char* device_name);
	/// Get the device name, copies into provided buffer.
	/// \param[out] buf	The buffer copy device name into
	void 		get_device_name(char* buf);
	/// Get the device name
	/// \return String literal of device name.
	const char*		get_device_name();



	

		// Should this return a char* or copy into char array passed by reference?
		// Could overload to do either like String does with .toCharArray() and .c_str()
	// char* packet_header() {}
	// void packet_header(char* buf) {}

	/// Copy device identification message header (to family level) string to buffer.
	/// \param[out]	buf The bufer to copy family name into
	void		packet_header_family(char* buf);
	/// Return device identification message header (to family level) string.
	/// \return	The device family string
	const char*	 packet_header_family();
	/// Copy device identification message header (to subnet level) string to buffer.
	/// \param[out] buf The bufer to copy subnet name into
	void		packet_header_subnet(char* buf);
	/// Return device identification message header (to subnet level) string.
	/// \return The device subnet string
	const char*	packet_header_subnet();
	/// Copy device identification message header (to device specific level) string to buffer. 
	/// \param[out] buf The bufer to copy device name into
	void		packet_header_device(char* buf);
	/// Return device identification message header (to device specific level) string.
	/// \return The device name and number string
	const char*	packet_header_device();


	/// Get device family name.
	/// \return Family name
	const char*	get_family();
	/// Set device family name.
	/// \param[out] family Family name
	void		set_family(const char* f);
	/// Get device family number.
	/// \return Family number
	int			get_family_num();
	/// Set device family number.
	/// \param[in] New family number
	void		set_family_num(int n);
	/// Get device instance number.
	/// \return Family number
	int			get_instance_num();
	/// Set device instance number.
	/// \param[in] New instance number
	void		set_instance_num(int n);



	DeviceType	get_device_type();
	// void set_device_type(DeviceType t) {device_type = t; }

	/// Get print verbosity.
	/// \return print verbosity
	Verbosity	get_print_verbosity();
	/// Set print verbosity.
	/// \param[in] v New print verbosity
	void		set_print_verbosity(Verbosity v);
	/// Get package verbosity.
	/// \return package verbosity
	Verbosity	get_package_verbosity();
		/// Set package verbosity.
	/// \param[in] v New package verbosity
	void		set_package_verbosity(Verbosity v);


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



	void		measure();  
	void		package();
	void		package(OSCBundle& bndl);

	void		print_current_bundle();


	void		flash_LED(uint count, uint time_high, uint time_low);
	void		flash_LED(uint sequence[3]);


	// Methods to set package and print verbosities all at once

private:



	void		add_module_aux(LoomModule** modules, LoomModule* module, uint& len, const int max_len);
	void		list_modules_aux(LoomModule** modules, uint len, char* module_type);
	void		measure_aux(LoomModule** modules, uint len);
	void		package_aux(LoomModule** modules, uint len);


};


#endif // of LOOM_MANAGER_h


