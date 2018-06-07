// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <string.h>
#include <Ethernet2.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define LORA_MESSAGE_SIZE RH_RF95_MAX_MESSAGE_LEN       // Defaults to 251 as 4 bytes are used for the header.
#define RFM95_CS 8
#define RFM95_RST 4

#ifdef is_m0
	#define RFM95_INT 3     // Use this for the M0
#endif
#ifdef is_32u4
	#define RFM95_INT 7     // Use this for the 32u4
#endif

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
union data_value {
	int32_t i;
	float f;
	uint32_t u;
};

#if lora_device_type == 0 // If Hub
	String data[NUM_FIELDS];
//	char device_id[]   = "v25CCAAB0F709665";     // Required by PushingBox, specific to each scenario
//	char server_name[] = "api.pushingbox.com";

	//Use this for OPEnS Lab
//	byte mac[] = {0x98, 0x76, 0xB6, 0x10, 0x61, 0xD6};
//	IPAddress ip(128,193,56,138); 
	EthernetClient client;            
#endif

RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(rf95, SERVER_ADDRESS);


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void lora_setup(RH_RF95 *rf95, RHReliableDatagram *manager);

#if lora_device_type == 0
	bool setup_ethernet();
	void translate_string_to_OSC(char *string, OSCBundle* bndl);
	void lora_receive_bundle(OSCBundle *bndl);
//	void lora_process_bundle(OSCBundle *bndl, char packet_header_string[]);

	String get_data_value(OSCMessage* msg, int pos);
	void sendToPushingBox(int num_fields, char *server_name, char *devid);
#endif

#if lora_device_type == 1
	void translate_OSC_to_string(OSCBundle *bndl, char *osc_string);
	bool lora_send_bundle(OSCBundle *bndl);
#endif

#if LOOM_DEBUG == 1
	void print_bundle(OSCBundle *bndl);
#endif


// ================================================================
// ===                          SETUP                           ===
// ================================================================


// ---  LORA_SETUP ---
//
// Changes the state of the device to allow it to use LoRa. 
//
// @param rf95     An instance of the radio to be initialized
// @param manager  An instance of the manager to be initialized
//
void lora_setup(RH_RF95 *rf95, RHReliableDatagram *manager) 
{
	#if lora_device_type == 0
		pinMode(8, INPUT_PULLUP);
	#endif
	#if LOOM_DEBUG == 1
		Serial.println("Initializing manager...");
	#endif
	if (!manager->init()){
		#if LOOM_DEBUG == 1
			Serial.println("init failed");
		#endif
	}

	#if LOOM_DEBUG == 1
		Serial.println("Setting Frequency...");
	#endif
	if (!rf95->setFrequency(RF95_FREQ)) {
		#if LOOM_DEBUG == 1
			Serial.println("setFrequency failed");
		#endif
	}

	#if LOOM_DEBUG == 1
		Serial.println("Setting up ethernet");
	#endif
	#if lora_device_type == 0
		if(!setup_ethernet()) {
			#if LOOM_DEBUG == 1
				Serial.println("Failed to setup ethernet");
			#endif
		}
	#endif
	
	#if LOOM_DEBUG == 1
		Serial.println("Setting power...");
	#endif
	rf95->setTxPower(23, false);
	#if LOOM_DEBUG == 1
		Serial.println("LoRa setup finished!");
	#endif
}







// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================



#if lora_device_type == 0

// --- SETUP ETHERNET ---
//
// Configures ethernet capabilities.
// 
bool setup_ethernet() 
{
	bool is_setup;
	if (Ethernet.begin(mac) == 0) {
		#if LOOM_DEBUG == 1
			Serial.println("Failed to configure Ethernet using DHCP");
		#endif
		// try to congifure using IP address instead of DHCP:
		Ethernet.begin(mac, ip);
	}
	
	if (client.connect("www.google.com", 80)) {
		is_setup = true;
		#if LOOM_DEBUG == 1
			Serial.println("Successfully connected to internet");
		#endif
		client.stop();
	}
	else {
		is_setup = false;
		#if LOOM_DEBUG == 1
			Serial.println("Failed to connect to internet");
		#endif
	}
	
	return is_setup;
}


// --- GET DATA VALUE ---
// 
// @param msg  An OSCMessage, 
// @param pos  The position of an argument inside the OSCMessage.
//
// @return The value of the argument as a string.
//
String get_data_value(OSCMessage* msg, int pos) 
{
	if (pos < msg->size()) {
		switch (msg->getType(pos)) {
			case 'i':
				return String(msg->getInt(pos));
				break;
			case 'f':
				return String(msg->getFloat(pos));
				break;
			case 's':
				char buf[80];
				msg->getString(pos, buf, 80);
				return String(buf);
				break;
			default:
				#if LOOM_DEBUG == 1
					Serial.println("Unsupported data data_type.");
				#endif
				return String("");
		}
	}
	#if LOOM_DEBUG == 1
		Serial.print("Message does not have an argument with position: ");
		Serial.println(pos);
	#endif
	return String("");
}



// --- GET OSC BUNDLE ---
// 
// Converts string used by LoRa to an equivalent OSCBundle 
//
// @param string  A char * created through the use of translate_OSC_to_string(), 
// @param bndl    The OSC bundle to be populated
//
void translate_string_to_OSC(char *string, OSCBundle* bndl) 
{
	bndl->empty();
	data_value value_union;
	char buf[strlen(string)+1];
	char *p = buf, *p2 = NULL;
	char *token = NULL, *msg_token = NULL; 
	strcpy(buf, string);
	OSCMessage *msg;
	msg_token = strtok_r(p, " ", &p);
	while (msg_token != NULL & strlen(msg_token) > 0) {
		p2 = msg_token;
		token = strtok_r(p2, ",", &p2);
		msg = &(bndl->add(token));
		token = strtok_r(NULL, ",", &p2);
		int k = 1;
		while (token != NULL & strlen(token) > 0) {
			if (token[0] == 'f') {
				value_union.u = strtoul(&token[1], NULL, 0);
				msg->add(value_union.f);
			}
			else if (token[0] == 'i') {
				value_union.u = strtoul(&token[1], NULL, 0);
				msg->add(value_union.i);
			}
			else if (token[0] == 's') {
				msg->add(&token[1]);
			}
			token = strtok_r(p2, ",", &p2);
		}
		msg_token = strtok_r(p, " ", &p);
	}
}



// --- SEND TO PUSHINGBOX ---
// 
// Sends a get request to PushingBox
//
// @param msg  The message containing the information to send to PB.
//
void sendToPushingBox(OSCMessage &msg) 
{
	#if LOOM_DEBUG == 1
		Serial.println("Sending to pushing box");
	#endif
	client.stop();
	if (client.connect(server_name, 80)) {  
		client.print("GET /pushingbox?devid="); client.print(device_id); 
		for(int i = 0; i < NUM_FIELDS; i++) {
			if((i % 2) == 0)
				client.print("&key" + String(i/2) + "=");
			else
				client.print("&val" + String(i/2) + "=");
			client.print(get_data_value(&msg, i));
		}
		client.println(" HTTP/1.1");
		client.print("Host: "); client.println(server_name);
		client.println("User-Agent: Arduino");
		client.println();
	 
	} 
	else {
		#if LOOM_DEBUG == 1
			Serial.println("Failed to connect to PB, attempting to re-setup ethernet.");
		#endif
		if(setup_ethernet()) {
			#if LOOM_DEBUG == 1
				Serial.println("Successfully re-setup ethernet.");
			#endif
		}
		#if LOOM_DEBUG == 1 
			else {
				Serial.println("Failed to re-setup ethernet.");
			}
		#endif
	}
}



// --- LoRa Receive Bundle ---
//
// Fills an OSC bundle with messages converted from strings as
// received on LoRa
// Changes the state/contents of the bundle and of the String Array, data
//
// @param bndl  A pointer to the OSCBundle to be filled
//
void lora_receive_bundle(OSCBundle *bndl)
{
	if (manager.available()) {
		uint8_t len = LORA_MESSAGE_SIZE;
		uint8_t from;
		uint8_t buf[LORA_MESSAGE_SIZE];
		memset(buf, '\0', LORA_MESSAGE_SIZE);
		if (manager.recvfromAck(buf, &len, &from)) {
			//If true, then the data being received is from the evaporimeter, which
			//is formatted differently as they use code not written by the CS Capstone team.
			if (((char)(buf[0])) == '/') {
				translate_string_to_OSC((char*)buf, bndl); 
				for(int i = 0; i < NUM_FIELDS; i++)
					data[i] = get_data_value(bndl->getOSCMessage(0), i);
			} else {
				char str[LORA_MESSAGE_SIZE];
				String((char*)buf).toCharArray(str, sizeof(str)-1);
				char *token;
				char *savept = str;
				String cols[6] = {"IDtag", "RTC_time", "temp", "humidity", "loadCell", "vbat"};
				for(int i = 0; i < NUM_FIELDS; i+=2) {
					token = strtok_r(savept, ",", &savept);
					if(token != NULL) {
						data[i] = cols[i/2];
						data[i+1] = String(token);
					}
				} // of for
			} // of else 
		} // of if (manager.recvfromAck(buf, &len, &from))
	} // of if (manager.available()) 
}

#endif // of lora_device_type == 0




#if lora_device_type == 1

// --- GET OSC STRING ---
// 
// Converts an OSC Bundle to equivalent string to be used in LoRa transmissions
// Osc_string's contents will now include the OSCBundle's formatted data.
//
// @param bndl        An OSCBundle to put into string format.
// @param osc_string  A char * to fill with the OSCBundle's data.
//
void translate_OSC_to_string(OSCBundle *bndl, char *osc_string) 
{
	char data_type;
	data_value value;
	int addr_len = 40;
	OSCMessage* msg;                                         // Can these be added to declarations file or merged with similar vars?
	char addr_buf[addr_len];

	memset(osc_string, '\0', sizeof(osc_string));

	for (int i = 0; i < bndl->size(); i++) {
		msg = bndl->getOSCMessage(i);
		memset(addr_buf, '\0', addr_len);
		msg->getAddress(addr_buf, 0);
		strcat(osc_string, addr_buf);

		for (int j = 0; j < msg->size(); j++) {
			data_type = msg->getType(j);
			switch (data_type) {
			case 'f':
				value.f = msg->getFloat(j);
				snprintf(addr_buf, addr_len, ",f%lu", value.u);
				strcat(osc_string, addr_buf);
				break;

			case 'i':
				value.i = msg->getInt(j);
				snprintf(addr_buf, addr_len, ",i%lu", value.u);
				strcat(osc_string, addr_buf);
				break;

			case 's':
				char data_buf[40];
				msg->getString(j, data_buf, sizeof(data_buf));
				snprintf(addr_buf, addr_len, ",s%s", data_buf);
				strcat(osc_string, addr_buf);
				break;

			default:
				if (data_type != '\0')
				#if LOOM_DEBUG == 1
					Serial.print("Invalid message arg type");
				#endif
				break;
			}
		}
		if (msg != NULL) strcat(osc_string, " ");
	}
}



// --- LORA SEND BUNDLE
//
// Sends an OSC Bundle over LoRa after converting the bundle 
// to an equivalent string
//
// @param bndl  OSC Bundle to send (will be converted to string)
//
// @return Whether or not bundle send was successful
//
bool lora_send_bundle(OSCBundle *bndl)
{
	char message[RH_RF95_MAX_MESSAGE_LEN];
	memset(message, '\0', sizeof(message));
	translate_OSC_to_string(bndl, message);
	#if LOOM_DEBUG == 1
		Serial.println(message);
		Serial.print("Message length: ");
		Serial.println(strlen(message));
		Serial.print("Max message length: ");
		Serial.println(RH_RF95_MAX_MESSAGE_LEN);
		Serial.print("Sending...");
	#endif
	 
	if (manager.sendtoWait((uint8_t*)message, strlen(message), SERVER_ADDRESS)) {
		#if LOOM_DEBUG == 1
			Serial.println("Sent bundle through LoRa!");
		#endif
		return true;
	}
	else {
		#if LOOM_DEBUG == 1
			Serial.println("Failed to send bundle!");
		#endif
		return false;
	}
}




#endif // of lora_device_type == 1




// --- PRINT BUNDLE ---
// 
// Prints the OSCBundle's contents to the Serial.
//
// @param bndl  An OSCBundle to be printed.
//
#if LOOM_DEBUG == 1
void print_bundle(OSCBundle *bndl)
{
	int n = 0;
	char buf[50];
	char data_type;
	Serial.println("Bundle Size: ");
	Serial.println(bndl->size());
	OSCMessage *msg = bndl->getOSCMessage(n);
	
	while (msg != NULL) {
		msg->getAddress(buf, 0);
		Serial.print("Address ");
		Serial.print(n + 1);
		Serial.print(": ");
		Serial.println(buf);

		int m = 0;
		data_type = msg->getType(m);
		while (data_type != '\0') {
			Serial.print("Value ");
			Serial.print(m + 1);
			Serial.print(": ");
			if (data_type == 'f') {
				Serial.println(msg->getFloat(m));
			}
			else if (data_type == 'i') {
				Serial.println(msg->getInt(m));
			}
			else if (data_type == 's') {
				msg->getString(m, buf, 50);
				Serial.println(buf);
			}

			m++;
			data_type = msg->getType(m);
		}
		n++;
		msg = bndl->getOSCMessage(n);
	}
}
#endif // of LOOM_DEBUG == 1





