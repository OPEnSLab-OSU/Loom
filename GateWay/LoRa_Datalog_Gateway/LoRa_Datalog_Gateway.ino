// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX
// sent data being stored as ((char*)buf)
//As of 6/28/17 at 4:00 p.m we were able to log data into a folder on the SD card 
// called logdata.txt!! 

#include <SPI.h>
#include <RH_RF95.h>
#include <Ethernet2.h>
#include <SD.h>

//------------------------------------------------------------------------------------------------------
// DEBUG MODE: Set to 1 if you want to see serial printouts, else, set to 0 for field use to save memory
//------------------------------------------------------------------------------------------------------
#ifndef DEBUG
#define DEBUG 0
#endif

// set to 1 if using SD Breakout for data logging
#define USE_SD 1
// set to 1 if using Ethernet for data logging
#define USE_ETHERNET 1

#if USE_ETHERNET == 1
//------------------------------------------------------------------------
// Ethernet Settings and Vars --------------------
//------------------------------------------------------------------------
const byte mac[] = { 0x98, 0x76, 0xB6, 0x10, 0x62, 0x35 };   // Be sure this address is unique in your network

//Your secret DevID from PushingBox.com. You can use multiple DevID  on multiple Pin if you want
const char DEVID1[] = "v5591D779849C80C";        //Scenario : "The mailbox is open"
const char pushGetHeader[] = "GET /pushingbox?devid=";
char serverName[] = "api.pushingbox.com";
// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
#endif

// place to save parsed received data
char *array[8]; // should be size of how many expected comma delimited elements in the buffer


//------------------------------------------------------------------------
// SD Card settings used for connecting to the feather board--------------------
//------------------------------------------------------------------------
#if USE_SD == 1
// set up variables using the SD utility library functions:
const byte chipSelect = 9;
File myFile;
#endif

//Set-up
/* for feather32u4 */
const short RFM95_CS = 8;
const short RFM95_RST = 4;
const short RFM95_INT = 7;
 

 
// Change to 434.0 or other frequency, must match RX's freq!
const int RF95_FREQ = 915.0;
 
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
 
void setup() 
{
      
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
 
  //while (!Serial); DO NOT UNCOMMENT IF USING WITH NON SERIAL CABLE
#if DEBUG == 1
  Serial.begin(9600);
  delay(100);
#endif
//----------
// Ethernet Init
//-----------
#if USE_ETHERNET == 1
// start the Ethernet connection:
#if DEBUG == 0
Ethernet.begin((unsigned char*)mac);
#endif
#if DEBUG == 1
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    while(true);
  }
  else{
    Serial.println("Ethernet ready");
    // print the Ethernet board/shield's IP address:
    Serial.print("My IP address: ");
    Serial.println(Ethernet.localIP());
  }
#endif
  
#endif
// give the Ethernet shield a second to initialize:
  delay(1000);
#if DEBUG == 1
  Serial.println("Feather LoRa Rec. Test!");
#endif 
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

#if DEBUG == 0
    rf95.init();
#endif

#if DEBUG == 1
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");
#endif
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
#if DEBUG == 0
    rf95.setFrequency(RF95_FREQ);
#endif
#if DEBUG == 1
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
#endif
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
 
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);

  //////////////////////////////////////////////////////////////////
  // SD CARD see if the card is present and can be initialized:////////////////////////
  //////////////////////////////////////////////////////////////////////////////////
#if USE_SD == 1
#if DEBUG == 0
    SD.begin(chipSelect);
#endif

#if DEBUG == 1
   if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
  }
  
  Serial.println("card initialized.");

  if (SD.exists("datalog.txt")) {
    Serial.println("datalog.txt exists.");
  } else {
    Serial.println("datalog.txt doesn't exist.");
  }

#endif
  myFile = SD.open("datalog.txt",FILE_WRITE);
  Serial.print ("datalog.txt created");
  
//  myFile.close(); 
#endif
}
 
void loop()
{
  
  if (rf95.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len))
    {
      RH_RF95::printBuffer("Received: ", buf, len);
#if DEBUG == 1
      Serial.print("Got: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      delay(10);
#endif

  // Parse comma delimited data
  // sourced from: https://stackoverflow.com/questions/15472299/split-string-into-tokens-and-save-them-in-an-array
    char *pt;
    short i = 0; 
    pt = strtok ((char*)buf,",");
    while (pt != NULL) {
        array[i++] = pt; // Save data into pointer array, defined in global vars
        pt = strtok (NULL, ",");
    }
    
#if DEBUG == 1  
    Serial.println("now the parse");
    // Now access them by index
    for (i = 0; i < 8; ++i)
    {
        Serial.println(array[i]);
    }
#endif 

      // Send a reply
      rf95.send((const unsigned char*)array[0], sizeof(array[0])+1); // Send Device ID back to transmitter in reply
      rf95.waitPacketSent();

#if DEBUG == 1
      Serial.println("Sent a reply");
      Serial.println(array[0]);
#endif
      
    }
    else
    {
#if DEBUG == 1
      Serial.println("Receive failed");
#endif
    }
    /////////////////////////////////////////////////////////////
   //////////// //SD CARD STUFF///////////////////////////////////
   /////////////////////////////////////////////////////////////
#if USE_SD == 1
    // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
//  File myFile = SD.open("datalog.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (myFile) {
    myFile.println((char*)buf);
    Serial.print("data written to card");
    //myFile.close();
   
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println(F("error opening datalog.txt"));
  }
#endif
 /////////////////////////////////////////////////////////////
 ////////////  Ethernet Stuf  ////////////////////////////////
 /////////////////////////////////////////////////////////////
#if USE_ETHERNET == 1
    // Send Pushingbox request here to upload data!
    sendToPushingBox((char*)DEVID1);
#endif 
  } // end if RF95.available

  
}

#if USE_ETHERNET == 1
//Function for sending the request to PushingBox
void sendToPushingBox(char devid[])
{
  client.stop();
#if DEBUG == 1
  Serial.println("connecting...");
#endif
  if (client.connect(serverName, 80)) {
#if DEBUG == 1
   Serial.println("connected");
   Serial.println("sendind request");
#endif
    
    client.print("GET /pushingbox?devid="); client.print(devid); 
    client.print("&IDtag=");client.print(array[0]);
    client.print("&TimeStamp=");client.print(array[1]);
    client.print("&TempC=");client.print(array[2]);
    client.print("&Humid=");client.print(array[3]);
    client.print("&LoadCell=");client.print(array[4]);
    client.print("&IRLight=");client.print(array[5]);
    client.print("&FullLight=");client.print(array[6]);
    client.print("&BatVolt=");client.print(array[7]);
    client.println(" HTTP/1.1");
    client.print("Host: "); client.println(serverName);
    client.println("User-Agent: Arduino");
    client.println();
   
  } 
  else {
    if(DEBUG){Serial.println("connection failed");}
  }
}
#endif
