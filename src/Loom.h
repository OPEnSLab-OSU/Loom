/*! \mainpage An Internet of Things Rapid Prototyping System for applications in environmental sensing

This is the Doxygen generated documentation of the Loom library code.

\section resources Resources

- [Github](https://github.com/OPEnSLab-OSU/Loom)
- [Loom Wiki](https://github.com/OPEnSLab-OSU/Loom/wiki)
- [Project Website](http://www.open-sensing.org/project-loom)

*/

#pragma once


// Find a better home for this thing at some point...
// Fix for undefined std::__throw_bad_alloc()
namespace std {
    void __throw_bad_alloc() { return; }
} 


// Preamble will only need to include leaves in the hierarchy
// abstract (non-leaves) already included by leaves

///////////////////////////////////////////////////////////////////////////////
// Core Classes
#include "Module_Factory.h"
#include "Manager.h"

// Actuators
#ifdef LOOM_INCLUDE_ACTUATORS
    #include "Actuators/Neopixel.h"
    #include "Actuators/Relay.h"
    #include "Actuators/Servo.h"
    #include "Actuators/Stepper.h"
#endif

// CommPlats
#ifdef LOOM_INCLUDE_RADIOS
    #include "CommPlats/LoRa.h"
    #include "CommPlats/nRF.h"
    #include "CommPlats/Bluetooth.h"
#endif

// InternetPlats
#ifdef LOOM_INCLUDE_WIFI
    #include "InternetPlats/InternetEthernet.h"
#endif
#ifdef LOOM_INCLUDE_ETHERNET
    #include "InternetPlats/InternetWiFi.h"
#endif
#ifdef LOOM_INCLUDE_LTE
    #include "InternetPlats/InternetLTE.h"
#endif

// PublishPlats
#if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))
    #include "PublishPlats/GoogleSheets.h"
    #include "PublishPlats/Max_Pub.h"
#endif

// SubscribePlats
#if defined(LOOM_INCLUDE_MAX) && (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET))
    #include "SubscribePlats/Max_Sub.h"
#endif

// LogPlats
#include "LogPlats/OLED.h"
#include "LogPlats/SD.h"
#include "LogPlats/BatchSD.h"

// // RTC
#include "RTC/DS3231.h"
#include "RTC/PCF8523.h"

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
    #include "Sensors/I2C/TSL2561.h"
    #include "Sensors/I2C/TSL2591.h"
    #include "Sensors/I2C/ZXGesture.h"
    #include "Sensors/I2C/STEMMA.h"

    #include "Sensors/SDI12/Decagon_5TM.h"
    #include "Sensors/SDI12/Decagon_GS3.h"

    #include "Sensors/SPI/MAX31855.h"
    #include "Sensors/SPI/MAX31856.h"

    #include "Sensors/SERIAL/K30.h"
    
    #include "Temperature_Sync.h"
    #include "Multiplexer.h" // this needs to be include after I2C sensors (due to conflict with enableInterrupt macro/function defined by EnableInterrupt library and AS726X sensors)
#endif

// Other
#if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
    #include "NTP_Sync.h"
#endif

// General
#include "Components/WarmUp/WarmUp_Manager.h"
#include "Interrupt_Manager.h"
#include "Sleep_Manager.h"
