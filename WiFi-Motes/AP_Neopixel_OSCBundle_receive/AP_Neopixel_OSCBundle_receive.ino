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
char ssid[] = "neopixel-feather"; //  your network SSID (name)
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
  
  //Initialize //Serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
     // wait for //Serial port to connect. Needed for native USB port only
  }

//  pinMode(led, OUTPUT);      // set the LED pin mode

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    //Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to WiFi network:
//  while ( status != WL_CONNECTED) {
//    //Serial.print("Attempting to connect to SSID: ");
//    //Serial.println(ssid);
//    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
//    status = WiFi.begin(ssid, pass);
//
//    // wait 10 seconds for connection:
//    delay(10000);
//  }
//  //Serial.println("Connected to wifi");
//  printWiFiStatus();

 // by default the local IP address of will be 192.168.1.1
  // you can override it with the following:
  // WiFi.config(IPAddress(10, 0, 0, 1));


//----------------
  // print the network name (SSID);
  //Serial.print("Creating access point named: ");
  //Serial.println(ssid);

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

int redVal = 0, greenVal = 0, blueVal = 0;

void setRed(OSCMessage &msg, int addrOffset){
  redVal = msg.getInt(0);
  Serial.print("set redVal to ");
  Serial.println(redVal);
}

void setGreen(OSCMessage &msg, int addrOffset){
  greenVal = msg.getInt(0);
  Serial.print("set greenVal to ");
  Serial.println(greenVal);
}
void setBlue(OSCMessage &msg, int addrOffset){
  blueVal = msg.getInt(0);
  Serial.print("set blueVal to ");
  Serial.println(blueVal);
}

void loop() {
  OSCBundle bndl;

 
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
//  Serial.println(packetSize);
  if (packetSize > 0)
  {
    bndl.empty();
    Serial.println("=========================================");
    Serial.print("received packet of size: ");
    Serial.println(packetSize);
    while (packetSize--){
      bndl.fill(Udp.read());
    }
    if (!bndl.hasError()){

      bndl.route("/LOOM/Ishield0/Port0/Neopixel/Red",setRed);
      bndl.route("/LOOM/Ishield0/Port0/Neopixel/Green",setGreen);
      bndl.route("/LOOM/Ishield0/Port0/Neopixel/Blue",setBlue);

      pixels.setPixelColor(0, pixels.Color((redVal > 255) ? 255 : redVal, (greenVal > 255) ? 255 : greenVal, (blueVal > 255) ? 255: blueVal));
      pixels.show();
    }
    else {
      error = bndl.getError();
      Serial.print("error: ");
      Serial.println(error);
    }

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
}





