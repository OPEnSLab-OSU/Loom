#include <Loom.h>

// Include configuration
const char* json_config =
#include "config.h"
;

// In Tools menu, set:
// Internet  > Disabled
// Sensors   > Enabled
// Radios    > Disabled
// Actuators > Disabled
// Max       > Disabled

using namespace Loom;

Loom::Manager Feather{};

// Can be whatever pin you want
const int PIN_NUM = 11;

// Typically a flag is set in an ISR since ISR's should be quick and concise
// Should be volatile since the variable can be changed outside normal program flow
volatile bool flag = false;

// Typical Interrupt Service Routine
void handle_ISR() {
  detachInterrupt(PIN_NUM);
  flag = true;
}

void setup()
{
  Feather.begin_serial(true);
  Feather.parse_config(json_config);
  Feather.print_config();

  // Creates an immediate interrupt for specified pin number, sets it as an internal pullup, and generates an interrupt on a LOW signal
  getInterruptManager(Feather).register_ISR(PIN_NUM, handle_ISR, LOW, ISR_Type::IMMEDIATE);

  LPrintln("\n ** Setup Complete ** ");
}

void loop() {
  // Reconnect interrupt since it is disabled in ISR
  getInterruptManager(Feather).reconnect_interrupt(PIN_NUM);
  LPrintln("Waiting for interrupt to trigger...");

  // Set flag to false and wait until the ISR sets it to true
  flag = false;
  while(!flag);
  LPrintln("Interrupt triggered!");

  // If using a button/switch, simple solution to bounce (just pause a bit for switch to settle)
  delay(5);
}
