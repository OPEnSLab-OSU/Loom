#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// Set enabled modules
LoomFactory<
	Enable::Internet::LTE,
	Enable::Sensors::Enabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

volatile bool rtc_flag = false;

void wakeISR_RTC() {
	// disable the interrupt
	detachInterrupt(12);
	rtc_flag = true;
}

void setup()
{
	// Needs to be done for Hypno Board
	pinMode(5, OUTPUT);		// Enable control of 3.3V rail
	pinMode(6, OUTPUT);		// Enable control of 5V rail
	pinMode(12, INPUT_PULLUP);		// Enable waiting for RTC interrupt, MUST use a pullup since signal is active low
  pinMode(13, OUTPUT);

	//See Above
	digitalWrite(5, LOW);	// Enable 3.3V rail
	digitalWrite(6, HIGH);	// Enable 5V rail
  digitalWrite(13, LOW);

	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();

  delay(5000);
	// Register an interrupt on the RTC alarm pin
	Loom.InterruptManager().register_ISR(12, wakeISR_RTC, LOW, ISR_Type::IMMEDIATE);
  delay(5000);

	LPrintln("\n ** Setup Complete ** ");
	Serial.flush();
}


void loop(){
  digitalWrite(13, HIGH);
  digitalWrite(5, LOW); // Enable 3.3V rail
  digitalWrite(6, HIGH);  // Enable 5V rail

// As it turns out, if the SD card is initialized and you change
// the states of the pins to ANY VALUE, the SD card will fail to
// write. As a result, we ensure that the board has been turned
// off at least once before we make any changes to the pin states
if (rtc_flag) {
  pinMode(23, OUTPUT);
  pinMode(24, OUTPUT);
  pinMode(10, OUTPUT);

  // delay(1000);

  Loom.power_up();
}

Loom.measure();
Loom.package();
Loom.display_data();

Loom.GoogleSheets().publish();

// set the RTC alarm to a duration of five minutes with TimeSpan
Loom.InterruptManager().RTC_alarm_duration(TimeSpan(0,0,5,0));
Loom.InterruptManager().reconnect_interrupt(12);


digitalWrite(13, LOW);
digitalWrite(5, HIGH); // Disable 3.3V rail
digitalWrite(6, LOW);  // Disable 5V rail

pinMode(23, INPUT);
pinMode(24, INPUT);
pinMode(10, INPUT);


rtc_flag = false;
Loom.power_down();
Loom.SleepManager().sleep();
Loom.power_up();
while (!rtc_flag);
}
