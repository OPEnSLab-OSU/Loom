///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Manager.h
/// @brief		File for LoomManager definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Misc.h"

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
class Loom_ADS1115;
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
class Loom_STEMMA;

// SDI12
class Loom_Decagon5TM;
class Loom_DecagonGS3;

// SPI
class Loom_MAX31855;
class Loom_MAX31856;

// SERIAL
class Loom_K30;

// CommPlats
class Loom_LoRa;
class Loom_nRF;
class Loom_Bluetooth;

// LogPlats
class Loom_OLED;
class Loom_SD;
class Loom_BatchSD;

// InternetPlats
class LoomInternetPlat;
class Loom_Ethernet;
class Loom_WiFi;
class Loom_LTE;

// PublishPlats
class LoomPublishPlat;
class Loom_SpoolPublish;
class Loom_GoogleSheets;
class Loom_MaxPub;

// SubscribePlats
class LoomSubscribePlat;
class Loom_MaxSub;

// RTC
class Loom_DS3231;
class Loom_PCF8523;

// Other
class Loom_WarmUp_Manager;
class Loom_Sleep_Manager;
class Loom_Interrupt_Manager;
class Loom_Multiplexer;
class LoomNTPSync;
class LoomTempSync;


class FactoryBase;



#define SERIAL_BAUD		115200	///< Serial Baud Rate
#define MAX_SERIAL_WAIT	20000	///< Maximum number of milliseconds to wait for user given 'begin_serial(true)'
#define SD_CS			10		///< SD chip select used in parse_config_SD().
								///< You can still instantiate a Loom_SD module with a different chip select


///////////////////////////////////////////////////////////////////////////////
///
/// Manager to contain Loom modules and provide users with a simpler API.
///
/// @par Resources
/// - [LoomManager Documentation](https://openslab-osu.github.io/Loom/html/class_loom_manager.html)
///
///////////////////////////////////////////////////////////////////////////////
class LoomManager
{

private:

	const FactoryBase* Factory;

public:

	/// Different general types of devices
	enum class DeviceType {
		HUB, 		///< Central device
		NODE, 		///< Data collecting / actuating node
		REPEATER 	///< Forwards messages between other devices
	};

protected:

	char		device_name[20];	///< The name of the device
	uint8_t		instance;			///< The instance / channel ID within the subnet

	uint16_t	interval;			///< Default value for pause()
									///< Used so that manager can control interval, rather than code in .ino

	/// Device type (Hub / Node)
	DeviceType	device_type;	// Maybe remove if using Hub, Node, and Repeater become subclasses of LoomManager

	// Sub Managers, so placed here for ease of access.
	Loom_WarmUp_Manager* warmup_manager = nullptr;
	Loom_Interrupt_Manager*	interrupt_manager = nullptr;
	Loom_Sleep_Manager*		sleep_manager = nullptr;
	LoomRTC*				rtc_module = nullptr;

	/// Vectors of LoomModule pointers
	std::vector<LoomModule*>		modules;

	Verbosity	print_verbosity;		///< Print detail verbosity
	Verbosity	package_verbosity;		///< Package detail verbosity

	StaticJsonDocument<2000> doc;		///< Json data

	uint		packet_number = 1;		///< Packet number, incremented each time package is called

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Loom Manager constructor.
	///
	/// @param[in]	device_name					String | <"Default"> | null | Manager name
	/// @param[in]	instance					Int | <1> | [0-99] | Device instance number on its subnet
	/// @param[in]	device_type					Set(DeviceType) | <1> | {0("Hub"), 1("Node"), 2("Repeater")} | Device's topological type
	/// @param[in]	print_verbosity				Set(Verbosity) | <1> | {0("Off"), 1("Low"), 2("High")} | How detailed prints to the Serial Monitor should be
	/// @param[in]	package_verbosity			Set(Verbosity) | <2> | {0("Off"), 1("Low"), 2("High")} | How detailed to package data
	/// @param[in]	interval					Int | <1> | [0-60000] | Default milliseconds to pause
	LoomManager(
			const FactoryBase*		factory_ptr,
			const char*			device_name			= "Device",
			const uint8_t		instance			= 1,
			const DeviceType	device_type			= DeviceType::NODE,
			const Verbosity		print_verbosity		= Verbosity::V_HIGH,
			const Verbosity		package_verbosity	= Verbosity::V_LOW,
			const uint16_t		interval			= 1000
		);

	//// Destructor
	virtual ~LoomManager();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Begin LED
	void		begin_LED() const { pinMode(LED_BUILTIN, OUTPUT);}

	/// Begin Serial, optionally wait for user.
	/// @param[in]	wait_for_monitor	True to wait for serial monitor to open
	void		begin_serial(const bool wait_for_monitor = false) const;

	/// Parse a JSON configuration string specifying enabled modules.
	/// Enabled modules are instantiated with specified settings
	/// and added to manager lists for managing
	/// @param[in]	json_config		Configuration
	/// @return True if success
	bool		parse_config(const char* json_config);

	/// Parse a JSON configuration on SD card specifying enabled modules.
	/// Enabled modules are instantiated with specified settings
	/// and added to manager lists for managing.
	/// Json should be on a single line in a .txt file.
	/// @param[in]	json_config		Configuration
	/// @return True if success
	bool		parse_config_SD(const char* config_file);



	bool		parse_config_serial();
	bool		check_serial_for_config();



	/// Parse a JSON configuration object specifying enabled modules.
	/// Enabled modules are instantiated with specified settings
	/// and added to manager lists for managing.
	/// Called by parse_config and parse_config_SD
	/// @param[in]	json_config		Configuration
	/// @return True if success
	bool		parse_config_json(JsonObject config);

	/// Get complete configuration of the device.
	/// Generally used to save configuration to SD
	void		get_config();

	/// Measure data of all managed sensors
	void		measure();

	/// Package data of all modules into provide JsonObject.
	/// How detailed data is can be modified with package_verbosity
	/// @param[out]	json	JsonObject of packaged data of enabled modules
	void		package(JsonObject json);

	/// Measure and package data.
	/// Convenience function, current just calls measure then package
	void		record() { measure(); package(); }

	/// Package data of all modules into JsonObject and return
	/// @return JsonObject of packaged data of enabled modules
	JsonObject	package();

	/// Publish
	/// @param[in]	json	Data object to publish
	/// @return True if success
	bool		publish_all(const JsonObject json);

	/// Publish.
	/// Calls publish_all(const JsonObject json) with interal json
	/// @return True if success
	bool		publish_all() { return publish_all(internal_json()); }

	/// Log
	/// @param[in]	json	Data object to log
	/// @return True if success
	bool		log_all(const JsonObject json);

	/// Log.
	/// Calls log_all(const JsonObject json) with interal json
	/// @return True if success
	bool		log_all() { return log_all(internal_json()); }

	/// Iterate over list of commands, forwarding to handling module
	/// @param[in] json		Object containing commands
	void		dispatch(JsonObject json);

	/// Iterate over list of commands, forwarding to handling module.
	/// Uses internal json
	void		dispatch() { dispatch( internal_json() ); }

	/// Delay milliseconds.
	void		pause(const uint16_t ms) const { delay(ms); }

	/// Delay milliseconds based on interval member.
	/// Uses interval member as value
	void		pause() const { delay(interval); }

	/// Iterate over modules, calling power up method
	void 		power_up();

	/// Iterate over modules, calling power down method
	void 		power_down();

	/// Append to a Json object of data.
	/// If object is non-empty and contains non-data,
	/// will not add and will return false.
	/// Only call this after package, otherwise the data will be overriden
	/// @param[in]	module	Which module to add data to (will create if it doesn't exist)
	/// @param[in]	key		Key of data to add
	/// @param[in]	val		Value of data to add
	/// @return True if success
	template<typename T>
	bool add_data(const char* module, const char* key, const T val)
	{
		if ( doc.isNull() ) {
			doc["type"] = "data";
		}
		JsonObject json = doc.as<JsonObject>();
		if (strcmp(json["type"], "data") == 0 ) {
			JsonObject data = get_module_data_object(json, module);
			data[key] = val;
			// print an error if the JSON is full
			if (doc.memoryUsage() > doc.capacity() - 10)
				LPrintln("Warning: JSON document overflowed! Loom does not have enough memory for the number of sensors you are reading, please use less sensors or submit an issue.");
			return true;
		} else {
			return false;
		}
	}

	/// Get a data value from Json object of data
	/// @param[in]	module	LoomModule key is associated with
	/// @param[in]	key		Key of data value to find
	/// @return Data value if found
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
	/// @param[in]	config_file		File to save configuration to
	/// @return True is success, false if fail or file not found
	// bool		save_SD_config(const char* config_file);

	/// Determine if the manager has a module of the specified type
	///	@param[in]	type	Module type to check for
	bool		has_module(const LoomModule::Type type) const;


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	/// Print the devices current configuration.
	/// Lists modules. Optionally also prints
	/// configuration of linked modules.
	void		print_config(const bool print_modules_config = false);

	// 	void print_state()

	/// Print the linked modules
	void 		list_modules() const;

	/// Print out the internal JSON object
	void		display_data() const;

//=============================================================================
///@name	ADD MODULE TO MANAGER
/*@{*/ //======================================================================

	/// Add a module to be managed.
	void		add_module(LoomModule* module);

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get device type
	/// @return Device type (Hub/Node)
	DeviceType	get_device_type() const { return device_type; }

	/// Return reference to internal json object
	/// @param[in]	clear	Whether or not to empty Json before returning it
	/// @return Reference to internal json object
	JsonObject	internal_json(const bool clear = false);

	/// Get the device name, copies into provided buffer.
	/// @param[out]	buf		The buffer copy device name into
	void 		get_device_name(char* buf) const;

	/// Get the device name
	/// @return String literal of device name.
	const char*	get_device_name() const;

	/// Get device instance number.
	/// @return Family number
	uint8_t		get_instance_num() const { return instance; }

	/// Get print verbosity.
	/// @return print verbosity
	Verbosity	get_print_verbosity() const { return print_verbosity; }

	/// Get package verbosity.
	/// @return package verbosity
	Verbosity	get_package_verbosity() const { return package_verbosity; }

	/// Get sampling interval.
	/// @return sampling interval, in ms
	uint16_t 	get_interval() const { return interval; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set the device name.
	/// @param[in]	device_name		The new device name
	void 		set_device_name(const char* device_name);

	/// Set device instance number.
	/// @param[in]	n	New instance number
	void		set_instance_num(const uint8_t n) { instance = n; }

	/// Set print verbosity.
	/// @param[in]	v			New print verbosity
	/// @param[in]	set_modules	Whether or not to also apply setting to modules
	void		set_print_verbosity(const Verbosity v, const bool set_modules = false);

	/// Set package verbosity.
	/// @param[in]	v	New package verbosity
	/// @param[in]	set_modules	Whether or not to also apply setting to modules
	void		set_package_verbosity(const Verbosity v, const bool set_modules = false);

	/// Set default time to use for .pause() \ .delay().
	/// Pause and delay can still take explicit times, but if not provided,
	/// this value will be used
	void		set_interval(const uint16_t ms);

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Flash the built in LED
	/// @param[in]	count		Number of times to flash
	/// @param[in]	time_high	Milliseconds to stay on for
	/// @param[in]	time_low	Milliseconds to stay off for
	void		flash_LED(const uint8_t count, const uint8_t time_high, const uint8_t time_low, const bool end_high=false) const;
	void		flash_LED(const uint8_t sequence[3]) const { flash_LED(sequence[0], sequence[1], sequence[2]); }

	/// Get c-string of name associated with device type enum
	/// @return C-string of device type
	const static char* enum_device_type_string(const DeviceType t);

//=============================================================================
///@name	MODULE ACCESS
/*@{*/ //======================================================================

	/// Auxiliary function to search a list of modules for a module of specified type
	/// @param[in]	type	Type to search for
	LoomModule*	find_module(const LoomModule::Type type, const uint8_t idx=0) const;

	/// Auxiliary function to search a list of modules for a module of specified category
	/// @param[in]	category	Category to search for
	LoomModule*	find_module_by_category(const LoomModule::Category category, const uint8_t idx) const;

	///////////////////////////////////////////////////////////////////////////

	// Other
	Loom_WarmUp_Manager&		WarmUpManager(const uint8_t idx = 0);
	Loom_Sleep_Manager&			SleepManager(const uint8_t idx = 0);
	Loom_Interrupt_Manager&		InterruptManager(const uint8_t idx = 0);
	Loom_Multiplexer&			Multiplexer(const uint8_t idx = 0);
	LoomNTPSync&				NTP(const uint8_t idx = 0);
	LoomTempSync&				TempSync(const uint8_t idx = 0);

	// CommPlats
	Loom_LoRa&			LoRa(const uint8_t idx = 0);
	Loom_nRF&			nRF(const uint8_t idx = 0);
	Loom_Bluetooth& 	Bluetooth(const uint8_t idx = 0);

	// LogPlats
	Loom_OLED&			OLED(const uint8_t idx = 0);
	Loom_SD&			SDCARD(const uint8_t idx = 0);
	Loom_BatchSD&	BATCHSD(const uint8_t idx = 0);

	// InternetPlats
	LoomInternetPlat&	InternetPlat(const uint8_t idx = 0);
	Loom_Ethernet&		Ethernet(const uint8_t idx = 0);
	Loom_WiFi&			WiFi(const uint8_t idx = 0);
	Loom_LTE&				LTE(const uint8_t idx = 0);

	// PublishPlats
	LoomPublishPlat& 	PublishPlat(const uint8_t idx = 0);
	Loom_GoogleSheets&	GoogleSheets(const uint8_t idx = 0);
	Loom_SpoolPublish&  Spool(const uint8_t idx = 0);
	Loom_MaxPub&		MaxPub(const uint8_t idx = 0);

	// SubscribePlats
	Loom_MaxSub&		MaxSub(const uint8_t idx = 0);


	// RTC
	Loom_DS3231&		DS3231(const uint8_t idx = 0);
	Loom_PCF8523&		PCF8523(const uint8_t idx = 0);

	// Actuators
	Loom_Neopixel&		Neopixel(const uint8_t idx = 0);
	Loom_Relay&			Relay(const uint8_t idx = 0);
	Loom_Servo&			Servo(const uint8_t idx = 0);
	Loom_Stepper&		Stepper(const uint8_t idx = 0);

	// Sensors
	Loom_Analog&		Analog(const uint8_t idx = 0);
	Loom_Digital&		Digital(const uint8_t idx = 0);

	// I2C
	Loom_ADS1115&		ADS1115(const uint8_t idx = 0);
	Loom_AS7262&		AS7262(const uint8_t idx = 0);
	Loom_AS7263&		AS7263(const uint8_t idx = 0);
	Loom_AS7265X&		AS7265X(const uint8_t idx = 0);
	Loom_FXAS21002&		FXAS21002(const uint8_t idx = 0);
	Loom_FXOS8700&		FXOS8700(const uint8_t idx = 0);
	Loom_LIS3DH&		LIS3DH(const uint8_t idx = 0);
	Loom_MB1232&		MB1232(const uint8_t idx = 0);
	Loom_MMA8451&		MMA8451(const uint8_t idx = 0);
	Loom_MPU6050&		MPU6050(const uint8_t idx = 0);
	Loom_MS5803&		MS5803(const uint8_t idx = 0);
	Loom_SHT31D&		SHT31D(const uint8_t idx = 0);
	Loom_TMP007&		TMP007(const uint8_t idx = 0);
	Loom_TSL2561&		TSL2561(const uint8_t idx = 0);
	Loom_TSL2591&		TSL2591(const uint8_t idx = 0);
	Loom_ZXGesture&		ZXGesture(const uint8_t idx = 0);
    Loom_STEMMA&        STEMMA(const uint8_t idx = 0);
	// SDI12
	Loom_Decagon5TM&	Decagon5TM(const uint8_t idx = 0);
	Loom_DecagonGS3&	DecagonGS3(const uint8_t idx = 0);

	// SPI
	Loom_MAX31855&		MAX31855(const uint8_t idx = 0);
	Loom_MAX31856&		MAX31856(const uint8_t idx = 0);

    //SERIAL
    Loom_K30&           K30(const uint8_t idx = 0);

protected:

	/// Print the device name as '[device_name]'
	void				print_device_label() const { LPrint("[", device_name, "] "); }

private:

	// Allow secondary managers to access private members of LoomManager
	friend class Loom_Interrupt_Manager;
	friend class Loom_Sleep_Manager;
	friend class LoomRTC;
	friend class Loom_SD;
	friend class Loom_BatchSD;
	friend class LoomNTPSync;

	Loom_Interrupt_Manager*	get_interrupt_manager() { return interrupt_manager; }
	Loom_Sleep_Manager*		get_sleep_manager() { return sleep_manager; }
	LoomRTC*				get_rtc_module() { return rtc_module; }

	/// Used to add device info to data object
	void add_device_ID_to_json(JsonObject json);

	/// Free modules.
	/// Used in destructor or when switching configuration
	void free_modules();

	/// Run dispatch on any commands directed to the manager
	bool dispatch_self(JsonObject json);

};
