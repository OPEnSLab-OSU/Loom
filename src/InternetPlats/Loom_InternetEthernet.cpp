
#include "Loom_InternetEthernet.h"
#include "Loom_Trust_Anchors.h"


///////////////////////////////////////////////////////////////////////////////
Loom_Ethernet_I::Loom_Ethernet_I(	
		const char*				module_name,
		const JsonArrayConst	mac,
		const JsonArrayConst	ip
	) 
	: LoomInternetPlat( "Ethernet", Type::Ethernet )
	, m_client( EthernetClient(), TAs, (size_t)TAs_NUM, A7, SSL_ERROR )
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


	// LPrintln("Connected : ", is_connected());
	// LPrintln("Time      : ", get_time());
	// LPrintln("Time      : ", get_time());

}

///////////////////////////////////////////////////////////////////////////////
Loom_Ethernet_I::Loom_Ethernet_I(JsonArrayConst p)
	: Loom_Ethernet_I( EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Ethernet_I::print_config()
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
void Loom_Ethernet_I::print_state()
{
	LoomInternetPlat::print_state();	
	print_module_label();
	LPrintln('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Ethernet_I::connect()
{
	// initialize ethernet shield for Feather
	Ethernet.init(10);

	if (Ethernet.begin(m_mac) == 0) {
		print_module_label();
		LPrintln("Failed to configure Ethernet using DHCP");
		// try to congifure using IP address instead of DHCP:
		Ethernet.begin(m_mac, m_ip);
	}
	else m_ip = Ethernet.localIP();
	m_is_connected = true;

	print_module_label();
	LPrintln("Connect to Ethernet ", (m_is_connected) ? "successful" : "failed" );

	if (!m_is_connected) m_client.stop();
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Ethernet_I::is_connected()
{
	return  m_is_connected;
}

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::ClientSession Loom_Ethernet_I::connect_to_domain(const char* domain) {
	// if the socket is somehow still open, close it
	if (m_client.connected()) m_client.stop();
	// * the rainbow connection *
	int status = m_client.connect(domain, 443);
	if (!status) {
		// log fail, and return
		print_module_label();
		LPrint("Ethernet connect failed with error ", m_client.getWriteError(), "\n");
		m_is_connected = false;
		return ClientSession();
	}
	m_is_connected = true;
	// return a pointer to the client for data reception
	return LoomInternetPlat::ClientSession(&m_client);
}

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::UDPPtr Loom_Ethernet_I::open_socket(const uint port)
{
	// create the unique pointer
	UDPPtr ptr = UDPPtr(new EthernetUDP());
	// use the object created to open a UDP socket
	if(ptr && ptr->begin(port)) return std::move(ptr);
	// return a nullptr if any of that failed
	return UDPPtr();
}

///////////////////////////////////////////////////////////////////////////////
