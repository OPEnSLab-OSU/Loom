// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
EthernetClient client;            


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_ethernet();


// ================================================================
// ===                          SETUP                           ===
// ================================================================

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



// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================




