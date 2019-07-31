#pragma once

// #include "Loom_Manager.h"
#include "Loom_Misc.h"
#include "Loom_Translator.h"
#include "Loom_Macros.h"
#include "Loom_Package.h"

#include <ArduinoJson.h>

/// Different levels of verbosity (for printing or packaging)
enum class Verbosity {
	V_OFF,		///< Disable
	V_LOW, 		///< Minimal/Stardard
	V_HIGH 		///< Full details
};

class LoomManager; // Specify that LoomManager exists, defined in own file


///////////////////////////////////////////////////////////////////////////////


/// Abstract root of Loom component modules inheritance hierarchy
class LoomModule
{

public:

	/// Enum to check against to when finding individual component
	/// managed by a LoomManager
	enum class Type {
		Unknown = 0,
		// Other
		Other=1000,			Interrupt_Manager, Sleep_Manager, Multiplexer,
		// Sensors
		Sensor=2000,		Analog, Digital,
		// I2C
		I2C=2100,			AS7262, AS7263, AS7265X, FXAS21002, FXOS8700, LIS3DH, MB1232, MMA8451, MPU6050, MS5803, SHT31D, TMP007, TSL2561, TSL2591, ZXGesture,
		// SDI12
		SDI12=2200,			Decagon5TM, DecagonGS3,
		// SPI	
		SPI=2300,			MAX31855, MAX31856,
		// RTC
		L_RTC=3000,			DS3231, PCF8523,
		// Actuators
		Actuator=4000,		Neopixel, Relay, Servo, Stepper,
		// LogPlats
		LogPlat=5000,		OLED, SDCARD,
		// CommPlats
		CommPlat=6000,		LoRa, nRF, SlipSerial, Bluetooth,
		// InternetPlats
		InternetPlat=7000,	Ethernet,
		// PublishPlats

	};

protected:

	const Type		module_type;		/// Module type (hub / node)
	LoomManager*	device_manager;		/// Pointer to manager
	const String	module_name_base;	/// The name of the module (Should have a DEFAULT but can be overriden if provided to constructor)
	const char* 	module_name;
	bool			active;				/// Whether or not the module should be treated as active
	bool			print_debug;		/// Individually togglable debug statements
	Verbosity		print_verbosity;	/// Print verbosity
	Verbosity		package_verbosity;	/// Package verbosity 

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	LoomModule();

	/// Constructor
	LoomModule( const char* module_name, Type type );

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
	/// \param[out]	json	Object to put data into
	virtual void 	package(JsonObject json) = 0;

	/// Route command to driver 
	virtual bool	dispatch(JsonObject json) = 0;

	/// Turn off any hardware
	virtual void	power_down() {}

	/// Turn on any hardware
	virtual void	power_up() {}

	/// Add configuration information to JsonObject.
	/// LoomManager iterates over modules to build complete configuration
	/// \param[in]	json	Json configuration object to add to
	virtual void	add_config(JsonObject json) {}//= 0;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	/// Display the configuration settings of the module
	virtual void	print_config();

	/// Display current state of the module
	virtual void	print_state();

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get module type
	/// \return Module type
	Type			get_module_type() { return module_type; }

	/// Get the device manager class if linked
	/// \return Pointer to the LoomManager, Null if not linked
	LoomManager*	get_device_manager() { return device_manager; }

	/// Copy module name into buffer
	/// \param[out]	buf	The buffer to copy module name into
	void			get_module_name(char* buf);
	
	/// Get module name
	/// \return	Module name
	const char*		get_module_name() { return module_name; }

	/// Get print verbosity
	/// \return		The current verbosity setting
	Verbosity		get_print_verbosity() { return print_verbosity; }

	/// Get package verbosity
	/// \return		The current verbosity setting
	Verbosity		get_package_verbosity() { return package_verbosity; }

	/// Get whether or not the module should be treated as active
	/// \return		Whether or not the module is active
	bool			get_active() { return active; }

	/// Get whether or not debug prints are enabled
	/// \return		Whether or not print statements are enabled
	bool			get_print_debug() { return print_debug; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Add pointer back to device manager.
	/// Generally only called when device manager links module
	/// to provide pointer both directions. 
	/// Derived modules may override this for increased function,
	/// such as linking a submanager or RTC module.
	/// \param[in]	LM	LoomManager to point to
	virtual void	link_device_manager(LoomManager* LM);

	/// Set print verbosity 
	/// Controlls level of detail included in debug prints 
	void			set_print_verbosity(Verbosity v);

	/// Set package verbosity.
	/// Controlls level of detail included in bundles
	/// \param[in]	v	The verbosity setting
	void			set_package_verbosity(Verbosity v);

	/// Set whether or not the module should be treated as active
	/// \param[in]	enable	Whether or not to enable module
	void			set_active(bool enable);

	/// Set whether or not debug print statements are enabled for this module
	/// \param[in]	enable	Whether or not to enable print statements
	void			set_print_debug(bool enable) { print_debug = enable; }

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Get string of name associated with verbosity enum
	/// \return String of verbosity
	static const char*	enum_verbosity_string(Verbosity v);
	
	static const char*	enum_category_string(Category c);

	Category			category();

protected:

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	/// Print the module name as a label.
	/// Used for matching debug prints to corresponding module
	void			print_module_label();

private:


};



///////////////////////////////////////////////////////////////////////////////


// Used by LoomManager to sort modules in its vector
struct module_sort_comp {
    bool operator() (LoomModule* left, LoomModule* right) const { 
       return left->get_module_type() < right->get_module_type();
    }
};



