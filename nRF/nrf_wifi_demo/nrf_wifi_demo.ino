
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <OSCBundle.h>

#define FAMILY "LOOM"
#define DEVICE "nrfHub"
#define INSTANCE_NUM 0
#define STR_(x) #x
#define STR(x) STR_(x) //to concatenate a predefined number to a string literal, use STR(x)

// C interprets subsequent string literals as concatenation: "/Loom" "/Ishield" "0" becomes "/Loom/Ishield0"
#define IDString FAMILY DEVICE STR(INSTANCE_NUM) 

//Delimits the FAMILY, DEVICE, and INSTANCE_NUM with '/' for use in OSC Bundles
#define PacketHeaderString STR(/) FAMILY STR(/) DEVICE STR(INSTANCE_NUM) STR()

RF24 radio(5,6);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio
const uint16_t this_node = 01;    // Address of our node in Octal format ( 04,031, etc)
const uint16_t other_node = 00;   // Address of the other node in Octal format

char message[300];

// Wifi Initializations

char ssid[] = IDString; // created AP name
char pass[] = "1234567890";      // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
int keyIndex = 0;                // your network key Index number (needed only for WEP)
char ip_broadcast[] = "192.168.1.100"; // IP to Broadcast data 
unsigned int localPort = 9436;      // local port to listen on

byte mac[6]; // place to save and recall this devices MAC address
IPAddress ip; // place to save and recall IP addressse

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

WiFiUDP Udp;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

int led =  LED_BUILTIN;
volatile bool ledState = LOW;


void setup(void)
{
  Serial.begin(57600);
  Serial.println("RF24Network/examples/helloworld_rx/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  //----------------------------------------------------
// -----              WiFi AP Setup              -----
//----------------------------------------------------
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
}

void loop(void){
  
  network.update();                  // Check the network regularly

  
  while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out
    memset(message, '\0', 300);
    network.read(header,&message,299);
 /*   Serial.print("Received packet #");
    Serial.print(payload.counter);
    Serial.print(" at ");
    Serial.println(payload.ms);
    Serial.print("Message: ");
    Serial.print(payload.message); */
    Serial.print("Message size: ");
    Serial.println(strlen(message));

    OSCBundle bndl;
    get_OSC_bundle(message, &bndl);

    
    Serial.println("Received Transmission");

    //bndl.add("addr").add("IDtag").add("six").add("VWC").add((float)1.0).add("Temp").add((int32_t)-1).add("ElecCond").add((float)3.0);

    //print_bundle(&bndl);

    Udp.beginPacket(ip_broadcast, 9436);
    bndl.send(Udp);
    Udp.endPacket();
  
    bndl.empty();
  }
}

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
