///////////////////////////////////////////////////////////////////////////////
// This is a simple example of using Loom. The code used in this examples shows
// how to attach an interrupt to every pin on the Feather M0. Every pin has a
// simple print statement flag when the interrupt is triggered, but this code
// can be modified for any purpose.

// Due to the structure of the how interrupts are assigned to the SAM Chip on
// the Feather, some pins are mutually exclusive and therefore, there are
// interrupt assingments that are commented out to not cause errors. To use these
// pins, comment out the conflicting interrupt assingment(s) and uncomment the
// interrupt assingment you would like to use.
// Read more here: https://learn.adafruit.com/assets/46244

// Configuration info:
// https://openslab-osu.github.io/Loom/doxygenV2/html/class_loom___interrupt___manager.html
///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

volatile bool flag = false;

// Set enabled modules
LoomFactory<
	Enable::Internet::Disabled,
	Enable::Sensors::Disabled,
	Enable::Radios::Disabled,
	Enable::Actuators::Disabled,
	Enable::Max::Disabled
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };


void setup()
{
  pinMode(24, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  pinMode(20, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(A3, INPUT_PULLUP);
  pinMode(A4, INPUT_PULLUP);
  pinMode(A5, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);

	Loom.begin_LED();
	Loom.begin_serial(true);
	Loom.parse_config(json_config);
	Loom.print_config();

  Loom.InterruptManager().register_ISR(24, ISR_pin24, LOW, ISR_Type::IMMEDIATE);
  Loom.InterruptManager().register_ISR(23, ISR_pin23, LOW, ISR_Type::IMMEDIATE);
  Loom.InterruptManager().register_ISR(21, ISR_pin21, LOW, ISR_Type::IMMEDIATE);
  Loom.InterruptManager().register_ISR(20, ISR_pin20, LOW, ISR_Type::IMMEDIATE);

  Loom.InterruptManager().register_ISR(13, ISR_pin13, LOW, ISR_Type::IMMEDIATE);
  Loom.InterruptManager().register_ISR(12, ISR_pin12, LOW, ISR_Type::IMMEDIATE);
  Loom.InterruptManager().register_ISR(11, ISR_pin11, LOW, ISR_Type::IMMEDIATE);
  Loom.InterruptManager().register_ISR(10, ISR_pin10, LOW, ISR_Type::IMMEDIATE);

  Loom.InterruptManager().register_ISR(6, ISR_pin6, LOW, ISR_Type::IMMEDIATE);
  Loom.InterruptManager().register_ISR(5, ISR_pin5, LOW, ISR_Type::IMMEDIATE);

  Loom.InterruptManager().register_ISR(A1, ISR_pinA1, LOW, ISR_Type::IMMEDIATE);
  Loom.InterruptManager().register_ISR(A2, ISR_pinA2, LOW, ISR_Type::IMMEDIATE);
  Loom.InterruptManager().register_ISR(A4, ISR_pinA4, LOW, ISR_Type::IMMEDIATE);

      // 0 uses the same interrupt number as pin 24, so these pins are exclusive. For more details look at the Feather M0 pinout
  // Loom.InterruptManager().register_ISR(0, ISR_pin0, LOW, ISR_Type::IMMEDIATE);
      // 1 uses the same interrupt number as pin 23, so these pins are exclusive. For more details look at the Feather M0 pinout
  // Loom.InterruptManager().register_ISR(1, ISR_pin1, LOW, ISR_Type::IMMEDIATE);
      // 9 uses the same interrupt number as pin 21, so these pins are exclusive. For more details look at the Feather M0 pinout
  // Loom.InterruptManager().register_ISR(9, ISR_pin9, LOW, ISR_Type::IMMEDIATE);
      // A0 and A5 use the same interrupt number as pin 10, so these three pins are exclusive. For more details look at the Feather M0 pinout
  // Loom.InterruptManager().register_ISR(A0, ISR_pinA0, FALLING, ISR_Type::IMMEDIATE);
  // Loom.InterruptManager().register_ISR(A5, ISR_pinA5, FALLING, ISR_Type::IMMEDIATE);
      // A3 use the same interrupt number as pin 6, so these two pins are exclusive. For more details look at the Feather M0 pinout
  // Loom.InterruptManager().register_ISR(A3, ISR_pinA3, FALLING, ISR_Type::IMMEDIATE);

	LPrintln("\n ** Setup Complete ** ");
}

void loop()
{
  LPrintln("Waiting for trigger");
  flag = false;
  while(!flag) Loom.pause();

  Loom.InterruptManager().reconnect_interrupt(24);
  Loom.InterruptManager().reconnect_interrupt(23);
  Loom.InterruptManager().reconnect_interrupt(21);
  Loom.InterruptManager().reconnect_interrupt(20);

  Loom.InterruptManager().reconnect_interrupt(13);
  Loom.InterruptManager().reconnect_interrupt(12);
  Loom.InterruptManager().reconnect_interrupt(11);
  Loom.InterruptManager().reconnect_interrupt(10);

  Loom.InterruptManager().reconnect_interrupt(6);
  Loom.InterruptManager().reconnect_interrupt(5);

  Loom.InterruptManager().reconnect_interrupt(A1);
  Loom.InterruptManager().reconnect_interrupt(A2);
  Loom.InterruptManager().reconnect_interrupt(A4);

  // Loom.InterruptManager().reconnect_interrupt(0);
  // Loom.InterruptManager().reconnect_interrupt(1);
	// Loom.InterruptManager().reconnect_interrupt(9);
  // Loom.InterruptManager().reconnect_interrupt(A0);
  // Loom.InterruptManager().reconnect_interrupt(A5);
  // Loom.InterruptManager().reconnect_interrupt(A3);
}

void ISR_pin24(){
  detachInterrupt(24);
  LPrintln("Pin SCK triggered");
  flag = true;
}

void ISR_pin23(){
  detachInterrupt(23);
  LPrintln("Pin MOSI triggered");
  flag = true;
}

void ISR_pin21(){
  detachInterrupt(21);
  LPrintln("Pin SCL triggered");
  flag = true;
}

void ISR_pin20(){
  detachInterrupt(20);
  LPrintln("Pin SDA triggered");
  flag = true;
}


void ISR_pin13(){
  detachInterrupt(13);
  LPrintln("Pin 13 triggered");
  flag = true;
}

void ISR_pin12(){
  detachInterrupt(12);
  LPrintln("Pin 12 triggered");
  flag = true;
}

void ISR_pin11(){
  detachInterrupt(11);
  LPrintln("Pin 11 triggered");
  flag = true;
}

void ISR_pin10(){
  detachInterrupt(10);
  LPrintln("Pin 10 triggered");
  flag = true;
}

void ISR_pin9(){
  detachInterrupt(9);
  LPrintln("Pin 9 triggered");
  flag = true;
}

void ISR_pin6(){
  detachInterrupt(6);
  LPrintln("Pin 6 triggered");
  flag = true;
}

void ISR_pin5(){
  LPrintln("Pin 5 triggered");
  detachInterrupt(5);
  flag = true;
}

void ISR_pinA0(){
  detachInterrupt(A0);
  LPrintln("Pin A0 triggered");
  flag = true;
}

void ISR_pinA1(){
  detachInterrupt(A1);
  LPrintln("Pin A1 triggered");
  flag = true;
}

void ISR_pinA2(){
  detachInterrupt(A2);
  LPrintln("Pin A2 triggered");
  flag = true;
}

void ISR_pinA3(){
  detachInterrupt(A3);
  LPrintln("Pin A3 triggered");
  flag = true;
}

void ISR_pinA4(){
  LPrintln("Pin A4 triggered");
  detachInterrupt(A4);
  flag = true;
}

void ISR_pinA5(){
  detachInterrupt(A5);
  LPrintln("Pin A5 triggered");
  flag = true;
}

void ISR_pin0(){
  detachInterrupt(0);
  LPrintln("Pin RX triggered");
  flag = true;
}

void ISR_pin1(){
  detachInterrupt(1);
  LPrintln("Pin TX triggered");
  flag = true;
}
