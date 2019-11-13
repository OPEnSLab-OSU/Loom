///////////////////////////////////////////////////////////////////////////////
//
// This sketch will be used during the Loom AGU demo
//
// Given a set of sensors and credentials, this sketch will publish data to Spool,
// where it can be viewed and manipulated. It is mostly designed to be hands free.
//
// On startup, this sketch will load a config from "config.json"
//
// In the config, you need:
// - An SSID/Pass for a wireless network
// - A public/private key pair for Spool, which will be inputed through the serial
// monitor on the first startup
//
///////////////////////////////////////////////////////////////////////////////
#include <SdFat.h>
#include <Loom.h>
#include "SpoolKeyCache.h"

// Set enabled modules
LoomFactory<
	Enable::Internet::WiFi,
	Enable::Sensors::Enabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

///
/// Popolates obj to with the following items:
/// {
///		"name": "Spool"
///		"params": [
///			"Spool",
///			7002,
///			"spool.open-sensing.org",
///			"/device/data",
///			${credentials.id},
///			${credentials.certificate},
///			${credentials.key}
///		]
/// }
/// The above object can then be appended into the components array
/// in a Loom configuration.
///
void make_spool_component(JsonObject obj, const SpoolKeys& credentials) {
	obj["name"] = "Spool";
	JsonArray params = obj.createNestedArray("params");
	params.add("Spool");
	params.add(7002);
	params.add("spool.open-sensing.org");
	params.add("/device/data");
	params.add(static_cast<const char*>(credentials.id));
	params.add(static_cast<const char*>(credentials.certificate));
	params.add(static_cast<const char*>(credentials.key));
}

bool read_config_from_sd(JsonDocument& doc, const int cs, const char* fname) {
	SdFat sd;	// File system object

	LPrintln("Read config from file: '", fname, "'");
	if ( !sd.begin(cs, SD_SCK_MHZ(50)) ) {	// Make sure we can communicate with SD
		LPrintln("SD failed to begin");
		return false;
	}

	File file = sd.open(fname);
	if (!file) {	// Make sure file exists
		LPrintln("Failed to open '", fname, "'");
		return false;
	}

	DeserializationError error = deserializeJson(doc, file);

	// Test if parsing succeeds.
	if (error) { // Make sure json was valid
		LPrintln("deserializeJson() failed: ", error.c_str());
		return false;
	}

	return true;
}

void setup() {
	// TODO: hypnos control
	// TODO: verify with eli that the device IDs are actually only alphanumerical
	// Needs to be done for Hypno Board
	pinMode(5, OUTPUT);   // Enable control of 3.3V rail 
	pinMode(6, OUTPUT);   // Enable control of 5V rail 

	//See Above
	digitalWrite(5, LOW); // Enable 3.3V rail
	digitalWrite(6, HIGH);  // Enable 5V rail

	// put your setup code here, to run once:
	Serial.begin(115200);
	while(!Serial);

	// handle the spool credentials
	SpoolKeys keys;
	getKeys(keys);

	Serial.print("Loaded keys for id \"");
	Serial.print(keys.id);
	Serial.print("\" from time ");
	Serial.println(keys.timestamp);

	// read the JSON config from an SD card
	StaticJsonDocument<2048> doc;
	read_config_from_sd(doc, 10, "data.json");

	// inject the spool credentials into this config
	JsonObject spoolComponent = doc.as<JsonObject>()["components"].createNestedObject();
	if (!spoolComponent.isNull())
		make_spool_component(spoolComponent, keys);
	else
		Serial.println("Could not make spool conponent, are we out of memory?");

	Loom.parse_config_json(doc.as<JsonObject>());
	// DO NOT PRINT THE CONFIG! this will leak the secrets to the serial monitor

	LPrintln("\n ** Setup Complete ** "); 
}

void loop() {
	// put your main code here, to run repeatedly:
	Loom.measure();
	Loom.package();
	Loom.display_data();

	if(!Loom.Spool().publish()){
		pinMode(13, OUTPUT);
		digitalWrite(13, HIGH);
		Serial.println("Broke! press any key to continue...");
		while(!Serial.available());
		while(Serial.available()) Serial.read();
	}

	Loom.pause();
}