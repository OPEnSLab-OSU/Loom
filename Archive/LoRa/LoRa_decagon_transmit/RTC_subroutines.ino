
#ifdef RTC3231

void InitalizeRTC()
{
  // RTC Timer settings here
  if (! RTC_DS.begin()) {
#if DEBUG == 1
    Serial.println("Couldn't find RTC");
#endif
    while (1);
  }
  // This may end up causing a problem in practice - what if RTC looses power in field? Shouldn't happen with coin cell batt backup
  if (RTC_DS.lostPower()) {
#if DEBUG == 1
    Serial.println("RTC lost power, lets set the time!");
#endif
    // following line sets the RTC to the date & time this sketch was compiled
    RTC_DS.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //clear any pending alarms
  clearAlarmFunction();

  // Querry Time and print
  DateTime now = RTC_DS.now();
#if DEBUG == 1
  Serial.print("RTC Time is: ");
  Serial.print(now.hour(), DEC); Serial.print(':'); Serial.print(now.minute(), DEC); Serial.print(':'); Serial.print(now.second(), DEC); Serial.println();
#endif
  //Set SQW pin to OFF (in my case it was set by default to 1Hz)
  //The output of the DS3231 INT pin is connected to this pin
  //It must be connected to arduino Interrupt pin for wake-up
  RTC_DS.writeSqwPinMode(DS3231_OFF);

  //Set alarm1
  setAlarmFunction();
}

// *********
// RTC helper function
// Function to query current RTC time and add the period to set next alarm cycle
// *********
void setAlarmFunction()
{
  DateTime now = RTC_DS.now(); // Check the current time

  // Calculate new time
  MIN = (now.minute() + WakePeriodMin) % 60; // wrap-around using modulo every 60 sec
  HR  = (now.hour() + ((now.minute() + WakePeriodMin) / 60)) % 24; // quotient of now.min+periodMin added to now.hr, wraparound every 24hrs
#if DEBUG == 1
  Serial.print("Resetting Alarm 1 for: "); Serial.print(HR); Serial.print(":"); Serial.println(MIN);
#endif

  //Set alarm1
  RTC_DS.setAlarm(ALM1_MATCH_HOURS, MIN, HR, 0);   //set your wake-up time here
  RTC_DS.alarmInterrupt(1, true);

}

//*********
// RTC helper function
// When exiting the sleep mode we clear the alarm
//*********
void clearAlarmFunction()
{
  //clear any pending alarms
  RTC_DS.armAlarm(1, false);
  RTC_DS.clearAlarm(1);
  RTC_DS.alarmInterrupt(1, false);
  RTC_DS.armAlarm(2, false);
  RTC_DS.clearAlarm(2);
  RTC_DS.alarmInterrupt(2, false);
}

void wake()
{
  TakeSampleFlag = true;
#ifdef is_M0
  detachInterrupt(digitalPinToInterrupt(wakeUpPin));
#endif //is_M0

#ifdef is_32U4
  disableInterrupt(wakeUpPin);
#endif //is_32U4
}


#endif
