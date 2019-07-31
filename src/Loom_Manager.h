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
class Loom_Neopixel; class Loom_Relay; class Loom_Servo; class Loom_Stepper;

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
class Loom_MMA8451;
class Loom_MPU6050;
class Loom_MS5803;
class Loom_SHT31D;
class Loom_TMP007;
class Loom_TSL2561;
class Loom_TSL2591;
class Loom_ZXGesture;

// SDI12
class Loom_Decagon5TM;
class Loom_DecagonGS3;

// SPI
class Loom_MAX31855;
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


#define SERIAL_BAUD		115200
#define MAX_SERIAL_WAIT	20000	/// Maximum number of milliseconds to wait for user given 'begin_serial(true)'
#define SD_CS			10


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
	uint8_t		instance;			/// The instance / channel ID within the subnet
	char		family[20];			/// The family the device belongs to
	uint8_t		family_num;			/// The subnet of the family

	/// Device type (Hub / Node)
	DeviceType	device_type;	// Maybe remove if using Hub, Node, and Repeater become subclasses of LoomManager

	// Sub Managers
	/// Pointer to an interrupt manager.
	/// Used for convenience, another pointer can exist in modules vector
	Loom_Interrupt_Manager*	interrupt_manager = nullptr;
	/// Pointer to a sleep manager.
	/// Used for convenience, another pointer can exist in modules vector
	Loom_Sleep_Manager*		sleep_manager = nullptr;
	/// RTC object pointer.
	/// Used for convenience, another pointer can exist in modules vector
	LoomRTC*				rtc_module = nullptr;

	// Vectors of Loom Modules, categorized by type
	std::vector<LoomModule*>		modules;		/// Miscellaneous modules

	Verbosity	print_verbosity;		/// Print detail verbosity
	Verbosity	package_verbosity;		/// Package detail verbosity

	StaticJsonDocument<1500> doc;		/// Json data

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
			const char*		device_name			= "Device",
			const char*		family				= "Loom",
			uint8_t			family_num			= 1,
			uint8_t			instance			= 1,
			DeviceType		device_type			= DeviceType::NODE,
			Verbosity		print_verbosity		= Verbosity::V_HIGH,
			Verbosity		package_verbosity	= Verbosity::V_LOW
		);

	//// Destructor
	virtual ~LoomManager();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Begin LED
	void		begin_LED();

	/// Begin Serial, optionally wait for user.
	/// \param[in]	wait_for_monitor	True to wait for serial monitor to open
	void		begin_serial(bool wait_for_monitor = false);

	/// Parse a JSON configuration string specifying enabled modules.
	/// Enabled modules are instantiated with specified settings
	/// and added to manager lists for managing
	/// \param[in]	json_config		Configuration
	/// \return True if success
	bool		parse_config(const char* json_config);
	
	/// Parse a JSON configuration on SD card specifying enabled modules.
	/// Enabled modules are instantiated with specified settings
	/// and added to manager lists for managing
	/// \param[in]	json_config		Configuration
	/// \return True if success
	bool		parse_config_SD(const char* config_file);

	/// Parse a JSON configuration object specifying enabled modules.
	/// Enabled modules are instantiated with specified settings
	/// and added to manager lists for managing.
	/// Called by parse_config and parse_config_SD
	/// \param[in]	json_config		Configuration
	/// \return True if success
	bool		parse_config_json(JsonObject config);

	/// Get complete configuration of the device.
	/// Generally used to save configuration to SD
	void		get_config();


	/// Measure data of all managed sensors
	void		measure();  

	/// Package data of all modules into provide JsonObject.
	/// How detailed data is can be modified with package_verbosity
	/// \param[out]	json	JsonObject of packaged data of enabled modules
	void		package(JsonObject json);

	/// Measure and package data.
	/// Convenience function, current just calls measure then package
	void		record();

	/// Package data of all modules into JsonObject and return
	/// \return JsonObject of packaged data of enabled modules
	JsonObject	package();

	/// Publish
	/// \param[in]	json	Data object to publish
	/// \return True if success
	bool		publish_all(const JsonObject json);

	/// Publish.
	/// Calls publish_all(const JsonObject json) with interal json
	/// \return True if success
	bool		publish_all() { publish_all(internal_json()); }

	/// Iterate over list of commands, forwarding to handling module
	/// \param[in] json		Object containing commands
	void		dispatch(JsonObject json);

	/// Iterate over list of commands, forwarding to handling module.
	/// Uses internal json
	void		dispatch();

	/// Pause for up to 16 seconds.
	/// You can use this instead of delay to put the device into a 
	/// semi-low power state.
	/// Use Loom_Sleep_Manager for extended, complete low-power sleep.
	/// \param[in]	ms	Number of milliseconds to pause
	void		pause(uint16_t ms);

	/// Iterate over modules, calling power up method
	void 		power_up();

	/// Iterate over modules, calling power down method
	void 		power_down();

	/// Append to a Json object of data.
	/// If object is non-empty and contains non-data, 
	/// will not add and will return false.
	/// Only call this after package, otherwise the data will be overriden
	/// \param[in]	module	Which module to add data to (will create if it doesn't exist) 
	/// \param[in]	key		Key of data to add
	/// \param[in]	val		Value of data to add
	/// \return True if success
	template<typename T>
	bool add_data(const char* module, const char* key, T val)
	{
		if ( doc.isNull() ) {
			doc["type"] = "data";
		} 
		JsonObject json = doc.as<JsonObject>();
		if (strcmp(json["type"], "data") == 0 ) {
			package_json(json, module, key, val);
			return true;
		} else {
			return false;
		}
	}

	/// Get a data value from Json object of data
	/// \param[in]	module	LoomModule key is associated with
	/// \param[in]	key		Key of data value to find
	/// \return Data value if found 
	template<typename T>
	T get_data_as(const char* module, const char* key)
	{
		JsonObject json = doc.as<JsonObject>();
		JsonArray contents = json["contents"];
		if (!contents.isNull()) {
			for (auto module_block : contents) {
				if ( strcmp(module_block["module"], module) == 0 ) {
					return module_block["data"][key].as<T>();
				}
			}
		}
		return (T)0;
	}

	/// Save current configuration to SD.
	/// \param[in]	config_file		File to save configuration to
	/// \return True is success, false if fail or file not found
	// bool		save_SD_config(const char* config_file);


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================
	
	/// Print the devices current configuration.
	/// Lists modules. Optionally also prints 
	/// configuration of linked modules.
	void		print_config(bool print_modules_config = false);
	
	// 	void print_state()

	/// Print the linked modules
	void 		list_modules();

	/// Print out the internal JSON object
	void		display_data();

//=============================================================================
///@name	ADD MODULE TO MANAGER
/*@{*/ //======================================================================

	/// Add a module to be managed.
	void		add_module(LoomModule* module);

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get device type
	/// \return Device type (Hub/Node)
	DeviceType	get_device_type() { return device_type; }

	/// Return reference to internal json object
	/// \param[in]	clear	Whether or not to empty Json before returning it
	/// \return Reference to internal json object
	JsonObject	internal_json(bool clear = false);

	/// Get the device name, copies into provided buffer.
	/// \param[out]	buf		The buffer copy device name into
	void 		get_device_name(char* buf);
	
	/// Get the device name
	/// \return String literal of device name.
	const char*	get_device_name();

	/// Get device family name.
	/// \return Family name
	const char*	get_family() { return family; }

	/// Get device family number.
	/// \return Family number
	uint8_t		get_family_num() { return family_num; }

	/// Get device instance number.
	/// \return Family number
	uint8_t		get_instance_num() { return instance; }

	/// Get print verbosity.
	/// \return print verbosity
	Verbosity	get_print_verbosity() { return print_verbosity; }

	/// Get package verbosity.
	/// \return package verbosity
	Verbosity	get_package_verbosity() { return package_verbosity; }

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
	void		set_family_num(uint8_t n) { family_num = n; }
	
	/// Set device instance number.
	/// \param[in]	n	New instance number
	void		set_instance_num(uint8_t n) { instance = n; }

	/// Set print verbosity.
	/// \param[in]	v			New print verbosity
	/// \param[in]	set_modules	Whether or not to also apply setting to modules
	void		set_print_verbosity(Verbosity v, bool set_modules = false);
		
	/// Set package verbosity.
	/// \param[in]	v	New package verbosity
	/// \param[in]	set_modules	Whether or not to also apply setting to modules
	void		set_package_verbosity(Verbosity v, bool set_modules = false);

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Flash the built in LED
	/// \param[in]	count		Number of times to flash
	/// \param[in]	time_high	Milliseconds to stay on for 
	/// \param[in]	time_low	Milliseconds to stay off for 
	void		flash_LED(uint8_t count, uint8_t time_high, uint8_t time_low, bool end_high=false);
	void		flash_LED(uint8_t sequence[3]);

	/// Get c-string of name associated with device type enum
	/// \return C-string of device type
	const static char* enum_device_type_string(DeviceType t);

//=============================================================================
///@name	MODULE ACCESS 
/*@{*/ //======================================================================

	// Other
	Loom_Sleep_Manager&			SleepManager(uint8_t idx = 0);
	Loom_Interrupt_Manager&		InterruptManager(uint8_t idx = 0);
	Loom_Multiplexer&			Multiplexer(uint8_t idx = 0);

	// CommPlats
	Loom_LoRa&			LoRa(uint8_t idx = 0);
	Loom_nRF&			nRF(uint8_t idx = 0);
	Loom_Bluetooth& 	Bluetooth(uint8_t idx = 0); 

	// Loom_SlipSerial&	SlipSerial(uint8_t idx = 0);

	// LogPlats
	Loom_OLED&			OLED(uint8_t idx = 0);
	Loom_SD&			SDCARD(uint8_t idx = 0);

	// InternetPlats
	LoomInternetPlat&	InternetPlat(const uint8_t idx = 0);

	// PublishPlats
	LoomPublishPlat& 	PublishPlat(const uint8_t idx = 0);

	// RTC
	Loom_DS3231&		DS3231(uint8_t idx = 0);
	Loom_PCF8523&		PCF8523(uint8_t idx = 0);

	// Actuators
	Loom_Neopixel&		Neopixel(uint8_t idx = 0);
	Loom_Relay&			Relay(uint8_t idx = 0);
	Loom_Servo&			Servo(uint8_t idx = 0);
	Loom_Stepper&		Stepper(uint8_t idx = 0);

	// Sensors
	Loom_Analog&		Analog(uint8_t idx = 0);
	Loom_Digital&		Digital(uint8_t idx = 0);

	// I2C
	Loom_AS7262&		AS7262(uint8_t idx = 0);
	Loom_AS7263&		AS7263(uint8_t idx = 0);
	Loom_AS7265X&		AS7265X(uint8_t idx = 0);
	Loom_FXAS21002&		FXAS21002(uint8_t idx = 0);
	Loom_FXOS8700&		FXOS8700(uint8_t idx = 0);
	Loom_LIS3DH&		LIS3DH(uint8_t idx = 0);
	Loom_MB1232&		MB1232(uint8_t idx = 0);
	Loom_MMA8451&		MMA8451(uint8_t idx = 0);
	Loom_MPU6050&		MPU6050(uint8_t idx = 0);
	Loom_MS5803&		MS5803(uint8_t idx = 0);
	Loom_SHT31D&		SHT31D(uint8_t idx = 0);
	Loom_TMP007&		TMP007(uint8_t idx = 0);
	Loom_TSL2561&		TSL2561(uint8_t idx = 0);
	Loom_TSL2591&		TSL2591(uint8_t idx = 0);
	Loom_ZXGesture&		ZXGesture(uint8_t idx = 0);

	// SDI12
	Loom_Decagon5TM&	Decagon5TM(uint8_t idx = 0);
	Loom_DecagonGS3&	DecagonGS3(uint8_t idx = 0);

	// SPI
	Loom_MAX31855&		MAX31855(uint8_t idx = 0);
	Loom_MAX31856&		MAX31856(uint8_t idx = 0);

protected:

	/// Print the device name as '[device_name]'
	void				print_device_label();

private:

	// Allow secondary managers to access private members of LoomManager
	friend class Loom_Interrupt_Manager;
	friend class Loom_Sleep_Manager;
	friend class Loom_SD;
	friend class LoomNTPSync;

	Loom_Interrupt_Manager*	get_interrupt_manager() { return interrupt_manager; }
	Loom_Sleep_Manager*		get_sleep_manager() { return sleep_manager; }
	LoomRTC*				get_rtc_module() { return rtc_module; }

	/// Used to add device info to data object
	void add_device_ID_to_json(JsonObject json);

	///////////////////////////////////////////////////////////////////////////
	/// Auxiliary function to search a list of modules for a module of specified type
	/// \param[in]	type	Type to search for
	LoomModule*	find_module(LoomModule::Type type, uint8_t idx);

	/// Free modules.
	/// Used in destructor or when switching configuration
	void free_modules();

};


