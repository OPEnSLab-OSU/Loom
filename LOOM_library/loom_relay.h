// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct state_relay_t {
	bool on; // Array to hold relay states (on/off)
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct state_relay_t state_relay;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_relay();
void handle_relay_msg(OSCMessage &msg);
void package_relay(OSCBundle *bndl, char packet_header_string[]);



// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Called by main setup
// Sets pin modes for relays (on pins 5 and 6)
// Initializes relays to off
// 
void setup_relay() 
{
	pinMode(relay_pin, OUTPUT);
	state_relay.on = false;
	digitalWrite(relay_pin, LOW);

}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- HANDLE RELAY MSG ---
//
// Updates stored state of relays, then calls write_relay_states()
// to set relays to those states
//
// @param msg  OSC message with data of state and relay to set 
//
void handle_relay_msg(OSCMessage &msg) 
{
	int relay  = msg.getInt(0);

	if (relay == relay_pin) {
		int set_to = msg.getInt(1);

		state_relay.on = (set_to == 1);
		
		LOOM_DEBUG_Print("Set relay to "); 
		LOOM_DEBUG_Println((state_relay.on) ? "ON" : "OFF");

		digitalWrite(relay_pin, (state_relay.on == true) ? HIGH : LOW);
	}	
}




// // --- PACKAGE RELAY --- 
// // 
// // Adds relay state to provided OSC bundle
// //
// // @param bndl                  The OSC bundle to be added to
// // @param packet_header_string  The device-identifying string to prepend to OSC messages
// //
// void package_relay(OSCBundle *bndl, char packet_header_string[])
// {
// 	char address_string[80];

// 	sprintf(address_string, "%s%s", packet_header_string, "/relay0");
// 	bndl->add(address_string).add(state_relay.on[0]);
// 	sprintf(address_string, "%s%s", packet_header_string, "/relay1");
// 	bndl->add(address_string ).add(state_relay.on[0]);
// }


