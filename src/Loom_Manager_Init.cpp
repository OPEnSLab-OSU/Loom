
// This is given its own file for the sake of separating the section of code that
// will change with module additions from the rest of the implementation of Loom_Manager

#include "Loom_Module.h"
#include "Loom_Manager.h"
#include "Loom_Module_Factory.h"

#include <ArduinoJson.h>


// const char* json_config = "{\"general\":{\"device_name\":\"name\",\"instance_num\":1,\"family_num\":0},\"components\":{\"Device4\":{\"desc\":\"Some description\",\"parameters\":[\"a string\",123,4.56,true,\"0\"]}}}";
const char* json_config = "{\"general\":{\"device_name\":\"name\",\"instance_num\":1,\"family_num\":0},\"components\":[{\"name\":\"Device1\",\"desc\":\"Some description of device 1\",\"parameters\":[123,4.56,true]},{\"name\":\"Device2\",\"desc\":\"Some description of device 2\",\"parameters\":[123,4.56,-789,123,123,123]},{\"name\":\"Device3\",\"desc\":\"Some description of device 3\",\"parameters\":[\"North\",0,1,2,3]}]}";

void LoomManager::parse_config()
{
	// Parse Json config

	Println();
	Println("= = = = = Parse Config = = = = =");

	DynamicJsonDocument doc(2048);
	DeserializationError error = deserializeJson(doc, json_config);

	// Test if parsing succeeds.
	if (error) {
		Serial.print(F("deserializeJson() failed: "));
		Serial.println(error.c_str());
		return;
	}


	Serial.println("\n\nConfig Pretty Version:");
	serializeJsonPretty(doc, Serial);

	Serial.println("\n\nGeneral Settings:");
	serializeJsonPretty(doc["general"], Serial);

	Serial.print("\n\nNum modules: ");
	Serial.println(doc["components"].size()); 
	Println();


	// Apply LoomManager General Settings
	Println("= = = = = LoomManager Settings = = = = =");
	Println();

	// JsonObject general = doc["general"];

	// if (general.containsKey("device_name")) {
	// 	strcpy(this->device_name, general["device_name"]);
	// }
	// if (general.containsKey("family")) {
	// 	strcpy(this->family, general["family"]);
	// }
	// if (general.containsKey("family_num")) {
	// 	this->family_num = general["family_num"];
	// }
	// if (general.containsKey("instance")) {
	// 	this->instance = general["instance"];
	// }
	// if (general.containsKey("device_type")) {
	// 	this->device_type = (DeviceType)(int)general["device_type"];
	// }
	// if (general.containsKey("print_verbosity")) {
	// 	this->print_verbosity = (Verbosity)(int)general["print_verbosity"];
	// }
	// if (general.containsKey("package_verbosity")) {
	// 	this->package_verbosity = (Verbosity)(int)general["package_verbosity"];
	// }

	print_config();


			// char*		device_name			= "Default",
			// char*		family				= "Loom",
			// uint		family_num			= 1,
			// uint		instance			= 1,
			// DeviceType	device_type			= DeviceType::NODE,
			// Verbosity	print_verbosity		= Verbosity::V_HIGH,
			// Verbosity	package_verbosity	=




	// Generate Module Objects

	Println("= = = = = Generate Objects = = = = =");
	Println();


	// Call module factory
	for ( JsonVariant module : doc["components"].as<JsonArray>()) {
		module_factory_aux( module );
	}

	Println("= = = = = = = = = = = = = = = = =");
	Println();


}




// LoomModule* module_factory_aux(JsonVariant module) 
// {

// 	Println("\n\nModule Info: ");
// 	serializeJsonPretty(module, Serial);
// 	Println("\n");


// 	const char* module_type = module["name"];
// 	JsonVariant p  = module["parameters"]; 

// 	Println2("Module Name:: ", module_type);
// 	Println2("Parameter count:: ", p.size());



	
// 	LoomModule* tmp =  new Loom_Interrupt_Manager(p[0], p[1]);

// 	return tmp;

// 	// --- General ---

// 	if (strcmp(module_type, "Interrupt_Manager") == 0) {
// 		Println("Create Interrupt_Manager");
// 		return (p.size() >= 2) 
// 			? new Loom_Interrupt_Manager(p[0], p[1])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "Sleep_Manager") == 0) {
// 		Println("Create Sleep_Manager");
// 		return (p.size() >= 2) 
// 			? new Loom_Sleep_Manager(p[0], p[1], p[2], p[3], (SleepMode)(int)p[4])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "Multiplexer") == 0) {
// 		Println("Create Multiplexer");
// 		return (p.size() >= 4) 
// 			? new Loom_Multiplexer(p[0], p[1], p[2], p[3], p[4])
// 			: nullptr;
// 	} 


// 	// --- Actuators ---

// 	if (strcmp(module_type, "Neopixel") == 0) {
// 		Println("Create Neopixel");
// 		return (p.size() >= 4) 
// 			? new Loom_Neopixel(p[0], p[1], p[2], p[3])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "Relay") == 0) {
// 		Println("Create Relay");
// 		return (p.size() >= 2) 
// 			? new Loom_Relay(p[0], p[1])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "Servo") == 0) {
// 		Println("Create Servo");
// 		return (p.size() >= 2) 
// 			? new Loom_Servo(p[0], p[1])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "Stepper") == 0) {
// 		Println("Create Stepper");
// 		return (p.size() >= 2) 
// 			? new Loom_Stepper(p[0], p[1])
// 			: nullptr;
// 	} 


// 	// --- CommPlats --- 

// 	if (strcmp(module_type, "LoRa") == 0) {
// 		Println("Create LoRa");
// 		return (p.size() >= 8) 
// 			? new Loom_LoRa(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "nRF") == 0) {
// 		Println("Create nRF");
// 		return (p.size() >= 10) 
// 			? new Loom_nRF(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "SlipSerial") == 0) {
// 		Println("Create SlipSerial");
// 		return (p.size() >= 3) 
// 			? new Loom_SlipSerial(p[0], p[1], p[2])
// 			: nullptr;
// 	} 


// 	// --- InternetPlats --- 



// 	// --- LogPlats ---

// 	if (strcmp(module_type, "OLED") == 0) {
// 		Println("Create OLED");
// 		return (p.size() >= 9) 
// 			? new Loom_OLED(p[0], p[1], p[2], (OLED_Version)(int)p[3], p[4], (OLED_Format)(int)p[5], p[6], p[7], (OLED_Freeze)(int)p[8])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "SD") == 0) {
// 		Println("Create SD");
// 		return (p.size() >= 5) 
// 			? new Loom_SD(p[0], p[1], p[2], p[3], p[4])
// 			: nullptr;
// 	} 


// 	// --- RTC --- 

// 	if (strcmp(module_type, "DS3231") == 0) {
// 		Println("Create DS3231");
// 		return (p.size() >= 5) 
// 			? new Loom_DS3231(p[0], (TimeZone)p[1], p[2], p[3], p[4])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "PCF8523") == 0) {
// 		Println("Create PCF8523");
// 		return (p.size() >= 5) 
// 			? new Loom_PCF8523(p[0], (TimeZone)p[1], p[2], p[3], p[4])
// 			: nullptr;
// 	} 


// 	// --- Sensors ---

// 	if (strcmp(module_type, "Analog") == 0) {
// 		Println("Create Analog");
// 		return (p.size() >= 15) 
// 			? new Loom_Analog(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], (AnalogConversion)(int)p[10], (AnalogConversion)(int)p[11], (AnalogConversion)(int)p[12], (AnalogConversion)(int)p[13], (AnalogConversion)(int)p[14])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "Digital") == 0) {
// 		Println("Create Digital");
// 		return (p.size() >= 13) 
// 			? new Loom_Digital(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10], p[11], p[12])
// 			: nullptr;
// 	} 


// 	//    --- I2C Sensors ---

// 	if (strcmp(module_type, "AS7262") == 0) {
// 		Println("Create AS7262");
// 		return (p.size() >= 6) 
// 			? new Loom_AS7262(p[0], p[1], p[2], p[3], p[4], p[5])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "AS7263") == 0) {
// 		Println("Create AS7263");
// 		return (p.size() >= 6) 
// 			? new Loom_AS7263(p[0], p[1], p[2], p[3], p[4], p[5])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "AS7265X") == 0) {
// 		Println("Create AS7265X");
// 		return (p.size() >= 6) 
// 			? new Loom_AS7265X(p[0], p[1], p[2], p[3], p[4], p[5])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "FXAS21002") == 0) {
// 		Println("Create FXAS21002");
// 		return (p.size() >= 2) 
// 			? new Loom_FXAS21002(p[0], p[1])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "FXOS8700") == 0) {
// 		Println("Create FXOS8700");
// 		return (p.size() >= 2) 
// 			? new Loom_FXOS8700(p[0], p[1])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "LIS3DH") == 0) {
// 		Println("Create LIS3DH");
// 		return (p.size() >= 2) 
// 			? new Loom_LIS3DH(p[0], p[1])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "MB1232") == 0) {
// 		Println("Create MB1232");
// 		return (p.size() >= 2) 
// 			? new Loom_MB1232(p[0], p[1])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "MS5803") == 0) {
// 		Println("Create MS5803");
// 		return (p.size() >= 2) 
// 			? new Loom_MS5803(p[0], p[1])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "SHT31D") == 0) {
// 		Println("Create SHT31D");
// 		return (p.size() >= 2) 
// 			? new Loom_SHT31D(p[0], p[1])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "TSL2561") == 0) {
// 		Println("Create TSL2561");
// 		return (p.size() >= 4) 
// 			? new Loom_TSL2561(p[0], p[1], p[2], p[3])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "TSL2591") == 0) {
// 		Println("Create TSL2591");
// 		return (p.size() >= 4) 
// 			? new Loom_TSL2591(p[0], p[1], p[2], p[3])
// 			: nullptr;
// 	} 

// 	if (strcmp(module_type, "ZXGesture") == 0) {
// 		Println("Create ZXGesture");
// 		return (p.size() >= 3) 
// 			? new Loom_ZXGesture(p[0], p[1], (ZXMode)(int)p[2])
// 			: nullptr;
// 	} 


// 	//    --- SDI12 Sensors ---

// 	if (strcmp(module_type, "5TM Decagon") == 0) {
// 		Println("Create 5TM Decagon");
// 		return (p.size() >= 2) 
// 			? new Loom_Decagon5TM(p[0], p[1])
// 			: nullptr;
// 	}

// 	if (strcmp(module_type, "GS3 Decagon") == 0) {
// 		Println("Create GS3 Decagon");
// 		return (p.size() >= 2) 
// 			? new Loom_DecagonGS3(p[0], p[1])
// 			: nullptr;
// 	}


// 	//    --- SPI Sensors ---

// 	if (strcmp(module_type, "MAX31856") == 0) {
// 		Println("Create MAX31856");
// 		return (p.size() >= 6) 
// 			? new Loom_MAX31856(p[0], p[1], p[2], p[3], p[4], p[5], p[6])
// 			: nullptr;
// 	} 



// 	Println2("No match found for: ", module_type);
// 	return nullptr;

// }


