///////////////////////////////////////////////////////////////////////////////

// This sketch will be used during the Loom AGU demo

// Given a set of sensors and credentials, this sketch will publish data to Spool,
// where it can be viewed and manipulated. It is mostly designed to be hands free.

// On startup, this sketch will load a config from "config.json"

// In the config, you need:
// - An SSID/Pass for a wireless network
// - A public/private key pair for Spool, which will be inputed through the serial
// monitor on the first startup

///////////////////////////////////////////////////////////////////////////////
#include <SdFat.h>
#include "time_macros.h"
#include "SpoolKeyCache.h"
#include <Loom.h>

// Include configuration
const char* json_config = 
#include "config.h"
;

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

  // get

  Loom.begin_serial(true);
  Loom.parse_config(json_config);
  Loom.print_config();

  LPrintln("\n ** Setup Complete ** "); 
}

void loop() {
    // put your main code here, to run repeatedly:
    Loom.measure();
	Loom.package();
	Loom.display_data();

	if(!Loom.GoogleSheets().publish()){
    pinMode(13, OUTPUT);
	  digitalWrite(13, HIGH);
    Serial.println("Broke! press any key to continue...");
    while(!Serial.available());
    while(Serial.available()) Serial.read();
	}

	Loom.pause();
}

bool read_config_from_sd(StaticJsonDocument& doc, const char* fname) {
  SdFat sd;	// File system object

	LPrintln("Read config from file: '", fname, "'");
	if ( !sd.begin(SD_CS, SD_SCK_MHZ(50)) ) {	// Make sure we can communicate with SD
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