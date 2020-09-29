#include <Loom.h>


// In Tools menu, set:
// Internet  > LTE
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Disabled
// Max       > Disabled


using namespace Loom;

Loom::Manager Exec{};

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

	Exec.begin_serial(true);
	Exec.parse_config(LCONFIG);
	Exec.print_config();

	delay(5000);
	// Register an interrupt on the RTC alarm pin
	Exec.get<Loom::InterruptManager>().register_ISR(12, wakeISR_RTC, LOW, ISR_Type::IMMEDIATE);
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

  Exec.power_up();
}

Exec.measure();
Exec.package();
Exec.display_data();

Exec.get<Loom::GoogleSheets>().publish();

// set the RTC alarm to a duration of five minutes with TimeSpan
Exec.get<Loom::InterruptManager>().RTC_alarm_duration(TimeSpan(0,0,5,0));
Exec.get<Loom::InterruptManager>().reconnect_interrupt(12);


digitalWrite(13, LOW);
digitalWrite(5, HIGH); // Disable 3.3V rail
digitalWrite(6, LOW);  // Disable 5V rail

pinMode(23, INPUT);
pinMode(24, INPUT);
pinMode(10, INPUT);


rtc_flag = false;
Exec.power_down();
Exec.get<Loom::SleepManager>().sleep();
Exec.power_up();
while (!rtc_flag);
}
