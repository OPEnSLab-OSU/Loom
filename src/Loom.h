/*! \mainpage An Internet of Things Rapid Prototyping System for applications in environmental sensing

This is the Doxygen generated documentation of the Loom library code.

\section resources Resources

- [Github](https://github.com/OPEnSLab-OSU/Loom)
- [Loom Wiki](https://github.com/OPEnSLab-OSU/Loom/wiki)
- [Project Website](http://www.open-sensing.org/project-loom)

*/

#pragma once


// // Find a better home for this thing at some point...
// // Fix for undefined std::__throw_bad_alloc()
// namespace std {
//     void __throw_bad_alloc() { return; }
// }


// Preamble will only need to include leaves in the hierarchy
// abstract (non-leaves) already included by leaves

///////////////////////////////////////////////////////////////////////////////
// Core Classes
#include "Module_Factory.h"
#include "Manager.h"

// Actuators
#ifdef LOOM_INCLUDE_ACTUATORS
    #include "Actuators/Neopixel.h"
    Loom::Neopixel& getNeopixel(const Loom::Manager& feather) {return *(feather.get<Loom::Neopixel>());}
    #include "Actuators/Relay.h"
    Loom::Relay& getRelay(const Loom::Manager& feather) {return *(feather.get<Loom::Relay>());}
    #include "Actuators/Servo.h"
    Loom::Servo& getServo(const Loom::Manager& feather) {return *(feather.get<Loom::Servo>());}
    #include "Actuators/Stepper.h"
    Loom::Stepper& getStepper(const Loom::Manager& feather) {return *(feather.get<Loom::Stepper>());}
#endif

// CommPlats
#ifdef LOOM_INCLUDE_RADIOS
    #include "CommPlats/LoRa.h"
    Loom::LoRa& getLoRa(const Loom::Manager& feather) {return *(feather.get<Loom::LoRa>());}
    #include "CommPlats/nRF.h"
    Loom::nRF& getnRF(const Loom::Manager& feather) {return *(feather.get<Loom::nRF>());}
    #include "CommPlats/Bluetooth.h"
    Loom::Bluetooth& getBluetooth(const Loom::Manager& feather) {return *(feather.get<Loom::Bluetooth>());}
#endif

// InternetPlats
#ifdef LOOM_INCLUDE_WIFI
    #include "InternetPlats/InternetWiFi.h"
#endif
#ifdef LOOM_INCLUDE_ETHERNET
    #include "InternetPlats/InternetEthernet.h"
#endif
#ifdef LOOM_INCLUDE_LTE
    #include "InternetPlats/InternetLTE.h"
#endif

// PublishPlats
#if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
    #include "PublishPlats/GoogleSheets.h"
    Loom::GoogleSheets& getGoogleSheets(const Loom::Manager& feather) {return *(feather.get<Loom::GoogleSheets>());}

    #include "PublishPlats/MQTT_Pub.h"
    Loom::MQTTPub& getMQTTPub(const Loom::Manager& feather) { return *(feather.get<Loom::MQTTPub>()); }
#endif

// Max
#if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))
    #include "PublishPlats/Max_Pub.h"
    Loom::MaxPub& getMaxPub(const Loom::Manager& feather) {return *(feather.get<Loom::MaxPub>());}
    #include "SubscribePlats/Max_Sub.h"
    Loom::MaxSub& getMaxSub(const Loom::Manager& feather) {return *(feather.get<Loom::MaxSub>());}
#endif

//Loom::SD& getSD(Loom::Manager feather) {return *(feather.get<Loom::SD>());}
// LogPlats
#include "LogPlats/OLED.h"
Loom::OLED& getOLED(const Loom::Manager& feather) { return *(feather.get<Loom::OLED>()); }
#include "LogPlats/SD.h"
//Loom::SD* getSD(Loom::Manager feather) {return feather.get<Loom::SD>();}
Loom::SD& getSD(const Loom::Manager& feather) { return *(feather.get<Loom::SD>()); }
#include "LogPlats/BatchSD.h"
Loom::BatchSD& getBatchSD(const Loom::Manager& feather) { return *(feather.get<Loom::BatchSD>()); }

// // RTC
#include "RTC/DS3231.h"
Loom::DS3231& getDS3231(const Loom::Manager& feather) { return *(feather.get<Loom::DS3231>()); }
#include "RTC/PCF8523.h"
Loom::PCF8523& getPCF8523(const Loom::Manager& feather) { return *(feather.get<Loom::PCF8523>()); }

// Sensors
#include "Sensors/Analog.h"
#include "Sensors/Digital.h"

#ifdef LOOM_INCLUDE_SENSORS
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
    #include "Sensors/I2C/TSL2591.h"
    #include "Sensors/I2C/ZXGesture.h"
    #include "Sensors/I2C/STEMMA.h"

    #include "Sensors/SDI12/SDI_Manager.h"

    #include "Sensors/SPI/MAX31855.h"
    #include "Sensors/SPI/MAX31856.h"

    #include "Sensors/SERIAL/K30.h"

    #include "TemperatureSync.h"
    #include "Multiplexer.h" // this needs to be include after I2C sensors (due to conflict with enableInterrupt macro/function defined by EnableInterrupt library and AS726X sensors)
#endif

// Other
#if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
    #include "NTPSync.h"
#endif

// General
#include "Components/WarmUp/WarmUpManager.h"
#include "InterruptManager.h"
Loom::InterruptManager& getInterruptManager(const Loom::Manager& feather) { return *(feather.get<Loom::InterruptManager>()); }
#include "SleepManager.h"
Loom::SleepManager& getSleepManager(const Loom::Manager& feather) { return *(feather.get<Loom::SleepManager>()); }
