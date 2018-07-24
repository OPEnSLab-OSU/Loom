// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <OSCBundle.h> // All LOOM devices rely on the Arduino OSC library in some way
#include <Ethernet2.h>

#define LOOM_DEBUG 1 // enables serial printing


#include "loom_translator.h" // scary

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


#define DEVICE    "TestDevice"
#define INIT_INST 42

#define MAX_FIELDS 32			// Maximum number of fields accepted by the PushingBox Scenario    
								// Probably just leave this be


// Pushingbox / Spreadsheet stuff
#define spreadsheet_id "1Hv2oME5sjumUXv36GtFV1Q7I83xnXu-f-ZrxUNsXS_U"
// Google Spreadsheet Sheet/Tab number. Sent as parameter to PushingBox/Google Scripts
#define tab_id  "test"   
// Required by PushingBox, specific to each scenario
char device_id[]   = "v7ECCEF7A460E57A"; // Luke's PushingBox scenario that sends to Luke's google script


// these are normally defined in the config.h file, 
// but I put it here so you dont need all of the config file
#define is_lora 0 



// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
			
//Use this for OPEnS Lab
byte mac[] = {0x00, 0x23, 0x12, 0x12, 0xCE, 0x7D};    // Luke's Mac's Mac Address
// byte mac[] = {0x98, 0x76, 0xB6, 0x10, 0x61, 0xD6}; 
IPAddress ip(128,193,56,138); 						  // device's IP address 

EthernetClient ethernet_client;            

int           led = LED_BUILTIN; 

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void sendToPushingBox(OSCMessage &msg);
void sendToPushingBox(OSCBundle *bndl);
bool setup_ethernet();



// ================================================================ 
// ===                         SETUP                            === 
// ================================================================
void setup() 
{
	pinMode(led, OUTPUT);   // Set the LED pin mode

	//Initialize serial and wait for port to open:
	#if LOOM_DEBUG == 1
		Serial.begin(9600);
		while(!Serial);        // Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
		delay(5000);
		Serial.println("Initialized Serial!");
	#endif

	setup_ethernet();
}

// ================================================================ 
// ===                          LOOP                            === 
// ================================================================



void loop()
{
	OSCBundle bndl;

	String keys[5]   = {"key0", "key1", "key2", "key3", "key4"};
	float  values[5] = {1.23, 4.56, 7.89, -0.74, 0.09};

	convert_assoc_arrays_to_bundle(keys, values,  &bndl, "/an/address/or/something", 5, SINGLEMSG);
	sendToPushingBox(&bndl);


	while(1); // stop awhile
}







// ================================================================ 
// ===                        ETHERNET                          === 
// ================================================================
bool setup_ethernet() 
{
	Serial.println("Setting up ethernet");

	#if is_lora == 1
		digitalWrite(8, HIGH);
	#endif
		
	bool is_setup;
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		// try to congifure using IP address instead of DHCP:
		Ethernet.begin(mac, ip);
	}
	
	if (ethernet_client.connect("www.google.com", 80)) {
		is_setup = true;
		Serial.println("Successfully connected to internet");
		ethernet_client.stop();
	} else {
		is_setup = false;
		Serial.println("Failed to connect to internet");
	}

	return is_setup;
}

// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================



// --- SEND TO PUSHINGBOX ---
// 
// Sends a get request to PushingBox
// Expects arguments of OSC message to be formated as:
// key, value, key, value...
// As the message router forwards messages not bundles, this is called by msg_router
// If you need to send a bundle, simply call the function (which is overloaded)
// with a correctly formatted OSCBundle
//
// The function tries platforms in the following order:
//   Ethernet, WiFi, cellular
// as enabled. The function returns immediately upon successful transmission
// else will try alternate platforms 
// (this might become an option later, that is, whether or not to try other enabled platforms or not upon failure)
//
//
// @param msg  The message containing the information to send to PB.
//
void sendToPushingBox(OSCMessage &msg) 
{
	if (msg.size() > 32) { // This also catches empty msgs, which seem to have a size around 1493 for some reason
		Serial.println("Message to large to send to PushingBox");
		return;
	}
	Serial.println("Sending to PushingBox");


	// Build url arguments from bundle
	char args[1024]; 
	sprintf(args, "/pushingbox?devid=%s&key0=sheetID&val0=%s&key1=tabID&val1=%s&key2=deviceID&val2=%s%d", 
		device_id, spreadsheet_id, tab_id, DEVICE, INIT_INST); 

	for (int i = 0, j = 3; (i < MAX_FIELDS-6) && (i < msg.size()); i+=2, j++) {
	    char buf1[30], buf2[30];
		(get_data_value(&msg, i  )).toCharArray(buf1, 30); 
		(get_data_value(&msg, i+1)).toCharArray(buf2, 30);
		sprintf(args, "%s&key%d=%s&val%d=%s", args, j, buf1, j, buf2);
	}
	Serial.println("URL get args: ");
	Serial.println(args);



	Serial.println("Running PushingBox for Ethernet");

	ethernet_client.stop();
	if (ethernet_client.connect("api.pushingbox.com", 80)) {  
		Serial.println("Connection good");

		ethernet_client.print("GET ");
		ethernet_client.print(args);
		ethernet_client.println(" HTTP/1.1\nHost: api.pushingbox.com\nUser-Agent: Arduino\n");

		Serial.println("Data done sending");

		return;  // data sent successfully, no need to try another platform

	} else {
		Serial.println("No Connection");
		Serial.println("Failed to connect to PB, attempting to re-setup ethernet.");

		if (setup_ethernet()) {
			Serial.println("Successfully re-setup ethernet.");
		}
		#if LOOM_DEBUG == 1 
		else {
			Serial.println("Failed to re-setup ethernet.");
		}
		#endif
	}

}


// --- SEND TO PUSHINGBOX ---
// 
// Sends a get request to PushingBox
// Expects OSC bundle to only have 1 message and its arguments to be formated as:
// key, value, key, value...
// Simple a wrapper for the version of this function that takes a message
// As the message router forwards messages not bundles
//
// @param msg  The message containing the information to send to PB.
//
void sendToPushingBox(OSCBundle *bndl) 
{
	OSCBundle tmpBndl;
	deep_copy_bundle(bndl, &tmpBndl);
	convert_bundle_structure(&tmpBndl, SINGLEMSG);
	#if LOOM_DEBUG == 1
		print_bundle(&tmpBndl);
	#endif
	sendToPushingBox(*(tmpBndl.getOSCMessage(0)));
}



