
#ifndef LOOM_INTERNET_ETHERNET_PLAT_h
#define LOOM_INTERNET_ETHERNET_PLAT_h

#include "Loom_InternetPlat.h"
#include "EthernetLarge.h"
#include "SSLClient.h"

class Loom_Ethernet_I : public LoomInternetPlat
{
protected:
	
	/// Underlying Ethernet SSLclient instance
	SSLClient<EthernetClient> m_client;         
	/// Underlying Ethernet UDP instance
	EthernetUDP 	m_UDP;

	/// The Ethernet MAC address
	byte 			m_mac[6];

	/// The devices IP address
	IPAddress 		m_ip;

	/// Whether or not ethernet initialized successfully
	bool m_is_connected;
	
public:

	// --- CONSTRUCTOR ---
	Loom_Ethernet_I(	const char* 			module_name		= "Ethernet",
						const JsonArrayConst	mac				= JsonArray(),
						const JsonArrayConst 	ip				= JsonArray()
					);

	Loom_Ethernet_I( JsonVariant p );

	// --- DESTRUCTOR ---
	~Loom_Ethernet_I() = default;

	void print_config() override;
	void print_state() override;

	// remember to close the socket!
	Client& http_request(const char* domain, const char* url, const char* body, const char* verb) override;

	void connect() override;
	bool is_connected() override;
	uint32_t get_time() override;

private:

	void m_send_NTP_packet(byte packet_buffer[]);

};
#endif
