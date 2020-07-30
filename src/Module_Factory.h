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



///////////////////////////////////////////////////////////////////////////////
///
/// Base class that factory is derived from.
/// Purpose is to allow LoomManger to have a pointer to a LoomFactory, which
/// it cannot do directly because different module list selections result in
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
    ///        Loom_Relay r = FactoryInst.CreateDefault<Loom_Relay>();
    /// @return The created LoomModule
    template <class T>
    static T* CreateDefault() { return ConstructDefault<T>(); }

    virtual LoomModule* Create(LoomManager* manager, JsonVariant module) const = 0;
};

///////////////////////////////////////////////////////////////////////////////
///
/// Factory is used by LoomManager when parsing Json to match module names to
/// their associated constructors, and calling with parameters from the Json.
/// The template parameters are used to indicate which modules are included in the factory. This is determined at compile
/// time to not include unnecessary module classes and supporting code, thus reducing code size.
/// LoomFactory is implemented as a variadic template in order to interact with an arbitrary number of template parameters. For more information on variadic templates see this excellent [blog post](https://eli.thegreenplace.net/2014/variadic-templates-in-c/)
/// @par Resources
/// - [LoomFactory Documentation](https://openslab-osu.github.io/Loom/html/class_loom_factory.html)
///
///////////////////////////////////////////////////////////////////////////////

template <typename ... Ts>
class LoomFactory : public FactoryBase {
public:
    
    /// Constructor
    LoomFactory() = default;

    /// Destructor
    ~LoomFactory() = default;
    
    /// Create LoomManager* manager, a module based on a subset of a Json configuration.
    /// Needs name and parameters to the constructor as an array (or the word 'default')
    /// if that module has default values for all parameters
    /// @param[in]    module        Subset of a Json configuration, used to identify module to create and with what parameters
    /// @return    LoomModule if created, nullptr if it could not be created
    LoomModule* Create(LoomManager* manager, JsonVariant module) const {
        return nullptr;
    }

    /// Print the contents of the lookup table
    void print_table() const {
        return;
    }
};


template <typename U, typename ... Ts>
class LoomFactory<U, Ts...> : public LoomFactory<Ts...> {
public:

    
    void print_table() const {
        LPrintln(U::name);
        LoomFactory<Ts...>::print_table();
    }

    LoomModule* Create(LoomManager* manager, JsonVariant module) const {

        const char* name = module["name"];

        if (!strcmp(name, U::name)) {
            if (module["params"].is<JsonArray>()) {
					// Generate according to list of parameters
					LPrintln("[Factory] Creating: ", name);
					return Construct<U>(manager, module["params"]);
			} else if ( module["params"].is<const char*>() && strcmp(module["params"], "default") == 0 ) {
					// Generate using default parameters

				LoomModule* defaultInstance = ConstructDefault<U>(manager);
				if (defaultInstance == nullptr) {
					LPrintln("[Factory] ", "No default constructor for module '", name, "'");
				}
				return defaultInstance;
			} else {
				// Invalid parameters
				LPrintln("[Factory] ", "Invalid parameters in module '", name, "'");
				return nullptr;
			}
		}
        return LoomFactory<Ts...>::Create(manager, module);
    }
};
