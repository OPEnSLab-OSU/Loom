/*
  January 19, 2018. Author: Trevor Swope
  This is the LOOM WiFi Mote template! Read this big 'ol comment for instructions on how to use the preprocessor statements in here to define the functionality of your board.
 
  DEBUG MODE: define DEBUG as 1 or 0. If DEBUG is set, make sure the serial monitor is open; all serial calls should be wrapped in an #if DEBUG == 1 ... #endif

  SEND_OSC: if the device is going to be sending readings from anything attached to it, set this to true
  RECEIVE_OSC: if the device is going to receive any commands from the hub, set this to true

  //ADD MORE DESCRIPTION HERE
  
 */

#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

enum WiFiMode{
  AP_MODE,
  WPA_CLIENT_MODE,
  WEP_CLIENT_MODE
  //more Wifi Modes?
};

//--PREPROCESSOR STATEMENTS BEGIN HERE

#ifndef DEBUG
#define DEBUG 1 //set to 1 if you want Serial statements from various functions to print
#endif

#define FAMILY "LOOM"    // Should always be "/LOOM", you can change this if you are setting up your own network
#define DEVICE "Ishield"  // The device name, should begin with a slash followed by an unbroken string with no more slashes i.e. "/RelayShield" or "/IShield"

#define SEND_OSC          // Comment this out to turn off sending of OSC messages
#define RECEIVE_OSC       // Comment this out to turn off receiving of OSC messages

#ifdef SEND_OSC or RECEIVE_OSC
  #include <OSCBundle.h>
#endif

#define is_analog 2
#define is_i2c 0x86
#define transmit_butt 10        // using on-board button, specify attached pin, transmitting 
#define VBATPIN A7       // Pin to check for battery voltage

#ifdef is_i2c
  #define is_mpu6050
  #define is_neopixel
#endif

#ifdef is_neopixel
  #include <Adafruit_NeoPixel.h>
  // Which pin on the Arduino is connected to the NeoPixels?
  #define PIN            16 //A2
  
  // How many NeoPixels are attached to the Arduino?
  #define NUMPIXELS      1
  
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
  int redVal = 0, greenVal = 0, blueVal = 0;
  void setRed(OSCMessage &msg){
    redVal = msg.getInt(0);
  #if DEBUG == 1
    Serial.print("set redVal to ");
    Serial.println(redVal);
  #endif
  }

  void setGreen(OSCMessage &msg){
    greenVal = msg.getInt(0);
  #if DEBUG == 1
    Serial.print("set greenVal to ");
    Serial.println(greenVal);
  #endif
  }
  void setBlue(OSCMessage &msg){
    blueVal = msg.getInt(0);
  #if DEBUG == 1
    Serial.print("set blueVal to ");
    Serial.println(blueVal);
  #endif
  }
#endif

#ifdef is_analog
  #define num_measurements 4 // must be 1, 2, 4, or 8)! number of analog measurements to sample and average per channel
  int16_t a0, a1, a2, a3, a4, a5; // Memory to store analog sensor values
#endif

#ifdef is_i2c
  // Include libraries for serial and i2c devices
  //#include "S_message.h"
  #include "I2Cdev.h"
  #include "Wire.h"
  #define INTERRUPT_PIN 11
  bool dmpReady = false; //set true if DMP init was successful
  #ifdef is_mpu6050
    #include "MPU6050_6Axis_MotionApps20.h"
    MPU6050 mpu; // create instance of MPU6050 called mpu
    MPU6050 accelgyro; // another called accelgyro
    // Uncoment one or more of these to determine which readings and format to send
    #define OUTPUT_READABLE_YAWPITCHROLL
    //#define OUTPUT_BINARY_YAWPITCHROLL
    //#define OUTPUT_READABLE_REALACCEL
    //#define OUTPUT_READABLE_WORLDACCEL  // this is pretty cool
    //#define OUTPUT_BINARY_ACCELGYRO
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
  #endif
#endif

#ifdef SEND_OSC
  #define is_sleep_period 50 // Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between. Change the value according to the length of your desired transmission interval
  //#define is_sleep_interrupt 11 // Uncomment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt
#endif

#ifdef is_sleep_period
  #include <Adafruit_SleepyDog.h> // This must be included if you are transmitting at timed intervals
#endif

#ifdef is_sleep_interrupt
  #include <LowPower.h> //Include this if transmitting on pin interrupt
#endif

#ifdef SEND_OSC or RECEIVE_OSC
  #define STR_(x) #x //helper function
  #define STR(x) STR_(x) //to concatenate a predefined number to a string literal, use STR(x)
  #define PacketHeaderString STR(/) FAMILY STR(/) DEVICE //results in a single string, i.e. /LOOM/Device. the full prefix sent to this device should be /LOOM/Device#, but the number is parsed in the OSC bundle routing function
  #include <OSCBundle.h> // Use this to handle all OSC messaging behavior (sending or receiving)
  OSCErrorCode error;
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

struct config_t {
  byte checksum;               //value is changed when flash memory is written to.
  uint8_t instance_number;     //default 0, should be set on startup from a patch
  char packet_header_string[80];
  IPAddress ip;                //Device's IP Address
  char* my_ssid;               //Default AP name
  char ssid[32];                  //Host network name
  char pass[32];                  //Host network password
  int   keyIndex;            //Key Index Number (needed only for WEP)
  char* ip_broadcast;        //IP to Broadcast data
  unsigned int localPort;      //Local port to listen on
  byte  mac[6];                 //Device's MAC Address
  WiFiMode wifi_mode;
  //add any other stuff that needs to be stored based on the shields with a wrapped preprocessor statement HERE
#ifdef is_mpu6050
  int   ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset; //mpu6050 config
#endif
  
};

struct config_t configuration;
const byte memValidationValue = 99;

#if MEM_TYPE == MEM_FLASH
  FlashStorage(flash_config,config_t);    //Setup the flash storage for the structure
#endif

int led =  LED_BUILTIN;
volatile bool ledState = LOW;

float vbat = 3.3;    // Place to save measured battery voltage

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;
int status = WL_IDLE_STATUS;
WiFiServer server(80);

void start_AP(){
#if DEBUG == 1
  // print the network name (SSID);
  Serial.print("Creating access point named: ");
  Serial.println(configuration.my_ssid);
#endif
  // Create open network. Change this line if you want to create an WEP network:
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
#if DEBUG == 1
  // you're connected now, so print out the status
  printWiFiStatus();

  Serial.println("\nStarting UDP connection over server...");
#endif
  // if you get a connection, report back via serial:
  Udp.begin(configuration.localPort);
}

bool connect_to_WPA(char ssid[],char pass[]){
status = WiFi.begin(ssid,pass);
    int attempt_count = 0;
      while (status != WL_CONNECTED && attempt_count < 10) { //arbitrary, attempt the connection 10 times
        #if DEBUG == 1
        Serial.println("Connecting to WPA host failed, trying again");
        #endif
        
        status = WiFi.begin(ssid,pass);
     
        attempt_count++;
      }
      if (status != WL_CONNECTED){
        #if DEBUG == 1
        Serial.println("Connecting to WPA host failed completely");
        #endif
        return false;
      }
      delay(8000);
      #if DEBUG == 1
    // you're connected now, so print out the status
    printWiFiStatus();
    
    
    Serial.println("Starting UDP connection over server...");
    
    #endif
      // if you get a connection, report back via serial:
      server.begin();
      Udp.begin(configuration.localPort);
      return true;
}

void init_config(){
#if MEM_TYPE == MEM_FLASH
  #if DEBUG == 1
    Serial.println("Reading from flash.");
  #endif
  configuration = flash_config.read();                    // read from flash memory
  #if DEBUG == 1
    Serial.print("Checksum: ");
    Serial.println(configuration.checksum);
  #endif
  if (configuration.checksum != memValidationValue){ //write default values to flash
        configuration.instance_number = 0;
        sprintf(configuration.packet_header_string,"%s%d",PacketHeaderString,configuration.instance_number);
        configuration.my_ssid = "featherM0"; //default AP name
        strcpy(configuration.ssid,"OPEnS");               // created AP name
        strcpy(configuration.pass,"Replace_with_your_wifi_password");                // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
        configuration.keyIndex = 0;                       // your network key Index number (needed only for WEP)
        configuration.ip_broadcast = "192.168.1.255";     // IP to Broadcast data 
        configuration.localPort = 9436;                   // local port to listen on
        configuration.wifi_mode = AP_MODE;
        //add any other behavior/calibration wrapped in an #ifdef is_Something preprocessor directive HERE
      #ifdef is_mpu6050
        calMPU6050(); //calibration writes memValidationValue for us
      #else
        configuration.checksum = memValidationValue;      // configuration has been written successfully, so we write the checksum
      #endif
  #if DEBUG == 1
        Serial.println("Writing to flash for the first time.");
  #endif
        //flash_config.write(configuration);                //don't uncomment this line until we're pretty confident that this behaves how we want; flash memory has limited writes and we don't want to waste it on unnecessary tests
  }
  #if DEBUG == 1          //If the read from memory is successful.
  else { //print out the files read from flash
    Serial.print("OSC address header: ");
    Serial.println(configuration.packet_header_string);
    Serial.print("AP SSID: ");
    Serial.println(configuration.my_ssid);
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
    //add any other debug outputs here, wrapped in a preprocessor #ifdef is_something directive
  }
  #endif
#endif
}

void setup() {
   // Neo Pixel Setup
  #ifdef is_neopixel
    pixels.begin(); // This initializes the NeoPixel library.
    pixels.show(); // Initialize all pixels to 'off'
  #endif
  //Initialize serial and wait for port to open:
#if DEBUG == 1
  Serial.begin(9600);
  while(!Serial) { }     //Ensure Serial is ready to go before anything happens in DEBUG mode.
#endif
  pinMode(led, OUTPUT);      // set the LED pin mode
#ifdef is_i2c
   #if DEBUG == 1
      Serial.println("starting i2c initialization");
   #endif
  // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    pinMode(INTERRUPT_PIN, INPUT);
  #if DEBUG == 1
    Serial.println("Using i2c");
  #endif
  // *** Init MPU 6050 and serial stuff
  #ifdef is_mpu6050
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
  #endif
  // ** end serial stuff
#endif
  init_config();

  
#ifdef transmit_butt
  pinMode(transmit_butt, INPUT_PULLUP);      // set the transmit_butt pin mode to input
#endif

//----------------------------------------------------
// -----              WiFi AP Setup              -----
//----------------------------------------------------
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);

  pinMode(led, OUTPUT);      // set the LED pin mode
  
  // check for the presence of the shield:
  
  if (WiFi.status() == WL_NO_SHIELD) {
#if DEBUG == 1
    Serial.println("WiFi shield not present, entering infinite loop");
#endif
    // don't continue
    while (true);
  }

  // by default the local IP address of will be 192.168.1.1
  // you can override it with the following:
  //WiFi.config(IPAddress(192, 168, 1, 10));
  switch(configuration.wifi_mode){
    case AP_MODE:
      start_AP();
    break;
    case WPA_CLIENT_MODE:
      if (connect_to_WPA(configuration.ssid,configuration.pass)){
        #if DEBUG == 1
        Serial.println("Success!");
        #endif
      }
      else {
        #if DEBUG == 1
        Serial.println("Failure :(");
        #endif
        while(true);
      }
    break;
  }
}

#ifdef is_mpu6050
void calMPU6050_OSC(OSCMessage &msg) {
#if DEBUG == 1
  Serial.println("Command received to calibrate MPU6050");
#endif
  calMPU6050();
  //flash_config.write(configuration); //uncomment when flash writing is enabled
#if DEBUG == 1
  Serial.println("New calibration values written to non-volatile memory");
#endif
}
#endif

char new_ssid[32];
char new_pass[32];
bool ssid_set;
bool pass_set;

void switch_to_AP(OSCMessage &msg){
  if (configuration.wifi_mode != AP_MODE){
  #if DEBUG == 1
    Serial.println("received command to switch to AP mode");
  #endif
    Udp.stop();
    WiFi.disconnect();
    WiFi.end();
    start_AP();
    configuration.wifi_mode = AP_MODE;
  }
  #if DEBUG == 1
  else{
    Serial.println("already in AP mode, no need to switch");
  }
  #endif
}

void set_instance_num(OSCMessage &msg){
  configuration.instance_number = msg.getInt(0);
  sprintf(configuration.packet_header_string,"%s%d",PacketHeaderString,configuration.instance_number);
  #if DEBUG == 1
  Serial.print("new address header: ");
  Serial.println(configuration.packet_header_string);
  #endif
}

void set_ssid(OSCMessage &msg){
  msg.getString(0,new_ssid,50);
  ssid_set = true;
}

void set_pass(OSCMessage &msg){
  msg.getString(0,new_pass,50);
  pass_set = true;
}

void msg_router(OSCMessage &msg, int addrOffset){
  #if DEBUG == 1
  char buffer[100];
  msg.getAddress(buffer,addrOffset);
  Serial.print("Parsed ");
  Serial.println(buffer);
  #endif
  #ifdef is_mpu6050
  msg.dispatch("/MPU6050/cal",calMPU6050_OSC,addrOffset);
  #endif
  #ifdef is_neopixel
  msg.dispatch("/Port0/Neopixel/Red",setRed);
  msg.dispatch("/Port0/Neopixel/Green",setGreen);
  msg.dispatch("/Port0/Neopixel/Blue",setBlue);
  #endif
  msg.dispatch("/Connect/SSID",set_ssid,addrOffset);
  msg.dispatch("/Connect/Password",set_pass,addrOffset);
  msg.dispatch("/wifiSetup/AP",switch_to_AP,addrOffset);
  msg.dispatch("/SetID",set_instance_num,addrOffset);
}

uint32_t button_timer;

void loop() {
  pass_set = ssid_set = false;
  OSCBundle bndl;
  char addressString[255];
  if ((uint32_t)digitalRead(transmit_butt)){
    button_timer = 0;
  }
  else{
    #ifdef is_sleep_period
    button_timer += is_sleep_period;
    #else
    button_timer++;
    #endif
    if (button_timer >= 8000){ //~about 8 seconds
    #if DEBUG == 1
      Serial.println("button held for 8 seconds, resetting to AP mode");
      
    #endif
      button_timer = 0;
      configuration.wifi_mode = AP_MODE;
      
      Udp.stop();
      WiFi.disconnect();
      WiFi.end();
      start_AP();
    }
  }
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
      for (int i = 0; i < 50; i++){ //We don't need to clear these buffers if there are no possible bundles
        new_ssid[i] = '\0';
        new_pass[i]= '\0';
      }
    
      bndl.route(configuration.packet_header_string,msg_router);
      /*#ifdef is_neopixel
      pixels.setPixelColor(0, pixels.Color((redVal > 255) ? 255 : redVal, (greenVal > 255) ? 255 : greenVal, (blueVal > 255) ? 255: blueVal));
      pixels.show();
      #endif*/
      
      if (ssid_set == true && pass_set == true){
        Serial.print("received command to connect to ");
        Serial.print(new_ssid);
        Serial.print(" with password ");
        Serial.println(new_pass);
        WiFi.disconnect();
        Udp.stop();
        WiFi.end();
        if(connect_to_WPA(new_ssid,new_pass)){
          configuration.wifi_mode = WPA_CLIENT_MODE;
          strcpy(configuration.ssid,new_ssid);
          strcpy(configuration.pass,new_pass);
        }
      }
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

// measure battery voltage
  vbat = analogRead(VBATPIN);
  vbat *= 2;    // we divided by 2, so multiply back
  vbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  vbat /= 1024; // convert to voltage
#ifdef SEND_OSC
  #ifdef is_mpu6050
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
