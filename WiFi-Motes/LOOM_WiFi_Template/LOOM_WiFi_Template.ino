/*
  January 19, 2018. Author: Trevor Swope
  This is the LOOM WiFi Mote template! Read this big 'ol comment for instructions on how to use the preprocessor statements in here to define the functionality of your board.
 
  DEBUG MODE: define DEBUG as 1 or 0. If DEBUG is set, make sure the serial monitor is open; all serial calls should be wrapped in an #if DEBUG == 1 ... #endif

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
#include "config.h"
//--PREPROCESSOR STATEMENTS BEGIN HERE
#include <OSCBundle.h>





#ifdef is_servo
  #include <Adafruit_PWMServoDriver.h>
  Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
  #define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
  #define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
  
  #if (num_servos==1)
    int predeg[1] = {0};
    double pre_pulselength[1]= {0.0};
  #elif (num_servos==2)
    int predeg[2] = {0, 0};
    double pre_pulselength[2] = {0.0,0.0};
  #elif (num_servos==3)
    int predeg[3] = {0, 0, 0};
    double pre_pulselength[3] = {0.0,0.0,0.0};
  #elif (num_servos==4)
    int predeg[4] = {0, 0, 0, 0};
    double pre_pulselength[4] = {0.0,0.0,0.0,0.0};
  #elif (num_servos==5)
    int predeg[5] = {0, 0, 0, 0, 0};
    double pre_pulselength[5] = {0.0,0.0,0.0,0.0,0.0};
  #elif (num_servos==6)
    int predeg[6] = {0, 0, 0, 0, 0, 0};
    double pre_pulselength[6] = {0.0,0.0,0.0,0.0,0.0,0.0};
  #elif (num_servos==7)
    int predeg[7] = {0, 0, 0, 0, 0, 0, 0};
    double pre_pulselength[7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  #elif (num_servos==8)
    int predeg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    double pre_pulselength[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
  #endif
  
  void set_servo_degree(int set_degree, int servo_choice){
    uint16_t pulselength = map(set_degree, 0, 180, SERVOMIN, SERVOMAX);
    if(set_degree < predeg[servo_choice]){
      for (double pulselen = pre_pulselength[servo_choice]; pulselen >= pulselength; pulselen--) {
        pwm.setPWM(servo_choice, 0, pulselen);
      }
    }
    //When input degree is greater than previous degree, it increases
    if(set_degree > predeg[servo_choice]){
      for (double pulselen = pre_pulselength[servo_choice]; pulselen < pulselength; pulselen++) {
        pwm.setPWM(servo_choice, 0, pulselen);
      }
    }
    predeg[servo_choice] = set_degree;
    pre_pulselength[servo_choice] = pulselength;
  }
  
  void set_servo(OSCMessage &msg){
    int servo_num = msg.getInt(0);
    int set_degree = msg.getInt(1);
  
    #if DEBUG == 1
    Serial.print("received message for servo ");
    Serial.print(servo_num);
    Serial.print(" with degree ");
    Serial.println(set_degree);
    #endif
    set_servo_degree(set_degree,servo_num); 
  }

#endif //END OF SERVO FUNCTIONS AND DECLARATIONS

#ifdef is_relay
  #define RELAY_PIN0 9
  #define RELAY_PIN1 10
  #define RELAY_PIN2 11
  #define RELAY_PIN3 14 //A0
  #define RELAY_PIN4 15 //A1
  #define RELAY_PIN5 16 //A2
  bool relay_on[6];
  void handleRelay(OSCMessage &msg){
    int relay;
    int set_to;
    relay = msg.getInt(0);
    set_to = msg.getInt(1);
    relay_on[relay] = (set_to==1);
    Serial.print("set ");
    Serial.print(relay);
    Serial.print(" to ");
    Serial.println((relay_on[relay]) ? "ON" : "OFF");
    
  }
#endif //END OF RELAY FUNCTIONS AND DECLARATIONS

#ifdef is_neopixel
  #include <Adafruit_NeoPixel.h>

  Adafruit_NeoPixel * pixels[3];
  bool pixel_enabled[3] = {NEO_0, NEO_1, NEO_2};

  int colorVals[3][3] = { {0,0,0},
                          {0,0,0},
                          {0,0,0} };
  //port x (r,g,b)
  
  void setColor(OSCMessage &msg){
    int port     = msg.getInt(0);
    int pixelNum = msg.getInt(1);
    int color    = msg.getInt(2);
    int val      = msg.getInt(3);
  
    #if DEBUG == 1
    Serial.println("========");
    Serial.print("Port: ");
    Serial.print(port);
    Serial.print("  Num: ");
    Serial.print(pixelNum);
    Serial.print("  Color: ");
    Serial.print(color);
    Serial.print("  Val: ");
    Serial.println(val);
    Serial.println("========\n");
    #endif

  if (pixel_enabled[port])
    colorVals[port][color] = val;

    if (pixel_enabled[port])
      pixels[port]->setPixelColor(pixelNum, pixels[port]->Color(colorVals[port][0], colorVals[port][1], colorVals[port][2]));


    for(int i = 0; i < 3; i++) {
      if (pixel_enabled[i])
        pixels[i]->show();
    }
  
   
  }
#endif //END OF NEOPIXEL FUNCTIONS


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
  #endif //END OF MPU6050 FUNCTIONS AND DECLARATIONS
#endif //END OF I2C FUNCTIONS AND DECLARATIONS

#ifdef is_sleep_period
  #include <Adafruit_SleepyDog.h> // This must be included if you are transmitting at timed intervals
#endif

#ifdef is_sleep_interrupt
  #include <LowPower.h> //Include this if transmitting on pin interrupt
#endif

#define STR_(x) #x //helper function
#define STR(x) STR_(x) //to concatenate a predefined number to a string literal, use STR(x)
#define PacketHeaderString STR(/) FAMILY STR(/) DEVICE //results in a single string, i.e. /LOOM/Device. the full prefix sent to this device should be /LOOM/Device#, but the number is parsed in the OSC bundle routing function
#include <OSCBundle.h> // Use this to handle all OSC messaging behavior (sending or receiving)
OSCErrorCode error;

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
        Serial.println("Reverting to AP mode");
        #endif

        // Start AP up again
        Udp.stop();
        WiFi.disconnect();
        WiFi.end();
        start_AP();
        configuration.wifi_mode = AP_MODE;
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
        configuration.instance_number = INIT_INST;
        sprintf(configuration.packet_header_string,"%s%d\0",PacketHeaderString,configuration.instance_number);
        #if DEBUG == 1
        Serial.print("expecting OSC header ");
        Serial.println(configuration.packet_header_string);
        #endif
        configuration.my_ssid = AP_NAME; //default AP name
//        sprintf(configuration.my_ssid,"featherM0-%d",configuration.instance_number);
        strcpy(configuration.ssid,DEFAULT_NETWORK);               // created AP name
        strcpy(configuration.pass,DEFAULT_PASSWORD);                // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
        configuration.keyIndex = 0;                       // your network key Index number (needed only for WEP)
        configuration.ip_broadcast = "192.168.1.255";     // IP to Broadcast data 
        configuration.localPort = INIT_PORT;                   // local port to listen on
        configuration.wifi_mode = DEFAULT_MODE;
        //add any other behavior/calibration wrapped in an #ifdef is_Something preprocessor directive HERE
      #ifdef is_mpu6050
        calMPU6050(); //calibration writes memValidationValue for us
      #else
        configuration.checksum = memValidationValue;      // configuration has been written successfully, so we write the checksum
      #endif
  #if DEBUG == 1
        Serial.println("Writing to flash for the first time.");
  #endif
        flash_config.write(configuration);                //don't uncomment this line until we're pretty confident that this behaves how we want; flash memory has limited writes and we don't want to waste it on unnecessary tests
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
    for(int i = 0; i < 3; i++) {
      if (pixel_enabled[i]){
        pixels[i] = new Adafruit_NeoPixel(1, 14+i, NEO_GRB + NEO_KHZ800);
        pixels[i]->begin(); // This initializes the NeoPixel library.
        pixels[i]->show(); // Initialize all pixels to 'off'
      }
    }

  #endif

  #ifdef is_servo
    pwm.begin();
  
    pwm.setPWMFreq(60);
  #endif

  #ifdef is_relay
    pinMode(RELAY_PIN0,OUTPUT);
    pinMode(RELAY_PIN1,OUTPUT);
    pinMode(RELAY_PIN2,OUTPUT);
    pinMode(RELAY_PIN3,OUTPUT);
    pinMode(RELAY_PIN4,OUTPUT);
    pinMode(RELAY_PIN5,OUTPUT);
    relay_on[0] = false;
    relay_on[1] = false;
    relay_on[2] = false;
    relay_on[3] = false;
    relay_on[4] = false;
    relay_on[5] = false;
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
  configuration.ip = WiFi.localIP();
}

#ifdef is_mpu6050
void calMPU6050_OSC(OSCMessage &msg) {
#if DEBUG == 1
  Serial.println("Command received to calibrate MPU6050");
#endif
  calMPU6050();
  flash_config.write(configuration); //uncomment when flash writing is enabled
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
    flash_config.write(configuration);
  }
  #if DEBUG == 1
  else{
    Serial.println("already in AP mode, no need to switch");
  }
  #endif
}

void set_instance_num(OSCMessage &msg){
  configuration.instance_number = msg.getInt(0);
  sprintf(configuration.packet_header_string,"%s%d\0",PacketHeaderString,configuration.instance_number);
  #if DEBUG == 1
  Serial.print("new address header: ");
  Serial.println(configuration.packet_header_string);
  #endif
  flash_config.write(configuration);
}

void set_port(OSCMessage &msg){
  #if DEBUG == 1
  Serial.print("Port changed from ");
  Serial.print(configuration.localPort);
  #endif
  
  configuration.localPort = msg.getInt(0);
  Udp.stop();
//  delay(500);
  Udp.begin(configuration.localPort);
  
  #if DEBUG == 1
  Serial.print(" to ");
  Serial.println(configuration.localPort);
  #endif
  flash_config.write(configuration);
}

void set_ssid(OSCMessage &msg){
  msg.getString(0,new_ssid,50);
  ssid_set = true;
}

void set_pass(OSCMessage &msg){
  msg.getString(0,new_pass,50);
  pass_set = true;
}

void broadcastIP(OSCMessage &msg){
  configuration.ip = WiFi.localIP();
  char addressString[255];

  sprintf(addressString,"%s%s",configuration.packet_header_string,"/NewIP");
  
  OSCBundle bndl;
  bndl.add(addressString).add((int32_t)configuration.ip[0]).add((int32_t)configuration.ip[1]).add((int32_t)configuration.ip[2]).add((int32_t)configuration.ip[3]);


  Udp.beginPacket(configuration.ip_broadcast, configuration.localPort);
  bndl.send(Udp);  // send the bytes to the SLIP stream
  Udp.endPacket(); // mark the end of the OSC Packet
  bndl.empty();
 
  // empty the bundle to free room for a new one
  bndl.empty(); 
  
  #if DEBUG == 1
  Serial.print("Broadcasted IP: ");
  Serial.println(configuration.ip);
  #endif
  
}

void msg_router(OSCMessage &msg, int addrOffset){
  #if DEBUG == 1
  char buffer[100];
  msg.getAddress(buffer,addrOffset);
  Serial.print("Parsed ");
  Serial.println(buffer);
  #endif
  #ifdef is_servo
  msg.dispatch("/Servo/Set",set_servo,addrOffset);
  #endif
  #ifdef is_relay
  msg.dispatch("/Relay/State", handleRelay, addrOffset);
  #endif 
  #ifdef is_mpu6050
  msg.dispatch("/MPU6050/cal",calMPU6050_OSC,addrOffset);
  #endif
  #ifdef is_neopixel
  msg.dispatch("/Neopixel",setColor,addrOffset);
  #endif
  
  msg.dispatch("/Connect/SSID",set_ssid,addrOffset);
  msg.dispatch("/Connect/Password",set_pass,addrOffset);
  msg.dispatch("/wifiSetup/AP",switch_to_AP,addrOffset);
  msg.dispatch("/SetID",set_instance_num,addrOffset);
  msg.dispatch("/SetPort",set_port,addrOffset);
  msg.dispatch("/requestIP",broadcastIP,addrOffset);
}

uint32_t button_timer;

void loop() {  
  pass_set = ssid_set = false;
  OSCBundle bndl;
  char addressString[255];
  #ifdef transmit_butt
  if ((uint32_t)digitalRead(transmit_butt)){
    button_timer = 0;
  }
  else{
    #ifdef is_sleep_period
    button_timer += is_sleep_period;
    #else
    button_timer++;
    #endif
    if (button_timer >= 5000){ //~about 5 seconds
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
  #endif
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize > 0)
  {
    
    #if DEBUG == 1
    Serial.println("=========================================");
    Serial.print("received packet of size: ");
    Serial.println(packetSize);
    #endif
    bndl.empty();
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
      for (int i = 0; i < 32; i++){ //We don't need to clear these buffers if there are no possible bundles
        new_ssid[i] = '\0';
        new_pass[i]= '\0';
      }
      
      
      bndl.route(configuration.packet_header_string,msg_router);
      #ifdef is_relay
          //digitalWrite(RELAY_PIN0,(relay_on[0]==true) ? HIGH : LOW); //NOTE: pin 9 is bad news
          digitalWrite(RELAY_PIN1,(relay_on[1]==true) ? HIGH : LOW);
          digitalWrite(RELAY_PIN2,(relay_on[2]==true) ? HIGH : LOW);
          digitalWrite(RELAY_PIN3,(relay_on[3]==true) ? HIGH : LOW);
          digitalWrite(RELAY_PIN4,(relay_on[4]==true) ? HIGH : LOW);
          digitalWrite(RELAY_PIN5,(relay_on[5]==true) ? HIGH : LOW);
      #endif
      if (ssid_set == true && pass_set == true){
        // Replace '~'s with spaces - as spaces cannot be sent via Max and are replaced with '~'
        int i = 0;
        while (new_ssid[i] != '\0'){
          if (new_ssid[i] == '~')
            new_ssid[i] = ' ';
          i++;
        }
        i = 0;
        while (new_pass[i] != '\0'){
          if (new_pass[i] == '~')
            new_pass[i] = ' ';
          i++;
        }
        
        #if DEBUG == 1
        Serial.print("received command to connect to ");
        Serial.print(new_ssid);
        Serial.print(" with password ");
        Serial.println(new_pass);
        #endif


        
        WiFi.disconnect();
        Udp.stop();
        WiFi.end();
        if(connect_to_WPA(new_ssid,new_pass)){
          configuration.wifi_mode = WPA_CLIENT_MODE;
          configuration.ip = WiFi.localIP();
          strcpy(configuration.ssid,new_ssid);
          strcpy(configuration.pass,new_pass);
          flash_config.write(configuration);
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
