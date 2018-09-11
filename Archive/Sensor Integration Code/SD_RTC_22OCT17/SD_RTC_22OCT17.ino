#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
#include "ArduinoLowPower.h"

//#include <RTCZero.h> 
//#include <WiFi101.h>

//Interrupt Variables
const byte interruptPin = 11;
volatile byte state = LOW;


//Uncomment to activate low power mode
//WiFi.lowPowerMode();

// Set the rtc variable
RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Set the pins used
#define cardSelect 10

// Set RTC values
  uint8_t a = 17;
  uint8_t b = 30;
  uint8_t c = 0;

File logfile;

// blink out an error code
void error(uint8_t errno) {
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}

void setup() {
  // Uncomment this function if you wish to attach function dummy when RTC wakes up the chip
  //LowPower.attachInterruptWakeup(RTC_ALARM_WAKEUP, grab_data, CHANGE);

    
  while (!Serial) {
    delay(1);  // for Leonardo/Micro/Zero
  }
  
  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  delay(2000);
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  
  Serial.println("\r\nAnalog logger test");
  pinMode(13, OUTPUT);


  // see if the card is present and can be initialized:
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }
 /* 
  // Setup RTC  
  //if (! rtc.initialized()) {
    // following line sets the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
   //}
  */
  
  char filename[11];
  strcpy(filename, "TEST.TXT");  

  /*
  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    error(3);
  }
  */
  Serial.print("Writing to "); 
  Serial.println(filename);

  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  //Set Interrupt
  pinMode(interruptPin, OUTPUT);
  //LowPower.attachInterruptWakeup(interruptPin, set_alarm, CHANGE);
  
  //Set Alarm
  rtc.setAlarmTime(16, 07, 00);
  //rtc.enableAlarm(rtc.MATCH_HHMMSS);

  //rtc.attachInterrupt(wake);
  
  Serial.println("Ready!");
  
}

uint8_t i=1;
void loop() {
  DateTime now = rtc.now();
  digitalWrite(8, HIGH);

  logfile = SD.open("TEST5.TXT",FILE_WRITE);

  logfile.print("Time Reading "); logfile.print(i++); logfile.print(": ");
  logfile.print(now.hour(), DEC);   logfile.print(':');
  logfile.print(now.minute(), DEC);   logfile.print(':');
  logfile.print(now.second(), DEC);     logfile.println();

  Serial.println(); Serial.println(); Serial.print("Current Time: ");
  Serial.print(now.hour(), DEC); Serial.print(':');
  Serial.print(now.minute(), DEC); Serial.print(':');
  Serial.print(now.second(), DEC);  Serial.println(); 

  logfile.close();
  digitalWrite(8, LOW);

  // re-open the file for reading:
  logfile = SD.open("TEST5.TXT", FILE_READ);
    
  if (logfile) {
    Serial.println("TEST5.TXT Contents:");

    // read from the file until there's nothing else in it:
    while (logfile.available()) {
      Serial.write(logfile.read());
    }
    // close the file:
    logfile.close();
    
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
  
  //LowPower.sleep();
  delay(10000);
}

//This function is only called when the timer pulls the system
//out of sleep every 60 seconds
void set_alarm(){
  int a,b,c; 
  Serial.println("Test1"); 
  DateTime now = rtc.now();
  a = now.hour();
  b = now.minute();
  c = now.second() + 10;
  
  if (c > 59){
    c = c%60;
    b = b+1;
  }
  Serial.println("Test2"); 
  rtc.setAlarmTime(a, b, c);
  Serial.println("Test3"); 
  rtc.enableAlarm(rtc.MATCH_HHMMSS);
  Serial.println("Test4"); 
  
}

//Wakeup out of sleep
void wake(){
  Serial.println("Test5"); 
  digitalWrite(interruptPin, state);
  state = !state;
}


