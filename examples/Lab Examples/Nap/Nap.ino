///////////////////////////////////////////////////////////////////////////////

// This example will NOT operate out of the box with Loom. Loom has deprecated
// it's built-in support for the Adafruit_SleepyDog Library in favor of general
// use of FeatherFault (https://github.com/OPEnSLab-OSU/FeatherFault).
//
// THESE TWO LIBRARIES ARE MUTUALLY EXCLUSIVE, BY USING NAP YOU ARE GIVING UP
// THE ABILITY TO USE FEATHERFAULT
//
// To use nap in an example with Loom, you have download the Adafruit_SleepyDog
// from here (https://github.com/adafruit/Adafruit_SleepyDog)
// and place it into your dependencies folder of your Arduino IDE
// This can be found in Documents/Arduino/libraries
// Alternatively, you can add it by searching in the Library Manager in the
// Arduino IDE
// If you have any issues with setting this up, please email or create an issue
// on our GitHub.

///////////////////////////////////////////////////////////////////////////////

#include <Adafruit_SleepyDog.h>
#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Enabled,
	Enable::Radios::Enabled,
	Enable::Actuators::Enabled,
	Enable::Max::Enabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

//This is the max amount of time that the Sleepy Dog library allows for napping, anything above this will be lowered down to 16000
//You can change this value to anything you'd like, so long as it's under 16000
uint16_t ms = 16000;


void setup()
{
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();

	LPrintln("\n ** Setup Complete ** ");
}


void loop()
{
	Loom.measure();
	Loom.package();
	Loom.display_data();


  //This is where the napping process starts
	LPrintln("Detaching USB Device");
  Serial.end();
  USBDevice.detach();

  // Sleep, with max time of 16000 milliseconds
	LPrintln("Napping");
  uint16_t sleepMS = Watchdog.sleep( (ms <= 16000) ? ms : 16000);

  //This is what occurs after waking up from the nap
  USBDevice.attach();
  Serial.begin(SERIAL_BAUD); //Serial Baud Rate is defined in Loom, you don't have to worry about it
	LPrintln("Woke up from nap");
	LPrintln("Slept for");
	LPrintln(sleepMS);
	LPrintln(" miliseconds");

  //After reattaching and begining the Serial, loop will start over again
}
