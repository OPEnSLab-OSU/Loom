/* Senseair K30 CO2 module - I2C Arduino library 
 * CO2 level is expressed in ppm
 * return code 0 indicates succesful sensor reading
 * alfredc333
 * April, 2017 
 * MIT License
 */


#ifndef K30_I2C_h
#define K30_I2C_h

#include "Arduino.h"

class K30_I2C
{
  public:
    K30_I2C(int i2c_address);
    int readCO2(int &CO2level);
  private:
    int _i2c_address;
};


#endif
