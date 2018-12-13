// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================

#include <EthernetUdp2.h> // currently just for testing getting time over internet


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ===============================================================
EthernetClient ethernet_client;            

// Used to be able to get time
EthernetUDP Udp;
unsigned int localPort = 8888;       // local port to listen for UDP packets


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool     setup_ethernet();
uint32_t get_time_ethernet();
void     sendNTPpacket(const char * address);


// ================================================================
// ===                          SETUP                           ===
// ================================================================

// --- SETUP ETHERNET ---
//
// Configures ethernet capabilities.
// 
bool setup_ethernet() 
{
	LOOM_DEBUG_Println("Setting up ethernet");

	// #if is_lora == 1
		digitalWrite(8, HIGH);
	// #endif
		


	bool is_setup;
	if (Ethernet.begin(mac) == 0) {
		LOOM_DEBUG_Println("Failed to configure Ethernet using DHCP");
		// try to congifure using IP address instead of DHCP:
		Ethernet.begin(mac, ip);
	}
	
	if (ethernet_client.connect("www.google.com", 80)) {
		is_setup = true;
		LOOM_DEBUG_Println("Successfully connected to internet");
		ethernet_client.stop();
	} else {
		is_setup = false;
		LOOM_DEBUG_Println("Failed to connect to internet");
	}


	// Used to be able to get time
	Udp.begin(localPort);


	return is_setup;
}



// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================

// Get NTP time over Ethernet code found here: 'https://www.arduino.cc/en/Tutorial/UdpNtpClient'

const char timeServer[] = "time.nist.gov"; 	// time.nist.gov NTP server
const int NTP_PACKET_SIZE = 48; 			// NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[NTP_PACKET_SIZE]; 		//buffer to hold incoming and outgoing packets



// --- GET TIME ETHERNET ---
//
// Gets the UTC time over internet
//
// @return Unix time (seconds since 1970)
//
uint32_t get_time_ethernet()
{
	LOOM_DEBUG_Println("get_time_ethernet");

	sendNTPpacket(timeServer); // send an NTP packet to a time server

	unsigned long epoch = 0;

	// wait to see if a reply is available
	delay(1000);
	if (Udp.parsePacket()) {
		// We've received a packet, read the data from it
		Udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

		// the timestamp starts at byte 40 of the received packet and is four bytes,
		// or two words, long. First, extract the two words:

		unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
		unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
		// combine the four bytes (two words) into a long integer
		// this is NTP time (seconds since Jan 1 1900):
		unsigned long secsSince1900 = highWord << 16 | lowWord;
		LOOM_DEBUG_Print("Seconds since Jan 1 1900 = ");
		LOOM_DEBUG_Println(secsSince1900);

		// now convert NTP time into everyday time:
		LOOM_DEBUG_Print("Unix time = ");
		// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
		const unsigned long seventyYears = 2208988800UL;
		// subtract seventy years:
		epoch = secsSince1900 - seventyYears;
		// print Unix time:
		LOOM_DEBUG_Println(epoch);


		// print the hour, minute and second:
		LOOM_DEBUG_Print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
		LOOM_DEBUG_Print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
		LOOM_DEBUG_Print(':');
		if (((epoch % 3600) / 60) < 10) {
			// In the first 10 minutes of each hour, we'll want a leading '0'
			LOOM_DEBUG_Print('0');
		}
		LOOM_DEBUG_Print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
		LOOM_DEBUG_Print(':');
		if ((epoch % 60) < 10) {
			// In the first 10 seconds of each minute, we'll want a leading '0'
			LOOM_DEBUG_Print('0');
		}
		LOOM_DEBUG_Println(epoch % 60); // print the second
	}
	// wait ten seconds before asking for the time again
	// delay(10000);
	// Ethernet.maintain();

	return epoch;
}


// send an NTP request to the time server at the given address
void sendNTPpacket(const char * address) 
{
	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	// (see URL above for details on the packets)
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     		// Stratum, or type of clock
	packetBuffer[2] = 6;     		// Polling Interval
	packetBuffer[3] = 0xEC;  		// Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12]  = 49;
	packetBuffer[13]  = 0x4E;
	packetBuffer[14]  = 49;
	packetBuffer[15]  = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	Udp.beginPacket(address, 123); // NTP requests are to port 123
	Udp.write(packetBuffer, NTP_PACKET_SIZE);
	Udp.endPacket();
}


