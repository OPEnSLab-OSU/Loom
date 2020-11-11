//////////////////////////////////////////////////////////////////////////////////////////////////////////

// This is basic example to use the K30 sensor with Exec.
// The K30 sensor will measure the CO2 from the range 0 to 10000 ppm and log to SD.

// In this example of program, it will measure the CO2 level and display on the Serial Monitor.
// Note that the K30 sensor needs at least 6 minutes of warm up to get accurate measurements.
// Therefore, you will get data values after 6 minutes have passed.
// Inside this program, there is a timer that will run for 6 minutes before it starts measuring values. 

// As you might see, there are some syntax that came from outside of the Loom language, 
// so take a look the link and the code carefully.  
// https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/creating-a-new-serial

///////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

#include "wiring_private.h"


// In Tools menu, set:
// Internet  > Disabled                                   
// Sensors   > Enabled                                     
// Radios    > Enabled                                      
// Actuators > Disabled                                  
// Max       > Disabled                                         

using namespace Loom;

Loom::Manager Exec{};

// Create Serial SERCOM for K30 Sensor: RX pin 12, TX pin 11
Uart Serial2 = Uart(&sercom1, 12, 11, SERCOM_RX_PAD_3, UART_TX_PAD_0);

void setup() {
  
  Serial2.begin(9600);
  
  Exec.begin_serial(true);
  Exec.parse_config(LCONFIG);
  Exec.print_config();

  //Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(12, PIO_SERCOM);
  
  Exec.K30().set_serial(&Serial2);

  LPrintln("\n ** Setup Complete ** ");

  warmUpTimer();

}

void loop() {  

  Exec.measure(); // Sample attached sensors
  Exec.package(); // Format data for display and SD
  Exec.display_data(); // display printed JSON formatted data on serial monitor
  Exec.get<Loom::SD>()->log(); // Loggin K30 Data value into SDCard
  Exec.pause();
}

void SERCOM1_Handler(){ // This function is require for the K30 Serial Sensor because of UART Type
  Serial2.IrqHandler();
}

void warmUpTimer(){ // This function is a timer to warm up the K30 sensor to get accurate measurements
  
  LPrintln("\n ** Set up 6 minutes Warm Up time to get accurate measurements ** ");

  for(int timePassed = 1; timePassed < 7; timePassed++){ // By pausing Loom, it will not measure CO2 value for 6 minutes
    Exec.pause(60000); // The max is only 1 min for pause, we loop it for 6 times to make it 6 minutes
    LPrint(timePassed); // Knowing the User that how many minutes have been passed
    LPrint(" minute(s) passed!");
    LPrint("\n");
  }
  
  LPrintln("\n ** Ready to Measure ** ");
}