/*
  January 19, 2018. Author: Trevor Swope
  This is the LOOM WiFi Mote template! Read this big 'ol comment for instructions on how to use the preprocessor statements in here to define the functionality of your board.
 
  DEBUG MODE: define DEBUG as 1 or 0. If DEBUG is set, make sure the serial monitor is open; all serial calls should be wrapped in an #if DEBUG == 1 ... #endif

  SEND_OSC: if the device is going to be sending readings from anything attached to it, set this to true
  RECEIVE_OSC: if the device is going to receive any commands from the hub, set this to true
  
 */

#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>

//--PREPROCESSOR STATEMENTS BEGIN HERE

#ifndef DEBUG
#define DEBUG 1 //set to 1 if you want Serial statements from various functions to print
#endif

#define FAMILY "/LOOM"    // Should always be "/LOOM", you can change this if you are setting up your own network
#define DEVICE "/Device"  // The device name, should begin with a slash followed by an unbroken string with no more slashes i.e. "/RelayShield" or "/IShield"
#define INSTANCE_NUM 0    // Unique instance number for this device, useful when using more than one of the same device type in same space

#define SEND_OSC          // Comment this out to turn off sending of OSC messages
#define RECEIVE_OSC       // Comment this out to turn off receiving of OSC messages

#ifdef SEND_OSC
  #define is_sleep_period 50 // Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between. Change the value according to the length of your desired transmission interval
  #define is_sleep_interrupt 11 // Uncomment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt
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
  #define IDString FAMILY DEVICE STR(INSTANCE_NUM) //results in a single string, i.e. /LOOM/Device0
  #include <OSCBundle.h> // Use this to handle all OSC messaging behavior (sending or receiving)
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
//#define is_analog 2      // also define number of channels
//#define is_i2c 0x68      // also define i2c address of device
#define transmit_butt 10        // using on-board button, specify attached pin, transmitting 
#define VBATPIN A7       // Pin to check for battery voltage
//#define CLIENT_REQUESTS_DATA 1 // Set to 1 if you only send data when requested by client, else, send data at sample/sleep rate
//------------------------------------------------------------------------------------------------------
//OSC identification: convention is FAMILY = "/LOOM", DEVICE is something like "/IShield" (whatever the name of the device is), and INSTANCE_NUM is a number
//------------------------------------------------------------------------------------------------------


// Set Sleep Mode Use one or the other or neither of the following 2 lines
#define is_sleep_period 50  // Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between
//#define is_sleep_interrupt 11  // Uncomment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt

#ifdef is_sleep_period
  #include <Adafruit_SleepyDog.h> // Include this if transmitting at timed intervals (use this one)
#endif

#ifdef is_sleep_interrupt
  #include <LowPower.h> //Include this if transmitting on pin interrupt
#endif

struct config_t {
  byte checksum;               //value is changed when flash memory is written to.
  IPAddress ip;                //Device's IP Address
  char* ssid;                //Created AP's name
  char* pass;                //AP Password (10 or 26 characters long)
  int   keyIndex;            //Key Index Number (needed only for WEP)
  char* ip_broadcast;        //IP to Broadcast data
  unsigned int localPort;      //Local port to listen on
  byte  mac[6];                 //Device's MAC Address
  //add any other stuff that needs to be stored based on the shields with a wrapped preprocessor statement
};

struct config_t configuration;

#if MEM_TYPE == MEM_FLASH
FlashStorage(flash_config,config_t);    //Setup the flash storage for the structure
#endif

const byte flashValidationValue = 99; // Value to test to see if flashMem has been written before

int led =  LED_BUILTIN;
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
  //i2c initialization here
#endif

void setup() {
  //Initialize serial and wait for port to open:
#if DEBUG == 1
  Serial.begin(9600);
  while(!Serial) { }     //Ensure Serial is ready to go before anything happens.
#endif
  pinMode(led, OUTPUT);      // set the LED pin mode

#if MEM_TYPE == MEM_FLASH
  #if DEBUG == 1
    Serial.println("Reading from flash.");
  #endif
  configuration = flash_config.read();                    // read from flash memory
  #if DEBUG == 1
    Serial.print("Checksum: ");
    Serial.println(configuration.checksum);
  #endif
  if (configuration.checksum != flashValidationValue){    //
    configuration.ssid = "wifi101-network";               // created AP name
        configuration.pass = "1234567890";                // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
        configuration.keyIndex = 0;                       // your network key Index number (needed only for WEP)
        configuration.ip_broadcast = "192.168.1.255";     // IP to Broadcast data 
        configuration.localPort = 9436;                   // local port to listen on
        configuration.checksum = flashValidationValue;    // configuration has been written successfully, so we write the checksum
        //add any other behavior/calibration wrapped in an #ifdef is_Something preprocessor directive
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
  //add any other debug outputs here, wrapped in a preprocessor #ifdef is_something directive
  }
#endif
  
#ifdef transmit_butt
  pinMode(transmit_butt, INPUT_PULLUP);      // set the transmit_butt pin mode to input
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
  Serial.print("Creating access point named: ");
  Serial.println(configuration.ssid);
#endif

  // Create open network. Change this line if you want to create an WEP network:
  status = WiFi.beginAP(configuration.ssid);
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


void loop() {
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
    //measure and send i2c stuff here
#endif
    
#ifdef is_analog
    //measure and send analog stuff here
    //measure_analog();
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
