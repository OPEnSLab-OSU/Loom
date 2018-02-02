/*
  WiFi Access Point
  Get MPU6050 DMP data
  Transmit OSC formatted data stream over UDP to other connected devices

  created Aug 29, 2017
  by Chet Udell
  
  Adapted from Tom Igoe
  Adapted from WiFi AP by Adafruit

  Dependencies:

  !! Note I2CDdev library has issue compiling with Feather MO, Simple fix is to Change in I2Cdev.h:
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include <Wire.h>
#endif

to
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include <Wire.h>
    #define BUFFER_LENGTH 32
#endif


  Pinouts:
  MPU6050:
  VCC => 3.3v
  GND => GND
  SCL => SCL
  SDA =>SDA
  INT => 11
  
  
 */

#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>
#include <Adafruit_NeoPixel.h>



// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            16 //A2

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int led =  LED_BUILTIN;

//------------------------------------------------------------------------------------------------------
// DEBUG MODE: Set to 1 if you want to see serial printouts, else, set to 0 for field use to save memory
//------------------------------------------------------------------------------------------------------
#ifndef DEBUG
#define DEBUG 1
#endif

//------------------------------------------------------------------------------------------------------
// MEMORY TYPE: M0 uses flash (MEM_TYPE = 0), 32u4 uses EEPROM (MEM_TYPE = 1)
//------------------------------------------------------------------------------------------------------
#define MEM_FLASH 0
#define MEM_EEPROM 1

#ifndef MEM_TYPE
#define MEM_TYPE MEM_FLASH
#endif  

#if MEM_TYPE == MEM_FLASH
#include <FlashStorage.h>
#endif 

// Define your sensor type here by commenting and un-commenting
#define is_analog 2      // also define number of channels
#define is_i2c 0x68      // also define i2c address of device
#define transmit_butt 10        // using on-board button, specify attached pin, transmitting 
#define VBATPIN A7       // Pin to check for battery voltage
//#define CLIENT_REQUESTS_DATA 1 // Set to 1 if you only send data when requested by client, else, send data at sample/sleep rate

//------------------------------------------------------------------------------------------------------
//OSC identification: convention is FAMILY = "/LOOM", DEVICE is something like "/IShield" (whatever the name of the device is), and INSTANCE_NUM is a number
//------------------------------------------------------------------------------------------------------
#define FAMILY "LOOM"
#define DEVICE "Ishield"
#define INSTANCE_NUM 0  // Unique instance number for this device, useful when using more than one of the same device type in same space
#include "LOOM_OSC_Scheme.h"

//Code used to return error information
OSCErrorCode error;

// Set Sleep Mode Use one or the other or neither of the following 2 lines
#define is_sleep_period 50  // Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between
//#define is_sleep_interrupt 11  // Uncomment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt

#ifdef is_sleep_period
  #include <Adafruit_SleepyDog.h> // Include this if transmitting at timed intervals (use this one)
#endif

#ifdef is_sleep_interrupt
  #include <LowPower.h> //Include this if transmitting on pin interrupt
#endif

//ADD sparkfun library for is_sleep_interrupt
#define CLIENT true
#define HOST false
//Structure to store device configuration information
struct config_t {
  byte checksum;               //value is changed when flash memory is written to.
  IPAddress ip;                //Device's IP Address
  char* my_ssid;
  char* ssid;                //Created AP's name
  char* pass;                //AP Password (10 or 26 characters long)
  int   keyIndex;            //Key Index Number (needed only for WEP)
  char* ip_broadcast;        //IP to Broadcast data
  unsigned int localPort;      //Local port to listen on
  byte  mac[6];                 //Device's MAC Address
  bool wifi_mode = CLIENT;
#ifdef is_i2c
  int   ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset; //mpu6050 config
#endif
};
struct config_t configuration;
const byte memValidationValue = 99; // Value to test to see if memory has been written before

#if MEM_TYPE == MEM_FLASH
  FlashStorage(flash_config,config_t);    //Setup the flash storage for the structure
#endif

//int led =  LED_BUILTIN;
volatile bool ledState = LOW;

float vbat = 3.3;    // Place to save measured battery voltage

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

#ifdef is_analog
  #define num_measurements 4 // must be 1, 2, 4, or 8)! number of analog measurements to sample and average per channel
  int16_t a0, a1, a2, a3, a4, a5; // Memory to store analog sensor values
#endif

#ifdef is_i2c
  // Include libraries for serial and i2c devices
  //#include "S_message.h"
  #include "I2Cdev.h"
  #include "Wire.h"
  #include "MPU6050_6Axis_MotionApps20.h"
  MPU6050 mpu; // create instance of MPU6050 called mpu
  MPU6050 accelgyro; // another called accelgyro
  // Uncoment one or more of these to determine which readings and format to send
  #define OUTPUT_READABLE_YAWPITCHROLL
  //#define OUTPUT_BINARY_YAWPITCHROLL
  //#define OUTPUT_READABLE_REALACCEL
  //#define OUTPUT_READABLE_WORLDACCEL  // this is pretty cool
  //#define OUTPUT_BINARY_ACCELGYRO
  #define INTERRUPT_PIN 11  // use pin 2 on Arduino Uno, pin 3 on Adafruit ProTrinket!
  // MPU control/status vars
  bool dmpReady = false;  // set true if DMP init was successful
  uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
  uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
  uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
  uint16_t fifoCount;     // count of all bytes currently in FIFO
  uint8_t fifoBuffer[64]; // FIFO storage buffer
  
  // orientation/motion vars
  Quaternion q;           // [w, x, y, z]         quaternion container
  VectorInt16 aa;         // [x, y, z]            accel sensor measurements
  VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
  VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
  VectorFloat gravity;    // [x, y, z]            gravity vector
  float euler[3];         // [psi, theta, phi]    Euler angle container
  float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
  
  // Place to save accel gyro mpu6050 readings
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  
  
  // ================================================================
  // ===               INTERRUPT DETECTION ROUTINE                ===
  // ================================================================
  
  volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
  void dmpDataReady() {
      mpuInterrupt = true;
  }
  
  // MPU calibration vars:
  int mean_ax,mean_ay,mean_az,mean_gx,mean_gy,mean_gz,state=0;
  //int ax_offset,ay_offset,az_offset,gx_offset,gy_offset,gz_offset;
  //Change this 3 variables if you want to fine tune the skecth to your needs.
  int buffersize=1000;     //Amount of readings used to average, make it higher to get more precision but sketch will be slower  (default:1000)
  int acel_deadzone=8;     //Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
  int giro_deadzone=1;     //Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)
  
  // ================================================================
  // ===       Struct for saving MPU params       ===
  // ================================================================
  // see http://playground.arduino.cc/Code/EEPROMWriteAnything
  /*struct config_t
  {
      int ax_offset,ay_offset,az_offset,gx_offset,gy_offset,gz_offset;
      byte written;
  } configuration;*/
#endif
// ** End if i2c check

void setup() {
 // Neo Pixel Setup
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // Initialize all pixels to 'off'
  
  //Initialize serial and wait for port to open:
#if DEBUG == 1
  Serial.begin(9600);
  while(!Serial) { }     //Ensure Serial is ready to go before anything happens.
#endif
  pinMode(led, OUTPUT);      // set the LED pin mode
  
#ifdef transmit_butt
  pinMode(transmit_butt, INPUT_PULLUP);      // set the transmit_butt pin mode to input
#endif

#ifdef is_i2c
  // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    pinMode(INTERRUPT_PIN, INPUT);
  #if DEBUG == 1
    Serial.println("Using MPU6050 i2c");
  #endif
// *** Init MPU 6050 and serial stuff
  accelgyro.initialize();
  mpu.initialize();
  devStatus = mpu.dmpInitialize();
  // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
// make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
    #if DEBUG == 1
        Serial.println(F("Enabling DMP..."));
    #endif
        mpu.setDMPEnabled(true);

        // Uncomment following 2 lines if using enable Arduino Uno, MO, or Trinket interrupt detection
    #if DEBUG == 1
      Serial.println(F("Enabling MPU interrupt detection (Arduino external interrupt 0)..."));
    #endif
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);

        // Uncomment following 2 lines if using Adafruit Feather 32u4
        // enable interrupt for PCINT7...
#if DEBUG == 1
        //Serial.println(F("Enabling MPU interrupt detection PCINT 7 (pin 11)"));
#endif
        //pciSetup(INTERRUPT_PIN);
        
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        #if DEBUG == 1
            Serial.println(F("DMP ready! Waiting for first interrupt..."));
        #endif
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        #if DEBUG == 1
            Serial.print(F("DMP Initialization failed (code "));
            Serial.print(devStatus);
            Serial.println(F(")"));
        #endif
    }
  // ** end serial stuff
#endif 


#if MEM_TYPE == MEM_FLASH
  #if DEBUG == 1
    Serial.println("Reading from flash.");
  #endif
  configuration = flash_config.read();                    // read from flash memory
  #if DEBUG == 1
    Serial.print("Checksum: ");
    Serial.println(configuration.checksum);
  #endif
  if (configuration.checksum != memValidationValue){    //The memory has not been written to before. 
    configuration.my_ssid = IDString;
    configuration.ssid = "OPEnS";
    configuration.pass = "Replace_with_your_wifi_password";                // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
    configuration.keyIndex = 0;                       // your network key Index number (needed only for WEP)
    configuration.ip_broadcast = "192.168.1.255";     // IP to Broadcast data 
    configuration.localPort = 9436;                   // local port to listen on
    configuration.wifi_mode = HOST;
#if DEBUG == 1
    Serial.println("\nReading sensors for first time...");
#endif
    
    #ifdef is_i2c
      calMPU6050();
    #else
      configuration.checksum = memValidationValue;      // configuration has been written successfully, so we write the checksum
    #endif
  #if DEBUG == 1
        Serial.println("Writing to flash for the first time.");
  #endif
        flash_config.write(configuration);                //don't uncomment this line until we're pretty confident that this behaves how we want; flash memory has limited writes and we don't want to waste it on unnecessary tests
  }
#endif
#if DEBUG == 1          //If the read from memory is successful.
  else {
    Serial.print("SSID: ");
    Serial.println(configuration.ssid);
    Serial.print("Pass: ");
    Serial.println(configuration.pass);
    Serial.print("Key Index: ");
    Serial.println(configuration.keyIndex);
    Serial.print("IP to Broadcast to: ");
    Serial.println(configuration.ip_broadcast);
    Serial.print("Local Port: ");
    Serial.println(configuration.localPort);
    #ifdef is_i2c
      Serial.print("ax_offset: ");
      Serial.println(configuration.ax_offset);
      Serial.print("ay_offset: ");
      Serial.println(configuration.ay_offset);
      Serial.print("az_offset: ");
      Serial.println(configuration.az_offset);
      Serial.print("gx_offset: ");
      Serial.println(configuration.gx_offset);
      Serial.print("gy_offset: ");
      Serial.println(configuration.gy_offset);
      Serial.print("gz_offset: ");
      Serial.println(configuration.gz_offset);
    #endif
  }
#endif
 
//----------------------------------------------------
// -----              WiFi AP Setup              -----
//----------------------------------------------------
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);
#if DEBUG == 1
  Serial.println("Access Point Web Server");
#endif

  pinMode(led, OUTPUT);      // set the LED pin mode
  // check for the presence of the shield:
  
  if (WiFi.status() == WL_NO_SHIELD) {
#if DEBUG == 1
    Serial.println("WiFi shield not present");
#endif
    // don't continue
    while (true);
  }

  // by default the local IP address of will be 192.168.1.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));

#if DEBUG == 1
  // print the network name (SSID);
  if (configuration.wifi_mode == CLIENT)
    Serial.print("Attempting to connect to: ");
  else 
    Serial.print("Creating access point named: ");
  Serial.println(configuration.my_ssid);
#endif

  // Create open network. Change this line if you want to create an WEP network:
  if (configuration.wifi_mode == CLIENT){
    status = WiFi.begin(configuration.ssid, configuration.pass);
    delay(10000);
    
    if (status != WL_CONNECTED) {
      #if DEBUG == 1
      Serial.println("Connecting to WPA host failed");
      #endif
    }
  }
  else if (configuration.wifi_mode == HOST){
    status = WiFi.beginAP(configuration.my_ssid);
  
    if (status != WL_AP_LISTENING) {
      #if DEBUG == 1
        Serial.println("Creating access point failed");
      #endif
      // don't continue
      while (true);
    }
  
    // wait 10 seconds for connection:
    delay(10000);

    // start the web server on port 80
    server.begin();
  }
#if DEBUG == 1
  // you're connected now, so print out the status
  printWiFiStatus();
  Serial.println("Starting UDP connection over server...");
#endif
  // if you get a connection, report back via serial:
  Udp.begin(configuration.localPort);
}


int redVal = 0, greenVal = 0, blueVal = 0;

void setRed(OSCMessage &msg, int addrOffset){
  redVal = msg.getInt(0);
#if DEBUG == 1
  Serial.print("set redVal to ");
  Serial.println(redVal);
#endif
}

void setGreen(OSCMessage &msg, int addrOffset){
  greenVal = msg.getInt(0);
#if DEBUG == 1
  Serial.print("set greenVal to ");
  Serial.println(greenVal);
#endif
}
void setBlue(OSCMessage &msg, int addrOffset){
  blueVal = msg.getInt(0);
#if DEBUG == 1
  Serial.print("set blueVal to ");
  Serial.println(blueVal);
#endif
}

void calMPU6050_OSC(OSCMessage &msg, int addrOffset) {
  #if DEBUG == 1
   Serial.println("Command received to calibrate MPU6050");
  #endif
  //meansensors();
  //calibration();
  calMPU6050();
  flash_config.write(configuration); 
  #if DEBUG == 1
   Serial.println("New calibration values written to non-volatile memory");
  #endif
  
  //Save calibrated values
}

char new_ssid[50];
char new_pass[50];

void setSSID(OSCMessage &msg, int addrOffset){
  msg.getString(0,new_ssid,49);
}

void setPassword(OSCMessage &msg, int addrOffset){
  msg.getString(0,new_pass,49);
}

void connectToWPA(char ssid[], char pass[]){
  Udp.stop();
  WiFi.disconnect();
  WiFi.end();
  status = WiFi.begin(ssid, pass);
  delay(5000);
  
  if (status != WL_CONNECTED) {
    #if DEBUG == 1
    Serial.println("Connecting to WPA host failed");
    
    #endif
    status = WiFi.beginAP(configuration.my_ssid);
    delay(5000);//delay for connection
    Udp.begin(configuration.localPort);
    #if DEBUG == 1
    Serial.print("should now be listening on port ");
    Serial.println(configuration.localPort);
    #endif
  }
  else{
    #if DEBUG == 1
    Serial.println("Successfully connected to WPA host");
    printWiFiStatus();
    #endif
    configuration.wifi_mode = CLIENT;
    configuration.ssid = ssid;
    configuration.pass = pass;
    Udp.begin(configuration.localPort);
  }
  
}

void loop() {
  for (int i = 0; i < 50; i++){
    new_ssid[i] = '\0';
    new_pass[i]= '\0';
  }
  OSCBundle bndl;
  char addressString[255];
 
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
//  Serial.println(packetSize);
  if (packetSize > 0)
  {
    bndl.empty();
#if DEBUG == 1
    Serial.println("=========================================");
    Serial.print("received packet of size: ");
    Serial.println(packetSize);
#endif
    while (packetSize--){
      bndl.fill(Udp.read());
    }
    if (!bndl.hasError()){
#if DEBUG == 1
          Serial.print("Number of items in bundle: ");
          Serial.println(bndl.size());
          Serial.print("First message address string: ");
          bndl.getOSCMessage(0)->getAddress(addressString, 0);
          Serial.println(addressString);
#endif
      bndl.route("/LOOM/Ishield0/Port0/Neopixel/Red",setRed);
      bndl.route("/LOOM/Ishield0/Port0/Neopixel/Green",setGreen);
      bndl.route("/LOOM/Ishield0/Port0/Neopixel/Blue",setBlue);
      bndl.route("/LOOM/Ishield0/Connect/SSID",setSSID);
      bndl.route("/LOOM/Ishield0/Connect/Password",setPassword);

      if (new_ssid[0] != '\0' && new_pass[0] != '\0'){
        Serial.print("Received command to connect to ");
        Serial.print(new_ssid);
        Serial.print(" with password ");
        Serial.println(new_pass);
        connectToWPA(new_ssid,new_pass);
      }
      
      pixels.setPixelColor(0, pixels.Color((redVal > 255) ? 255 : redVal, (greenVal > 255) ? 255 : greenVal, (blueVal > 255) ? 255: blueVal));
      pixels.show();
#ifdef is_i2c
       bndl.route(PacketHeaderString "/MPU6050/cal", calMPU6050_OSC); 
#endif
    }
    else {
      error = bndl.getError();
#if DEBUG == 1
      Serial.print("error: ");
      Serial.println(error);
#endif
    }

  }
  

  
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      byte remoteMac[6];

#if DEBUG == 1
      // a device has connected to the AP
      Serial.print("Device connected to AP, MAC address: ");
#endif
      WiFi.APClientMacAddress(remoteMac);
#if DEBUG == 1
      Serial.print(remoteMac[5], HEX);
      Serial.print(":");
      Serial.print(remoteMac[4], HEX);
      Serial.print(":");
      Serial.print(remoteMac[3], HEX);
      Serial.print(":");
      Serial.print(remoteMac[2], HEX);
      Serial.print(":");
      Serial.print(remoteMac[1], HEX);
      Serial.print(":");
      Serial.println(remoteMac[0], HEX);
#endif
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
#if DEBUG == 1
      Serial.println("Device disconnected from AP");
#endif
    }
  }

#if (CLIENT_REQUESTS_DATA)
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
  IPAddress remoteIp = Udp.remoteIP();
  int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
#if DEBUG == 1
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer

    Serial.println("Contents:");
    Serial.println(packetBuffer);
#endif
#endif

// measure battery voltage
  vbat = analogRead(VBATPIN);
  vbat *= 2;    // we divided by 2, so multiply back
  vbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  vbat /= 1024; // convert to voltage

#ifdef is_i2c
    // Update MPU6050 Data
    // Now measure MPU6050, update values in global registers
    measure_mpu6050();

    udp_mpu6050();
    
    // flush MPU6050 FIFO to avoid overflows if using i2c
    mpu.resetFIFO();

#endif
    
#ifdef is_analog
    measure_analog();
#endif

#if (CLIENT_REQUESTS_DATA)
  } // end if(packetsize) check (true if client sends request for data)
#endif

#ifdef is_sleep_period
  #if DEBUG == 0
    int sleepMS = Watchdog.sleep(is_sleep_period); // sleep MCU for transmit period duration
  #else
    delay(is_sleep_period); // demo transmit every 1 second
  #endif
#endif  


} // End loop section

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  configuration.ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(configuration.ip);

  // print your MAC address:
  WiFi.macAddress(configuration.mac);
  Serial.print("MAC address: ");
  Serial.print(configuration.mac[5], HEX);
  Serial.print(":");
  Serial.print(configuration.mac[4], HEX);
  Serial.print(":");
  Serial.print(configuration.mac[3], HEX);
  Serial.print(":");
  Serial.print(configuration.mac[2], HEX);
  Serial.print(":");
  Serial.print(configuration.mac[1], HEX);
  Serial.print(":");
  Serial.println(configuration.mac[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(configuration.ip);
}
