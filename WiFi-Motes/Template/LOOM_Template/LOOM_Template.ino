/*
Put any specific information about this file here.
 */

#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>
#include <FlashStorage.h>
#include "LOOM_OSC_Scheme.h"

#define FAMILY "/Loom"
#define DEVICE "/Devicename"
#define INSTANCE_NUM 0
#define IDString FAMILY DEVICE STR(INSTANCE_NUM) // C interprets subsequent string literals as concatenation: "/Loom" "/Ishield" "0" becomes "/Loom/Ishield0"
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
