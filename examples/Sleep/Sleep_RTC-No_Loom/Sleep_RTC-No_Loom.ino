
#include "LowPower.h" 		// Sparkfun low power library found here https://github.com/rocketscream/Low-Power
#include <OPEnS_RTC.h>

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>

const int DELAY_COUNT = 5; 		// Number of seconds to wait before running loop()
#define WAKE_PIN 12				// Attach DS3231 RTC Interrupt pin to this pin on Feather

#define DEBUG 1  //test to allow print to serial monitor


// Macros for printing to Serial if Loom Debug is enabled
#define DEBUG_Print(X)          (DEBUG==0) ? :  Serial.print(X)
#define DEBUG_Println(X)        (DEBUG==0) ? :  Serial.println(X)
#define DEBUG_Print2(X,Y)       DEBUG_Print(X); DEBUG_Print(Y)
#define DEBUG_Println2(X,Y)     DEBUG_Print(X); DEBUG_Println(Y)

// Instance of DS3231 RTC
RTC_DS3231 RTC_DS; 


// ======= FUNCTION PROTOTYPES ======
void sleep();
void countdown();
void InitializeRTC();
void setRTCAlarm();
void clearRTCAlarm();
void print_DateTime(DateTime time);


// ======= RTC ISR ======
void wakeUp_RTC()
{
	detachInterrupt(digitalPinToInterrupt(WAKE_PIN));
}


// ======= SETUP ======
void setup()
{
	pinMode(WAKE_PIN, INPUT_PULLUP);  // Pull up resistors required for Active-Low interrupts
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

#if DEBUG
	Serial.begin(115200);
	while (!Serial); // Won't start anything until serial is open, comment this line out if powering from battery
#endif
	countdown(); 

	InitializeRTC();
	DEBUG_Println("Setup Complete");
}


// ======= LOOP ======
void loop()
{
	// Pretent to do work
	DEBUG_Print("\nProcessing ");
	for (int i = 0; i < 3; i++) {
		delay(1000);
		DEBUG_Print(". ");
	}
	DEBUG_Println(" Done Processing");

	// Set alarms (5 seconds here)
	setRTCAlarm_Relative(0, 0, DELAY_COUNT);
	
	// Go to sleep
	sleep();
}


// ======= FUNCTIONS ======


// Simple function to call to hide
// Pre/post aux functions
void sleep()
{
	pre_sleep();
	LowPower.standby();			// Go to sleep here
	post_sleep();
}


// Prepare for sleep
// Disconnect Serial, attach interrupt, turn off LED
void pre_sleep()
{
	DEBUG_Println("\nEntering STANDBY");
	delay(50);
	Serial.end();
	USBDevice.detach();

	// Don't know why this has to happen twice but it does
	attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wakeUp_RTC, LOW);
	attachInterrupt(digitalPinToInterrupt(WAKE_PIN), wakeUp_RTC, LOW);

	digitalWrite(LED_BUILTIN, LOW);
}


// Post sleep actions that should happen
// Clear alarms, reconnect Serial, turn on LED
void post_sleep()
{
	clearRTCAlarm(); //prevent double trigger of alarm interrupt
	USBDevice.attach();
	Serial.begin(115200);
	digitalWrite(LED_BUILTIN, HIGH);
#if DEBUG == 1
	// Give user 5s to reopen Serial monitor!
	// Note that the serial may still take a few seconds 
	// to fully setup after the LED turns on
	delay(5000); 
#endif
	DEBUG_Println("WAKE");

	print_DateTime(RTC_DS.now());
}


/* RTC helper function */
/* When exiting the sleep mode we clear the alarm */
void clearRTCAlarm()
{
	// Clear any pending alarms on the RTC
	RTC_DS.armAlarm(1, false);
	RTC_DS.clearAlarm(1);
	RTC_DS.alarmInterrupt(1, false);
	RTC_DS.armAlarm(2, false);
	RTC_DS.clearAlarm(2);
	RTC_DS.alarmInterrupt(2, false);
}


// Setup RTC time
void InitializeRTC()
{
	// RTC Timer settings here
	if (! RTC_DS.begin()) {
		DEBUG_Println("Couldn't find RTC");
		while (1);
	}
	// This may end up causing a problem in practice - what if RTC looses power in field? Shouldn't happen with coin cell batt backup
	if (RTC_DS.lostPower()) {
		DEBUG_Println("RTC lost power, lets set the time!");
		// Set the RTC to the date & time this sketch was compiled
		RTC_DS.adjust(DateTime(F(__DATE__), F(__TIME__)));
	}
	// Clear any pending alarms
	clearRTCAlarm();

	// Query Time and print
	DateTime now = RTC_DS.now();
	RTC_DS.writeSqwPinMode(DS3231_OFF);
}


// Set an RTC alarm some duration from now
void setRTCAlarm_Relative(int hours, int minutes, int seconds)
{
	DEBUG_Print("\nCurrent Time: ");
	print_DateTime( RTC_DS.now() );

	DateTime future( RTC_DS.now() + TimeSpan(0, hours, minutes, seconds) );
	DEBUG_Print("\nReset Alarm for: ");
	print_DateTime(future);

	RTC_DS.setAlarm(ALM1_MATCH_HOURS, future.second(), future.minute(), future.hour(), 0);
	RTC_DS.alarmInterrupt(1, true);
}


// Debugging and upload helper,
// Allow uploads by waiting for serial monitor to open,
// prevent device from going to standby too soon
void countdown()
{
	DEBUG_Println("Countdown:");
	for (int count = DELAY_COUNT; count > 0; count--) {
		DEBUG_Println2(count, " s");
		delay(1000);
	}
}


// Print an RTC time
void print_DateTime(DateTime time)
{
	DEBUG_Print2(time.year(), '/');
	DEBUG_Print2(time.month(), '/');
	DEBUG_Print2(time.day(), ' ');
	DEBUG_Print2(time.hour(), ':');
	DEBUG_Print2(time.minute(), ':');
	DEBUG_Println(time.second());
}
