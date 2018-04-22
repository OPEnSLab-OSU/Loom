// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <RH_RF95.h>
#include <RHReliableDatagram.h>
#include <string.h>

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


#if lora_device_type == 0
  EthernetClient client;            
#endif

RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHReliableDatagram manager(rf95, SERVER_ADDRESS);


// ================================================================
// ===                          SETUP                           ===
// ================================================================
// --- LORA SETUP ---
// 
// Arguments: An instance of the radio, and an instance of the manager, both to be initialized.
// Return: Nothing. Simply changes the state of the device to allow it to use LoRa.
void lora_setup(RH_RF95 *rf95, RHReliableDatagram *manager) {
  #if lora_device_type == 0
		pinMode(8, INPUT_PULLUP);
  #endif
  #if DEBUG == 1
		Serial.println("Initializing manager...");
  #endif
  if (!manager->init()){
    #if DEBUG == 1
			Serial.println("init failed");
		#endif
  }

  #if DEBUG == 1
    Serial.println("Setting Frequency...");
  #endif
  if (!rf95->setFrequency(RF95_FREQ)) {
		#if DEBUG == 1
			Serial.println("setFrequency failed");
		#endif
    while (1);
  }

  #if lora_device_type == 0
    if (Ethernet.begin(mac) == 0) {
			#if DEBUG == 1
				Serial.println("Failed to configure Ethernet using DHCP");
			#endif
      // Try to congifure using IP address instead of DHCP:
      Ethernet.begin(mac, ip);
    }
  #endif
  
  #if DEBUG == 1
    Serial.println("Setting power...");
  #endif
  rf95->setTxPower(23, false);
  #if DEBUG == 1
    Serial.println("LoRa setup finished!");
  #endif
}



// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================
// --- GET OSC STRING ---
// 
// Arguments: An OSCBundle to put into string format. A char * to fill with the OSCBundle's data.
// Return: Nothing, but osc_string's contents will now include the OSCBundle's formatted data.
#if lora_device_type == 1
void get_OSC_string(OSCBundle *bndl, char *osc_string) {
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
			#if DEBUG == 1
				Serial.print("Invalid message arg type");
			#endif
			break;
		}
	}
	if (msg != NULL) strcat(osc_string, " ");
	}
}
#endif




// --- GET OSC BUNDLE ---
// 
// Arguments: A char * created through the use of get_OSC_string(), an OSCBundle to fill.
// Return: Nothing, but the OSCBundle is filled with the data from the string.
#if lora_device_type == 0
void get_OSC_bundle(char *string, OSCBundle* bndl) {
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
#endif



// --- PRINT BUNDLE ---
// 
// Arguments: An OSCBundle to be printed.
// Return: Nothing. Prints the OSCBundle's contents to the Serial.
#if DEBUG == 1
void print_bundle(OSCBundle *bndl) {
	int n = 0;
	char buf[50];
	char data_type;
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
#endif



// --- GET DATA VALUE ---
// 
// Arguments: An OSCMessage, the position of an argument inside the OSCMessage.
// Return: The value of the argument as a string.
#if lora_device_type == 0
String get_data_value(OSCMessage* msg, int pos) {
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
			#if DEBUG == 1
				Serial.println("Unsupported data data_type.");
			#endif
			return String("");
	}
}
#endif




// --- SEND TO PUSHINGBOX ---
// 
// Arguments: The number fields in the PB scenario, the base address of the URL
// the devid specifying the PB scenario.
// Return: Nothing. Sends a get request to PB.
#if lora_device_type == 0
void sendToPushingBox(int num_fields, char *server_name, char *devid) {
  client.stop();
  if (client.connect(server_name, 80)) {  
    client.print("GET /pushingbox?devid="); client.print(devid); 
    for(int i = 0; i < num_fields; i++) {
      if((i % 2) == 0)
        client.print("&key" + String(i/2) + "=");
      else
        client.print("&val" + String(i/2) + "=");
      client.print(data[i]);
    }
    client.println(" HTTP/1.1");
    client.print("Host: "); client.println(server_name);
    client.println("User-Agent: Arduino");
    client.println();
   
  } 
  #if DEBUG == 1
    else {
      Serial.println("connection failed");
    }
  #endif
}
#endif
