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

#define is_i2c = 0x68    // also define i2c address of device

int led =  LED_BUILTIN;

char ssid[] = "wifi101-network"; // created AP name
char pass[] = "1234567890";      // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
int keyIndex = 0;                // your network key Index number (needed only for WEP)

unsigned int localPort = 9436;      // local port to listen on

char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

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
#endif
// ** End if i2c check

void setup() {
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

        // Uncomment following 2 lines if using enable Arduino Uno, MO, or Trinket interrupt detection
        Serial.println(F("Enabling MPU interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);

        // Uncomment following 2 lines if using Adafruit Feather 32u4
        // enable interrupt for PCINT7...
        //Serial.println(F("Enabling MPU interrupt detection PCINT 7 (pin 11)"));
        //pciSetup(INTERRUPT_PIN);
        
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

// ** end serial stuff
 #endif 
//----------------------------------------------------
// -----              WiFi AP Setup              -----
//----------------------------------------------------
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);

  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  Serial.println("Access Point Web Server");

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
}


void loop() {
  // compare the previous status to the current status
  if (status != WiFi.status()) {
    // it has changed update the variable
    status = WiFi.status();

    if (status == WL_AP_CONNECTED) {
      byte remoteMac[6];

      // a device has connected to the AP
      Serial.print("Device connected to AP, MAC address: ");
      WiFi.APClientMacAddress(remoteMac);
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
    } else {
      // a device has disconnected from the AP, and we are back in listening mode
      Serial.println("Device disconnected from AP");
    }
  }

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) packetBuffer[len] = 0;
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    
#ifdef is_i2c
    // Update MPU6050 Data
    // Now measure MPU6050, update values in global registers
    measure_mpu6050();

    udp_mpu6050();
    
    // flush MPU6050 FIFO to avoid overflows if using i2c
    mpu.resetFIFO();

#endif
    
#ifdef is_analog
    // send a reply, to the IP address and port that sent us the packet we received
    //the message wants an OSC address as first argument
    OSCMessage msg("/analog/0");
    msg.add((int32_t)analogRead(0));
    Udp.beginPacket("192.168.1.100", 9436);
    msg.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    msg.empty(); // free space occupied by message
#endif
    
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);

}
