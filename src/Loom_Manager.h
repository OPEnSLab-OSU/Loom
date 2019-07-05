#pragma once

#include "Loom_Misc.h"
#include "Loom_Translator.h"
#include "Loom_Module.h"

#include <ArduinoJson.h>

// Need to undef max and min for vector to work
#undef max
#undef min
#include <vector>


// Forward declarations, specify that these classes
// exist but are defined in their own respective files
class LoomModule;
class LoomSensor;
class LoomActuator;
class LoomRTC;
class LoomCommPlat;
class LoomLogPlat;

// Actuators
class Loom_Neopixel;
class Loom_Relay;
class Loom_Servo;
class Loom_Stepper;

// Sensors
class Loom_Analog;
class Loom_Digital;

// I2C
class Loom_AS7262;
class Loom_AS7263;
class Loom_AS7265X;
class Loom_FXAS21002;
class Loom_FXOS8700;
class Loom_LIS3DH;
class Loom_MB1232;
class Loom_MPU6050;
class Loom_MS5803;
class Loom_SHT31D;
class Loom_TSL2561;
class Loom_TSL2591;
class Loom_ZXGesture;

// SDI12
class Loom_Decagon5TM;
class Loom_DecagonGS3;

// SPI
class Loom_MAX31856;

// CommPlats
class Loom_LoRa;
class Loom_nRF;
class Loom_Bluetooth;
class Loom_SlipSerial;

// LogPlats
class Loom_OLED;
class Loom_SD;

// InternetPlats
class LoomInternetPlat;

// Publish plat
class LoomPublishPlat;

// RTC
class Loom_DS3231;
class Loom_PCF8523;

// Other
class Loom_Sleep_Manager;
class Loom_Interrupt_Manager;
class Loom_Multiplexer;


#define SERIAL_BAUD 115200



///////////////////////////////////////////////////////////////////////////////


// ### () | dependencies: [] | conflicts: []
/// Manager class to simplify with enabled modules
// ###
class LoomManager
{

public:

	/// Different general types of devices
	enum class DeviceType {
		HUB, 		///< Central device
		NODE, 		///< Data collecting / actuating node
		REPEATER 	///< Forwards messages between other devices
	};

protected:


	char		device_name[20];	/// The name of the device
	char		family[20];			/// The family the device belongs to
	uint		family_num;			/// The subnet of the family
	uint		instance;			/// The instance / channel ID within the subnet


	DeviceType	device_type;	// Maybe remove if using Hub, Node, and Repeater become subclasses of LoomManager


	// Sub Managers
	Loom_Interrupt_Manager*	interrupt_manager = nullptr;
	Loom_Sleep_Manager*		sleep_manager = nullptr;

	/// RTC object pointer
	LoomRTC*			rtc_module = nullptr;

	// Vectors of Loom Modules, categorized by type
	std::vector<LoomModule*> other_modules;
	std::vector<LoomSensor*> sensor_modules;
	std::vector<LoomActuator*> actuator_modules;
	std::vector<LoomCommPlat*> comm_modules;
	std::vector<LoomInternetPlat*> internet_modules;
	std::vector<LoomPublishPlat*> publish_modules;
	std::vector<LoomLogPlat*> log_modules;

	Verbosity	print_verbosity;			/// Print detail verbosity
	Verbosity	package_verbosity;			/// Package detail verbosity

	StaticJsonDocument<2000> doc;			/// Json data

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Loom Manager constructor.
	///
	/// \param[in]	device_name					String | <"Default"> | null | Manager name
	/// \param[in]	family						String | <"Loom"> | null | Which family the device belongs to
	/// \param[in]	family_num					Int | <1> | [0-99] | Which family subnet the device belongs to
	/// \param[in]	instance					Int | <1> | [0-99] | Device instance number on its subnet
	/// \param[in]	device_type					Set(DeviceType) | <1> | {0("Hub"), 1("Node"), 2("Repeater")} | Device's topological type
	/// \param[in]	print_verbosity				Set(Verbosity) | <1> | {0("Off"), 1("Low"), 2("High")} | How detailed prints to the Serial Monitor should be
	/// \param[in]	package_verbosity			Set(Verbosity) | <2> | {0("Off"), 1("Low"), 2("High")} | How detailed to package data
	LoomManager(
			const char*		device_name			= "Default",
			const char*		family				= "Loom",
			uint			family_num			= 1,
			uint			instance			= 1,
			DeviceType		device_type			= DeviceType::NODE,
			Verbosity		print_verbosity		= Verbosity::V_HIGH,
			Verbosity		package_verbosity	= Verbosity::V_LOW
		);

	//// Destructor
	virtual ~LoomManager();

//=============================================================================
///@name	OPERATION
/// Desription
/*@{*/ //======================================================================

	/// Parse a JSON configuration specifying enabled modules.
	/// Enabled modules are instantiated with specified settings
	/// and added to manager lists for managing
	/// \param[in]	json_config		Configuration
	void		parse_config(const char* json_config);

	/// Measure data of all managed sensors
	void		measure();  

	/// Package data of all modules into provide JsonObject.
	/// How detailed data is can be modified with package_verbosity
	/// \param[out]	json	JsonObject of packaged data of enabled modules
	void		package(JsonObject json);

	/// Package data of all modules into JsonObject and return
	/// \return JsonObject of packaged data of enabled modules
	JsonObject	package();

	/// Publish
	bool		publish(const JsonObject json);

	/// Iterate over array of commands
	/// \param[in] json		Object containing commands
	void		cmd_route(JsonObject json);

//=============================================================================
///@name	ADD MODULE TO MANAGER
/*@{*/ //======================================================================

	/// Add a module to be managed.
	/// Overloaded as to sort by module type
	void		add_module(Loom_Interrupt_Manager* interrupt_manager);
	void		add_module(Loom_Sleep_Manager* sleep_manager);
	void		add_module(LoomRTC* rtc);
	void		add_module(LoomModule* module);
	void		add_module(LoomSensor* sensor);
	void		add_module(LoomActuator* actuator);
	void		add_module(LoomCommPlat* comm_plat);
	void		add_module(LoomInternetPlat* internet_plat);
	void 		add_module(LoomPublishPlat* publish_module); 
	void		add_module(LoomLogPlat* log_plat);
	
//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================
	
	/// Print the devices current configuration.
	/// Also prints configuration of linked modules.
	void		print_config();
	
	// 	void print_state()

	/// Print the linked modules
	void 		list_modules();

	/// Print out the internal JSON object
	void		print_internalJson();

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get device type
	/// \return Device type (Hub/Node)
	DeviceType	get_device_type();

	/// Return reference to internal json object
	/// \param[in]	clear	Whether or not to empty Json before returning it
	/// \return Reference to internal json object
	JsonObject	internalJson(bool clear = true);

	/// Get the device name, copies into provided buffer.
	/// \param[out]	buf		The buffer copy device name into
	void 		get_device_name(char* buf);
	
	/// Get the device name
	/// \return String literal of device name.
	const char*	get_device_name();

	/// Get device family name.
	/// \return Family name
	const char*	get_family();

	/// Get device family number.
	/// \return Family number
	int			get_family_num();

	/// Get device instance number.
	/// \return Family number
	int			get_instance_num();

	/// Get print verbosity.
	/// \return print verbosity
	Verbosity	get_print_verbosity();

	/// Get package verbosity.
	/// \return package verbosity
	Verbosity	get_package_verbosity();

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set the device name.
	/// \param[in]	device_name		The new device name
	void 		set_device_name(const char* device_name);

	/// Set device family name.
	/// \param[out]	family	Family name
	void		set_family(const char* family);
	
	/// Set device family number.
	/// \param[in]	n	New family number
	void		set_family_num(int n);
	
	/// Set device instance number.
	/// \param[in]	n	New instance number
	void		set_instance_num(int n);

	/// Set print verbosity.
	/// \param[in]	v	New print verbosity
	void		set_print_verbosity(Verbosity v);
		
	/// Set package verbosity.
	/// \param[in]	v	New package verbosity
	void		set_package_verbosity(Verbosity v);

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	void		flash_LED(uint count, uint time_high, uint time_low);
	void		flash_LED(uint sequence[3]);

	/// Get c-string of name associated with device type enum
	/// \return C-string of device type
	const static char* enum_device_type_string(DeviceType t);

// these might become obsolete
	Loom_Interrupt_Manager*	get_interrupt_manager();
	Loom_Sleep_Manager*		get_sleep_manager();
	LoomRTC*				get_rtc_module();



	// maybe overload to take JsonVariant or const char* of json?



//but probably put here, because measure and package aren't managed elsewhere

	// void measure();
	// void package();
	// void receive(); // not sure if this should take arg to specify platform
	// void send();
	// void log(Enum );   Enum SD, GOOGLE, OLED ... 
	// void sleep(); // could have default sleep behavior?
	// bool		log()


	// Methods to set package and print verbosities all at once





//=============================================================================
///@name	MODULE ACCESS 
/*@{*/ //======================================================================

	// Other
	Loom_Sleep_Manager&			SleepManager(int idx = 0);
	Loom_Interrupt_Manager&		InterruptManager(int idx = 0);
	Loom_Multiplexer&			Multiplexer(int idx = 0);

	// CommPlats
	Loom_LoRa&			LoRa(int idx = 0);
	Loom_nRF&			nRF(int idx = 0);
	Loom_Bluetooth& 	Bluetooth(int idx = 0); 

	// Loom_SlipSerial&	SlipSerial(int idx = 0);

	// LogPlats
	Loom_OLED&			OLED(int idx = 0);
	Loom_SD&			SDCARD(int idx = 0);

	// InternetPlats
	LoomInternetPlat&	InternetPlat(const int idx = 0);

	// PublishPlats
	LoomPublishPlat& 	PublishPlat(const int idx = 0);

	// RTC
	Loom_DS3231&		DS3231(int idx = 0);
	Loom_PCF8523&		PCF8523(int idx = 0);

	// Actuators
	Loom_Neopixel&		Neopixel(int idx = 0);
	Loom_Relay&			Relay(int idx = 0);
	Loom_Servo&			Servo(int idx = 0);
	Loom_Stepper&		Stepper(int idx = 0);

	// Sensors
	Loom_Analog&		Analog(int idx = 0);
	Loom_Digital&		Digital(int idx = 0);

	// I2C
	Loom_AS7262&		AS7262(int idx = 0);
	Loom_AS7263&		AS7263(int idx = 0);
	Loom_AS7265X&		AS7265X(int idx = 0);
	Loom_FXAS21002&		FXAS21002(int idx = 0);
	Loom_FXOS8700&		FXOS8700(int idx = 0);
	Loom_LIS3DH&		LIS3DH(int idx = 0);
	Loom_MB1232&		MB1232(int idx = 0);
	Loom_MPU6050&		MPU6050(int idx = 0);
	Loom_MS5803&		MS5803(int idx = 0);
	Loom_SHT31D&		SHT31D(int idx = 0);
	Loom_TSL2561&		TSL2561(int idx = 0);
	Loom_TSL2591&		TSL2591(int idx = 0);
	Loom_ZXGesture&		ZXGesture(int idx = 0);

	// SDI12
	Loom_Decagon5TM&	Decagon5TM(int idx = 0);
	Loom_DecagonGS3&	DecagonGS3(int idx = 0);

	// SPI
	Loom_MAX31856&		MAX31856(int idx = 0);

protected:

	/// Print the device name as '[device_name]'
	void		print_device_label();

private:

	/// Add module to a list of modules
	template<typename T>
	bool add_module_aux(std::vector<T>& modules, const T module) 
	{
		print_device_label();

		if (module == nullptr) {
			LPrintln("Cannot add null module");
			return false;
		}

		LPrintln("Adding Module: ", ((LoomModule*)module)->get_module_name() );

		modules.emplace_back(module);
		module->link_device_manager(this);
		return true;	
	}

	/// Auxiliary function for printing a list of modules
	template<typename T>
	void list_modules_aux(const std::vector<T>& modules, const char* module_type)
	{
		LPrintln("\t", module_type, " (", modules.size(), "):");
		for (auto module : modules) {
			if ( (module != nullptr) && ( ((LoomModule*)module)->get_active()) ) {
				LPrintln( "\t\t[", ( ((LoomModule*)module)->get_active()) ? "+" : "-" , "] ", ((LoomModule*)module)->get_module_name() );
			}
		}	
	}

	/// Auxiliary function for packaging data of a list of modules
	template<typename T>
	void package_aux(JsonObject json, const std::vector<T>& modules)
	{
		for (auto module : modules) {
			if ( (module != nullptr) && ( ((LoomModule*)module)->get_active() ) ){
				((LoomModule*)module)->package( json );
			}
		}	
	}

	/// Auxiliary function for packaging data of a single module
	void package_aux(JsonObject json, LoomModule* module);
	
	/// Have each module check against provided command
	template<typename T>
	bool cmd_route_aux(JsonObject json, const std::vector<T>& modules)
	{
		for (auto module : modules) {
			if ( (module != nullptr) && ( ((LoomModule*)module)->get_active() ) ){
				if ( ((LoomModule*)module)->cmd_route( json ) ) return true;
			}
		}
		return false;
	}
	
	/// Have module check against provided command	
	bool cmd_route_aux(JsonObject json, LoomModule* module);

	template<typename T>
	void second_stage_ctor_aux(const std::vector<T>& modules)
	{
		for (auto module : modules) {
			if ( (module != nullptr) && ( ((LoomModule*)module)->get_active() ) ){
				((LoomModule*)module)->second_stage_ctor();
			}
		}	
	}

	/// Auxiliary function to search a list of modules for a module of specified type
	/// \param[in]	type	Type to search for
	template<typename T>
	LoomModule*	find_module(LoomModule::Type type, int idx, const std::vector<T>& modules)
	{
		int current = 0;

		for (auto module : modules) {
			if (type == ((LoomModule*)module)->get_module_type()) {
				if (current == idx) {
					return (LoomModule*)module;
				} else {
					current++;
				}
			}
		}
		return nullptr;
	}


};


