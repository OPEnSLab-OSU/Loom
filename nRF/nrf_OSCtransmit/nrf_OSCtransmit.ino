
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <OSCBundle.h>

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x) //to concatenate a predefined number to a string literal, use STR(x)

#define FAMILY "/LOOM"
#define DEVICE "/Ishield"
#define INSTANCE_NUM 0  // Unique instance number for this device, useful when using more than one of the same device type in same space

#define IDString FAMILY DEVICE STR(INSTANCE_NUM) // C interprets subsequent string literals as concatenation: "/Loom" "/Ishield" "0" becomes "/Loom/Ishield0"

RF24 radio(5,6);                    // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 00;        // Address of our node in Octal format
const uint16_t other_node = 01;       // Address of the other node in Octal format

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already


void setup(void)
{
  Serial.begin(57600);
  Serial.println("RF24Network/examples/helloworld_tx/");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop() {
  
  network.update();                          // Check the network regularly

  
  unsigned long now = millis();              // If it's time to send a message, send it!
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    Serial.print("Sending...");

    OSCBundle bndl;
    bndl.add(IDString "/VWC").add((float)1.0);
    bndl.add(IDString "/Temp").add((int32_t)-1);
    bndl.add(IDString "/ElecCond").add((float)3.0);
    bndl.add(IDString "/TestVal").add("OkayOkayOkayOkay").add("MoreValuesHere");
    bndl.add(IDString "/WowMoreValues").add((float)6);

    char message[300];

    memset(message, '\0', 300);

    get_OSC_string(&bndl, message);
    //char msg[120];
    //strcpy(msg, message);

    Serial.println(message);
    Serial.println(strlen(message));
    
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header,message,strlen(message));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");

  }
}


