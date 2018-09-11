/* ts12591 Digital Light Sensor */
/* Dynamic Range: 600M:1 */
/* Maximum Lux: 88K */

#include "Wire.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
#include "Adafruit_SHT31.h"
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>


#define TCAADDR 0x70

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
  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
  Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
  Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution); Serial.println(F(" lux"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
  delay(500);
}

void displaySensorDetails_FXAS(void)
{
  sensor_t sensor;
  gyro.getSensor(&sensor);
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
  Serial.println(F("------------------------------------"));
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
 
/**************************************************************************/
/*
    Program entry point for the Arduino sketch
*/
/**************************************************************************/
void setup(void) 
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("Starting Adafruit ts12591 Test!");

 /*Initialize 1st sensor*/
  tcaseselect(1);
  if (ts1.begin()) 
  {
    Serial.println(F("Found a ts12591 sensor"));
  } 
  else 
  {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }

  /*Check a port with no sensor  
  tcaseselect(5);
  if (ts1.begin()) 
  {
    Serial.println(F("Found a ts12591 sensor"));
  } 
  else 
  {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }
*/
  /*Initialize 2nd sensor */ 
  tcaseselect(2);
  if (ts2.begin()) 
  {
    Serial.println(F("Found a ts12591 sensor"));
  } 
  else 
  {
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1);
  }

  //Initialize SHT31
  Serial.println("SHT31 test");
  tcaseselect(6);
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  else
  {
    Serial.println(F("Found a sht31 sensor"));
  }

  //Initialize FAXS Test
  Serial.println("Gyroscope Test"); Serial.println("");
  tcaseselect(7);
  /* Initialise the sensor */
  if(!gyro.begin())
  {
    /* There was a problem detecting the FXAS21002C ... check your connections */
    Serial.println("Ooops, no FXAS21002C detected ... Check your wiring!");
    while(1);
  }


  Serial.println("FXOS8700 Test"); Serial.println("");

  /* Initialise the sensor */
  if(!accelmag.begin(ACCEL_RANGE_4G))
  {
    /* There was a problem detecting the FXOS8700 ... check your connections */
    Serial.println("Ooops, no FXOS8700 detected ... Check your wiring!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  tcaseselect(1);
  displaySensorDetails(&ts1);
  configureSensor(&ts1);
  tcaseselect(2);
  displaySensorDetails(&ts2);
  configureSensor(&ts2);
  tcaseselect(7);
  displaySensorDetails_FXAS();
  displaySensorDetails_FXOS();


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

  Serial.println("");
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void) 
{ 

  //Sensor 0

  //Sensor 1
  tcaseselect(1);
  Serial.print("Lux Sensor 1:\n");
  simpleRead(&ts1); 
  //advancedRead(&ts1);
  //unifiedSensorAPIRead(&ts1);

  //Sensor 2
  tcaseselect(2);
  Serial.print("Lux Sensor 2:\n");
  simpleRead(&ts2); 
  //advancedRead(&ts2);
  //unifiedSensorAPIRead(&ts2);

  //Sensor 3
  //Sensor 4
  //Sensor 5
  
  //Sensor 6
  tcaseselect(6);
  Serial.print("SHT31 Sensor 6:\n");
  sht31_sensor_read(&sht31);
  Serial.print("\n");
  
  //Sensor 7
  tcaseselect(7);
  Serial.print("FXAS Sensor 7:\n");
  fxas_sensor_read(&gyro);
  fxos_sensor_read(&accelmag);
  delay(5000);
}
