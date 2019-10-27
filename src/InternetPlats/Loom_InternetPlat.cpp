///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_InternetPlat.cpp
/// @brief		File for LoomInternetPlat implementation.
/// @author		Noah Koontz
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Loom_InternetPlat.h"

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::LoomInternetPlat(	
		const char*				module_name,
		const LoomModule::Type	module_type
	) 
	: LoomModule( module_name, module_type ) {}

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::ClientSession LoomInternetPlat::http_request(const char* domain, const char* url, const char* body, const char* verb) 
{
	// * the rainbow connection *
	ClientSession client = connect_to_domain(domain);
	if (!client) return ClientSession();
	// ok next, make the http request
	print_module_label();
	LPrint("Writing http: ", domain, "\n");
	write_http_request(*client, domain, url, body, verb);
	// return the client for data reception
	return std::move(client);
}

///////////////////////////////////////////////////////////////////////////////
void LoomInternetPlat::write_http_request(Stream& client, const char* domain, const char* url, const char* body, const char* verb) 
{
	// print the initial http request
	client.print(verb);
	client.print(" ");
	client.print(url == nullptr ? "/" : url);
	client.print(" ");
	client.print("HTTP/1.1\r\nUser-Agent: LoomOverSSLClient\r\nHost: ");
	client.print(domain);
	client.print("\r\nConnection: close\r\n");
	// add the optional body
	if(body != nullptr) client.print(body);
	client.print("\r\n");
	// all ready to go!
}

///////////////////////////////////////////////////////////////////////////////
constexpr static unsigned int localPort = 8888;		// Local port to listen for UDP packets on
constexpr static int NTP_PACKET_SIZE = 48; 			// NTP time stamp is in the first 48 bytes of the message
/// Static NTP time server
constexpr static char time_server[] = "pool.ntp.org"; 	// pool.ntp.org NTP server

static void print_unix_time(unsigned long epoch) 
{
	// print Unix time:
	LPrint("Epoch:", epoch, "(");


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
	LPrint(epoch % 60); // print the second
	LPrintln(")");
}

///////////////////////////////////////////////////////////////////////////////
uint32_t LoomInternetPlat::get_time() 
{
	auto udp_dev = open_socket(localPort);
	
	if (!udp_dev) {
		LPrint("Failed to open UDP for NTP!\n");
		return 0;
	}

	byte packet_buffer[NTP_PACKET_SIZE]; 		//buffer to hold incoming and outgoing packets

	m_send_NTP_packet(*udp_dev, packet_buffer); // send an NTP packet to a time server

	unsigned long epoch = 0;

	// wait to see if a reply is available
	delay(1000);
	if (udp_dev->parsePacket()) {
		// We've received a packet, read the data from it
		udp_dev->read(packet_buffer, NTP_PACKET_SIZE); // read the packet into the buffer

		// the timestamp starts at byte 40 of the received packet and is four bytes,
		// or two words, long. First, extract the two words:
		unsigned long highWord = word(packet_buffer[40], packet_buffer[41]);
		unsigned long lowWord  = word(packet_buffer[42], packet_buffer[43]);
		// combine the four bytes (two words) into a long integer
		// this is NTP time (seconds since Jan 1 1900):
		unsigned long secsSince1900 = highWord << 16 | lowWord;

		// now convert NTP time into everyday time:
		// LPrint("Unix time = ");
		// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
		const unsigned long seventyYears = 2208988800UL;
		// subtract seventy years:
		epoch = secsSince1900 - seventyYears;
		
		print_module_label();
		print_unix_time(epoch);
	}
	else LPrint("Failed to parse UDP packet!\n");

	return epoch;
}

///////////////////////////////////////////////////////////////////////////////
void LoomInternetPlat::m_send_NTP_packet(UDP& udp_dev, byte packet_buffer[]) const 
{
	// set all bytes in the buffer to 0
	memset(packet_buffer, 0, NTP_PACKET_SIZE);
	// Initialize values needed to form NTP request
	packet_buffer[0] = 0b11100011;	// LI, Version, Mode
	packet_buffer[1] = 0;			// Stratum, or type of clock
	packet_buffer[2] = 6;			// Polling Interval
	packet_buffer[3] = 0xEC;		// Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packet_buffer[12] = 49;
	packet_buffer[13] = 0x4E;
	packet_buffer[14] = 49;
	packet_buffer[15] = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	udp_dev.beginPacket(time_server, 123); // NTP requests are to port 123
	udp_dev.write(packet_buffer, NTP_PACKET_SIZE);
	udp_dev.endPacket();
}

///////////////////////////////////////////////////////////////////////////////


