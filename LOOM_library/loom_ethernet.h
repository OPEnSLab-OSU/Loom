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
		LOOM_DEBUG_Println("Failed to configure Ethernet using DHCP");
		// try to congifure using IP address instead of DHCP:
		Ethernet.begin(mac, ip);
	}
	
	if (client.connect("www.google.com", 80)) {
		is_setup = true;
		LOOM_DEBUG_Println("Successfully connected to internet");
		client.stop();
	} else {
		is_setup = false;
		LOOM_DEBUG_Println("Failed to connect to internet");
	}
	
	return is_setup;
}



// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================
















