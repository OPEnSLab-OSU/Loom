/*
 * RTC Counter Periodic Wakeup example
 * Created by Gabriel Notman
 *
 * This example demonstrates how to use the RTC Counter
 * library to periodically wake the board from deep sleep.
 *
 * This example code is in the public domain.
 * 
 * Created 05 November 2018
 */


#include <RTCCounter.h>

void setup()
{
  // Sleep mode can intefere with reprogramming.
  // A startup delay makes things easier as it
  // provides a window to upload new code
  delay(5000);
  
  // Setup the RTCCounter
  rtcCounter.begin();

  // Set the alarm to generate an interrupt every 20s
  rtcCounter.setPeriodicAlarm(20);

  // Setup the LED pin
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // Set the sleep mode
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
}

void loop()
{
//    systemSleep();

  // If the alarm interrupt has been triggered
//  if (rtcCounter.getFlag()) {

    // Clear the interrupt flag
    rtcCounter.clearFlag();

    // Blink the LED
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
        delay(3000);

    digitalWrite(LED_BUILTIN, LOW);

//    digitalWrite(LED_BUILTIN, HIGH);
//    while(1);

//  }

  // Sleep until the next interrupt
  systemSleep();
}

void systemSleep()
{
  // If the alarm interrupt has not yet triggered
  if (!rtcCounter.getFlag()) {
    
    // Wait For Interrupt
    __DSB();// added this
    __WFI();
  }
}
