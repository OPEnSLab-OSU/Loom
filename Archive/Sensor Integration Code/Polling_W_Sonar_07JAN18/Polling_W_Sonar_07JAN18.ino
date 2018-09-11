/* ts12591 Digital Light Sensor */
/* Dynamic Range: 600M:1 */
/* Maximum Lux: 88K */

//Max Sonar Definitions
#define SCL_PIN 0              //Default SDA is Pin5 PORTC for the UNO -- you can set this to any tristate pin
#define SCL_PORT PORTD 
#define SDA_PIN 1              //Default SCL is Pin4 PORTC for the UNO -- you can set this to any tristate pin
#define SDA_PORT PORTD
#define I2C_TIMEOUT 100        //Define a timeout of 100 ms -- do not wait for clock stretching longer than this time

//Included Libraries
#include "Wire.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
#include "Adafruit_SHT31.h"
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>
#include <ZX_Sensor.h>
extern "C" { 
#include "utility/twi.h"  // from Wire library, so we can do bus scanning
}


#define TCAADDR 0x71

// Example for demonstrating the ts12591 library - public domain!

// connect SCL to analog 5
// connect SDA to analog 4
// connect Vin to 3.3-5V DC
// connect GROUND to common ground

//Connected Sensors
Adafruit_TSL2591 ts1 = Adafruit_TSL2591(1); // pass in a number for the sensor identifier (for your use later)
Adafruit_TSL2591 ts2 = Adafruit_TSL2591(2); // pass in a number for the sensor identifier (for your use later)

Adafruit_SHT31 sht31 = Adafruit_SHT31();

Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);

Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

ZX_Sensor zx_sensor = ZX_Sensor(0x10);
uint8_t x_pos;
uint8_t z_pos;
GestureType gesture;
uint8_t gesture_speed;

//Sonar sensor definitions
//The Arduino Wire library uses the 7-bit version of the address, so the code example uses 0x70 instead of the 8‑bit 0xE0
#define MB1232SensorAddress byte(0x70)
//The Sensor ranging command has a value of 0x51
#define RangeCommand byte(0x51)
//These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand1 byte(0xAA)
#define ChangeAddressCommand2 byte(0xA5)

int measurement_count = 1;
/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(Adafruit_TSL2591 *ts)
{
  sensor_t sensor;
  ts->getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("LUX");
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" lux"));  
  //Serial.println(F("------------------------------------"));
  //Serial.println(F(""));
  delay(500);
}

void displaySensorDetails_FXAS(void)
{
  sensor_t sensor;
  gyro.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.println("GYROSCOPE");
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    0x"); Serial.println(sensor.sensor_id, HEX);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" rad/s");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" rad/s");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" rad/s");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displaySensorDetails_FXOS(void)
{
  sensor_t accel, mag;
  accelmag.getSensor(&accel, &mag);
  Serial.println("------------------------------------");
  Serial.println("ACCELEROMETER");
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(accel.name);
  Serial.print  ("Driver Ver:   "); Serial.println(accel.version);
  Serial.print  ("Unique ID:    0x"); Serial.println(accel.sensor_id, HEX);
  Serial.print  ("Min Delay:    "); Serial.print(accel.min_delay); Serial.println(" s");
  Serial.print  ("Max Value:    "); Serial.print(accel.max_value, 4); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(accel.min_value, 4); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(accel.resolution, 8); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  Serial.println("------------------------------------");
  Serial.println("MAGNETOMETER");
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(mag.name);
  Serial.print  ("Driver Ver:   "); Serial.println(mag.version);
  Serial.print  ("Unique ID:    0x"); Serial.println(mag.sensor_id, HEX);
  Serial.print  ("Min Delay:    "); Serial.print(accel.min_delay); Serial.println(" s");
  Serial.print  ("Max Value:    "); Serial.print(mag.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(mag.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(mag.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displaySensorDetails_ZX(void)
{
  uint8_t ver;
  ver = zx_sensor.getModelVersion();
  Serial.println("------------------------------------");
  Serial.println("ZX SENSOR");
  Serial.println("------------------------------------");
  if ( ver == ZX_ERROR ) {
    Serial.println("Error reading model version number");
  } else {
    Serial.print("Model version: ");
    Serial.println(ver);
  }
  if ( ver != ZX_MODEL_VER ) {
    Serial.print("Model version needs to be ");
    Serial.print(ZX_MODEL_VER);
    Serial.print(" to work with this library. Stopping.");
    while(1);
  }
  
  // Read the register map version and ensure the library will work
  ver = zx_sensor.getRegMapVersion();
  if ( ver == ZX_ERROR ) {
    Serial.println("Error reading register map version number");
  } else {
    Serial.print("Register Map Version: ");
    Serial.println(ver);
  }
  if ( ver != ZX_REG_MAP_VER ) {
    Serial.print("Register map version needs to be ");
    Serial.print(ZX_REG_MAP_VER);
    Serial.print(" to work with this library. Stopping.");
    while(1);
  }
  Serial.println("");
}

/**************************************************************************/
/*
    Configures the gain and integration time for the ts12591
*/
/**************************************************************************/
void configureSensor(Adafruit_TSL2591 *ts)
{
  // You can change the gain on the fly, to adapt to brighter/dimmer light situations
  //ts1.setGain(ts12591_GAIN_LOW);    // 1x gain (bright light)
  ts->setGain(TSL2591_GAIN_MED);      // 25x gain
  // ts1.setGain(ts12591_GAIN_HIGH);   // 428x gain
  
  // Changing the integration time gives you a longer time over which to sense light
  // longer timelines are slower, but are good in very low light situtations!
  ts->setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
  // ts1.setTiming(ts12591_INTEGRATIONTIME_200MS);
  // ts1.setTiming(ts12591_INTEGRATIONTIME_300MS);
  // ts1.setTiming(ts12591_INTEGRATIONTIME_400MS);
  // ts1.setTiming(ts12591_INTEGRATIONTIME_500MS);
  // ts1.setTiming(ts12591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)

  /* Display the gain and integration time for reference sake */  
  //Serial.println(F("------------------------------------"));
  Serial.print  (F("Gain:         "));
  tsl2591Gain_t gain = ts->getGain();
  switch(gain)
  {
    case TSL2591_GAIN_LOW:
      Serial.println(F("1x (Low)"));
      break;
    case TSL2591_GAIN_MED:
      Serial.println(F("25x (Medium)"));
      break;
    case TSL2591_GAIN_HIGH:
      Serial.println(F("428x (High)"));
      break;
    case TSL2591_GAIN_MAX:
      Serial.println(F("9876x (Max)"));
      break;
  }
  Serial.print  (F("Timing:       "));
  Serial.print((ts->getTiming() + 1) * 100, DEC); 
  Serial.println(F(" ms"));
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
}

void tcaseselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

/*************************************************************************
 *  
 * Compares the address with known sensors and displays their info
 *
 ***********************************************************************/
void sensor_info(uint8_t addr){
  if(addr == 0x29){
    if (ts1.begin()) {
    Serial.print(F("Found a TS12591 sensor with address: 0x"));
    Serial.println(addr,HEX);
    Serial.println("TS12591 Sensor Details:");
    displaySensorDetails(&ts1);
    configureSensor(&ts1);
    } 
    else 
    {
      Serial.println(F("No sensor found ... check your wiring?"));
      while (1);
    }
   }

   else if((addr == 0x1C) || (addr == 0x1D) || (addr == 0x1E) ||(addr == 0x1F)){
     if(accelmag.begin(ACCEL_RANGE_4G))
     {
      Serial.print(F("Found a FXOS8700 sensor with address: 0x"));
      Serial.println(addr,HEX);
      Serial.println("FXOS8700 Sensor Details:");
      displaySensorDetails_FXOS();
      }
     else
     {
      Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
      while(1);
     }
   }
   
   else if((addr == 0x20) || (addr == 0x21)){
     if(gyro.begin())
     {
      Serial.print(F("Found a FXAS21002 sensor with address: 0x"));
      Serial.println(addr,HEX);
      Serial.println("FXAS21002 Sensor Details:");
      displaySensorDetails_FXAS();
      }
     else
     {
      Serial.println("Ooops, no FXAS21002 detected ... Check your wiring!");
      while(1);
     }
   }
   
   else if((addr == 0x10) || (addr == 0x11)){
     if ( zx_sensor.init() )
     {
      Serial.print(F("Found a Sparkfun ZX sensor with address: 0x"));
      Serial.println(addr,HEX);
      Serial.println("Sparkfun ZX Sensor Details:");
      displaySensorDetails_ZX();
      }
     else
     {
      Serial.println("Ooops, no Sparkfun ZX detected ... Check your wiring!");
      while(1);
     }
   }

   else if((addr == 0x44) || (addr == 0x45)){
     if (sht31.begin(0x44))
     {
      Serial.print(F("Found a SHT31-D sensor with address: 0x"));
      Serial.println(addr,HEX);
      }
     else
     {
      Serial.println("Ooops, no SHT31-D detected ... Check your wiring!");
      while(1);
     }
   }

   else if(addr == 0x70){
      //if ( i2c_init() )
     //{
      Serial.print(F("Found a MB1232 sensor with address: 0x"));
      Serial.println(addr,HEX);
      //}
     //else
   //  {
     // Serial.println("Ooops, no MB1232 detected ... Check your wiring!");
     // while(1);
     //}
     //Serial.print("Test");
   }
   else if(addr == 0x00){
   }

   else{
     Serial.println("This sensor is not currently supported by the Project LOOM sytem");
   }
//Seral.print("Found I2C 0x");  Serial.println(addr,HEX);
}

 

/**************************************************************************/
/*
    Program entry point for the Arduino sketch
*/
/**************************************************************************/
void setup(void) 
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("Starting Sensor Test!");

  int sensor_count = 0;
  
  //Poll the multiplexer to find all sensor addresses
  Serial.println(" ");
  Serial.println("Sensors Currently Connected:");
  Serial.println(" ");
      for (uint8_t t=0; t<8; t++) {
      tcaseselect(t);
      Serial.print("TCA Port #"); Serial.println(t);
 
      for (uint8_t addr = 0; addr<=127; addr++) {
        if (addr == TCAADDR) continue;
        
        uint8_t data;
        if (! twi_writeTo(addr, &data, 0, 1, 1)) {
           sensor_info(addr);
        }
        else{
          sensor_count++;
        }
        
        if (sensor_count == 127){
          Serial.println("No sensor connected to this port");
        }
      }
     sensor_count = 0;
     Serial.println(" ");
   }

  // (OPTIONAL) Check each address for a sensor
 // address_polling_example();
 
  // Now we're ready to get readings ... move on to loop()!
}

/**************************************************************************/
/*
    Shows how to perform a basic read on visible, full spectrum or
    infrared light (returns raw 16-bit ADC values)
*/
/**************************************************************************/
void simpleRead(Adafruit_TSL2591 *ts)
{
  // Simple data read example. Just read the infrared, fullspecrtrum diode 
  // or 'visible' (difference between the two) channels.
  // This can take 100-600 milliseconds! Uncomment whichever of the following you want to read
  uint16_t x = ts->getLuminosity(TSL2591_VISIBLE);
  //uint16_t x = ts1.getLuminosity(ts12591_FULLSPECTRUM);
  //uint16_t x = ts1.getLuminosity(ts12591_INFRARED);

  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print(F("Luminosity: "));
  Serial.println(x, DEC);
}

/**************************************************************************/
/*
    Show how to read IR and Full Spectrum at once and convert to lux
*/
/**************************************************************************/
void advancedRead(Adafruit_TSL2591 *ts)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = ts->getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print(F("IR: ")); Serial.print(ir);  Serial.print(F("  "));
  Serial.print(F("Full: ")); Serial.print(full); Serial.print(F("  "));
  Serial.print(F("Visible: ")); Serial.print(full - ir); Serial.print(F("  "));
  Serial.print(F("Lux: ")); Serial.println(ts1.calculateLux(full, ir));
}

/**************************************************************************/
/*
    Performs a read using the Adafruit Unified Sensor API.
*/
/**************************************************************************/
void unifiedSensorAPIRead(Adafruit_TSL2591 *ts)
{
  /* Get a new sensor event */ 
  sensors_event_t event;
  ts->getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  Serial.print(F("[ ")); Serial.print(event.timestamp); Serial.print(F(" ms ] "));
  if ((event.light == 0) |
      (event.light > 4294966000.0) | 
      (event.light <-4294966000.0))
  {
    /* If event.light = 0 lux the sensor is probably saturated */
    /* and no reliable data could be generated! */
    /* if event.light is +/- 4294967040 there was a float over/underflow */
    Serial.println(F("Invalid data (adjust gain or timing)"));
  }
  else
  {
    Serial.print(event.light); Serial.println(F(" lux"));
  }
}

/**************************************************************************/
/*
    Performs a read using the Adafruit SHT31 Temp/Humidity Sensor
*/
/**************************************************************************/
void sht31_sensor_read(Adafruit_SHT31 *sht31)
{
  float t = sht31->readTemperature();
  float h = sht31->readHumidity();

  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  
  if (! isnan(t)) {  // check if 'is not a number'
    Serial.print("Temp *C = "); Serial.print(t);
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("  Hum. % = "); Serial.print(h);
  } else { 
    Serial.println("Failed to read humidity");
  }
  
}

/**************************************************************************/
/*
    Performs a read using the Adafruit fxas  Sensor
*/
/**************************************************************************/
void fxas_sensor_read(Adafruit_FXAS21002C *gyro)
{
 /* Get a new sensor event */
  sensors_event_t event;
  gyro->getEvent(&event);

  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
 
  /* Display the results (speed is measured in rad/s) */
  Serial.print("X: "); Serial.print(event.gyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.gyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.gyro.z); Serial.print("  ");
  Serial.println("rad/s ");
  
}

/**************************************************************************/
/*
    Performs a read using the Adafruit fxos  Sensor
*/
/**************************************************************************/
void fxos_sensor_read(Adafruit_FXOS8700 *accelmag)
{
  sensors_event_t aevent, mevent;

  /* Get a new sensor event */
  accelmag->getEvent(&aevent, &mevent);

  /* Display the accel results (acceleration is measured in m/s^2) */
  
  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print("A ");
  Serial.print("X: "); Serial.print(aevent.acceleration.x, 4); Serial.print("  ");
  Serial.print("Y: "); Serial.print(aevent.acceleration.y, 4); Serial.print("  ");
  Serial.print("Z: "); Serial.print(aevent.acceleration.z, 4); Serial.print("  ");
  Serial.println("m/s^2");

  /* Display the mag results (mag data is in uTesla) */
  
  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print("M ");
  Serial.print("X: "); Serial.print(mevent.magnetic.x, 1); Serial.print("  ");
  Serial.print("Y: "); Serial.print(mevent.magnetic.y, 1); Serial.print("  ");
  Serial.print("Z: "); Serial.print(mevent.magnetic.z, 1); Serial.print("  ");
  Serial.println("uT");
}

/**************************************************************************/
/*
    Performs a read using the Sparkfun ZX  Sensor
*/
/**************************************************************************/

void zx_sensor_read_pos(ZX_Sensor *zx_sensor)
{
  // If there is position data available, read and print it
  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  if ( zx_sensor->positionAvailable() ) {
    x_pos = zx_sensor->readX();
    if ( x_pos != ZX_ERROR ) {
      Serial.print("X: ");
      Serial.print(x_pos);
    }
    z_pos = zx_sensor->readZ();
    if ( z_pos != ZX_ERROR ) {
      Serial.print(" Z: ");
      Serial.println(z_pos);
    }
   }
   else{
      Serial.println("No object present in front of sensor");
   }
}

void zx_sensor_read_gesture(ZX_Sensor *zx_sensor){
  // If there is gesture data available, read and print it
  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  if ( zx_sensor->gestureAvailable() ) {
    gesture = zx_sensor->readGesture();
    gesture_speed = zx_sensor->readGestureSpeed();
    switch ( gesture ) {
      case NO_GESTURE:
        Serial.println("No Gesture");
        break;
      case RIGHT_SWIPE:
        Serial.print("Right Swipe. Speed: ");
        Serial.println(gesture_speed, DEC);
        break;
      case LEFT_SWIPE:
        Serial.print("Left Swipe. Speed: ");
        Serial.println(gesture_speed, DEC);
        break;
      case UP_SWIPE:
        Serial.print("Up Swipe. Speed: ");
        Serial.println(gesture_speed, DEC);
        break;
      default:
        break;
    }
  }
  else{
  Serial.println("No gesture sensed in front of sensor");
  }
 }

/**************************************************************************/
/*
    Performs a read using the Maxbox Sonar  Sensor
*/
/**************************************************************************/
//Commands the sensor to take a range reading
void takeRangeReading(){
   Wire.beginTransmission(MB1232SensorAddress);             //Start addressing
   Wire.write(RangeCommand);                             //send range command
   Wire.endTransmission();                                  //Stop and do something else now
}

//Returns the last range that the sensor determined in its last ranging cycle in centimeters. Returns 0 if there is no communication.
word requestRange(){
   Wire.requestFrom(MB1232SensorAddress, byte(2));
   if(Wire.available() >= 2){                            //Sensor responded with the two bytes
      byte HighByte = Wire.read();                        //Read the high byte back
      byte LowByte = Wire.read();                        //Read the low byte back
      word range = word(HighByte, LowByte);         //Make a 16-bit word out of the two bytes for the range
      return range;
   }
   else {
   return word(0);                                             //Else nothing was received, return 0
   }
}

//sensor read function called by the main program
void mb1232_sensor_read(){
   takeRangeReading();                                       //Tell the sensor to perform a ranging cycle
   delay(100);                                                    //Wait for the sensor to finish
   word range = requestRange();                           //Get the range from the sensor
   Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
   Serial.print("Range:");
   Serial.println(range);          //Print to the user
}

/* Commands a sensor at oldAddress to change its address to newAddress
oldAddress must be the 7-bit form of the address that is used by Wire
7BitHuh determines whether newAddress is given as the new 7 bit version or the 8 bit version of the address
\ If true, if is the 7 bit version, if false, it is the 8 bit version
*/
void changeAddress(byte oldAddress, byte newAddress, boolean SevenBitHuh){
   Wire.beginTransmission(oldAddress);                 //Begin addressing
   Wire.write(ChangeAddressCommand1);              //Send first change address command
   Wire.write(ChangeAddressCommand2);              //Send second change address command

   byte temp;
   if(SevenBitHuh){ temp = newAddress << 1; }     //The new address must be written to the sensor
   else     { temp = newAddress;         }               //in the 8bit form, so this handles automatic shifting
   Wire.write(temp);                                          //Send the new address to change to
   Wire.endTransmission();
}
/*************************************************************************
 *  
 * Compares the address with known sensors and displays their info
 *
 ***********************************************************************/
void sensor_measurement(uint8_t addr){
  if(addr == 0x29){
    Serial.print("Lux Sensor:\n");
    simpleRead(&ts1); 
    //advancedRead(&ts1);
    //unifiedSensorAPIRead(&ts1);
   }

   else if((addr == 0x1C) || (addr == 0x1D) || (addr == 0x1E) ||(addr == 0x1F)){
     Serial.print("FXOS Sensor:\n");
     fxos_sensor_read(&accelmag);
   }
   
   else if((addr == 0x20) || (addr == 0x21)){
    Serial.print("FXAS Sensor:\n");
    fxas_sensor_read(&gyro);
   }
   
   else if((addr == 0x10) || (addr == 0x11)){
     Serial.print("ZX Sensor:\n");
     zx_sensor_read_pos(&zx_sensor);
     //zx_sensor_read_gesture(&zx_sensor);
   }

   else if((addr == 0x44) || (addr == 0x45)){
      Serial.print("SHT31 Sensor:\n");
      sht31_sensor_read(&sht31);
      Serial.print("\n");
   }

   else if(addr == 0x70){
       Serial.print("MB1232 Sensor:\n");
       mb1232_sensor_read();
     }

   else if(addr == 0x00){
   }

   else{
     Serial.println("This sensor is not currently supported by the Project LOOM sytem");
   }
//Seral.print("Found I2C 0x");  Serial.println(addr,HEX);
}


/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void) 
{   
  Serial.print("Measurements Round ");
  Serial.println(measurement_count);
      for (uint8_t t=0; t<8; t++) {
      tcaseselect(t);
       for (uint8_t addr = 0; addr<=127; addr++) {
        if (addr == TCAADDR) continue;
        
        uint8_t data;
        if (! twi_writeTo(addr, &data, 0, 1, 1)) {
           sensor_measurement(addr);
        }
   }
  }
  measurement_count++;
  Serial.print("\n");
  delay(5000);
}
