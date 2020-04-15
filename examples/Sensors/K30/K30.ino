//////////////////////////////////////////////////////////////////////////////////////////////////////////

// This is basic example to use the K30 sensor with Loom.
// The K30 sensor will measure the CO2 from the range 0 to 10000.

// In this example of program, it will measure the CO2 level and display on the Serial Monitor.
// Note that the K30 sensor needs at least 6 minutes of warm up to get accurate measurements.
// Therefore, make sure let it on for 6 minutes to warm up the K30 sensor before record data values. 

// As you might see, there are some syntax that came from outside of the Loom language, 
// so take a look the link and the code carefully.  
// https://learn.adafruit.com/using-atsamd21-sercom-to-add-more-spi-i2c-serial-ports/creating-a-new-serial

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

// Create Serial SERCOM for K30 Sensor: RX pin 12, TX pin 11
Uart Serial2 = Uart(&sercom1, 12, 11, SERCOM_RX_PAD_3, UART_TX_PAD_0);


void setup() {
  
  Serial2.begin(9600);
  
  Loom.begin_serial(true);
  Loom.parse_config(json_config);
  Loom.print_config();

  //Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(12, PIO_SERCOM);
  
  Loom.K30().set_serial(&Serial2);

  LPrintln("\n ** Setup Complete ** ");
}

void loop() {  
  
  Loom.measure(); // Sample attached sensors
  Loom.package(); // Format data for display and SD
  Loom.display_data(); // display printed JSON formatted data on serial monitor
  Loom.pause();
}

void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}
