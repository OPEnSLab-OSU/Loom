#pragma once

#include "Loom_Module.h"

#include <ArduinoJson.h>


// Actuators
#include "Actuators/Loom_Neopixel.h"
#include "Actuators/Loom_Relay.h"
#include "Actuators/Loom_Servo.h"
#include "Actuators/Loom_Stepper.h"

// CommPlats
#include "CommPlats/Loom_LoRa.h"
#include "CommPlats/Loom_nRF.h"
#include "CommPlats/Loom_Bluetooth.h"

// InternetPlats
#include "InternetPlats/Loom_InternetEthernet.h"
#include "InternetPlats/Loom_InternetWiFi.h"

// PublishPlats
#include "PublishPlats/Loom_GoogleSheets.h"
#include "PublishPlats/Loom_Max_Pub.h"
#include "PublishPlats/Loom_SpoolPublish.h"

// SubscribePlats
#include "SubscribePlats/Loom_Max_Sub.h"

// LogPlats
#include "LogPlats/Loom_OLED.h"
#include "LogPlats/Loom_SD.h"

// // RTC
#include "RTC/Loom_DS3231.h"
#include "RTC/Loom_PCF8523.h"

// Sensors
#include "Sensors/Loom_Analog.h"
#include "Sensors/Loom_Digital.h"

#include "Sensors/I2C/Loom_AS7262.h"
#include "Sensors/I2C/Loom_AS7263.h"
#include "Sensors/I2C/Loom_AS7265X.h"
#include "Sensors/I2C/Loom_FXAS21002.h"
#include "Sensors/I2C/Loom_FXOS8700.h"
#include "Sensors/I2C/Loom_LIS3DH.h"
#include "Sensors/I2C/Loom_MB1232.h"
#include "Sensors/I2C/Loom_MMA8451.h"
#include "Sensors/I2C/Loom_MPU6050.h"
#include "Sensors/I2C/Loom_MS5803.h"
#include "Sensors/I2C/Loom_SHT31D.h"
#include "Sensors/I2C/Loom_TMP007.h"
#include "Sensors/I2C/Loom_TSL2561.h"
#include "Sensors/I2C/Loom_TSL2591.h"
#include "Sensors/I2C/Loom_ZXGesture.h"

#include "Sensors/SDI12/Loom_Decagon_5TM.h"
#include "Sensors/SDI12/Loom_Decagon_GS3.h"

#include "Sensors/SPI/Loom_MAX31855.h"
#include "Sensors/SPI/Loom_MAX31856.h"

// Other
#include "Loom_NTP_Sync.h"
#include "Loom_Temperature_Sync.h"

// General
#include "Loom_Interrupt_Manager.h"
#include "Loom_Sleep_Manager.h"
#include "Loom_Multiplexer.h" // this needs to be include after I2C sensors (due to conflict with enableInterrupt macro/function defined by EnableInterrupt library and AS726X sensors)

// For functions that build constexpr lookup table 
#include "Loom_Module_Factory_Aux.h"



///////////////////////////////////////////////////////////////////////////////



/// Creates a LoomModule with default parameters
/// \return The created LoomModule
template<class T> LoomModule* ConstructDefault() { return new T(); }

/// Creates a LoomModule with Json array of parameters
/// \return The created LoomModule
template<class T> LoomModule* Construct(JsonArrayConst p) { return new T(p); }



namespace factory {

	/// Function pointer to 'template<class T> LoomModule* Construct(JsonArrayConst p)'
	using FactoryPtr = LoomModule* (*)(JsonArrayConst p);

	/// Function pointer to 'template<class T> LoomModule* ConstructDefault()'
	using FactoryPtrDefault = LoomModule* (*)();

	/// Struct to contain the elements of factory lookup table
	typedef struct {
		/// Module type to compare against
		const char*			name;				
		/// Pointer to 'template<class T> LoomModule* Create(JsonArrayConst p)' with the type T set
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

	/// Sensor modules
	constexpr auto Sensors = std::make_tuple(
				factory::NameModulePair{"Multiplexer",	Construct<Loom_Multiplexer>,	ConstructDefault<Loom_Multiplexer> },
				// I2C
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
				// SDI12
				factory::NameModulePair{"Decagon5TM",	Construct<Loom_Decagon5TM>,		ConstructDefault<Loom_Decagon5TM> },
				factory::NameModulePair{"DecagonGS3",	Construct<Loom_DecagonGS3>,		ConstructDefault<Loom_DecagonGS3> },
				// SPI
				factory::NameModulePair{"MAX31855",		Construct<Loom_MAX31855>,		ConstructDefault<Loom_MAX31855> },
				factory::NameModulePair{"MAX31856",		Construct<Loom_MAX31856>,		ConstructDefault<Loom_MAX31856> },
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

/// Base class that factory is derived from.
/// Purpose is to allow LoomManger to have a pointer to a LoomFactory, which 
/// it cannot do directly because different parameter selections result in 
/// different factory classes
class FactoryBase
{
public:

	virtual void print_table() = 0;

	/// Creates a LoomModule with its default parameters.
	/// Usage example:
	///		Loom_Relay r = FactoryInst.CreateDefault<Loom_Relay>();
	/// \return The created LoomModule
	template<class T> 
	static T* CreateDefault() { return ConstructDefault<T>(); }

	virtual LoomModule* Create(JsonVariant module) = 0;
};


///////////////////////////////////////////////////////////////////////////////


/// Factory is used by LoomManager when parsing Json to match module names to 
/// their associated constructors, and calling with parameters from the Json.
/// The template parameters are used to select whether certain blocks of 
/// modules are included in the lookup table. This is determined at compile
/// time to not include unnecessary module classes and supporting code, 
/// thus reducing code size
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
	void print_table()
	{
		LPrintln("[Factory] Module Lookup Table:\n");
		for (auto item : factory::to_array(LookupTable) ) {
			LPrintln(item.name);
		}
	}

	/// Create a module based on a subset of a Json configuration.
	/// Needs name and parameters to the constructor as an array (or the word 'default')
	/// if that module has default values for all parameters
	/// \param[in]	module		Subset of a Json configuration, used to identify module to create and with what parameters
	/// \return	LoomModule if created, nullptr if it could not be created
	LoomModule* Create(JsonVariant module)
	{
		const char* name = module["name"];

		for (auto elem : factory::to_array(LookupTable) ) {
			if ( strcmp(name, elem.name) == 0 ) {

				if (module["params"].is<JsonArray>()) {
					// Generate according to list of parameters
					LPrintln("[Factory] Creating: ", name);
					return elem.Construct(module["params"]);
				} else if ( module["params"].is<const char*>() && strcmp(module["params"], "default") == 0 ) {
					// Generate using default parameters
					if (elem.ConstructDefault == nullptr) {
						LPrintln("[Factory] ", "No default constructor for module '", name, "'");
						return nullptr;
					} else {
						return elem.ConstructDefault();
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







