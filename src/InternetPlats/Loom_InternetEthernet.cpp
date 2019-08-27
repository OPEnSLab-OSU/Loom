
#include "Loom_InternetEthernet.h"
#include "Loom_Trust_Anchors.h"

static std::vector<unsigned char> make_key_from_asn1(std::vector<unsigned char> asn1) {
	// we need to extract the private key from the ASN1 object given to us
	// to do that, some ASN.1 parsing!
	// get the tag and length of the first octet string, and verify it's reasonable
	if (asn1[5] != 0x04 
		|| asn1.size() < 6 
		|| asn1[6] >= asn1.size() - 6) return {};

	const unsigned char length = asn1[6];
	// delete everything around the key that we don't need
	asn1.erase(asn1.begin(), asn1.begin() + 7);
	asn1.erase(asn1.begin() + length, asn1.end());
	// shrink it
	asn1.shrink_to_fit();
	return asn1;
}

///////////////////////////////////////////////////////////////////////////////
Loom_Ethernet::Loom_Ethernet(	
		const char*				module_name,
		const JsonArrayConst	mac,
		const JsonArrayConst	ip,
		const char*				cli_cert,
		const char*				cli_key
	) 
	: LoomInternetPlat( "Ethernet", Type::Ethernet )
	, m_cli_cert(SSLObj::make_vector_pem(cli_cert, (cli_cert ? strlen(cli_cert) : 0)))
	, m_cli_key(make_key_from_asn1(SSLObj::make_vector_pem(cli_key, (cli_key ? strlen(cli_key) : 0))))
	, m_cert({ m_cli_cert.data(), m_cli_cert.size() })
	, m_params({
		&m_cert,
		1,
		{
			BR_EC_secp256r1,
			m_cli_key.data(), 
			m_cli_key.size()
		}
	})
	, m_client( EthernetClient(), TAs, (size_t)TAs_NUM, A7, SSL_ERROR)
	, m_mac{}
	, m_ip()
	, m_is_connected(false)
{
	if (!mac.isNull()) {
		for (auto i = 0; i < 6; i++)
			this->m_mac[i] = mac[i];
	}
	if (!ip.isNull()) this->m_ip = IPAddress(ip[0], ip[1], ip[2], ip[3]);

	// set mutual auth, if needed
	if (cli_cert && cli_key) {
		print_module_label();
		if (cli_cert && !m_cli_cert.size()) LPrintln("Failed to decode client certificate");
		else if (cli_key && !m_cli_key.size()) LPrintln("Failed to decode client private key");
		else {
			LPrintln("Adding mutual auth! Public key: ");
			LPrintln(cli_cert);
			// setup the certificate
			m_client.setMutualAuthParams(&m_params);
		}
	}
	
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
Loom_Ethernet::Loom_Ethernet(JsonArrayConst p)
	: Loom_Ethernet( EXPAND_ARRAY(p, 5) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Ethernet::print_config()
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
void Loom_Ethernet::print_state()
{
	LoomInternetPlat::print_state();	
	LPrintln('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Ethernet::connect()
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

	if (!m_is_connected) m_client.stop();
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Ethernet::is_connected()
{
	return  m_is_connected;
}

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::ClientSession Loom_Ethernet::connect_to_domain(const char* domain) {
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
LoomInternetPlat::ClientSession Loom_Ethernet::connect_to_ip(const IPAddress& ip, const uint16_t port) {
	// if the socket is somehow still open, close it
	if (m_client.connected()) m_client.stop();
	// * the rainbow connection *
	int status = m_client.connect(ip, port);
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
LoomInternetPlat::UDPPtr Loom_Ethernet::open_socket(const uint port)
{
	// create the unique pointer
	UDPPtr ptr = UDPPtr(new EthernetUDP());
	// use the object created to open a UDP socket
	if(ptr && ptr->begin(port)) return std::move(ptr);
	// return a nullptr if any of that failed
	return UDPPtr();
}

///////////////////////////////////////////////////////////////////////////////