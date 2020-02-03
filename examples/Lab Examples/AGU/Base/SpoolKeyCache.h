///
/// Spool key cache code
/// This file contains functions which ensure the Spool credentials
/// are loaded from flash on startup, or if none exist are loaded 
/// from serial monitor input.
#pragma once
#include <FlashStorage.h>
#include "time_macros.h"

struct SpoolKeys {
  bool valid;
  unsigned long timestamp;
  char certificate[900];
  char key[300];
  char id[32];
};

///
/// Checks flash storage to see if this device has already been provisioned, and
/// populates dest those values if so. If the device has not been provisioned yet, 
/// this function will use input from the serial monitor to populator dest instead.
/// @pre The serial monitor must be enabled and functional on the first startup
/// after a sketch is uploaded, otherwise this function will fail.
/// @param[out] dest The varible to put the credentials this function gets.
/// @returns true if keys were retrieved, false if not (will only return false
/// if flash storage did not contain keys and the serial monitor is not open).
///
bool getKeys(SpoolKeys& dest);

