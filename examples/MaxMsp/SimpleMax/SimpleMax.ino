///////////////////////////////////////////////////////////////////////////////

// This example has a few different configurations provided, but only one can 
// be used at a time. 

// The purpose of each config is as follows:

// - config.h          : A basic analog and digital logging to Max
// - config_neopixel.h : Use to control a neopixel on pins A0, A1, or A2
// - config_relay.h    : Use to control a relay on pin 10
// - config_servo.h    : Use to control servos using Adafruit fetherwing 
//							servo controller V2
// - config_stepper.h  : Use to control steppers using Adafruit featherwing 
//							stepper controller V2

// To switch between which configuration will be loaded to the device, change 
//	the '#include "config.h"' line in the SimpleMax.ino sketch file to to 
//	configuration you want to use. 

// You can combine modules from the different configurations as hardware permits.

// See the documentation (https://openslab-osu.github.io/Loom/html/index.html) 
//	for available options for modules.

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// In config, instance number is number to set ModuleCore to in Max MSP

// In Tools menu, set:
// Internet  > WiFi
// Sensors   > Enabled
// Radios    > Enabled
// Actuators > Enabled
// Max       > Enabled

using namespace Loom;

Loom::Manager Exec{};


void setup() 
{ 
	Exec.begin_LED();
	Exec.begin_serial(false);
	Exec.parse_config(LCONFIG);
	Exec.print_config(true);
	
	LPrintln("\n ** Setup Complete ** ");
}

void loop() 
{
	Exec.measure();
	Exec.package();
	Exec.display_data();
	Exec.get<Loom::MaxPub>().publish();
	Exec.get<Loom::MaxSub>().subscribe();
	Exec.pause();
}
