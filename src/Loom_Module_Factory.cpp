
#include "Loom_Module_Factory.h"

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
#include "CommPlats/Loom_SlipSerial.h"

// InternetPlats

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

// General
#include "Loom_Interrupt_Manager.h"
#include "Loom_Sleep_Manager.h"
#include "Loom_Multiplexer.h" // this needs to be include after I2C sensors (due to conflict with enableInterrupt macro/function defined by EnableInterrupt library and AS726X sensors)


// typedef

// typedef struct {
// 	const char* module_name,
// 	const 
// } NameCtorPair








void module_factory_aux(
	JsonVariant module, 
	Loom_Interrupt_Manager*& interrupt_manager, 
	Loom_Sleep_Manager*& sleep_manager,
	LoomRTC*& rtc,
	LoomModule*& other_module, 
	LoomSensor*& sensor,
	LoomActuator*& actuator,
	LoomCommPlat*& comm_plat,
	LoomInternetPlat*& internet_plat,
	LoomLogPlat*& log_plat
	) 
{
	Println("= = = = = Generate Objects = = = = =");
	Println();


	Println("\n\nModule Info: ");
	serializeJsonPretty(module, Serial);
	Println("\n");


	const char* module_type = module["name"];
	JsonVariant p  = module["parameters"]; 

	Println2("Module Name:: ", module_type);
	Println2("Parameter count:: ", p.size());





	// --- General ---

	if (strcmp(module_type, "Loom_Interrupt_Manager") == 0) {
		Println("Create Interrupt_Manager");
		if (p.size() >= 2) {
			interrupt_manager = new Loom_Interrupt_Manager(p);
			// interrupt_manager = new Loom_Interrupt_Manager(p[0], nullptr);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_Sleep_Manager") == 0) {
		Println("Create Sleep_Manager");
		if (p.size() >= 2) {
			sleep_manager = new Loom_Sleep_Manager(p);
			// sleep_manager = new Loom_Sleep_Manager(p[0], nullptr, p[2], p[3], (SleepMode)(int)p[4]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_Multiplexer") == 0) {
		Println("Create Multiplexer");
		if (p.size() >= 5) {
			other_module = new Loom_Multiplexer(p);
			// other_module = new Loom_Multiplexer(p[0], p[1], p[2], p[3], p[4]);
		}
		return;
	} 


	// --- Actuators ---

	if (strcmp(module_type, "Loom_Neopixel") == 0) {
		Println("Create Neopixel");
		if (p.size() >= 4) {
			actuator = new Loom_Neopixel(p);
			// actuator = new Loom_Neopixel(p[0], p[1], p[2], p[3]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_Relay") == 0) {
		Println("Create Relay");
		if (p.size() >= 2) {
			// actuator = new Loom_Relay(p[0], p[1]);
			actuator = new Loom_Relay(p);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_Servo") == 0) {
		Println("Create Servo");
		if (p.size() >= 2) {
			actuator = new Loom_Servo(p);
			// actuator = new Loom_Servo(p[0], p[1]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_Stepper") == 0) {
		Println("Create Stepper");
		if (p.size() >= 2) {
			actuator = new Loom_Stepper(p);
			// actuator = new Loom_Stepper(p[0], p[1]);
		}
		return;
	} 


	// --- CommPlats --- 

	if (strcmp(module_type, "Loom_LoRa") == 0) {
		Println("Create LoRa");
		if (p.size() >= 8) {
			comm_plat = new Loom_LoRa(p);
			// comm_plat = new Loom_LoRa(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_nRF") == 0) {
		Println("Create nRF");
		if (p.size() >= 10) {
			comm_plat = new Loom_nRF(p);
			// comm_plat = new Loom_nRF(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
		}
		return;
	} 

	// if (strcmp(module_type, "SlipSerial") == 0) {
	// 	Println("Create SlipSerial");
	// 	if (p.size() >= 3) {
	// 		? new Loom_SlipSerial(p);
			// ? new Loom_SlipSerial(p[0], p[1], p[2]);


	// --- InternetPlats --- 



	// --- LogPlats ---

	if (strcmp(module_type, "Loom_OLED") == 0) {
		Println("Create OLED");
		if (p.size() >= 9) {
			log_plat = new Loom_OLED(p);
			// log_plat = new Loom_OLED(p[0], p[1], p[2], (OLED_Version)(int)p[3], p[4], (OLED_Format)(int)p[5], p[6], p[7], (OLED_Freeze)(int)p[8]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_SD") == 0) {
		Println("Create SD");
		if (p.size() >= 5) {
			log_plat = new Loom_SD(p);
			// log_plat = new Loom_SD(p[0], p[1], p[2], p[3], p[4]);
		}
		return;
	} 


	// --- RTC --- 

	if (strcmp(module_type, "Loom_DS3231") == 0) {
		Println("Create DS3231");
		if (p.size() >= 5) {
			rtc = new Loom_DS3231(p);
			// rtc = new Loom_DS3231(p[0], (TimeZone)(int)p[1], p[2], p[3], p[4]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_PCF8523") == 0) {
		Println("Create PCF8523");
		if (p.size() >= 5) {
			rtc = new Loom_PCF8523(p);
			// rtc = new Loom_PCF8523(p[0], (TimeZone)(int)p[1], p[2], p[3], p[4]);
		}
		return;
	} 


	// --- Sensors ---

	if (strcmp(module_type, "Loom_Analog") == 0) {
		Println("Create Analog");
		if (p.size() >= 15) {
			// sensor = new Loom_Analog(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], (AnalogConversion)(int)p[9], (AnalogConversion)(int)p[10], (AnalogConversion)(int)p[11], (AnalogConversion)(int)p[12], (AnalogConversion)(int)p[13], (AnalogConversion)(int)p[14]);
			sensor = new Loom_Analog(p);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_Digital") == 0) {
		Println("Create Digital");
		if (p.size() >= 13) {
			// sensor = new Loom_Digital(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12]);
			sensor = new Loom_Digital(p);
		}
		return;
	} 


	//    --- I2C Sensors ---

	if (strcmp(module_type, "Loom_AS7262") == 0) {
		Println("Create AS7262");
		if (p.size() >= 6) {
			sensor = new Loom_AS7262(p);
			// sensor = new Loom_AS7262(p[0], p[1], p[2], p[3], p[4], p[5]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_AS7263") == 0) {
		Println("Create AS7263");
		if (p.size() >= 6) {
			sensor = new Loom_AS7263(p);
			// sensor = new Loom_AS7263(p[0], p[1], p[2], p[3], p[4], p[5]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_AS7265X") == 0) {
		Println("Create AS7265X");
		if (p.size() >= 6) {
			sensor = new Loom_AS7265X(p);
			// sensor = new Loom_AS7265X(p[0], p[1], p[2], p[3], p[4], p[5]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_FXAS21002") == 0) {
		Println("Create FXAS21002");
		if (p.size() >= 2) {
			sensor = new Loom_FXAS21002(p);
			// sensor = new Loom_FXAS21002(p[0], p[1]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_FXOS8700") == 0) {
		Println("Create FXOS8700");
		if (p.size() >= 2) {
			sensor = new Loom_FXOS8700(p);
			// sensor = new Loom_FXOS8700(p[0], p[1]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_LIS3DH") == 0) {
		Println("Create LIS3DH");
		if (p.size() >= 2) {
			sensor = new Loom_LIS3DH(p);
			// sensor = new Loom_LIS3DH(p[0], p[1]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_MB1232") == 0) {
		Println("Create MB1232");
		if (p.size() >= 2) {
			sensor = new Loom_MB1232(p);
			// sensor = new Loom_MB1232(p[0], p[1]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_MS5803") == 0) {
		Println("Create MS5803");
		if (p.size() >= 2) {
			sensor = new Loom_MS5803(p);
			// sensor = new Loom_MS5803(p[0], p[1]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_SHT31D") == 0) {
		Println("Create SHT31D");
		if (p.size() >= 2) {
			sensor = new Loom_SHT31D(p);
			// sensor = new Loom_SHT31D(p[0], p[1]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_TSL2561") == 0) {
		Println("Create TSL2561");
		if (p.size() >= 4) {
			sensor = new Loom_TSL2561(p);
			// sensor = new Loom_TSL2561(p[0], p[1], p[2], p[3]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_TSL2591") == 0) {
		Println("Create TSL2591");
		if (p.size() >= 4) {
			sensor = new Loom_TSL2591(p);
			// sensor = new Loom_TSL2591(p[0], p[1], p[2], p[3]);
		}
		return;
	} 

	if (strcmp(module_type, "Loom_ZXGesture") == 0) {
		Println("Create ZXGesture");
		if (p.size() >= 3) {
			sensor = new Loom_ZXGesture(p);
			// sensor = new Loom_ZXGesture(p[0], p[1], (ZXMode)(int)p[2]);
		}
		return;
	} 


	//    --- SDI12 Sensors ---

	if (strcmp(module_type, "Loom_Decagon5TM") == 0) {
		Println("Create 5TM Decagon");
		if (p.size() >= 2) {
			sensor = new Loom_Decagon5TM(p);
			// sensor = new Loom_Decagon5TM(p[0], p[1]);
		}
		return;
	}

	if (strcmp(module_type, "Loom_DecagonGS3") == 0) {
		Println("Create GS3 Decagon");
		if (p.size() >= 2) {
			sensor = new Loom_DecagonGS3(p);
			// sensor = new Loom_DecagonGS3(p[0], p[1]);
		}
		return;
	}


	//    --- SPI Sensors ---

	if (strcmp(module_type, "Loom_MAX31856") == 0) {
		Println("Create MAX31856");
		if (p.size() >= 6) {
			sensor = new Loom_MAX31856(p);
			// sensor = new Loom_MAX31856(p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
		}
		return;
	} 



	Println2("No match found for: ", module_type);

}

















// void module_factory_aux(
// 	JsonVariant module, 
// 	Loom_Interrupt_Manager*& interrupt_manager, 
// 	Loom_Sleep_Manager*& sleep_manager,
// 	LoomRTC*& rtc,
// 	LoomModule*& other_module, 
// 	LoomSensor*& sensor,
// 	LoomActuator*& actuator,
// 	LoomCommPlat*& comm_plat,
// 	LoomInternetPlat*& internet_plat,
// 	LoomLogPlat*& log_plat
// 	) 
// {
// 	Println("= = = = = Generate Objects = = = = =");
// 	Println();


// 	Println("\n\nModule Info: ");
// 	serializeJsonPretty(module, Serial);
// 	Println("\n");


// 	const char* module_type = module["name"];
// 	JsonVariant p  = module["parameters"]; 

// 	Println2("Module Name:: ", module_type);
// 	Println2("Parameter count:: ", p.size());





// 	// --- General ---

// 	if (strcmp(module_type, "Loom_Interrupt-Manager") == 0) {
// 		Println("Create Interrupt_Manager");
// 		if (p.size() >= 2) {
// 			interrupt_manager = new Loom_Interrupt_Manager(p[0], nullptr);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_Sleep-Manager") == 0) {
// 		Println("Create Sleep_Manager");
// 		if (p.size() >= 2) {
// 			sleep_manager = new Loom_Sleep_Manager(p[0], nullptr, p[2], p[3], (SleepMode)(int)p[4]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_Multiplexer") == 0) {
// 		Println("Create Multiplexer");
// 		if (p.size() >= 5) {
// 			other_module = new Loom_Multiplexer(p[0], p[1], p[2], p[3], p[4]);
// 		}
// 		return;
// 	} 


// 	// --- Actuators ---

// 	if (strcmp(module_type, "Loom_Neopixel") == 0) {
// 		Println("Create Neopixel");
// 		if (p.size() >= 4) {
// 			actuator = new Loom_Neopixel(p[0], p[1], p[2], p[3]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_Relay") == 0) {
// 		Println("Create Relay");
// 		if (p.size() >= 2) {
// 			actuator = new Loom_Relay(p[0], p[1]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_Servo") == 0) {
// 		Println("Create Servo");
// 		if (p.size() >= 2) {
// 			actuator = new Loom_Servo(p[0], p[1]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_Stepper") == 0) {
// 		Println("Create Stepper");
// 		if (p.size() >= 2) {
// 			actuator = new Loom_Stepper(p[0], p[1]);
// 		}
// 		return;
// 	} 


// 	// --- CommPlats --- 

// 	if (strcmp(module_type, "Loom_LoRa") == 0) {
// 		Println("Create LoRa");
// 		if (p.size() >= 8) {
// 			comm_plat = new Loom_LoRa(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_nRF") == 0) {
// 		Println("Create nRF");
// 		if (p.size() >= 10) {
// 			comm_plat = new Loom_nRF(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9]);
// 		}
// 		return;
// 	} 

// 	// if (strcmp(module_type, "SlipSerial") == 0) {
// 	// 	Println("Create SlipSerial");
// 	// 	if (p.size() >= 3) {
// 	// 		? new Loom_SlipSerial(p[0], p[1], p[2]);
// // }
// // return;
// 	// } 


// 	// --- InternetPlats --- 



// 	// --- LogPlats ---

// 	if (strcmp(module_type, "Loom_OLED") == 0) {
// 		Println("Create OLED");
// 		if (p.size() >= 9) {
// 			log_plat = new Loom_OLED(p[0], p[1], p[2], (OLED_Version)(int)p[3], p[4], (OLED_Format)(int)p[5], p[6], p[7], (OLED_Freeze)(int)p[8]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_SD") == 0) {
// 		Println("Create SD");
// 		if (p.size() >= 5) {
// 			log_plat = new Loom_SD(p[0], p[1], p[2], p[3], p[4]);
// 		}
// 		return;
// 	} 


// 	// --- RTC --- 

// 	if (strcmp(module_type, "Loom_DS3231") == 0) {
// 		Println("Create DS3231");
// 		if (p.size() >= 5) {
// 			rtc = new Loom_DS3231(p[0], (TimeZone)(int)p[1], p[2], p[3], p[4]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_PCF8523") == 0) {
// 		Println("Create PCF8523");
// 		if (p.size() >= 5) {
// 			rtc = new Loom_PCF8523(p[0], (TimeZone)(int)p[1], p[2], p[3], p[4]);
// 		}
// 		return;
// 	} 


// 	// --- Sensors ---

// 	if (strcmp(module_type, "Loom_Analog") == 0) {
// 		Println("Create Analog");
// 		if (p.size() >= 15) {
// 			sensor = new Loom_Analog(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], (AnalogConversion)(int)p[9], (AnalogConversion)(int)p[10], (AnalogConversion)(int)p[11], (AnalogConversion)(int)p[12], (AnalogConversion)(int)p[13], (AnalogConversion)(int)p[14]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_Digital") == 0) {
// 		Println("Create Digital");
// 		if (p.size() >= 13) {
// 			sensor = new Loom_Digital(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12]);
// 		}
// 		return;
// 	} 


// 	//    --- I2C Sensors ---

// 	if (strcmp(module_type, "Loom_AS7262") == 0) {
// 		Println("Create AS7262");
// 		if (p.size() >= 6) {
// 			sensor = new Loom_AS7262(p[0], p[1], p[2], p[3], p[4], p[5]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_AS7263") == 0) {
// 		Println("Create AS7263");
// 		if (p.size() >= 6) {
// 			sensor = new Loom_AS7263(p[0], p[1], p[2], p[3], p[4], p[5]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_AS7265X") == 0) {
// 		Println("Create AS7265X");
// 		if (p.size() >= 6) {
// 			sensor = new Loom_AS7265X(p[0], p[1], p[2], p[3], p[4], p[5]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_FXAS21002") == 0) {
// 		Println("Create FXAS21002");
// 		if (p.size() >= 2) {
// 			sensor = new Loom_FXAS21002(p[0], p[1]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_FXOS8700") == 0) {
// 		Println("Create FXOS8700");
// 		if (p.size() >= 2) {
// 			sensor = new Loom_FXOS8700(p[0], p[1]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_LIS3DH") == 0) {
// 		Println("Create LIS3DH");
// 		if (p.size() >= 2) {
// 			sensor = new Loom_LIS3DH(p[0], p[1]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_MB1232") == 0) {
// 		Println("Create MB1232");
// 		if (p.size() >= 2) {
// 			sensor = new Loom_MB1232(p[0], p[1]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_MS5803") == 0) {
// 		Println("Create MS5803");
// 		if (p.size() >= 2) {
// 			sensor = new Loom_MS5803(p[0], p[1]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_SHT31D") == 0) {
// 		Println("Create SHT31D");
// 		if (p.size() >= 2) {
// 			sensor = new Loom_SHT31D(p[0], p[1]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_TSL2561") == 0) {
// 		Println("Create TSL2561");
// 		if (p.size() >= 4) {
// 			sensor = new Loom_TSL2561(p[0], p[1], p[2], p[3]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_TSL2591") == 0) {
// 		Println("Create TSL2591");
// 		if (p.size() >= 4) {
// 			sensor = new Loom_TSL2591(p[0], p[1], p[2], p[3]);
// 		}
// 		return;
// 	} 

// 	if (strcmp(module_type, "Loom_ZXGesture") == 0) {
// 		Println("Create ZXGesture");
// 		if (p.size() >= 3) {
// 			sensor = new Loom_ZXGesture(p[0], p[1], (ZXMode)(int)p[2]);
// 		}
// 		return;
// 	} 


// 	//    --- SDI12 Sensors ---

// 	if (strcmp(module_type, "Loom_Decagon5TM") == 0) {
// 		Println("Create 5TM Decagon");
// 		if (p.size() >= 2) {
// 			sensor = new Loom_Decagon5TM(p[0], p[1]);
// 		}
// 		return;
// 	}

// 	if (strcmp(module_type, "Loom_DecagonGS3") == 0) {
// 		Println("Create GS3 Decagon");
// 		if (p.size() >= 2) {
// 			sensor = new Loom_DecagonGS3(p[0], p[1]);
// 		}
// 		return;
// 	}


// 	//    --- SPI Sensors ---

// 	if (strcmp(module_type, "Loom_MAX31856") == 0) {
// 		Println("Create MAX31856");
// 		if (p.size() >= 6) {
// 			sensor = new Loom_MAX31856(p[0], p[1], p[2], p[3], p[4], p[5], p[6]);
// 		}
// 		return;
// 	} 



// 	Println2("No match found for: ", module_type);

// }















