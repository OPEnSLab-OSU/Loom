//#include <TinyWireM.h>
//#include <USI_TWI_Master.h>
#include <Wire.h>

/**
 * TCA9548 I2CScanner.pde -- I2C bus scanner for Arduino
 *
 * Based on code c. 2009, Tod E. Kurt, http://todbot.com/blog/
 *
 */
 
#include "Wire.h"
//extern "C" { 
//#include "utility/twi.h"  // from Wire library, so we can do bus scanning
//}
 
#define TCAADDR 0x71

byte error, address;
int nDevices;
 
void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}
 
 
// standard Arduino setup()
void setup()
{
    while (!Serial);
    delay(1000);
 
    Wire.begin();
    
    Serial.begin(115200);
    Serial.println("\nTCAScanner ready!");
    
    for (uint8_t t=0; t<8; t++) {
      nDevices = 0;
      tcaselect(t);
      Serial.print("TCA Port #"); Serial.println(t);

      for (uint8_t addr = 0; addr<=127; addr++) {
          Wire.beginTransmission(addr);
          error = Wire.endTransmission();

          if (addr == 0x71){}
          else if (error == 0)
          {
            Serial.print("I2C device found at address 0x");
            if (addr<16)
              Serial.print("0");
            Serial.println(addr,HEX);         
            nDevices++;
          }
          else if (error==4)
          {
            Serial.print("Unknown error at address 0x");
            if (addr<16)
              Serial.print("0");
            Serial.println(addr,HEX);
          }    
        }
        if (nDevices == 0)
          Serial.println("No I2C devices found");
        Serial.println();
    }
    Serial.println("\ndone");
    
}
 
void loop() 
{
}
