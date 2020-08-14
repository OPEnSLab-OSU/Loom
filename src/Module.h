///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Module.h
/// @brief		File for LoomModule definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Misc.h"
#include "Macros.h"
#include "Package.h"

#include <ArduinoJson.h>

class LoomManager; // Specify that LoomManager exists, defined in own file


///////////////////////////////////////////////////////////////////////////////
///
/// Abstract root of Loom component modules inheritance hierarchy.
/// All modules in Loom inherit from LoomModule
///
/// @par Resources
/// - [LoomModule Documentation](https://openslab-osu.github.io/Loom/html/class_loom_module.html)
///
///////////////////////////////////////////////////////////////////////////////
class LoomModule
{

public:

	/// Enum to check against to when finding individual component
	/// managed by a LoomManager.
	/// Used because we cannot use dynamic_cast to check type of modules
	/// (rtti disabled by Arduino IDE)
	enum class Type {
		Unknown = 0,
		// Other
		Other=1000,			Interrupt_Manager, Sleep_Manager, Multiplexer, NTP, TempSync, WarmUp_Manager,
		// Sensors
		Sensor=2000,		Analog, Digital,
		// I2C
		I2C=2100,			ADS1115, AS7262, AS7263, AS7265X, FXAS21002, FXOS8700, LIS3DH, MB1232, MMA8451, MPU6050, MS5803, SHT31D, TMP007, TSL2561, TSL2591, ZXGesture, STEMMA,
		// SDI12
		SDI12=2200,			Decagon5TM, DecagonGS3,
		// SPI
		SPI=2300,			MAX31855, MAX31856,
        // SERIAL
        L_SERIAL=2400,        K30,
		// RTC
		L_RTC=3000,			DS3231, PCF8523,
		// Actuators
		Actuator=4000,		Neopixel, Relay, Servo, Stepper,
		// LogPlats
		LogPlat=5000,		OLED, SDCARD, BATCHSD,
		// CommPlats
		CommPlat=6000,		LoRa, nRF, Bluetooth,
		// InternetPlats
		InternetPlat=7000,	Ethernet, WiFi, LTE,
		// PublishPlats
		PublishPlat=8000,	GoogleSheets, MaxPub, SpoolPub,
		// SubscribePlats
		SubscribePlats=9000, MaxSub
	};

	/// Enum to classify modules.
	/// Similar LoomModule::Type but represents the astract classes rather than
	/// the leaf node modules in the inheritance tree.
	/// For a given module, its category often shares a name with the abstract
	/// class it is derived from
	enum class Category {
		Unknown=0,			///< Unknown
		Other=1,			///< Other
		Sensor=2,			///< Sensors
		L_RTC=3,			///< RTC
		Actuator=4,			///< Actuators
		LogPlat=5,			///< LogPlats
		CommPlat=6,			///< CommPlats
		InternetPlat=7,		///< InternetPlats
		PublishPlat=8,		///< PublishPlats
		SubscribePlat=9		///< SubscribePlats
	};

protected:

	const Type		module_type;		///< Module type
	LoomManager*	device_manager;		///< Pointer to manager.
										///< LoomManager provides to any modules passed to add_module
	const String	module_name_base;	///< The name of the module (Should have a DEFAULT but can be overriden if provided to constructor)
	const char* 	module_name;
	bool			active;				///< Whether or not the module should be treated as active.
										///< If inactive at setup (due to failed initialization, module will be deleted)
	Verbosity		print_verbosity;	///< Print verbosity
	Verbosity		package_verbosity;	///< Package verbosity

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// @param[in]	module_name		Name of the module (provided by derived classes)
	/// @param[in]	module_type		Type of the module (provided by derived classes)
	LoomModule(
		LoomManager* manager,
		const char*		module_name		= "Unknown",
		const Type		module_type		= Type::Unknown
	);

	/// Destructor
	virtual ~LoomModule() = default;

	/// Second stage construction.
	/// Perform any actions that need all of the devices to be
	/// initized beforehand - such as looking up an internet platform
	/// This function is guranteed to be called AFTER all modules have been
	/// constructed.
	virtual void	second_stage_ctor() {}

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Package a modules measurements or state.
	/// @param[out]	json	Object to put data into
	virtual void 	package(JsonObject json) = 0;

	/// Route command to driver
	virtual bool	dispatch(JsonObject json) {};

	/// Turn off any hardware
	virtual void	power_down() {}

	/// Turn on any hardware
	virtual void	power_up() {}

	/// Add configuration information to JsonObject.
	/// LoomManager iterates over modules to build complete configuration
	/// @param[in]	json	Json configuration object to add to
	virtual void	add_config(JsonObject json) {}//= 0;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	/// Display the configuration settings of the module
	virtual void	print_config() const;

	/// Display current state of the module
	virtual void	print_state() const;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get module type
	/// @return Module type
	Type			get_module_type() const { return module_type; }

	/// Get the device manager class if linked
	/// @return Pointer to the LoomManager, Null if not linked
	LoomManager*	get_device_manager() const { return device_manager; }

	/// Copy module name into buffer
	/// @param[out]	buf	The buffer to copy module name into
	void			get_module_name(char* buf) const;

	/// Get module name
	/// @return	Module name
	const char*		get_module_name() const { return module_name; }

	/// Get print verbosity
	/// @return		The current verbosity setting
	Verbosity		get_print_verbosity() const { return print_verbosity; }

	/// Get package verbosity
	/// @return		The current verbosity setting
	Verbosity		get_package_verbosity() const { return package_verbosity; }

	/// Get whether or not the module should be treated as active
	/// @return		Whether or not the module is active
	bool			get_active() const { return active; }

	/// Get the category of the module.
	Category		category() const;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Add pointer back to device manager.
	/// Generally only called when device manager links module
	/// to provide pointer both directions.
	/// Derived modules may override this for increased function,
	/// such as linking a submanager or RTC module.
	/// @param[in]	LM	LoomManager to point to
	virtual void	link_device_manager(LoomManager* LM);

	/// Set print verbosity
	/// Controlls level of detail included in debug prints
	void			set_print_verbosity(const Verbosity v);

	/// Set package verbosity.
	/// Controlls level of detail included in bundles
	/// @param[in]	v	The verbosity setting
	void			set_package_verbosity(const Verbosity v);

	/// Set whether or not the module should be treated as active
	/// @param[in]	enable	Whether or not to enable module
	void			set_active(const bool enable) { active = enable; }

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Get string of name associated with verbosity enum
	/// @param[in]	v	Verbosity value to get string representation of
	/// @return String of verbosity
	static const char*	enum_verbosity_string(const Verbosity v);

	/// Get string of the category associated with a Category
	/// @param[in]	c	Category value to get string representation of
	/// @return String of category
	static const char*	enum_category_string(const Category c);

protected:

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	/// Print the module name as a label.
	/// Used for matching debug prints to corresponding module
	void			print_module_label() const;

private:


};



///////////////////////////////////////////////////////////////////////////////


/// Used by LoomManager to sort modules in its vector
struct module_sort_comp {
    bool operator() (LoomModule* left, LoomModule* right) const {
       return left->get_module_type() < right->get_module_type();
    }
};
