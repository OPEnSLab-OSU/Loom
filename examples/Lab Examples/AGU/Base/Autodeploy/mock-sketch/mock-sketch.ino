#include <cstdio>
#include "ArduinoJson.h"
#include "Bootloader.h"


void setup() {
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