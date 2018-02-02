#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>
#include "LOOM_OSC_Scheme.h"

//Code used to return error information
OSCErrorCode error;

// Define your sensor type here by commenting and un-commenting
#define transmit_butt 10        // using on-board button, specify attached pin, transmitting 
#define VBATPIN A7              // Pin to check for battery voltage

#define TRANSMISSION_PERIOD 6000     //The number of milliseconds between when the decagon finishes transmitting
                                     // and starts the reading again   
#define FAMILY "/LOOM"
#define DEVICE "/Decagon"
#define INSTANCE_NUM 0  // Unique instance number for this device, useful when using more than one of the same device type in same space

// Set Sleep Mode Use one or the other or neither of the following 2 lines
#define is_sleep_period 50  // Uncomment to use SleepyDog to transmit at intervals up to 16s and sleep in between
//#define is_sleep_interrupt 11  // Uncoment to use Low-Power library to sit in idle sleep until woken by pin interrupt, parameter is pin to interrupt

#ifdef is_sleep_period
  #include <Adafruit_SleepyDog.h> // Include this if transmitting at timed intervals (use this one)
#endif
const byte flashValidationValue = 99; // Value to test to see if flashMem has been written before

int led =  LED_BUILTIN;
volatile bool ledState = LOW;

float vbat = 3.3;    // Place to save measured battery voltage

char ssid[] = "wifi101-network"; // created AP name
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

// ================================================================
// ===         Decagon Inclusions               ===
// ================================================================

#define is_decagon

#ifdef is_decagon
float dielec_p = 0;
float temp = 0;
float elec_c = 0;
#endif

// ================================================================
// ===         Thermocouple Inclusions         ===
// ================================================================

void setup() {
  Serial.begin(9600);
#ifdef is_decagon
  deca_gs3_setup();
#endif

  //Initialize serial and wait for port to open:
  pinMode(led, OUTPUT);      // set the LED pin mode
#ifdef transmit_butt
  pinMode(transmit_butt, INPUT_PULLUP);      // set the transmit_butt pin mode to input
#endif

//----------------------------------------------------
// -----              WiFi AP Setup              -----
//----------------------------------------------------
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);

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

void setTrans_OSC(OSCMessage &msg, int addrOffset) {
  Serial.println("setTrans_OSC called");
}

void loop() {
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
      #ifdef is_i2c
        #if DEBUG == 1
          Serial.print("Number of items in bundle: ");
          Serial.println(bndl.size());
          Serial.print("First message address string: ");
          bndl.getOSCMessage(0)->getAddress(addressString, 0);
          Serial.println(addressString);
        #endif
        bndl.route(PacketHeaderString "/setTrans", setTrans_OSC); 
      #endif
    }
    else {
      error = bndl.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }

// measure battery voltage
  vbat = analogRead(VBATPIN);
  vbat *= 2;    // we divided by 2, so multiply back
  vbat *= 3.3;  // Multiply by 3.3V, our reference voltage
  vbat /= 1024; // convert to voltage

#ifdef is_decagon
    measure_decagon();

    udp_decagon();

    delay((uint32_t)TRANSMISSION_PERIOD);
#endif

#ifdef is_sleep_period
  int sleepMS = Watchdog.sleep(is_sleep_period); // sleep MCU for transmit period duration
#endif  
  // delay(is_sleep_period); // demo transmit every 1 second

} // End loop section

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
