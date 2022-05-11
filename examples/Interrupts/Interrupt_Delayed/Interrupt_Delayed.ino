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

// This is not actually called immediately upon an interrupt being generated
// It is called on next iteration of loop()
// Thus, this function can be as complicated as you wish since it is not called directly upon interrupt trigger
void handle_ISR() {
  detachInterrupt(PIN_NUM);
  
  LPrintln("Interrupt triggered!");
  LPrintln("Waiting for interrupt to trigger...");

  // If using a button/switch, simple solution to bounce (just pause a bit for switch to settle)
  delay(5);

  // Reconnect interrupt since it was previously detached
  getInterruptManager(Feather).reconnect_interrupt(PIN_NUM);
}

void setup()
{
  Feather.begin_serial(true);
  Feather.parse_config(json_config);
  Feather.print_config();

  // Creates a delayed interrupt for specified pin number, sets it as an internal pullup, and generates an interrupt on a LOW signal
  getInterruptManager(Feather).register_ISR(PIN_NUM, handle_ISR, LOW, ISR_Type::CHECK_FLAG);

  LPrintln("\n ** Setup Complete ** ");
  LPrintln("Waiting for interrupt to trigger...");
}

void loop()
{
  // Runs the ISR for any interrupts that may have been triggered since last call
  getInterruptManager(Feather).run_pending_ISRs();
}
