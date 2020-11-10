///////////////////////////////////////////////////////////////////////////////

// This code is for the OPEnS Lab Smart Rock and Smart Rock Beta Group

// Includes
// - SD
// - DS3231 RTC
// - Deep Sleep functionality thanks to Hypnos (link below for more information)
// - EC
// - Turbidity
// - Pressure
// - Temp

// Further details about the Hypnos board can be found here:
// https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/wiki/Hypnos

///////////////////////////////////////////////////////////////////////////////

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


const int switchPin = 9;
int switchPos = 0;

volatile bool rtc_flag = false;

void wakeISR_RTC() {
	// disable the interrupt
	detachInterrupt(12);
	rtc_flag = true;
}

void setup() 
{

  pinMode(switchPin, INPUT);
	// Needs to be done for Hypno Board
	pinMode(5, OUTPUT);		// Enable control of 3.3V rail 
	pinMode(6, OUTPUT);		// Enable control of 5V rail 
	pinMode(12, INPUT_PULLUP);		// Enable waiting for RTC interrupt, MUST use a pullup since signal is active low
  pinMode(13, OUTPUT);

  //digitalWrite(switchPin, HIGH);
	//See Above
	digitalWrite(5, LOW);	// Enable 3.3V rail
	digitalWrite(6, HIGH);	// Enable 5V rail
  digitalWrite(13, LOW);

	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();

	// Register an interrupt on the RTC alarm pin
	Loom.InterruptManager().register_ISR(12, wakeISR_RTC, LOW, ISR_Type::IMMEDIATE);

	LPrintln("\n ** Setup Complete ** ");
	Serial.flush();
}


void loop() 
{
  //re-enable pin upon wake up
  pinMode(switchPin, INPUT);

  /////////////////////////////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////////////////////
  //Start of loop that prevents Smart Rock from going to sleep
  //for calibration mode, where continuous data is logged to SD
  NO_SLEEP:
  
  //change interval depending the on position of pin
  //interval is the variable that gets passed to TimeSpan function below
  switchPos = digitalRead(switchPin);


  //delay(1000) function further down in the sketch takes 1000 miliseconds (or 1 second) to
  //ensure SD card has enough time to wake up before next data log
  //minus 1 second from your desired interval time below to gaurantee actual desired inteval
  //Example: 
  // 1199 seconds == 19 minutes 59 seconds 
  // 1199 seconds + delay() function = 1200 seconds
  // 1200 seconds == 20 minutes

  //sets all values to 0 upon start of loop
  //do not modify these
  int secs = 0;
  int mins = 0;
  int hours = 0;
  int days = 0;
  
  if (switchPos == HIGH) {

    //modifiable 
    //set desired seconds
    secs = 1199;
    
    //set desired minutes
    mins = 0;
    
    //set desired hours
    hours = 0;
    
    //set desired days
    days = 0;
    
  }else{

      Loom.measure();
      Loom.package();
      Loom.display_data();

      // Log using default filename as provided in configuration
      // in this case, 'datafile.csv'
      Loom.SDCARD().log();

      Loom.pause();
      
      //goes to the beginning of NO_SLEEP loop only if this else block is entered
      goto NO_SLEEP;
    }
  //////////////////////////////////////////////////////////////////////////////////////////////
  //////////////////////////////////////////////////////////////////////////////////////////////
  
	digitalWrite(5, LOW); // Disable 3.3V rail
  digitalWrite(6, HIGH);  // Disable 5V rail
  digitalWrite(13, HIGH);

	// As it turns out, if the SD card is initialized and you change
	// the states of the pins to ANY VALUE, the SD card will fail to
	// write. As a result, we ensure that the board has been turned
	// off at least once before we make any changes to the pin states
	if (rtc_flag) {
		pinMode(23, OUTPUT);
		pinMode(24, OUTPUT);
		pinMode(10, OUTPUT);

    delay(1000);

		Loom.power_up();
	}

  
	Loom.measure();
	Loom.package();
	Loom.display_data();
	Loom.SDCARD().log();

	// set the RTC alarm to a duration of 10 seconds with TimeSpan
	Loom.InterruptManager().RTC_alarm_duration(TimeSpan(days,hours,mins,secs));
	Loom.InterruptManager().reconnect_interrupt(12);

  digitalWrite(13, LOW);
	digitalWrite(5, HIGH); // Enable 3.3V rail
	digitalWrite(6, LOW);  // Enable 5V rail
	pinMode(23, INPUT);
	pinMode(24, INPUT);
	pinMode(10, INPUT);

	// Sleep Manager autmatically calls power_down on every sensor before sleeping
	// And power_up after waking.

  rtc_flag = false;
	Loom.SleepManager().sleep();
	while (!rtc_flag);
}
