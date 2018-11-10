/* 
 * TimeSerial.pde
 * example code illustrating Time library set through serial port messages.
 *
 * Messages consist of the letter T followed by ten digit time (as seconds since Jan 1 1970)
 * you can send the text on the next line using Serial Monitor to set the clock to noon Jan 1 2013
 T1357041600  
 *
 * A Processing example sketch to automatically send the messages is included in the download
 * On Linux, you can use "date +T%s\n > /dev/ttyACM0" (UTC time zone)
 */ 
 
#include <TimeLib.h>
#include "RTClib.h"


#define is_rtc3231 0
#define is_rtc8523 1

#define second_adjustment -25200 


////////////////////////////////

#define TIME_HEADER  "T"   // Header tag for serial time sync message
#define TIME_REQUEST  7    // ASCII bell character requests a time sync message 

#if  is_rtc3231 == 1
	RTC_DS3231  rtc_inst;
#elif is_rtc8523 == 1
	RTC_PCF8523 rtc_inst;
#endif


void setup()  {
  Serial.begin(115200);
//  while (!Serial) ; 
  pinMode(13, OUTPUT);

	if (!rtc_inst.begin()) {
		Serial.println("Couldn't find RTC");
		while (1);
	}

	// The following section checks if RTC is running, else sets 
	// the time to the time that the sketch was compiled
//	#if is_rtc3231 == 1
//		// This may end up causing a problem in practice - what if RTC looses power in field? Shouldn't happen with coin cell batt backup
//		if (rtc_inst.lostPower()) {
//			Serial.println("RTC lost power, lets set the time!");
//			rtc_inst.adjust(DateTime(F(__DATE__), F(__TIME__)));
//		}
//	#elif is_rtc8523 == 1
//		if (!rtc_inst.initialized()) {
//			Serial.println("RTC is NOT running!");
//			rtc_inst.adjust(DateTime(F(__DATE__), F(__TIME__)));
//		}
//	#endif

	// Query Time and print
	digitalClockDisplay();

	delay(3000);
	
	setSyncProvider( requestSync);  //set function to call when sync required
	Serial.println("Waiting for sync message");
	Serial.println("Run processing, stop processing, reopen serial");
}

void loop(){    
	if (Serial.available()) {
		processSyncMessage();
	}
	if (timeStatus()!= timeNotSet) {
//		Serial.println("Arduino Time");
//		digitalClockDisplay();  
		Serial.println("RTC Time");
		rtcTime();
		
	}
	if (timeStatus() == timeSet) {
		digitalWrite(13, HIGH); // LED on if synced
	} else {
		digitalWrite(13, LOW);  // LED off if needs refresh
	}
	delay(1000);
}

void digitalClockDisplay(){
	// digital clock display of the time
	Serial.print(hour());
	printDigits(minute());
	printDigits(second());
	Serial.print(" ");
	Serial.print(day());
	Serial.print(" ");
	Serial.print(month());
	Serial.print(" ");
	Serial.print(year()); 
	Serial.println(); 
}


void rtcTime()
{
	DateTime now = rtc_inst.now();

	Serial.print(now.year());
	Serial.print(" ");
	Serial.print(now.month());
	Serial.print(" ");
	Serial.print(now.day());
	Serial.print(" ");
	Serial.print(now.hour());
	Serial.print(":");
	Serial.print(now.minute());
	Serial.print(":");
	Serial.println(now.second());
}

void printDigits(int digits){
	// utility function for digital clock display: prints preceding colon and leading 0
	Serial.print(":");
	if(digits < 10)
		Serial.print('0');
	Serial.print(digits);
}


void processSyncMessage() {
  unsigned long pctime;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt() + second_adjustment;
     if( pctime >= DEFAULT_TIME) { // check the integer is a valid time (greater than Jan 1 2013)
       setTime(pctime); // Sync Arduino clock to the time received on the serial port
		while(!Serial);
		Serial.println(pctime);
	 rtc_inst.adjust(DateTime(pctime));

     }
  }
}

time_t requestSync()
{
  Serial.write(TIME_REQUEST);  
  return 0; // the time will be sent later in response to serial mesg
}


