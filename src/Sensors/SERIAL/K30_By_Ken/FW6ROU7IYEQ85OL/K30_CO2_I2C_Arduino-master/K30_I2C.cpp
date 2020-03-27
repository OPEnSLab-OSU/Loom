/* Senseair K30 CO2 module I2C Arduino library 
 * CO2 level is expressed in ppm
 * return code 0 indicates succesful sensor reading
 * alfredc333
 * April, 2017 
 * MIT License
 */

#include "Arduino.h"
#include "K30_I2C.h" 
#include "Wire.h"


K30_I2C::K30_I2C(int i2c_address){

  _i2c_address = i2c_address;
  Wire.begin();
}


int K30_I2C::readCO2(int &CO2level)
{
  
  byte recValue[4] = {0,0,0,0};
  

  Wire.beginTransmission(_i2c_address);
  Wire.write(0x22);
  Wire.write(0x00);
  Wire.write(0x08);
  Wire.write(0x2A);
  Wire.endTransmission();
  delay(30); 
  

  Wire.requestFrom(_i2c_address,4);
  delay(20);
   
  byte i=0;
  while(Wire.available())
  {
    recValue[i] = Wire.read();
    i++;
  }
  
  byte checkSum = recValue[0] + recValue[1] + recValue[2];
  CO2level = (recValue[1] << 8) + recValue[2];
  
  if (i == 0){
    return 2;
  }
  else if(checkSum == recValue[3]){
    return 0;
  }
  else{
    return 1;
  }

}
