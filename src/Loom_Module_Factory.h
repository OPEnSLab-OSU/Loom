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
// #include "Loom_Module_Factory_Aux.h"



///////////////////////////////////////////////////////////////////////////////

///Forward declare any classes that will be served to created objects
class Scope;

/// Creates a new T, returned as S*. S must be base of T
/// @return The created T
template<class S, class T> S* ConstructDefault(Scope* scope) { return new T(scope); }

/// Creates a new T, returned as S*. S must be base of T
/// @return The created T
template<class S, class T> S* Construct(Scope* scope, JsonArrayConst p) { return new T(scope, p); }

/// A generic registry/factory that collects constructors of a give base type and allows for any number
/// of base classes as static singleton templates will only create new instances of a new type is introduced
template<typename T>
class Registry {
public:
    using FactoryFunction   = T*(*)(Scope*);///< Pointer to function of type //* void T::funct (void) *//
		using FactoryFunctionJson = LoomModule* (*)(Scope*, JsonArrayConst);
    using FactoryPair       = struct {      ///<  *Needed as an alternative to std::map
        const char* name;           				///< Name of module that will be used to make a new copy
        const FactoryFunction ctor; 				///< Pointer to the Creation function which will be used to CTOR
				const FactoryFunctionJson ctorJson; ///< Pointer to the CreationJSON function which will be used to CTOR form JSON
    };
    
    static bool add(const char*, const FactoryFunction);///< Adds a new Factory Pair derived from args to lookup table
    template<class U>
    static bool add(const char*);
    static T* create(const char*, Scope* scope);        ///< Returns an instance of type T created by using the FactoryFunction coresponding to the provided name
    static T* create(JsonVariant, Scope*);              ///< Returns an instance of type T created from provided JSON
    
private:
    /// Use Meyer's singleton to prevent SIOF
    static std::vector<FactoryPair>& getFactoryTable();
};

template<typename T>
template<class U>
bool Registry<T>::add(const char* name) {
    return add(name, ConstructDefault<T, U>);
}

template<typename T>
bool Registry<T>::add(const char* name, const Registry<T>::FactoryFunction ctor) {
    auto lookUp = getFactoryTable();
    for(auto elem : lookUp) {
        ///Add string processing for fuzzy search and debug suggestions
        if(!strcmp(name, elem.name)) {
            ///On match, fail. Items two items cannot have the same creation name;
            return false;
        }
    }   ///< No match found, exiting for
    getFactoryTable().push_back(FactoryPair{name, ctor});
    return true;
}

template<typename T>
T* Registry<T>::create(const char* name, Scope* scope) {
    auto lookUp = getFactoryTable();
    for(auto elem : lookUp) {
        ///Add string processing for fuzzy search and debug suggestions
        if(!strcmp(name, elem.name)) {
            ///On match, return a *new* item of type name
            return elem.ctor(scope);
        }
    }   ///< No match found, exiting for
    return nullptr;
}

template<typename T>
T* Registry<T>::create(JsonVariant target, Scope* scope) {
    const char* name = target["name"].as<char*>();
    auto lookUp = getFactoryTable();
    
    for(auto elem : lookUp) {
        if(!strcmp(name, elem.name)) {
            return elem.ctor(scope);
        }
    }
    return nullptr;
}

/// This function explots C++'s treatment of static initialization, where the compiler only expects a single instance to ever exist
/// Due to this behavior, we can use this 
template<typename T>
std::vector<typename Registry<T>::FactoryPair>& Registry<T>::getFactoryTable() {
    static std::vector<FactoryPair> lookUpTable;
    return lookUpTable;
}

///////////////////////////////////////////////////////////////////////////////

/// Creates a LoomModule with default parameters
/// @return The created LoomModule
template<class T> LoomModule* ConstructDefault() { return new T(); }

/// Creates a LoomModule with Json array of parameters
/// @return The created LoomModule
template<class T> LoomModule* Construct(JsonArrayConst p) { return new T(p); }
