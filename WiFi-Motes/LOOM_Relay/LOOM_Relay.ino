#include <Wire.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <OSCBundle.h>

// WiFi Stuff here:
int status = WL_IDLE_STATUS;
char ssid[] = "relay-host"; //  your network SSID (name)
char pass[] = "1234567890";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
unsigned int localPort = 9436;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

#define RELAY_PIN1 9
#define RELAY_PIN2 10
bool relay_on_1;
bool relay_on_2;
OSCErrorCode error;

WiFiUDP Udp;
void setup() {
  pinMode(RELAY_PIN1,OUTPUT);
  pinMode(RELAY_PIN2,OUTPUT);
  relay_on_1 = false;
  relay_on_2 = false;
  WiFi.setPins(8,7,4,2);
  Serial.begin(9600);
  
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }
  status = WiFi.beginAP(ssid);
  if (status != WL_AP_LISTENING) {
    Serial.println("Creating access point failed");
    // don't continue
    while (true);
  }
  delay(10000);

  // you're connected now, so print out the status
  printWiFiStatus();


  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}

void handleRelay(OSCMessage &msg, int addrOffset){
  int set_degree;
  set_degree = msg.getInt(0);
  if (msg.fullMatch("/Relay0/SwitchOn",addrOffset)){
      relay_on_1 = true;
      Serial.println("turn on 0");
  }
  else if (msg.fullMatch("/Relay0/SwitchOff",addrOffset)){
      relay_on_1 = false;
      Serial.println("turn off 0");
  }
  else if (msg.fullMatch("/Relay1/SwitchOn",addrOffset)){
      relay_on_2 = true;
      Serial.println("turn on 1");
  }
  else if (msg.fullMatch("/Relay1/SwitchOff",addrOffset)){
      relay_on_2 = false;
      Serial.println("turn off 0");
  }
}

void loop() {
  OSCBundle bndl;

  int packetSize = Udp.parsePacket();
  if (packetSize > 0){
    Serial.println("=========================================");
    Serial.print("received packet of size: ");
    Serial.println(packetSize);
    while (packetSize--){
      bndl.fill(Udp.read());
    }
    if (!bndl.hasError()){
      bndl.route("/LOOM/RelayShield0",handleRelay);
    }
    else {
      error = bndl.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
  digitalWrite(RELAY_PIN1,(relay_on_1==true) ? HIGH : LOW);
  digitalWrite(RELAY_PIN2,(relay_on_2==true) ? HIGH : LOW);
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

