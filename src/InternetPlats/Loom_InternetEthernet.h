
#ifndef LOOM_INTERNET_ETHERNET_PLAT_h
#define LOOM_INTERNET_ETHERNET_PLAT_h

#include "Loom_InternetPlat.h"

#include <Ethernet2.h>
#include <EthernetUdp2.h>




// byte mac[] = {0x98, 0x76, 0xB6, 0x10, 0x61, 0xD6}; 		//Use this for OPEnS Lab
// byte mac[] = {0x00, 0x23, 0x12, 0x12, 0xCE, 0x7D};   	// MAC address of Luke's Ethernet port
// IPAddress ip(128,193,56,138); 						  // device's IP address


const byte default_mac[6] = {0x00, 0x23, 0x12, 0x12, 0xCE, 0x7D};
const byte default_ip[4]  = {128, 193, 56, 138};




class Loom_Ethernet_I : public LoomInternetPlat
{

protected:
	
	/// Underlying Ethernet client instance
	EthernetClient*	client;            
	/// Underlying Ethernet UDP instance
	EthernetUDP* 	UDP;

	/// The Ethernet MAC address
	byte 			mac[6];

	/// The devices IP address
	IPAddress 		ip;
	
public:

	// --- CONSTRUCTOR ---
	Loom_Ethernet_I(	char* 	module_name		= "Ethernet",

						byte	mac[6] 			= (byte*)default_mac,
						byte 	ip[4]			= (byte*)default_ip

					);

	// --- DESTRUCTOR ---
	virtual ~Loom_Ethernet_I();

	void print_config();
	void print_state();
	void package(OSCBundle& bndl, char* suffix="") {}
	bool message_route(OSCMessage& msg, int address_offset) {}

	// virtual void measure() {}
	// virtual void package(OSCBundle& bndl) {}
	// virtual bool message_route(OSCMessage& msg, int address_offset) {}


	bool connect();
	bool is_connected();
	uint32_t get_time();

private:

	void send_NTP_packet(byte packet_buffer[]);

};



#endif

