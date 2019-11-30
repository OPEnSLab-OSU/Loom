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
#include "Bootloader.h"

// Set enabled modules
LoomFactory<
	Enable::Internet::WiFi,
	Enable::Sensors::Enabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

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

	// delay 5 seconds, running the bootloader during that time, so we can report back status
	uint32_t start = millis();
	while(millis() - start < 5000) 
		Bootloader::run_bootloader();

	// get the config
    StaticJsonDocument<2048> doc;
    if (!Bootloader::get_config(doc))
		digitalWrite(13, LOW);
	// start Loom!
	Loom.set_print_verbosity(Verbosity::V_LOW);
	Loom.parse_config_json(doc.as<JsonObject>());

	LPrintln("\n ** Setup Complete ** "); 
}

void loop() {
	Bootloader::run_bootloader();
	// put your main code here, to run repeatedly:
	Loom.measure();
	Loom.package();
	Loom.display_data();

	/*
	if(!Loom.Spool().publish()){
		pinMode(13, OUTPUT);
		digitalWrite(13, HIGH);
		Serial.println("Broke! press any key to continue...");
		while(!Serial.available());
		while(Serial.available()) Serial.read();
	}*/

	const uint32_t start = millis();
	while (millis() - start < static_cast<uint32_t>(Loom.get_interval())) 
		Bootloader::run_bootloader();
}