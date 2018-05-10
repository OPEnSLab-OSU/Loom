/* I2C Integration */
/* Supported Sensors:
 *  TSL2591 - Light 
 *  SHT31-D - Temperature and Humidity
 *  FXAS21002C - Gyroscope
 *  FXOS8700 - Accelerometer and Magnetometer
 *  Sparkfun ZX - IR Distance
 *  MB1232 - Sonar Distance
 *  MPU 6050 - Gyroscope
*/

/*Included Libraries*/
/**Adafruit Libraries**/
#include "Wire.h"
#include <Adafruit_Sensor.h>  
#include "Adafruit_TSL2591.h"
#include "Adafruit_SHT31.h"
#include <Adafruit_FXAS21002C.h>
#include <Adafruit_FXOS8700.h>
#include "I2Cdev.h"
#include "MPU6050.h"

/**Sparkfun Libraries**/
#include <ZX_Sensor.h>
#include "I2Cdev.h"

/** OSC Libraris **/
#include <SPI.h>
#include <OSCBundle.h>

/** LoRa Libraries **/
//#include <RH_RF95.h>
//#include <RHReliableDatagram.h>

/** WiFi Libraries **/
#include <WiFi101.h>
#include <WiFiUdp.h>
#include "LOOM_OSC_Scheme.h"



/*Wireless Protocol Setups*/
//**************LoRa Setup*****************//
/*
 
//If M0 processor is being used, initiate proper RF pins
#ifdef __SAMD21G18A__
  // Use for all SAMD processors
  #define processor 1
  #define RFM95_INT 3 //Use this for the M0
  byte error;
#endif

//If 32u4 processor is being used, initiate proper RF pins
#ifdef __AVR_ATmega32U4__
  //Use for all AVR processors
  #define processor 0
  #define RFM95_INT 7 //Use this for the 32u4
  extern "C" { 
    #include "utility/twi.h"  // from Wire library, so we can do bus scanning
  }
#endif

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

uint8_t data[] = "Hello world";
*/
//***********************************************//

//**************WiFi Setup*****************//
  //Code used to return error information
  OSCErrorCode error;

  // Define your sensor type here by commenting and un-commenting
  #define transmit_butt 10            // using on-board button, specify attached pin, transmitting 
  #define VBATPIN A7                  // Pin to check for battery voltage

  int transmission_period = 15000;     //The number of milliseconds between when the decagon finishes transmitting
                                      // and starts the reading again   
  
  //------------------------------------------------------------------------------------------------------
  // DEBUG MODE: Set to 1 if you want to see serial printouts, else, set to 0 for field use to save memory
  //------------------------------------------------------------------------------------------------------
  #ifndef DEBUG
  #define DEBUG 1
  #endif
                                       
  #define FAMILY "LOOM"
  #define DEVICE "Decagon"
  #define INSTANCE_NUM 0  // Unique instance number for this device, useful when using more than one of the same device type in same space
  
  // Set Sleep Mode Use one or the other or neither of the following 2 lines
  //#define is_sleep_period 50  // Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between
  //#define is_sleep_interrupt 11  // Uncoment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt
  
  #ifdef is_sleep_period
    #include <Adafruit_SleepyDog.h> // Include this if transmitting at timed intervals (use this one)
  #endif
  const byte flashValidationValue = 99; // Value to test to see if flashMem has been written before
  
  int led =  LED_BUILTIN;
  volatile bool ledState = LOW;
  
  float vbat = 3.3;    // Place to save measured battery voltage
  
  char ssid[] = IDString; // created AP name
  char pass[] = "1234567890";      // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
  int keyIndex = 0;                // your network key Index number (needed only for WEP)
  char ip_broadcast[] = "192.168.1.255"; // IP to Broadcast data 
  unsigned int localPort = 9436;      // local port to listen on
  
  byte mac[6]; // place to save and recall this devices MAC address
  IPAddress ip; // place to save and recall IP addressse
  
  char packetBuffer[255]; //buffer to hold incoming packet
  char  ReplyBuffer[] = "acknowledged";       // a string to send back
  
  WiFiUDP Udp;
  
  int status = WL_IDLE_STATUS;
  WiFiServer server(80);
  
  #ifdef is_analog
    #define num_measurements 4 // must be 1, 2, 4, or 8)! number of analog measurements to sample and average per channel
    int16_t a0, a1, a2, a3, a4, a5; // Memory to store analog sensor values
  #endif
//***********************************************//

//Define the multiplexer address
#define TCAADDR 0x71

/*Connected Sensors*/
/**TSL2591**/
Adafruit_TSL2591 ts1 = Adafruit_TSL2591(1); // pass in a number for the sensor identifier (for your use later)
Adafruit_TSL2591 ts2 = Adafruit_TSL2591(2); // pass in a number for the sensor identifier (for your use later)

/**SHT31-d**/
Adafruit_SHT31 sht31 = Adafruit_SHT31();

/**FXAS21002C**/
Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);

/**FXOS8700**/
Adafruit_FXOS8700 accelmag = Adafruit_FXOS8700(0x8700A, 0x8700B);

/**Sparkfun ZX**/
ZX_Sensor zx_sensor = ZX_Sensor(0x10);
uint8_t x_pos;
uint8_t z_pos;
GestureType gesture;
uint8_t gesture_speed;

/**Max Sonar**/
#define SCL_PIN 0              //Default SDA is Pin5 PORTC for the UNO -- you can set this to any tristate pin
#define SCL_PORT PORTD 
#define SDA_PIN 1              //Default SCL is Pin4 PORTC for the UNO -- you can set this to any tristate pin
#define SDA_PORT PORTD
#define I2C_TIMEOUT 100        //Define a timeout of 100 ms -- do not wait for clock stretching longer than this time
//The Arduino Wire library uses the 7-bit version of the address, so the code example uses 0x70 instead of the 8‑bit 0xE0
#define MB1232SensorAddress byte(0x70)
//The Sensor ranging command has a value of 0x51
#define RangeCommand byte(0x51)
//These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand1 byte(0xAA)
#define ChangeAddressCommand2 byte(0xA5)

/**MPU6050**/
MPU6050 accelgyro;
//MPU6050 accelgyro(0x69); // <-- use for AD0 high
int16_t ax, ay, az;
int16_t gx, gy, gz;

/**FSR**/
int fsrReading;  // the analog reading from the FSR resistor divider
int fsrVoltage;     // the analog reading converted to voltage
unsigned long fsrResistance;  // The voltage converted to resistance, can be very big so make "long"
unsigned long fsrConductance; 
long fsrForce;       // Finally, the resistance converted to force

int measurement_count = 1; // Initial Measurement round set to 0

/***************************** Sensor Detail Functions *************************/

/** TSL2591 **/
/* 
 *Displays some basic information on this sensor from the unified
 *sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
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

//Configures the gain and integration time for the ts12591
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

/**FXAS21002C**/
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

/**FXOS8700**/
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

/** Sparkfun ZX **/
void displaySensorDetails_ZX(void)
{
  uint8_t ver;
  ver = zx_sensor.getModelVersion();
 
  //Serial.println("------------------------------------");
  //Serial.println("ZX SENSOR");
  //Serial.println("------------------------------------");
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

/** MPU 6050 **/
void displaySensorDetails_MPU6050(void)
{
  Serial.println("------------------------------------");
  Serial.println("MPU 6050 SENSOR");
  Serial.println("------------------------------------");

  // initialize device
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  // use the code below to change accel/gyro offset values
  /*
  Serial.println("Updating internal sensor offsets...");
  // -76  -2359 1688  0 0 0
  Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
  Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
  Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
  Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
  Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
  Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
  Serial.print("\n");
  accelgyro.setXGyroOffset(220);
  accelgyro.setYGyroOffset(76);
  accelgyro.setZGyroOffset(-85);
  Serial.print(accelgyro.getXAccelOffset()); Serial.print("\t"); // -76
  Serial.print(accelgyro.getYAccelOffset()); Serial.print("\t"); // -2359
  Serial.print(accelgyro.getZAccelOffset()); Serial.print("\t"); // 1688
  Serial.print(accelgyro.getXGyroOffset()); Serial.print("\t"); // 0
  Serial.print(accelgyro.getYGyroOffset()); Serial.print("\t"); // 0
  Serial.print(accelgyro.getZGyroOffset()); Serial.print("\t"); // 0
  Serial.print("\n");
  */
  /*
  Serial.println("");
}


/** Multiplexer **/
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

  //If TSL2591, configure sensor and display sensor details
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

   //If FXOS, display sensor details
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

   //If FXAS, display sensor details
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

   //If Sparkfun ZX, display sensor details
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

   //If SHT31-D, display sensor address
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

   //If Max Sonar, display sensor address
   else if(addr == 0x70){
      if ( i2c_init() )
     {
      Serial.print(F("Found a MB1232 sensor with address: 0x"));
      Serial.println(addr,HEX);
      }
     else
     {
      Serial.println("Ooops, no MB1232 detected ... Check your wiring!");
      while(1);
     }
     Serial.print("Test");
   }
   
   //If MPU6050, display sensor details
   else if(addr == 0x68){
      MPU6050 accelgyro;
      Serial.print(F("Found a MPU6050 sensor with address: 0x"));
      Serial.println(addr,HEX);
      displaySensorDetails_MPU6050();
   }

   //If MPU6050 with higher address, display sensor details
   else if(addr == 0x69){
      MPU6050 accelgyro(0x69); // <-- use for AD0 high
      Serial.print(F("Found a MPU6050 sensor with address: 0x"));
      Serial.println(addr,HEX);
      displaySensorDetails_MPU6050();
      
   }

   //Do nothing if zero address is given
   else if(addr == 0x00){
   }

   //If an invalid address is presented, print a message stating the sensor isn't supported
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

  /** LoRa Setup **/
  /*
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
  */
  /****************/

  /** WiFi AP Setup **/
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);

  Serial.println("Access Point Web Server");
  Serial.print("Packet Header String: ");
  Serial.println(PacketHeaderString);

  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // by default the local IP address of will be 192.168.1.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(ssid);

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(ssid);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();

  Serial.println("\nStarting UDP connection over server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
  
  
  Wire.begin();
  
  Serial.println("Starting Sensor Test!");

  int sensor_count = 0;
  
  //Poll the multiplexer to find all sensor addresses
  Serial.println(" ");
  Serial.println("Sensors Currently Connected:");
  Serial.println(" ");
      //Cycle through all 8 ports on the multiplexer and find out if any sensors are connected
      for (uint8_t t=0; t<8; t++) {
      tcaseselect(t);
      Serial.print("TCA Port #"); Serial.println(t);

      //If 32u4 is being used, use this process to access connected sensors
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

     //If M0 is being used, use this process to access connected sensors
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
}

//Transmission function for OSC Bundles
void setTrans_OSC(OSCMessage &msg, int addrOffset) {
  transmission_period = msg.getInt(0);
  #if DEBUG == 1
  Serial.print("Transmission period set to: ");
  Serial.println(transmission_period);
  #endif
}

/***************************** Sensor Read Functions *************************/

/** TSL2591 **/
/*
    Shows how to perform a basic read on visible, full spectrum or
    infrared light (returns raw 16-bit ADC values)
*/
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

//Show how to read IR and Full Spectrum at once and convert to lux
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

//Performs a read using the Adafruit Unified Sensor API.
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

/**SHT31-d**/
//Performs a read using the Adafruit SHT31 Temp/Humidity Sensor
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

/** FXAS21002C **/
//Performs a read using the Adafruit fxas  Sensor
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

/** FXOS8700 **/
//Performs a read using the Adafruit fxos  Sensor
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

/** Sparkfun ZX **/
//Performs a read using the Sparkfun ZX  Sensor
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

//Displays what direction the gesture was moving
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

/** Max Sonar **/
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

/*Commands a sensor at oldAddress to change its address to newAddress
oldAddress must be the 7-bit form of the address that is used by Wire
7BitHuh determines whether newAddress is given as the new 7 bit version or the 8 bit version of the address
If true, if is the 7 bit version, if false, it is the 8 bit version*/
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


/** MPU6050 **/
//Performs a read using the MPU6050 Sensor
void mpu6050_sensor_read(void){
   // read raw accel/gyro measurements from device
   accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

   //Functions for Acceleration and Rotation Specifically
   //accelgyro.getAcceleration(&ax, &ay, &az);
   //accelgyro.getRotation(&gx, &gy, &gz);
  
   Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
   Serial.print("Accelerometer x: "); Serial.print(ax); Serial.print("   y: ");
   Serial.print(ay); Serial.print("   z: ");
   Serial.print(az); Serial.print("\n");
   
   Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
   Serial.print("Gyroscope x: "); Serial.print(gx); Serial.print("   y: ");
   Serial.print(gy); Serial.print("   z: ");
   Serial.println(gz);
   Serial.println();
}

/*************************************************************************
 *  
 * Compares the address with known sensors and displays their info
 *
 ***********************************************************************/
void sensor_measurement(uint8_t addr){

  //If TSL2591, collect simple lux measurement
  if(addr == 0x29){
    Serial.print("Lux Sensor:\n");
    simpleRead(&ts1); 
    //advancedRead(&ts1);
    //unifiedSensorAPIRead(&ts1);
   }

   //If FXOS, collect accelerometer measurement
   else if((addr == 0x1C) || (addr == 0x1D) || (addr == 0x1E) ||(addr == 0x1F)){
     Serial.print("FXOS Sensor:\n");
     fxos_sensor_read(&accelmag);
   }

   //If FXAS, collect gyroscope measurement
   else if((addr == 0x20) || (addr == 0x21)){
    Serial.print("FXAS Sensor:\n");
    fxas_sensor_read(&gyro);
   }

   //If Sparkfun ZX, collect distance measurement
   else if((addr == 0x10) || (addr == 0x11)){
     Serial.print("ZX Sensor:\n");
     zx_sensor_read_pos(&zx_sensor);
     //zx_sensor_read_gesture(&zx_sensor);
   }

   //If SHT31-D, collect temperature and humidity measurement
   else if((addr == 0x44) || (addr == 0x45)){
      Serial.print("SHT31 Sensor:\n");
      sht31_sensor_read(&sht31);
      Serial.print("\n");
   }

   //If Max Sonar, collect distance measurement
   else if(addr == 0x70){
       Serial.print("MB1232 Sensor:\n");
       mb1232_sensor_read();
   }

   //If MPU6050, collect gyroscope measurement
   else if((addr==0x68) || (addr ==0x69)){
       Serial.print("MPU6050 Sensor:\n");
       mpu6050_sensor_read();
   }

   //If zero address, do nothing
   else if(addr == 0x00){
   }

   //If invalid address is supplied, state that the sensor is not currently supported
   else{
     Serial.println("This sensor is not currently supported by the Project LOOM sytem");
   }
   
//Seral.print("Found I2C 0x");  Serial.println(addr,HEX);
}

//** Analog Sensor: FSR **/
//reads measurements from fsr
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
  //Loop through all the ports on the multiplexer, checking for sensors to collect measurements from
  for (uint8_t t=0; t<8; t++) {
     tcaseselect(t);

    //If 32u4 processor, use this process to access sensors
    #ifdef __AVR_ATmega32U4__
       for (uint8_t addr = 0; addr<=127; addr++) {
          if (addr == TCAADDR) continue;
          uint8_t data;
    
          if (! twi_writeTo(addr, &data, 0, 1, 1)) {
            sensor_measurement(addr);
          }
        }
     #endif

     //If M0 processor, use this process to access sensors
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

  OSCBundle bndl;

  bndl.add(IDString "/VWC").add((int32_t)value1);
  //bndl.add(IDString "/Temp").add(value2);
  //bndl.add(IDString "/ElecCond").add((int32_t)value3);
  Serial.print("TEST");

  // ===== Send message ===== //
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

  /** WiFi Functionality **/
  OSCBundle bndl;
  char addressString[255];
  
  // if there's data available, read a packet
  //parsePacket() returns 0 if unreadable, packetSize if readable.
  //Must be called before Udp.read()

  int packetSize = Udp.parsePacket();
  
  //Read packet byte by byte into the bundle.
    if(packetSize > 0) {
    bndl.empty();
    #if DEBUG == 1
      Serial.println("=========================================");
      Serial.print("received packet of size: ");
      Serial.println(packetSize);
    #endif
    while (packetSize--){
      bndl.fill(Udp.read());
    }
    
    if(!bndl.hasError()) { 
      #if DEBUG == 1
        Serial.print("Number of items in bundle: ");
        Serial.println(bndl.size());
        Serial.print("First message address string: ");
      #endif
        bndl.getOSCMessage(0)->getAddress(addressString, 0);
      #if DEBUG == 1
        Serial.println(addressString);
      #endif
        bndl.route(PacketHeaderString "/setTrans", setTrans_OSC);
    }
    else {
      error = bndl.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
  /** End WiFi Functionality **/

  delay(5000);
}

//Function used to print WiFi status
void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print your MAC address:
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
