
/*
Make an OSC bundle and send it over UDP

OSCBundles allow OSCMessages to be grouped together
to  preserve the order and completeness of related messages.
They also allow for timetags to be carried to represent the presentation time
of the messages.
 */
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <SPI.h>    
#include <OSCBundle.h>

int status = WL_IDLE_STATUS;
char ssid[] = "WiPhi";
char pass[] = "fourwordsalluppercase";
int keyIndex = 0;

WiFiUDP Udp;

//the Arduino's IP
IPAddress ip(192, 168, 1, 1);

//destination IP
IPAddress outIp(192, 168, 1, 2);
const unsigned int outPort = 9999;

 byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // you can find this written on the board of some Arduino Ethernets or shields

void setup() {
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);
 
  delay(2000);
  
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(outPort);
}



void loop(){
  //declare the bundle
    OSCBundle bndl;

    //BOSCBundle's add' returns the OSCMessage so the message's 'add' can be composed together
    bndl.add("/analog/0").add((int32_t)analogRead(0));
    bndl.add("/analog/1").add((int32_t)analogRead(1));
    bndl.add("/digital/5").add((digitalRead(5)==HIGH)?"HIGH":"LOW");

    Udp.beginPacket(outIp, outPort);
        bndl.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    bndl.empty(); // empty the bundle to free room for a new one

    bndl.add("/mouse/step").add((int32_t)analogRead(0)).add((int32_t)analogRead(1));
    bndl.add("/units").add("pixels");

    Udp.beginPacket(outIp, outPort);
        bndl.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
    bndl.empty(); // empty the bundle to free room for a new one

    delay(1000);
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
}
