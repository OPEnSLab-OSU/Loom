/*
  Connect to a wifi network
  Get MPU6050 DMP data
  Transmit OSC formatted data stream over UDP to other connected devices

  Like the transmit UDP MPU6050 example, but only sends data upon request from a host computer

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
#include <Adafruit_SleepyDog.h> // Include this if transmitting at timed intervals (use this one)
//#include "LowPower.h" // Include this if going to sleep forever and woken up by external interrupt (don't use)

//------------------------------------------------------------------------------------------------------
// DEBUG MODE: Set to 1 if you want to see serial printouts, else, set to 0 for field use to save memory
//------------------------------------------------------------------------------------------------------
#ifndef DEBUG
#define DEBUG 0
#endif


#define OUTPUT_READABLE 1 // set to 1 for debug messages print to serial window

// Define your sensor type here by commenting and un-commenting
#define is_analog 3      // also define number of channels
#define is_i2c 0x68    // also define i2c address of device
#define VBATPIN A7

int led =  LED_BUILTIN;
volatile bool ledState = LOW;
char ssid[] = "OPEnS"; // created AP name
char pass[] = "Replace_with_your_wifi_password";      // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
char ip_broadcast[] = "192.168.1.255"; // IP to Broadcast data 
byte mac[6]; // place to save and recall this devices MAC address
IPAddress ip; // place to save and recall IP address
int status = WL_IDLE_STATUS;     // the WiFi radio's status

unsigned int localPort = 9436;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet

int transmitMS = 1000; // milliseconds period to transmit data
float vbat = 3.3;    // Place to save measured battery voltage
//char packetBuffer[255]; //buffer to hold incoming packet
//char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

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
//  #define OUTPUT_READABLE_WORLDACCEL  // this is pretty cool
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
#endif
// ** End if i2c check

void setup() {

pinMode(led, OUTPUT);      // set the LED pin mode

#ifdef OUTPUT_READABLE
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
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
  Serial.println("Using MPU6050 i2c");
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
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);
#if DEBUG == 1
        // Uncomment following 2 lines if using enable Arduino Uno, MO, or Trinket interrupt detection
        Serial.println(F("Enabling MPU interrupt detection (Feather pin 11)..."));
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
//----------------------------------------------------
// -----              WiFi Setup              -----
//----------------------------------------------------
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);

#if DEBUG == 1
  Serial.println("Transmit UDP messages to a wireless router, WPA secured");
#endif

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
#if DEBUG == 1
    Serial.println("WiFi shield not present");
#endif
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
#if DEBUG == 1
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
#endif
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 1 seconds for connection:
    delay(1000);
  }
 
#if DEBUG == 1
  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWiFiData();
#endif

  WiFi.lowPowerMode(); // Turn on low-power functions, May need to move this into the end of the loop transmission?
  //WiFi.setSleepMode(M2M_PS_MANUAL, 1); // This function does not work, haven't figured it out yet.
  
#if DEBUG == 1
  Serial.println("\nStarting UDP connection over server...");
#endif
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}


void loop() {
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


// measure battery voltage
  vbat = analogRead(VBATPIN);
  vbat *= 2;    // we divided by 2, so multiply back
  vbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  vbat /= 1024; // convert to voltage
   
#ifdef is_i2c
    // flush MPU6050 FIFO to avoid overflows if using i2c
    mpu.resetFIFO();
    // Update MPU6050 Data
    // Now measure MPU6050, update values in global registers
    measure_mpu6050();

    udp_mpu6050();
    
    // flush MPU6050 FIFO to avoid overflows if using i2c
    mpu.resetFIFO();

#endif
    
#ifdef is_analog
    measure_analog();

/*
    // send a reply, to the IP address and port that sent us the packet we received
    //the message wants an OSC address as first argument
    OSCMessage msg("/analog/0");
    msg.add((int32_t)analogRead(0));
    Udp.beginPacket("192.168.1.3", 9436);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
*/
#endif
  
  // Watchdog.sleep(transmitMS); // sleep MCU for transmit period duration
  }
  
}

void printWiFiData() {
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

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}


