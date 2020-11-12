///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_InternetEthernet.cpp
/// @brief		File for Loom_Ethernet implementation.
/// @author		Noah Koontz
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "InternetEthernet.h"
#include "Trust_Anchors.h"

///////////////////////////////////////////////////////////////////////////////
Loom_Ethernet::Loom_Ethernet(	
		LoomManager* manager,
		const char* module_name,
		const JsonArrayConst	mac,
		const JsonArrayConst	ip
	) 
	: LoomInternetPlat(manager, "Ethernet", Type::Ethernet )
	, m_base_client()
	, m_client(m_base_client, TAs, (size_t)TAs_NUM, A7, 1, SSLClient::SSL_ERROR)
	, m_mac{}
	, m_ip()
	, m_is_connected(false)
{
	if (!mac.isNull()) {
		for (auto i = 0; i < 6; i++)
			this->m_mac[i] = mac[i];
	}
	if (!ip.isNull()) this->m_ip = IPAddress(ip[0], ip[1], ip[2], ip[3]);
	
	// try to connect to internet
	connect();
	
	print_module_label();
	if (is_connected()) {
	 	LPrintln("Successfully connected to internet");
	} else {
	 	LPrintln("Failed to connect to internet");
	}
}

///////////////////////////////////////////////////////////////////////////////
Loom_Ethernet::Loom_Ethernet(LoomManager* manager, JsonArrayConst p)
	: Loom_Ethernet(manager, EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Ethernet::print_config() const
{
	LoomInternetPlat::print_config();
	LPrint('\t', "MAC:                : [");
	for (auto i = 0; i < 6; i++) {
		LPrint_Hex(m_mac[i]);
		if (i < 5) LPrint(",");
		else 	   LPrintln("]");
	}
	LPrint('\t', "IP:                 : " );
	LPrint(  m_ip[0], ".", m_ip[1], ".");
	LPrintln(m_ip[2], ".", m_ip[3]);
	
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Ethernet::print_state() const
{
	LoomInternetPlat::print_state();	
	LPrintln('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Ethernet::connect()
{
	pinMode(8, OUTPUT);
	digitalWrite(8, HIGH);
	// initialize ethernet shield for Feather
	Ethernet.init(10);

	// clear the write error
	m_base_client.clearWriteError();

	if (Ethernet.begin(m_mac) == 0) {
		print_module_label();
		LPrintln("Failed to configure Ethernet using DHCP");
		// try to congifure using IP address instead of DHCP:
		Ethernet.begin(m_mac, m_ip);
	}
	else m_ip = Ethernet.localIP();
	m_is_connected = true;

	if (!m_is_connected) m_client.stop();
}

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::UDPPtr Loom_Ethernet::open_socket(const uint port)
{
	pinMode(8, OUTPUT);
	digitalWrite(8, HIGH);
	// create the unique pointer
	UDPPtr ptr = UDPPtr(new EthernetUDP());
	// use the object created to open a UDP socket
	if(ptr && ptr->begin(port)) return std::move(ptr);
	// return a nullptr if any of that failed
	return UDPPtr();
}

///////////////////////////////////////////////////////////////////////////////