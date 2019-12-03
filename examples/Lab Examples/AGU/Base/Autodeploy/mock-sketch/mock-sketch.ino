#include <cstdio>
#include "ArduinoJson.h"
#include "Bootloader.h"


void setup() {
    // Needs to be done for Hypno Board
	pinMode(5, OUTPUT);   // Enable control of 3.3V rail 
	pinMode(6, OUTPUT);   // Enable control of 5V rail 

	//See Above
	digitalWrite(5, LOW); // Enable 3.3V rail
	digitalWrite(6, HIGH);  // Enable 5V rail
    // put your setup code here, to run once:
    Serial.begin(9600);
    while(!Serial);
	// get the config
    StaticJsonDocument<2048> doc;
    if (!Bootloader::get_config(doc)) 
        digitalWrite(13, LOW);
}

void loop() {
    // put your main code here, to run repeatedly:
    Bootloader::run_bootloader();
}