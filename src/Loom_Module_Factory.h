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
#include "Sensors/I2C/Loom_MS5803.h"
#include "Sensors/I2C/Loom_SHT31D.h"
#include "Sensors/I2C/Loom_TSL2561.h"
#include "Sensors/I2C/Loom_TSL2591.h"
#include "Sensors/I2C/Loom_ZXGesture.h"

#include "Sensors/SDI12/Loom_Decagon_5TM.h"
#include "Sensors/SDI12/Loom_Decagon_GS3.h"

#include "Sensors/SPI/Loom_MAX31856.h"

// Other
#include "Loom_NTP_Sync.h"

// General
#include "Loom_Interrupt_Manager.h"
#include "Loom_Sleep_Manager.h"
#include "Loom_Multiplexer.h" // this needs to be include after I2C sensors (due to conflict with enableInterrupt macro/function defined by EnableInterrupt library and AS726X sensors)


/// Enum used to determine where to sort module in 
/// device manager lists 
enum class ModuleSortType {
	InterruptManager,
	SleepManager,
	Multiplexer,
	Actuator,
	CommPlat,
	InternetPlat,
	PublishPlat,
	LogPlat,
	Rtc,
	Sensor,
	Other,
	Unknown
};


/// Creates a LoomModule with default parameters
/// \return The created LoomModule
template<class T> T* ConstructDefault() {
	return new T();
}

/// Creates a LoomModule with Json of parameters
/// \return The created LoomModule
template<class T> LoomModule* Construct(JsonVariant p) {
	return new T(p);
}

/// Function pointer to 'template<class T> LoomModule* Construct(JsonVariant p)'
using FactoryPtr = LoomModule* (*)(JsonVariant p);

/// Struct to contain the elements of factory lookup table
typedef struct {
	const char*		name;		// Module type to compare against
	FactoryPtr		Construct;	// Pointer to 'template<class T> LoomModule* Create(JsonVariant p)' with the type T set
	ModuleSortType	sort_type;	// Which array of LoomManager to sort into 
} NameModulePair;


///////////////////////////////////////////////////////////////////////////////


/// LoomModule Factory
class Factory 
{

public:
	
	/// Constructor
	Factory() = default;
	//// Destructor
	~Factory() = default;

	/// Create a LoomModule accoding to Json parameters
	/// \param[in]	module		Json of the module name and settings
	/// \return The created LoomModule
	LoomModule* Create(JsonVariant module);

	/// Creates a LoomModule with its default parameters
	/// \return The created LoomModule
	template<class T> T* CreateDefault() {
		return ConstructDefault<T>();
	}

	/// Creates a LoomModule according to Json
	void CreateAndSort(
		JsonVariant module, 
		Loom_Interrupt_Manager*& interrupt_manager, 
		Loom_Sleep_Manager*& sleep_manager,
		LoomRTC*& rtc,
		LoomModule*& other_module, 
		LoomSensor*& sensor,
		LoomActuator*& actuator,
		LoomCommPlat*& comm_plat,
		LoomInternetPlat*& internet_plat,
		LoomPublishPlat*& publish_plat,
		LoomLogPlat*& log_plat
	);

private:

	/// Determine which array of manager to sort new module into
	/// \param[in]	module		Json of the module name and settings
	/// \return Type indicating manager array to sort into 
	ModuleSortType get_sort_type(JsonVariant module);

	/// Factory lookup table
	const static NameModulePair LookupTable[];

};

