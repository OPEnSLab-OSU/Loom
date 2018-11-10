
// Add necessary libraries here
#include <SPI.h>
#include "LowPower.h" 		// from sparkfun low power library found here https://github.com/rocketscream/Low-Power
#include "RTClibExtended.h"	// from sparkfun low power library found here https://github.com/FabioCuomo/FabioCuomo-DS3231/
#include "Wire.h"

// Define macro constants
#define DEBUG 1  //test to allow print to serial monitor


#define LOOM_DEBUG DEBUG
// Macros for printing to Serial iff Loom Debug is enabled
#define LOOM_DEBUG_Print(X)          (LOOM_DEBUG==0) ? :  Serial.print(X)
#define LOOM_DEBUG_Println(X)        (LOOM_DEBUG==0) ? :  Serial.println(X)
#define LOOM_DEBUG_Print2(X,Y)       LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y)
#define LOOM_DEBUG_Println2(X,Y)     LOOM_DEBUG_Print(X); LOOM_DEBUG_Println(Y)
#define LOOM_DEBUG_Print3(X,Y,Z)     LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Print(Z)
#define LOOM_DEBUG_Println3(X,Y,Z)   LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Println(Z)
#define LOOM_DEBUG_Print4(W,X,Y,Z)   LOOM_DEBUG_Print(W); LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Print(Z)
#define LOOM_DEBUG_Println4(W,X,Y,Z) LOOM_DEBUG_Print(W); LOOM_DEBUG_Print(X); LOOM_DEBUG_Print(Y); LOOM_DEBUG_Println(Z)



// ======= BOARD SPECIFIC SETTINGS ======

//NOTE: Must include the following line in the RTClibExtended.h file to use with M0:
//#define _BV(bit) (1 << (bit))
#include <RTClibExtended.h>

#define EI_NOTEXTERNAL
#include <EnableInterrupt.h>


// ======== General Settings ==========

RTC_DS3231 RTC_DS; //instance of DS3231 RTC


// ========= Declare Global Variables ==============
volatile bool TimerFlag = false; // Flag to know when to go back to sleep
volatile bool RTCFlag   = false; // Flag set by RTC when waking


volatile int  count = 10; 		//number of seconds to wait before running loop()
const byte    wakeUpPin = 12;  	// attach to SQW pin on RTC
unsigned long timer;


/**********************************************************************************************
	 wakeUp()
	 Description: function that takes place after device wakes up
	 wakeUp_alert
	 - sets SampleFlag and AlertFlag TRUE;
	 wakeUp_RTC
	 - sets SampleFlag TRUE upon typical wakeup
**********************************************************************************************/


// ======= FUNCTION PROTOTYPES ======
void print_countdown();
void InitializeRTC();
void setRTCAlarm();
void clearRTCAlarm();
void interrupt_reset();
void print_DateTime(DateTime time);


// ======= RTC ISR ======
void wakeUp_RTC()
{
	detachInterrupt(digitalPinToInterrupt(wakeUpPin));
	TimerFlag = true;
	RTCFlag   = true;
}


// ======= SETUP ======
void setup() 
{
	pinMode(wakeUpPin, INPUT_PULLUP);  // pull up resistors required for Active-Low interrupts
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	Serial.begin(9600);

	#if DEBUG
		print_countdown(); //give the device time to wake up and upload sketch if necessary
		LOOM_DEBUG_Println("Processor Setup Successful.\n");
	#else
		// when not in debug mode, dont wait for serial to start code segment
		// delay 10s for sketch upload
		delay(10000);
	#endif //DEBUG == 1

	InitializeRTC();

	timer = millis();
}


// ======= LOOP ======
void loop() 
{
	if ( TimerFlag ) { //triggered with every wake-up interrupt

		//**************** VERY IMPORTANT ****************
		//clear interrupt registers, attach interrupts EVERY TIME THE INTERRUPT IS CALLED
		interrupt_reset();
		timer = millis();
		TimerFlag = false;
	}

	if ( RTCFlag ) {
		clearRTCAlarm();
		print_DateTime(RTC_DS.now());
		delay(75);  // delay so serial stuff has time to print out all the way
		RTCFlag = false;
	}

	//this is the standby loop, execute last in order
	if ( millis() - timer > 10000 ) { //10s delay, then sleep
		//reset all flags
		TimerFlag = false;
		setRTCAlarm_Relative(0,0,7); // 7 seconds
		prep_before_sleep();
		
		LowPower.standby();			// Go to sleep here
		
		prep_after_sleep();
	}
}


void prep_before_sleep()
{
	LOOM_DEBUG_Println("Entering STANDBY");
	Serial.end();
	USBDevice.detach();
	interrupt_reset(); //clear interrupt registers, attach interrupts
	delay(10);
	#if DEBUG == 1
		digitalWrite(LED_BUILTIN, LOW);
	#endif

}

void prep_after_sleep()
{
	#if DEBUG == 1
		digitalWrite(LED_BUILTIN, HIGH);
		delay(5000); // give user 5s to close and reopen serial monitor!
	#endif
	clearRTCAlarm(); //prevent double trigger of alarm interrupt
	USBDevice.attach();
	Serial.begin(9600);
	LOOM_DEBUG_Println("WAKE");
}


void InitializeRTC()
{
	// RTC Timer settings here
	if (! RTC_DS.begin()) {
		LOOM_DEBUG_Println("Couldn't find RTC");
		while (1);
	}
	// This may end up causing a problem in practice - what if RTC looses power in field? Shouldn't happen with coin cell batt backup
	if (RTC_DS.lostPower()) {
		Serial.println("RTC lost power, lets set the time!");
		// Set the RTC to the date & time this sketch was compiled
		RTC_DS.adjust(DateTime(F(__DATE__), F(__TIME__)));
	}
	// Clear any pending alarms
	clearRTCAlarm();

	// Query Time and print
	DateTime now = RTC_DS.now();
	RTC_DS.writeSqwPinMode(DS3231_OFF);

}


void setRTCAlarm_Relative(int hours, int minutes, int seconds)
{
	LOOM_DEBUG_Print("Current Time:\n\t"); 
	print_DateTime( RTC_DS.now() );

	DateTime future( RTC_DS.now() + TimeSpan(0,hours,minutes,seconds) );
	LOOM_DEBUG_Print("Resetting Alarm 1 for:\n\t"); 
	print_DateTime(future);

	RTC_DS.setAlarm(ALM1_MATCH_HOURS, future.second(), future.minute(), future.hour(), 0); 
	RTC_DS.alarmInterrupt(1, true);
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


/* Debugging and upload helper,
	 Allow uploads by waiting for serial monitor to open,
	 prevent device from going to standby too soon
*/
void print_countdown()
{
	while (!Serial); // Won't start anything until serial is open, comment this line out if powering from battery
	
	// ***** IMPORTANT *****
	// Delay is required to allow the USB interface to be active during
	// sketch upload process
	LOOM_DEBUG_Println("Countdown:");
	for (count; count > 0; count--) {
		LOOM_DEBUG_Println2(count, " s");
		delay(1000);
	}
}


void interrupt_reset()
{
	// Clears any interrupts than may be pending on M0
	detachInterrupt(digitalPinToInterrupt(wakeUpPin));
	delay(20);
	attachInterrupt(digitalPinToInterrupt(wakeUpPin), wakeUp_RTC, LOW);
}


// Prints an RTC time
void print_DateTime(DateTime time) 
{
	#if DEBUG == 1
		Serial.print(time.year(),   DEC); Serial.print('/');
		Serial.print(time.month(),  DEC); Serial.print('/');
		Serial.print(time.day(),    DEC); Serial.print(' ');
		Serial.print(time.hour(),   DEC); Serial.print(':');
		Serial.print(time.minute(), DEC); Serial.print(':');
		Serial.print(time.second(), DEC); Serial.println();
	#endif
}




