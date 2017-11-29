/*
  WiFi UDP Send and Receive String

 This sketch wait an UDP packet on localPort using a WiFi shield.
 When a packet is received an Acknowledge packet is sent to the client on port remotePort

 Circuit:
 * WiFi shield attached

 created 30 December 2012
 by dlf (Metodo2 srl)

 */


#include <SPI.h>
#include <WiFi101.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>
#include <OSCBundle.h>


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN            16

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


int led =  LED_BUILTIN;


int status = WL_IDLE_STATUS;
char ssid[] = "feather"; //  your network SSID (name)
char pass[] = "1234567890";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
unsigned int localPort = 9436;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

OSCErrorCode error;

WiFiUDP Udp;
//WiFiServer server(80);




void setup() {
  // Neo Pixel Setup
  pixels.begin(); // This initializes the NeoPixel library.
  pixels.show(); // Initialize all pixels to 'off'
  
  //Configure pins for Adafruit ATWINC1500 Feather
  WiFi.setPins(8,7,4,2);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
//  while ( status != WL_CONNECTED) {
//    Serial.print("Attempting to connect to SSID: ");
//    Serial.println(ssid);
//    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
//    status = WiFi.begin(ssid, pass);
//
//    // wait 10 seconds for connection:
//    delay(10000);
//  }
//  Serial.println("Connected to wifi");
//  printWiFiStatus();

 // by default the local IP address of will be 192.168.1.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));


//----------------
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
//--------------

  // wait 10 seconds for connection:
  delay(10000);

  // start the web server on port 80
//  server.begin();

  // you're connected now, so print out the status
  printWiFiStatus();


  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}
int redVal = - 1, greenVal = -1, blueVal = -1;
void loop() {
  OSCMessage bndl;
  
 
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize > 0)
  {
    Serial.println("=========================================");
    while (packetSize--){
      bndl.fill(Udp.read());
    }
    if (!bndl.hasError()){
      Serial.print("messages in bundle: ");
      Serial.println(bndl.size());
      if (bndl.fullMatch("/LOOM/Ishield0/Port0/Neopixel/Red")){
        redVal = bndl.getInt(0);
        Serial.print("red value received: ");
        Serial.println(redVal);
      }
      if (bndl.fullMatch("/LOOM/Ishield0/Port0/Neopixel/Green")){
        greenVal = bndl.getInt(0);
        Serial.print("green value received: ");
        Serial.println(greenVal);
      }
      if (bndl.fullMatch("/LOOM/Ishield0/Port0/Neopixel/Blue")){
        blueVal = bndl.getInt(0);
        Serial.print("blue value received: ");
        Serial.println(blueVal);
      }
      if (redVal != -1 && greenVal != -1 && blueVal != -1){
      pixels.setPixelColor(0, pixels.Color((redVal > 255) ? 255 : redVal, (greenVal > 255) ? 255 : greenVal, (blueVal > 255) ? 255: blueVal));
      pixels.show();
      redVal = -1;
      greenVal = -1;
      blueVal = -1; //flag to indicate that a full set of three messages has not yet been received
      }
    }
    else {
      error = bndl.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
    /*
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
    */

//    Serial.println("=========================================");
//    for (int i=0; i< 256; i++){
//      Serial.print(packetBuffer[i]);
//      if (((i % 32) == 0) && (i != 0))
//        Serial.println();
//    }
/*    Serial.println("=========================================");

    char *tmp1, *tmp2, *tmp3;
    
    int r_val;
    int g_val;
    int b_val;

    tmp1 =strndup(packetBuffer+1, 3);
    Serial.print("Tmp1: ");
    Serial.println(tmp1);
    r_val = atoi(tmp1);
    Serial.println("Red: ");
    Serial.println(r_val);
    
    tmp2 =strndup(packetBuffer+6, 3);
    Serial.print("Tmp2: ");
    Serial.println(tmp2);
    g_val = atoi(tmp2);
    Serial.println("Green: ");
    Serial.println(g_val);
    
    tmp3 =strndup(packetBuffer+11, 3);
    Serial.print("Tmp3: ");
    Serial.println(tmp3);
    b_val = atoi(tmp3);
    Serial.println("Blue: ");
    Serial.println(b_val);
*/




//    int received_val = atoi(packetBuffer);
//    Serial.println(received_val);
//    Serial.println((received_val*25)%255);
//
//    pixels.setPixelColor(0, pixels.Color(0,0,(received_val*25)%255));
//    pixels.show(); 


    
    // send a reply, to the IP address and port that sent us the packet we received
    /*Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();*/

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




