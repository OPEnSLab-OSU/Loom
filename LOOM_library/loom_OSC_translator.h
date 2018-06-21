// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <string.h>


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void translate_string_to_OSC(char *osc_string, OSCBundle* bndl);
void translate_OSC_to_string(OSCBundle *bndl, char *osc_string);
String get_data_value(OSCMessage* msg, int pos);
#if LOOM_DEBUG == 1
	void print_bundle(OSCBundle *bndl);
#endif
int get_bundle_bytes(OSCBundle *bndl); 			// relatively untested


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
union data_value {
	int32_t i;
	float f;
	uint32_t u;
};

// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================



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
// @param osc_string  A char * created through the use of translate_OSC_to_string(), 
// @param bndl        The OSC bundle to be populated
//
void translate_string_to_OSC(char *osc_string, OSCBundle* bndl) 
{
	bndl->empty();
	data_value value_union;
	char buf[strlen(osc_string)+1];
	char *p = buf, *p2 = NULL;
	char *token = NULL, *msg_token = NULL; 
	strcpy(buf, osc_string);
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
	OSCMessage* msg; 
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



// --- GET BUNDLE BYTES ---
//
// Gets the size of a bundle in bytes
//
// @param bndl  The bndl to get the size of
// 
// @return The size of the bundle in bytes
// 
int get_bundle_bytes(OSCBundle *bndl)
{
	int total = 0;
	for (int i = 0; i < bndl->size(); i++) {
		total += bndl->getOSCMessage(i)->bytes();
	}
}




