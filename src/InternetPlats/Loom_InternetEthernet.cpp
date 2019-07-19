
#include "Loom_InternetEthernet.h"
#include "Loom_Trust_Anchors.h"


///////////////////////////////////////////////////////////////////////////////
Loom_Ethernet_I::Loom_Ethernet_I(	
		const char*	module_name,
		const JsonArrayConst	mac,
		const JsonArrayConst	ip
	) 
	: LoomInternetPlat( module_name )
	, m_client( EthernetClient(), TAs, (size_t)TAs_NUM, A7, SSL_ERROR )
	, m_UDP()
	, m_mac{}
	, m_ip()
	, m_is_connected(false)
{
	if(!mac.isNull()) {
		for (int i = 0; i < 6; i++)
			this->m_mac[i] = mac[i];
	}
	if(!ip.isNull()) this->m_ip = IPAddress(ip[0], ip[1], ip[2], ip[3]);

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
	for (int i = 0; i < 6; i++) {
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
	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

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
Client& Loom_Ethernet_I::connect_to_domain(const char* domain) 
{
	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

	// if the socket is somehow still open, close it
	if (m_client.connected()) m_client.stop();
	// * the rainbow connection *
	int status = m_client.connect(domain, 443);
	if (!status) {
		// log fail, and return
		print_module_label();
		LPrint("Ethernet connect failed with error ", m_client.getWriteError(), "\n");
		m_is_connected = false;
	}
	else m_is_connected = true;
	// return the client for data reception
	return m_client;
}


///////////////////////////////////////////////////////////////////////////////
constexpr unsigned int localPort = 8888;		// Local port to listen for UDP packets on

constexpr char time_server[] = "pool.ntp.org"; 	// pool.ntp.org NTP server
constexpr int NTP_PACKET_SIZE = 48; 			// NTP time stamp is in the first 48 bytes of the message

void print_unix_time(unsigned long epoch) 
{
	// print Unix time:
	LPrint("Epoch:", epoch, " = ");

	// print the hour, minute and second:
	// LPrint("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
	LPrint((epoch  % 86400L) / 3600, ":"); // print the hour (86400 equals secs per day)
	if (((epoch % 3600) / 60) < 10) {
		// In the first 10 minutes of each hour, we'll want a leading '0'
		LPrint('0');
	}
	LPrint((epoch  % 3600) / 60, ":"); // print the minute (3600 equals secs per minute)
	if ((epoch % 60) < 10) {
		// In the first 10 seconds of each minute, we'll want a leading '0'
		LPrint('0');
	}
	LPrintln(epoch % 60); // print the second
}

///////////////////////////////////////////////////////////////////////////////
uint32_t Loom_Ethernet_I::get_time()
{	
	digitalWrite(8, HIGH); // if using LoRa, need to temporarily prevent it from using SPI

	if (!is_connected()) return 0;
	
	if (!m_UDP.begin(localPort)) {
		print_module_label();
		LPrint("Failed to open UDP for NTP!\n");
		return 0;
	}

	byte packet_buffer[NTP_PACKET_SIZE]; 		//buffer to hold incoming and outgoing packets

	m_send_NTP_packet(packet_buffer); // send an NTP packet to a time server

	unsigned long epoch = 0;

	// wait to see if a reply is available
	delay(1000);
	if (m_UDP.parsePacket()) {
		// We've received a packet, read the data from it
		m_UDP.read(packet_buffer, NTP_PACKET_SIZE); // read the packet into the buffer

		// the timestamp starts at byte 40 of the received packet and is four bytes,
		// or two words, long. First, extract the two words:
		unsigned long highWord = word(packet_buffer[40], packet_buffer[41]);
		unsigned long lowWord  = word(packet_buffer[42], packet_buffer[43]);
		// combine the four bytes (two words) into a long integer
		// this is NTP time (seconds since Jan 1 1900):
		unsigned long secsSince1900 = highWord << 16 | lowWord;
		
		if (print_verbosity == Verbosity::V_HIGH) {
			print_module_label();
			LPrint("Seconds since Jan 1 1900 = ", secsSince1900, "\n");
		}

		// now convert NTP time into everyday time:
		// LPrint("Unix time = ");
		// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
		const unsigned long seventyYears = 2208988800UL;
		// subtract seventy years:
		epoch = secsSince1900 - seventyYears;

		if (print_verbosity == Verbosity::V_HIGH) {
			print_module_label();
			print_unix_time(epoch);
		}
	} else {
		print_module_label();
		LPrint("Failed to parse UDP packet!\n");
	}

	m_UDP.stop();

	return epoch;

}

///////////////////////////////////////////////////////////////////////////////
void Loom_Ethernet_I::m_send_NTP_packet(byte packet_buffer[])
{
	// set all bytes in the buffer to 0
	memset(packet_buffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	packet_buffer[0] = 0b11100011;   // LI, Version, Mode
	packet_buffer[1] = 0;     		// Stratum, or type of clock
	packet_buffer[2] = 6;     		// Polling Interval
	packet_buffer[3] = 0xEC;  		// Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packet_buffer[12]  = 49;
	packet_buffer[13]  = 0x4E;
	packet_buffer[14]  = 49;
	packet_buffer[15]  = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	m_UDP.beginPacket(time_server, 123); // NTP requests are to port 123
	m_UDP.write(packet_buffer, NTP_PACKET_SIZE);
	m_UDP.endPacket();
}

///////////////////////////////////////////////////////////////////////////////
