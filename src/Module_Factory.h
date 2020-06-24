///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Module_Factory.h
/// @brief		File for LoomFactory, FactoryBase definition and implementations.
///				Also includes all the supporting code to build the LoomFactory lookup table.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Module.h"

#include <ArduinoJson.h>


// Actuators
#include "Actuators/Neopixel.h"
#include "Actuators/Relay.h"
#include "Actuators/Servo.h"
#include "Actuators/Stepper.h"

// CommPlats
#include "CommPlats/LoRa.h"
#include "CommPlats/nRF.h"
#include "CommPlats/Bluetooth.h"

// InternetPlats
#include "InternetPlats/InternetEthernet.h"
#include "InternetPlats/InternetWiFi.h"
#include "InternetPlats/InternetLTE.h"

// PublishPlats
#include "PublishPlats/GoogleSheets.h"
#include "PublishPlats/Max_Pub.h"

// SubscribePlats
#include "SubscribePlats/Max_Sub.h"

// LogPlats
#include "LogPlats/OLED.h"
#include "LogPlats/SD.h"

// // RTC
#include "RTC/DS3231.h"
#include "RTC/PCF8523.h"

// Sensors
#include "Sensors/Analog.h"
#include "Sensors/Digital.h"

#include "Sensors/I2C/ADS1115.h"
#include "Sensors/I2C/AS7262.h"
#include "Sensors/I2C/AS7263.h"
#include "Sensors/I2C/AS7265X.h"
#include "Sensors/I2C/FXAS21002.h"
#include "Sensors/I2C/FXOS8700.h"
#include "Sensors/I2C/LIS3DH.h"
#include "Sensors/I2C/MB1232.h"
#include "Sensors/I2C/MMA8451.h"
#include "Sensors/I2C/MPU6050.h"
#include "Sensors/I2C/MS5803.h"
#include "Sensors/I2C/SHT31D.h"
#include "Sensors/I2C/TMP007.h"
#include "Sensors/I2C/TSL2561.h"
#include "Sensors/I2C/TSL2591.h"
#include "Sensors/I2C/ZXGesture.h"
#include "Sensors/I2C/STEMMA.h"



#include "Sensors/SDI12/Decagon_5TM.h"
#include "Sensors/SDI12/Decagon_GS3.h"

#include "Sensors/SPI/MAX31855.h"
#include "Sensors/SPI/MAX31856.h"

#include "Sensors/SERIAL/K30.h"

// Other
#include "NTP_Sync.h"
#include "Temperature_Sync.h"

// General
#include "Components/WarmUp/WarmUp_Manager.h"
#include "Interrupt_Manager.h"
#include "Sleep_Manager.h"
#include "Multiplexer.h" // this needs to be include after I2C sensors (due to conflict with enableInterrupt macro/function defined by EnableInterrupt library and AS726X sensors)

// For functions that build constexpr lookup table
#include "Module_Factory_Aux.h"



///////////////////////////////////////////////////////////////////////////////



/// Creates a LoomModule with default parameters
/// @return The created LoomModule
template<class T> LoomModule* ConstructDefault(LoomManager* manager) { return new T(manager); }

/// Creates a LoomModule with Json array of parameters
/// @return The created LoomModule
template<class T> LoomModule* Construct(LoomManager* manager, JsonArrayConst p) { return new T(manager, p); }



namespace factory {

	/// Function pointer to 'template<class T> LoomModule* Construct(LoomManager* manager, LoomManager* manager, JsonArrayConst p)'
	using FactoryPtr = LoomModule* (*)(LoomManager* manager, JsonArrayConst p);

	/// Function pointer to 'template<class T> LoomModule* ConstructDefault()'
	using FactoryPtrDefault = LoomModule* (*)(LoomManager* manager);

	/// Struct to contain the elements of factory lookup table
	typedef struct {
		/// Module type to compare against
		const char*			name;
		/// Pointer to 'template<class T> LoomModule* Create(LoomManager* manager, JsonArrayConst p)' with the type T set
		FactoryPtr			Construct;
		/// Pointer to 'template<class T> LoomModule* CreateDefault()' with the type T set
		FactoryPtrDefault	ConstructDefault;
	} NameModulePair;

} // end factory



///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////



/// Blocks modules the can be toggled on/off or
/// selected between a variety of selection.
/// Below, in LoomFactory, the integer index (zero indexed) of the enums is
/// used to select between different includes, so make sure the includes
/// correctly correspond to the index
namespace Enable
{
	enum class Internet  {
		All,			///< All internet modules enabled
		Ethernet,		///< Only Ethernet relevant modules enabled
		WiFi,			///< Only WiFi relevant modules enabled
		LTE,			///< Only LTE relevant modules enabled
		Disabled		///< Internet modules disabled
	};

	enum class Sensors   { Enabled, Disabled };
	enum class Actuators { Enabled, Disabled };
	enum class Radios    { Enabled, Disabled };
	enum class Max       { Enabled, Disabled };
}

/// Possible sections of the module lookup table to include.
/// Factory concatenates selected blocks to form its lookup table
/// for module creation.
/// Blocks are made up of tuples of NameModulePair
namespace Include
{
	/// Empty block (used if a block of modules is disabled)
	constexpr auto None = std::make_tuple();

	/// Common modules
	constexpr auto Common = std::make_tuple(
				factory::NameModulePair{"Interrupt_Manager",	Construct<Loom_Interrupt_Manager>,		ConstructDefault<Loom_Interrupt_Manager> },
				factory::NameModulePair{"WarmUp_Manager", Construct<Loom_WarmUp_Manager>, ConstructDefault<Loom_WarmUp_Manager> },
				factory::NameModulePair{"Sleep_Manager",		Construct<Loom_Sleep_Manager>,			ConstructDefault<Loom_Sleep_Manager> },
				factory::NameModulePair{"Analog",		Construct<Loom_Analog>,			ConstructDefault<Loom_Analog> },
				factory::NameModulePair{"Digital",		Construct<Loom_Digital>,		ConstructDefault<Loom_Digital> },
				// LogPlat
				factory::NameModulePair{"OLED",			Construct<Loom_OLED>,			ConstructDefault<Loom_OLED> },
				factory::NameModulePair{"SD",			Construct<Loom_SD>,				ConstructDefault<Loom_SD> },
				// // RTC
				factory::NameModulePair{"DS3231",		Construct<Loom_DS3231>,			ConstructDefault<Loom_DS3231> },
				factory::NameModulePair{"PCF8523",		Construct<Loom_PCF8523>,		ConstructDefault<Loom_PCF8523> }
			);

	/// Radios modules
	constexpr auto Radios = std::make_tuple(
				factory::NameModulePair{"LoRa",			Construct<Loom_LoRa>,			ConstructDefault<Loom_LoRa> },
				factory::NameModulePair{"nRF",			Construct<Loom_nRF>,			ConstructDefault<Loom_nRF> },
				factory::NameModulePair{"Bluetooth",	Construct<Loom_Bluetooth>,		ConstructDefault<Loom_Bluetooth> }
			);

	/// Max modules
	constexpr auto Max = std::make_tuple(
				factory::NameModulePair{"MaxPub",		Construct<Loom_MaxPub>,			nullptr },
				factory::NameModulePair{"MaxSub",		Construct<Loom_MaxSub>,			nullptr }
			);

	/// Ethernet and WiFi modules
	constexpr auto EthernetAndWiFi = std::make_tuple(
				factory::NameModulePair{"Ethernet",		Construct<Loom_Ethernet>,		ConstructDefault<Loom_Ethernet> },
				factory::NameModulePair{"WiFi",			Construct<Loom_WiFi>,			ConstructDefault<Loom_WiFi> },
				factory::NameModulePair{"GoogleSheets",	Construct<Loom_GoogleSheets>,	nullptr },
				factory::NameModulePair{"NTP_Sync", 	Construct<LoomNTPSync>,			ConstructDefault<LoomNTPSync> }
			);

	/// Ethernet modules
	constexpr auto Ethernet = std::make_tuple(
				factory::NameModulePair{"Ethernet",		Construct<Loom_Ethernet>,		ConstructDefault<Loom_Ethernet> },
				factory::NameModulePair{"GoogleSheets",	Construct<Loom_GoogleSheets>,	nullptr },
				factory::NameModulePair{"NTP_Sync", 	Construct<LoomNTPSync>,			ConstructDefault<LoomNTPSync> }
			);

	/// WiFi modules
	constexpr auto WiFi = std::make_tuple(
				factory::NameModulePair{"WiFi",			Construct<Loom_WiFi>,			ConstructDefault<Loom_WiFi> },
				factory::NameModulePair{"GoogleSheets",	Construct<Loom_GoogleSheets>,	nullptr },
				factory::NameModulePair{"NTP_Sync", 	Construct<LoomNTPSync>,			ConstructDefault<LoomNTPSync> }
			);

	/// LTE modules
	constexpr auto LTE = std::make_tuple(
		factory::NameModulePair{"LTE",			Construct<Loom_LTE>,			ConstructDefault<Loom_LTE> },
		factory::NameModulePair{"GoogleSheets",	Construct<Loom_GoogleSheets>,	nullptr },
		factory::NameModulePair{"NTP_Sync", 	Construct<LoomNTPSync>,			ConstructDefault<LoomNTPSync> }
	);

	/// Sensor modules
	constexpr auto Sensors = std::make_tuple(
				factory::NameModulePair{"Multiplexer",	Construct<Loom_Multiplexer>,	ConstructDefault<Loom_Multiplexer> },
				// I2C
				factory::NameModulePair{"ADS1115",		Construct<Loom_ADS1115>,			ConstructDefault<Loom_ADS1115> },
				factory::NameModulePair{"AS7262",		Construct<Loom_AS7262>,			ConstructDefault<Loom_AS7262> },
				factory::NameModulePair{"AS7263",		Construct<Loom_AS7263>,			ConstructDefault<Loom_AS7263> },
				factory::NameModulePair{"AS7265X",		Construct<Loom_AS7265X>,		ConstructDefault<Loom_AS7265X> },
				factory::NameModulePair{"FXAS21002",	Construct<Loom_FXAS21002>,		ConstructDefault<Loom_FXAS21002> },
				factory::NameModulePair{"FXOS8700",		Construct<Loom_FXOS8700>,		ConstructDefault<Loom_FXOS8700> },
				factory::NameModulePair{"LIS3DH",		Construct<Loom_LIS3DH>,			ConstructDefault<Loom_LIS3DH> },
				factory::NameModulePair{"MB1232",		Construct<Loom_MB1232>,			ConstructDefault<Loom_MB1232> },
				factory::NameModulePair{"MMA8451",		Construct<Loom_MMA8451>,		ConstructDefault<Loom_MMA8451> },
				factory::NameModulePair{"MPU6050",		Construct<Loom_MPU6050>,		ConstructDefault<Loom_MPU6050> },
				factory::NameModulePair{"MS5803",		Construct<Loom_MS5803>,			ConstructDefault<Loom_MS5803> },
				factory::NameModulePair{"SHT31D",		Construct<Loom_SHT31D>,			ConstructDefault<Loom_SHT31D> },
				factory::NameModulePair{"TMP007",		Construct<Loom_TMP007>,			ConstructDefault<Loom_TMP007> },
				factory::NameModulePair{"TSL2561",		Construct<Loom_TSL2561>,		ConstructDefault<Loom_TSL2561> },
				factory::NameModulePair{"TSL2591",		Construct<Loom_TSL2591>,		ConstructDefault<Loom_TSL2591> },
				factory::NameModulePair{"ZXGesture",	Construct<Loom_ZXGesture>,		ConstructDefault<Loom_ZXGesture> },
                factory::NameModulePair{"STEMMA",       Construct<Loom_STEMMA>,         ConstructDefault<Loom_STEMMA> },
                                             
				// SDI12
				factory::NameModulePair{"Decagon5TM",	Construct<Loom_Decagon5TM>,		ConstructDefault<Loom_Decagon5TM> },
				factory::NameModulePair{"DecagonGS3",	Construct<Loom_DecagonGS3>,		ConstructDefault<Loom_DecagonGS3> },
				// SPI
				factory::NameModulePair{"MAX31855",		Construct<Loom_MAX31855>,		ConstructDefault<Loom_MAX31855> },
				factory::NameModulePair{"MAX31856",		Construct<Loom_MAX31856>,		ConstructDefault<Loom_MAX31856> },
                // SERIAL
                factory::NameModulePair{"K30",        Construct<Loom_K30>,        ConstructDefault<Loom_K30> },
				// Other
				factory::NameModulePair{"TempSync",		Construct<LoomTempSync>,		ConstructDefault<LoomTempSync> }
			);

	/// Actuator modules
	constexpr auto Actuators = std::make_tuple(
				factory::NameModulePair{"Neopixel",		Construct<Loom_Neopixel>,		ConstructDefault<Loom_Neopixel> },
				factory::NameModulePair{"Relay",		Construct<Loom_Relay>,			ConstructDefault<Loom_Relay> },
				factory::NameModulePair{"Servo",		Construct<Loom_Servo>,			ConstructDefault<Loom_Servo> },
				factory::NameModulePair{"Stepper",		Construct<Loom_Stepper>,		ConstructDefault<Loom_Stepper> }
			);

} // end Include namespace



///////////////////////////////////////////////////////////////////////////////
///
/// Base class that factory is derived from.
/// Purpose is to allow LoomManger to have a pointer to a LoomFactory, which
/// it cannot do directly because different parameter selections result in
/// different factory classes
///
/// @par Resources
/// - [FactoryBase Documentation](https://openslab-osu.github.io/Loom/html/class_factory_base.html)
///
///////////////////////////////////////////////////////////////////////////////
class FactoryBase
{
public:

	virtual void print_table() const = 0;

	/// Creates a LoomModule with its default parameters.
	/// Usage example:
	///		Loom_Relay r = FactoryInst.CreateDefault<Loom_Relay>();
	/// @return The created LoomModule
	template<class T>
	static T* CreateDefault() { return ConstructDefault<T>(); }

	virtual LoomModule* Create(LoomManager* manager, JsonVariant module) const = 0;
};


///////////////////////////////////////////////////////////////////////////////
///
/// Factory is used by LoomManager when parsing Json to match module names to
/// their associated constructors, and calling with parameters from the Json.
/// The template parameters are used to select whether certain blocks of
/// modules are included in the lookup table. This is determined at compile
/// time to not include unnecessary module classes and supporting code,
/// thus reducing code size
///
/// @par Resources
/// - [LoomFactory Documentation](https://openslab-osu.github.io/Loom/html/class_loom_factory.html)
///
///////////////////////////////////////////////////////////////////////////////
template<
	Enable::Internet INTERNET	= Enable::Internet::All,
	Enable::Sensors SENSORS		= Enable::Sensors::Enabled,
	Enable::Radios RADIOS		= Enable::Radios::Enabled,
	Enable::Actuators ACTUATORS	= Enable::Actuators::Enabled,
	Enable::Max MAX				= Enable::Max::Disabled
>
class LoomFactory : public FactoryBase
{

private:

	/// The factory's lookup table of modules that can be created, based on the
	/// enable values of the classes template parameters
	static constexpr auto LookupTable = tuple_cat(
			Include::Common,
			factory::select<(int)INTERNET>(
				Include::EthernetAndWiFi,
				Include::Ethernet,
				Include::WiFi,
				Include::LTE,
				Include::None
			),
			factory::select<(int)SENSORS>(
				Include::Sensors,
				Include::None
			),
			factory::select<(int)RADIOS>(
				Include::Radios,
				Include::None
			),
			factory::select<(int)ACTUATORS>(
				Include::Actuators,
				Include::None
			),
			factory::select<(int)MAX>(
				Include::Max,
				Include::None
			)
		);

public:

	/// Constructor
	LoomFactory() = default;

	/// Destructor
	~LoomFactory() = default;

	/// Print the contents of the lookup table
	void print_table() const
	{
		LPrintln("[Factory] Module Lookup Table:\n");
		for (auto item : factory::to_array(LookupTable) ) {
			LPrintln(item.name);
		}
	}

	/// Create LoomManager* manager, a module based on a subset of a Json configuration.
	/// Needs name and parameters to the constructor as an array (or the word 'default')
	/// if that module has default values for all parameters
	/// @param[in]	module		Subset of a Json configuration, used to identify module to create and with what parameters
	/// @return	LoomModule if created, nullptr if it could not be created
	LoomModule* Create(LoomManager* manager, JsonVariant module) const
	{
		const char* name = module["name"];

		for (auto elem : factory::to_array(LookupTable) ) {
			if ( strcmp(name, elem.name) == 0 ) {

				if (module["params"].is<JsonArray>()) {
					// Generate according to list of parameters
					LPrintln("[Factory] Creating: ", name);
					return elem.Construct(manager, module["params"]);
				} else if ( module["params"].is<const char*>() && strcmp(module["params"], "default") == 0 ) {
					// Generate using default parameters
					if (elem.ConstructDefault == nullptr) {
						LPrintln("[Factory] ", "No default constructor for module '", name, "'");
						return nullptr;
					} else {
						return elem.ConstructDefault(manager);
					}
				} else {
					// Invalid parameters
					LPrintln("[Factory] ", "Invalid parameters in module '", name, "'");
					return nullptr;
				}
			}
		}

		LPrintln("'", name, "' could not be created");
		return nullptr;
	}

};
