
#define LOOM_DEBUG 1



// Preamble will only need to include leaves in the heirarchy
// abstract (non-leaves) already included by leaves

// #include <OSCBundle.h>

/////////////////////////////////////////////////////////////////////
// General Macros
// #include "Loom_Macros.h"

/////////////////////////////////////////////////////////////////////
// Type and Format Conversion Functions
// #include "Loom_Translator.h"

/////////////////////////////////////////////////////////////////////
// Core Classes
#include "Loom_Manager.h"
#include "Loom_Module.h"

/////////////////////////////////////////////////////////////////////
// Communiciation Platforms
#include "CommPlats/Loom_LoRa.h"
#include "CommPlats/Loom_nRF.h"
//#include "Loom_SlipSerial.h"

/////////////////////////////////////////////////////////////////////
// Logging Platforrms
#include "LogPlats/Loom_OLED.h"
#include "LogPlats/Loom_SD.h"

/////////////////////////////////////////////////////////////////////
// Internet Platforms
#include "InternetPlats/Loom_InternetWiFi.h"
// #include "InternetPlats/Loom_InternetEthernet.h"

/////////////////////////////////////////////////////////////////////
// Mulitplexer
#include "Loom_Multiplexer.h"

/////////////////////////////////////////////////////////////////////
// Sensors
#include "Sensors/Loom_Analog.h"
#include "Sensors/Loom_Digital.h"

/////////////////////////////////////////////////////////////////////
// I2C Sensors
#include "Sensors/I2C/Loom_AS7262.h"
#include "Sensors/I2C/Loom_AS7263.h"
#include "Sensors/I2C/Loom_AS7265X.h"
#include "Sensors/I2C/Loom_FXAS21002.h"
#include "Sensors/I2C/Loom_FXOS8700.h"
#include "Sensors/I2C/Loom_LIS3DH.h"
#include "Sensors/I2C/Loom_MB1232.h"
#include "Sensors/I2C/Loom_MPU6050.h"
#include "Sensors/I2C/Loom_MS5803.h"
#include "Sensors/I2C/Loom_SHT31D.h"
#include "Sensors/I2C/Loom_TSL2561.h"
#include "Sensors/I2C/Loom_TSL2591.h"
#include "Sensors/I2C/Loom_ZXGesture.h"

/////////////////////////////////////////////////////////////////////
// SPI Sensors
#include "Sensors/SPI/Loom_MAX31856.h"

/////////////////////////////////////////////////////////////////////
// SDI-12 Sensors
#include "Sensors/SDI12/Loom_Decagon_GS3.h"
#include "Sensors/SDI12/Loom_Decagon_5TM.h"

/////////////////////////////////////////////////////////////////////
// RTC
#include "RTC/Loom_DS3231.h"
#include "RTC/Loom_PCF8523.h"

/////////////////////////////////////////////////////////////////////
// Sleep Manager
#include "Loom_Interrupt_Manager.h"
#include "Loom_Sleep_Manager.h"

/////////////////////////////////////////////////////////////////////
// Actuators
#include "Actuators/Loom_Neopixel.h"
#include "Actuators/Loom_Relay.h"
#include "Actuators/Loom_Servo.h"
#include "Actuators/Loom_Stepper.h"

/////////////////////////////////////////////////////////////////////


// Device Manager
// #include "Loom_Manager.h"

#include "Loom_Module_Factory.h"
