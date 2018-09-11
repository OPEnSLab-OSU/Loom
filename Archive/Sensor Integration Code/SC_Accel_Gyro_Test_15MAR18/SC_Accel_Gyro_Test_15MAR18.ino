/* Dynamic Range: 600M:1 */
/* Maximum Lux: 88K */
//Included Libraries
#include "Wire.h"
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
//#include "Adafruit_SHT31.h"
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>
#include <ZX_Sensor.h>
#include "I2Cdev.h"
//#include "MPU6050.h"
#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <OSCBundle.h>

#ifdef __SAMD21G18A__
  // Use for all SAMD processors
  #define processor 1
  #define RFM95_INT 3 //Use this for the M0
  byte error;
#endif

#ifdef __AVR_ATmega32U4__
  //Use for all AVR processors
  #define processor 0
  #define RFM95_INT 7 //Use this for the 32u4
  extern "C" { 
    #include "utility/twi.h"  // from Wire library, so we can do bus scanning
  }
#endif

//Max Sonar Definitions
#define SCL_PIN 0              //Default SDA is Pin5 PORTC for the UNO -- you can set this to any tristate pin
#define SCL_PORT PORTD 
#define SDA_PIN 1              //Default SCL is Pin4 PORTC for the UNO -- you can set this to any tristate pin
#define SDA_PORT PORTD
#define I2C_TIMEOUT 100        //Define a timeout of 100 ms -- do not wait for clock stretching longer than this time

//**************LoRa Setup*****************//
#define RFM95_CS 8
#define RFM95_RST 4

#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 88

//IDString constructor
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x) //to concatenate a predefined number to a string literal, use STR(x)

#define FAMILY "/LOOM"
#define DEVICE "/Ishield"
#define INSTANCE_NUM 0  // Unique instance number for this device, useful when using more than one of the same device type in same space

#define IDString FAMILY DEVICE STR(INSTANCE_NUM) // C interprets subsequent string literals as concatenation: "/Loom" "/Ishield" "0" becomes "/Loom/Ishield0"
 
// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

RHReliableDatagram manager(rf95, CLIENT_ADDRESS);

OSCBundle bndl;
char message[241];
uint8_t data[] = "Hello world";
//***********************************************//

#define TCAADDR 0x71

// Example for demonstrating the ts12591 library - public domain!

// connect SCL to analog 5
// connect SDA to analog 4
// connect Vin to 3.3-5V DC
// connect GROUND to common ground

//Connected Sensors
Adafruit_TSL2591 ts1 = Adafruit_TSL2591(1); // pass in a number for the sensor identifier (for your use later)
Adafruit_TSL2591 ts2 = Adafruit_TSL2591(2); // pass in a number for the sensor identifier (for your use later)

//Adafruit_SHT31 sht31 = Adafruit_SHT31();

Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);

Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

/*
ZX_Sensor zx_sensor = ZX_Sensor(0x10);
uint8_t x_pos;
uint8_t z_pos;
GestureType gesture;
uint8_t gesture_speed;
*/
//FSR Sensor
int fsrReading;  // the analog reading from the FSR resistor divider
int fsrVoltage;     // the analog reading converted to voltage
unsigned long fsrResistance;  // The voltage converted to resistance, can be very big so make "long"
unsigned long fsrConductance; 
long fsrForce;       // Finally, the resistance converted to force

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
  delay(500);
}


void displaySensorDetails_FXAS(void)
{
  sensor_t sensor;
  gyro.getSensor(&sensor);
  delay(500);
}

void displaySensorDetails_FXOS(void)
{
  sensor_t accel, mag;
  accelmag.getSensor(&accel, &mag);
  delay(500);
}

/*
void displaySensorDetails_ZX(void)
{
  uint8_t ver;
  ver = zx_sensor.getModelVersion();
 
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
*/
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
   }

   else if((addr == 0x1C) || (addr == 0x1D) || (addr == 0x1E) ||(addr == 0x1F)){
     if(accelmag.begin(ACCEL_RANGE_4G))
     {
      Serial.print(F("Found a FXOS8700 sensor with address: 0x"));
      Serial.println(addr,HEX);
      Serial.println("FXOS8700 Sensor Details:");
      displaySensorDetails_FXOS();
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
   }
   /*
   else if((addr == 0x10) || (addr == 0x11)){
     if ( zx_sensor.init() )
     {
      Serial.print(F("Found a Sparkfun ZX sensor with address: 0x"));
      Serial.println(addr,HEX);
      Serial.println("Sparkfun ZX Sensor Details:");
      displaySensorDetails_ZX();
      }
   }
*/
/*
   else if((addr == 0x44) || (addr == 0x45)){
     if (sht31.begin(0x44))
     {
      Serial.print(F("Found a SHT31-D sensor with address: 0x"));
      Serial.println(addr,HEX);
      }
   }

*/
   else if(addr == 0x00){
   }
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

  //LoRa Setup 
  Serial.println("Initializing manager...");
  if (!manager.init())
    Serial.println("init failed");
  Serial.println("Setting Frequency...");
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.println("Setting power...");
  rf95.setTxPower(23, false);
  
  Wire.begin();
  
  Serial.println("Starting Sensor Test!");

  int sensor_count = 0;
  
  //Poll the multiplexer to find all sensor addresses
  Serial.println(" ");
  Serial.println("Sensors Currently Connected:");
  Serial.println(" ");
      for (uint8_t t=0; t<8; t++) {
      tcaseselect(t);
      Serial.print("TCA Port #"); Serial.println(t);

      #ifdef __AVR_ATmega32U4__
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
      #endif

     #ifdef __SAMD21G18A__
        sensor_count = 0;
        for (uint8_t addr = 0; addr<=127; addr++) {
            Wire.beginTransmission(addr);
            error = Wire.endTransmission();
           
            if (addr == 0x71){}
            else if (error == 0){
                sensor_info(addr);
                sensor_count++;
             }
             else if (error==4){
             Serial.print("Unknown error at address 0x");
             if (addr<16)
             Serial.print("0");
             Serial.println(addr,HEX);
             }   
          }
          if (sensor_count == 0){
             Serial.println("No sensor connected to this port");
             Serial.println();
          } 
          #endif
        }
   

  // (OPTIONAL) Check each address for a sensor
 // address_polling_example();
 
  // Now we're ready to get readings ... move on to loop()!
  
   memset(message, '\0', 241);
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
  bndl.add(IDString "/Lux:").add((int32_t)x);

}


/**************************************************************************/
/*
    Performs a read using the Adafruit SHT31 Temp/Humidity Sensor
*/
/**************************************************************************/
/*
void sht31_sensor_read(Adafruit_SHT31 *sht31)
{
  float t = sht31->readTemperature();
  float h = sht31->readHumidity();

  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  
  if (! isnan(t)) {  // check if 'is not a number'
    Serial.print("Temp *C = "); Serial.print(t);
    bndl.add(IDString "/Temp:").add((float)t);
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(h)) {  // check if 'is not a number'
    Serial.print("  Hum. % = "); Serial.print(h);
    bndl.add(IDString "/Humidity:").add((float)h);
  } else { 
    Serial.println("Failed to read humidity");
  }
  
}
*/
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
  
  bndl.add(IDString "/X").add((float)event.gyro.x);
  bndl.add(IDString "/Y").add((float)event.gyro.y);
  bndl.add(IDString "/Z").add((float)event.gyro.z);
  
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
  bndl.add(IDString "/X").add(aevent.acceleration.x);
  bndl.add(IDString "/Y").add(aevent.acceleration.y);
  bndl.add(IDString "/Z").add(aevent.acceleration.z);
  
  /* Display the mag results (mag data is in uTesla) */
  /*
  Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
  Serial.print("M ");
  Serial.print("X: "); Serial.print(mevent.magnetic.x, 1); Serial.print("  ");
  Serial.print("Y: "); Serial.print(mevent.magnetic.y, 1); Serial.print("  ");
  Serial.print("Z: "); Serial.print(mevent.magnetic.z, 1); Serial.print("  ");
  Serial.println("uT");
  */
}

/**************************************************************************/
/*
    Performs a read using the Sparkfun ZX  Sensor
*/
/**************************************************************************/
/*
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
      Serial.print(z_pos);
      Serial.println("mm");
    }
   }
   else{
      Serial.println("No object present in front of sensor");
   }
}
*/
/*************************************************************************
 *  
 * Compares the address with known sensors and displays their info
 *
 ***********************************************************************/
void sensor_measurement(uint8_t addr){
  if(addr == 0x29){
    Serial.print("Lux Sensor:\n");
    simpleRead(&ts1); 
   }

   else if((addr == 0x1C) || (addr == 0x1D) || (addr == 0x1E) ||(addr == 0x1F)){
     Serial.print("FXOS Sensor:\n");
     fxos_sensor_read(&accelmag);
   }
   
   else if((addr == 0x20) || (addr == 0x21)){
    Serial.print("FXAS Sensor:\n");
    fxas_sensor_read(&gyro);
   }
   /*
   else if((addr == 0x10) || (addr == 0x11)){
     Serial.print("ZX Sensor:\n");
     zx_sensor_read_pos(&zx_sensor);
     //zx_sensor_read_gesture(&zx_sensor);
   }
   */
/*
   else if((addr == 0x44) || (addr == 0x45)){
      Serial.print("SHT31 Sensor:\n");
      sht31_sensor_read(&sht31);
      Serial.print("\n");
   }
   */
   
   else if(addr == 0x00){
   }

   else{
     Serial.println("This sensor is not currently supported by the Project LOOM sytem");
   }
   get_OSC_string(&bndl, message);

   Serial.println(message);
   //Serial.print("Message length: ");
   //Serial.println(strlen(message));
   //Serial.print("Max message length: ");
   //Serial.println(RH_RF95_MAX_MESSAGE_LEN);

   delay(2000);
   
   Serial.print("Sending...");
   if (manager.sendtoWait((uint8_t*)message, strlen(message), SERVER_ADDRESS))
      Serial.println("ok");
   else
      Serial.println("failed");

   bndl.empty();
   memset(message, '\0', 241);
      //Seral.print("Found I2C 0x");  Serial.println(addr,HEX);
}

//reads measurements from fsr
/*
void fsr_sensor_read(){
 fsrReading = analogRead(0);  
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);
 
  // analog voltage reading ranges from about 0 to 1023 which maps to 0V to 5V (= 5000mV)
  fsrVoltage = map(fsrReading, 0, 1023, 0, 5000);
  Serial.print("Voltage reading in mV = ");
  Serial.println(fsrVoltage);  
 
  if (fsrVoltage == 0) {
    Serial.println("No pressure");  
  } else {
    // The voltage = Vcc * R / (R + FSR) where R = 10K and Vcc = 5V
    // so FSR = ((Vcc - V) * R) / V        yay math!
    fsrResistance = 5000 - fsrVoltage;     // fsrVoltage is in millivolts so 5V = 5000mV
    fsrResistance *= 10000;                // 10K resistor
    fsrResistance /= fsrVoltage;
    Serial.print("FSR resistance in ohms = ");
    Serial.println(fsrResistance);
 
    fsrConductance = 1000000;           // we measure in micromhos so 
    fsrConductance /= fsrResistance;
    Serial.print("Conductance in microMhos: ");
    Serial.println(fsrConductance);
 
    // Use the two FSR guide graphs to approximate the force
    if (fsrConductance <= 1000) {
      fsrForce = fsrConductance / 10;
      Serial.print("Force in Newtons: ");
      Serial.println(fsrForce);      
    } else {
      fsrForce = fsrConductance - 1000;
      fsrForce /= 30;
      Serial.print("Force in Newtons: ");
      Serial.println(fsrForce);            
    }
  }
}

*/
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

    #ifdef __AVR_ATmega32U4__
       for (uint8_t addr = 0; addr<=127; addr++) {
          if (addr == TCAADDR) continue;
          uint8_t data;
    
          if (! twi_writeTo(addr, &data, 0, 1, 1)) {
            sensor_measurement(addr);
          }
        }
     #endif

     #ifdef __SAMD21G18A__
        for (uint8_t addr = 0; addr<=127; addr++) {
          Wire.beginTransmission(addr);
          error = Wire.endTransmission();

          if (addr == 0x71){}
          else if (error == 0){
            sensor_measurement(addr);
          }
        }
     #endif
  }
  //fsr_sensor_read();
  measurement_count++;
  Serial.print("\n");


  //******************LoRa Functionality**************//
  /*
  int value1 = 1;
  float value2 = 2.5;
  int value3 = -12;

  // ===== Create OSCBundle =====
  // Create your own data identification strings
  // Replace "/VWC", "/Temp", etc. with your specific values
  // bndl.add().add() -> The first 'add' adds a message to the bundle.
  //                    All subsequent 'add' operations add data to a given message.
  //
  // Ex: bndl.add("addr1").add(12).add(-6)
  //     This example adds a message with address/routing information "addr1" to the
  //     OSCBundle 'bndl'.  It adds two data values to this message: 12 and -6
  //     NOTE: this version of OSCInterpret does not support multiple data values
  //     attached to a single value.  If this is functionality you desire, let
  //     Kenny know.


  bndl.add(IDString "/VWC").add((int32_t)value1);
  bndl.add(IDString "/Temp").add(value2);
  bndl.add(IDString "/ElecCond").add((int32_t)value3);
  //Serial.print("TEST");

  // ===== Send message =====
  char message[241];

  memset(message, '\0', 241);
  
  get_OSC_string(&bndl, message);

  //Serial.println(message);
  //Serial.print("Message length: ");
  //Serial.println(strlen(message));
  //Serial.print("Max message length: ");
  //Serial.println(RH_RF95_MAX_MESSAGE_LEN);

  delay(2000);
  
  Serial.print("Sending...");
  if (manager.sendtoWait((uint8_t*)message, strlen(message), SERVER_ADDRESS))
    Serial.println("ok");
  else
    Serial.println("failed");
*/
  //******************End LoRa Functionality**************//

  delay(5000);
}
