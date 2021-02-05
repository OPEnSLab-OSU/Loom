///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_InternetEthernet.cpp
/// @brief		File for Ethernet implementation.
/// @author		Noah Koontz
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_ETHERNET

#include "InternetEthernet.h"
#include "Trust_Anchors.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
Ethernet::Ethernet(
		const char* module_name,
		const JsonArrayConst	mac,
		const JsonArrayConst	ip
	)
	: InternetPlat("Ethernet")
	, m_base_client()
	, m_client(m_base_client, TAs, (size_t)TAs_NUM, A7, 1, SSLClient::SSL_ERROR)
	, m_mac{}
	, m_ip()
	, m_is_connected(false)
{
  LMark;
	if (!mac.isNull()) {
   	LMark;
		for (auto i = 0; i < 6; i++)
			this->m_mac[i] = mac[i];
	}
	if (!ip.isNull()) this->m_ip = IPAddress(ip[0], ip[1], ip[2], ip[3]);
  LMark;

	// try to connect to internet
	connect();

  LMark;

	print_module_label();
  LMark;
	if (is_connected()) {
    LMark;
	 	LPrintln("Successfully connected to internet");
  	LMark;
	} else {
	 	LPrintln("Failed to connect to internet");
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
Ethernet::Ethernet(JsonArrayConst p)
	: Ethernet( EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void Ethernet::print_config() const
{
  LMark;
	InternetPlat::print_config();
  LMark;
	LPrint('\t', "MAC:                : [");
  LMark;
	for (auto i = 0; i < 6; i++) {
   	LMark;
		LPrint_Hex(m_mac[i]);
   	LMark;
		if (i < 5) LPrint(",");
		else 	   LPrintln("]");
  	LMark;
	}
	LPrint('\t', "IP:                 : " );
  LMark;
	LPrint(  m_ip[0], ".", m_ip[1], ".");
  LMark;
	LPrintln(m_ip[2], ".", m_ip[3]);
  LMark;

}

///////////////////////////////////////////////////////////////////////////////
void Ethernet::print_state() const
{
  LMark;
	InternetPlat::print_state();
  LMark;
	LPrintln('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Ethernet::connect()
{
  LMark;
	pinMode(8, OUTPUT);
  LMark;
	digitalWrite(8, HIGH);
  LMark;
	// initialize ethernet shield for Feather
	::Ethernet.init(10);
  LMark;

	// clear the write error
	m_base_client.clearWriteError();
  LMark;

	if (::Ethernet.begin(m_mac) == 0) {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("Failed to configure Ethernet using DHCP");
   	LMark;
		// try to congifure using IP address instead of DHCP:
		::Ethernet.begin(m_mac, m_ip);
  	LMark;
	}
	else m_ip = ::Ethernet.localIP();
  LMark;
	m_is_connected = true;
  LMark;

	if (!m_is_connected) m_client.stop();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
InternetPlat::UDPPtr Ethernet::open_socket(const uint port)
{
  LMark;
	pinMode(8, OUTPUT);
  LMark;
	digitalWrite(8, HIGH);
  LMark;
	// create the unique pointer
	UDPPtr ptr = UDPPtr(new EthernetUDP());
  LMark;
	// use the object created to open a UDP socket
	if(ptr && ptr->begin(port)) return std::move(ptr);
  LMark;
	// return a nullptr if any of that failed
	return UDPPtr();
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_ETHERNET
