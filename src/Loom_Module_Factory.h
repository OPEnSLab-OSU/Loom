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
#include "CommPlats/Loom_SlipSerial.h"

// InternetPlats
#include "InternetPlats/Loom_InternetEthernet.h"

// PublishPlats

#include "PublishPlats/Loom_GoogleSheets.h"

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

// General
#include "Loom_Interrupt_Manager.h"
#include "Loom_Sleep_Manager.h"
#include "Loom_Multiplexer.h" // this needs to be include after I2C sensors (due to conflict with enableInterrupt macro/function defined by EnableInterrupt library and AS726X sensors)


/// Creates a LoomModule with default parameters
/// \return The created LoomModule
template<class T> 
LoomModule* ConstructDefault() {
	return new T();
}

/// Creates a LoomModule with Json array of parameters
/// \return The created LoomModule
template<class T> 
LoomModule* Construct(JsonArrayConst p) {
	return new T(p);
}

///////////////////////////////////////////////////////////////////////////////


/// LoomModule Factory
class Factory 
{

public:

	/// Constructor
	Factory() = default;
	
	/// Destructor
	~Factory() = default;

	/// Create a LoomModule accoding to Json parameters
	/// \param[in]	module			Json of the module name and settings
	/// \return The created LoomModule
	LoomModule* Create(JsonVariant module);

	/// Creates a LoomModule with its default parameters.
	/// Usage example:
	///		Loom_Relay r = FactoryInst.CreateDefault<Loom_Relay>();
	/// \return The created LoomModule
	template<class T> 
	T* CreateDefault() {
		return ConstructDefault<T>();
	}

private:

	/// Function pointer to 'template<class T> LoomModule* Construct(JsonArrayConst p)'
	using FactoryPtr = LoomModule* (*)(JsonArrayConst p);
	using FactoryPtrDefault = LoomModule* (*)();

	/// Struct to contain the elements of factory lookup table
	typedef struct {
		const char*			name;				// Module type to compare against
		FactoryPtr			Construct;			// Pointer to 'template<class T> LoomModule* Create(JsonArrayConst p)' with the type T set
		FactoryPtrDefault	ConstructDefault;	// Pointer to 'template<class T> LoomModule* CreateDefault()' with the type T set
	} NameModulePair;

	/// Factory lookup table
	const static NameModulePair LookupTable[];
};

