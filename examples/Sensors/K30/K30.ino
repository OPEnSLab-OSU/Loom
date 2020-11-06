//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// This is basic example to use the K30 sensor with Loom.
// The K30 sensor will measure the CO2 from the range 0 to 10000 ppm and log to SD.
//
// In this example of program, it will measure the CO2 level and display on the Serial Monitor.
// Note that the K30 sensor needs at least 6 minutes of warm up to get accurate measurements.
// Therefore, you will get data values after 6 minutes have passed.
// Inside this program, there is a timer that will run for 6 minutes before it starts measuring values. 
//
// As you might see, there are some syntax that came from outside of the Loom language, 
// so take a look the link and the code carefully. 
// The related syntax code came from the following website 
// https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/creating-a-new-serial

// Documentation for K30: https://openslab-osu.github.io/Loom/doxygenV2/html/class_loom___k30.htmls

///////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Loom.h>

#include "wiring_private.h"

//Include Configuration
const char* json_config =
#include "config.h"
;

// Set enabled modules
LoomFactory<
  Enable::Internet::Disabled,                                   
  Enable::Sensors::Enabled,                                     
  Enable::Radios::Enabled,                                      
  Enable::Actuators::Disabled,                                  
  Enable::Max::Disabled                                         
> ModuleFactory{};

LoomManager Loom{ &ModuleFactory };

// Create Serial SERCOM for K30 Sensor: RX pin 13, TX pin 11
Uart Serial2 = Uart(&sercom1, 13, 11, SERCOM_RX_PAD_1, UART_TX_PAD_0);

void setup() {
  
  Serial2.begin(9600);
  
  Loom.begin_serial(true);
  Loom.parse_config(json_config);
  Loom.print_config();

  //Assign pins 13 & 11 SERCOM functionality
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);
  
  Loom.K30().set_serial(&Serial2);

  LPrintln("\n ** Setup Complete ** ");

  warmUpTimer();

}

void loop() {  

  Loom.measure(); // Sample attached sensors
  Loom.package(); // Format data for display and SD
  Loom.display_data(); // display printed JSON formatted data on serial monitor
  Loom.SDCARD().log(); // Loggin K30 Data value into SDCard
  Loom.pause();
}

void SERCOM1_Handler(){ // This function is require for the K30 Serial Sensor because of UART Type
  Serial2.IrqHandler();
}

void warmUpTimer(){ // This function is a timer to warm up the K30 sensor to get accurate measurements
  
  LPrintln("\n ** Set up 6 minutes Warm Up time to get accurate measurements ** ");

  for(int timePassed = 1; timePassed < 7; timePassed++){ // By pausing Loom, it will not measure CO2 value for 6 minutes
    Loom.pause(60000); // The max is only 1 min for pause, we loop it for 6 times to make it 6 minutes
    LPrint(timePassed); // Knowing the User that how many minutes have been passed
    LPrint(" minute(s) passed!");
    LPrint("\n");
  }
  
  LPrintln("\n ** Ready to Measure ** ");
}