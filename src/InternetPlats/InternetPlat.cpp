///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_InternetPlat.cpp
/// @brief		File for LoomInternetPlat implementation.
/// @author		Noah Koontz
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "InternetPlat.h"

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::LoomInternetPlat(
		LoomManager* 			manager,
		const char* 						module_name,
		const LoomModule::Type	module_type
	)
	: LoomModule(manager, module_name, module_type ) {}

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::ClientSession LoomInternetPlat::http_request(const char* domain, const char* url, const char* body, const char* verb)
{
  LMark;
	// * the rainbow connection *
	ClientSession client = connect_to_domain(domain);
  LMark;
	if (!client) return ClientSession();
  LMark;
	// ok next, make the http request
	print_module_label();
  LMark;
	LPrint("Writing http: ", domain, "\n");
  LMark;
	write_http_request(*client, domain, url, body, verb);
  LMark;
	// return the client for data reception
	return std::move(client);
}

///////////////////////////////////////////////////////////////////////////////
void LoomInternetPlat::write_http_request(Stream& client, const char* domain, const char* url, const char* body, const char* verb)
{
  LMark;
	// print the initial http request
	client.print(verb);
  LMark;
	client.print(" ");
  LMark;
	client.print(url == nullptr ? "/" : url);
  LMark;
	client.print(" ");
  LMark;
	client.print("HTTP/1.1\r\nUser-Agent: LoomOverSSLClient\r\nHost: ");
  LMark;
	client.print(domain);
  LMark;
	client.print("\r\nConnection: close\r\n");
  LMark;
	// add the optional body
	if(body != nullptr) client.print(body);
  LMark;
	client.print("\r\n");
  LMark;
	// all ready to go!
}

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::ClientSession LoomInternetPlat::connect_to_domain(const char* domain) {
  LMark;
	pinMode(8, OUTPUT);
  LMark;
	digitalWrite(8, HIGH);
  LMark;
	// if we're not currently connected to the internet, try to bring it back
	if (!is_connected()) connect();
  LMark;
	if (!is_connected()) {
   	LMark;
		print_module_label();
   	LMark;
		LPrint("Did not attempt to connect, as we are not connected to the internet\n");
   	LMark;
		print_state();
   	LMark;
		return ClientSession();
	}
	// I guess we are connected! lets go
	SSLClient& client = get_client();
  LMark;
	// if the socket is somehow still open, close it
	if (client.connected()) client.stop();
  LMark;
	// and clear a write error if there is any
	if (client.getWriteError()) client.clearWriteError();
  LMark;
	// * the rainbow connection *
	int status = client.connect(domain, 443);
  LMark;
	if (!status) {
   	LMark;
		// log fail, attempt repair, and return
		print_module_label();
   	LMark;
		LPrint("Connect failed with error ", client.getWriteError(), "\n");
   	LMark;
		print_state();
   	LMark;
		// if the underlying client is unhappy, we may need to reset the connection
		if (client.getWriteError() == SSLClient::SSL_CLIENT_WRTIE_ERROR || client.getWriteError() == SSLClient::SSL_CLIENT_CONNECT_FAIL) {
    	LMark;
			print_module_label();
    	LMark;
			LPrintln("Attempting to cycle the connection: ");
    	LMark;
			disconnect();
    	LMark;
			delay(500);
    	LMark;
			connect();
    	LMark;
			print_state();
   		LMark;
		}
		return ClientSession();
	}
	// return a pointer to the client for data reception
	return LoomInternetPlat::ClientSession(&client);
}

///////////////////////////////////////////////////////////////////////////////
LoomInternetPlat::ClientSession LoomInternetPlat::connect_to_ip(const IPAddress& ip, const uint16_t port) {
  LMark;
	pinMode(8, OUTPUT);
  LMark;
	digitalWrite(8, HIGH);
  LMark;
	// if we're not currently connected to the internet, try to bring it back
	if (!is_connected()) connect();
  LMark;
	if (!is_connected()) {
   	LMark;
		print_module_label();
   	LMark;
		LPrint("Did not attempt to connect, as we are not connected to the internet\n");
   	LMark;
		print_state();
   	LMark;
		return ClientSession();
	}
	SSLClient& client = get_client();
  LMark;
	// if the socket is somehow still open, close it
	if (client.connected()) client.stop();
  LMark;
	// and clear a write error if there is any
	if (client.getWriteError()) client.clearWriteError();
  LMark;
	// * the rainbow connection *
	int status = client.connect(ip, port);
  LMark;
	if (!status) {
   	LMark;
		// log fail, and return
		print_module_label();
   	LMark;
		LPrint("Connect failed with error ", client.getWriteError(), "\n");
   	LMark;
		print_state();
   	LMark;
		// if the underlying client is unhappy, we may need to reset the connection
		if (client.getWriteError() == SSLClient::SSL_CLIENT_WRTIE_ERROR) {
    	LMark;
			print_module_label();
    	LMark;
			LPrint("Attempting to cycle the connection: ");
    	LMark;
			disconnect();
    	LMark;
			delay(500);
    	LMark;
			connect();
    	LMark;
			print_state();
   		LMark;
		}
		return ClientSession();
	}
	// return a pointer to the client for data reception
	return LoomInternetPlat::ClientSession(&client);
}

///////////////////////////////////////////////////////////////////////////////
constexpr static unsigned int localPort = 8888;		// Local port to listen for UDP packets on
constexpr static int NTP_PACKET_SIZE = 48; 			// NTP time stamp is in the first 48 bytes of the message
/// Static NTP time server
constexpr static char time_server[] = "pool.ntp.org"; 	// pool.ntp.org NTP server

static void print_unix_time(unsigned long epoch)
{
  LMark;
	// print Unix time:
	LPrint("Epoch:", epoch, "(");
  LMark;


	// print the hour, minute and second:
	// LPrint("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
	LPrint((epoch  % 86400L) / 3600, ":"); // print the hour (86400 equals secs per day)
  LMark;
	if (((epoch % 3600) / 60) < 10) {
   	LMark;
		// In the first 10 minutes of each hour, we'll want a leading '0'
		LPrint('0');
  	LMark;
	}
	LPrint((epoch  % 3600) / 60, ":"); // print the minute (3600 equals secs per minute)
  LMark;
	if ((epoch % 60) < 10) {
   	LMark;
		// In the first 10 seconds of each minute, we'll want a leading '0'
		LPrint('0');
  	LMark;
	}
	LPrint(epoch % 60); // print the second
  LMark;
	LPrintln(")");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
uint32_t LoomInternetPlat::get_time()
{
  LMark;
	auto udp_dev = open_socket(localPort);
  LMark;

	if (!udp_dev) {
   	LMark;
		LPrint("Failed to open UDP for NTP!\n");
   	LMark;
		return 0;
	}

	byte packet_buffer[NTP_PACKET_SIZE]; 		//buffer to hold incoming and outgoing packets
  LMark;

	m_send_NTP_packet(*udp_dev, packet_buffer); // send an NTP packet to a time server
  LMark;

	unsigned long epoch = 0;
  LMark;

	// wait to see if a reply is available
	delay(1000);
  LMark;
	if (udp_dev->parsePacket()) {
   	LMark;
		// We've received a packet, read the data from it
		udp_dev->read(packet_buffer, NTP_PACKET_SIZE); // read the packet into the buffer
  	LMark;

		// the timestamp starts at byte 40 of the received packet and is four bytes,
		// or two words, long. First, extract the two words:
		unsigned long highWord = word(packet_buffer[40], packet_buffer[41]);
   	LMark;
		unsigned long lowWord  = word(packet_buffer[42], packet_buffer[43]);
   	LMark;
		// combine the four bytes (two words) into a long integer
		// this is NTP time (seconds since Jan 1 1900):
		unsigned long secsSince1900 = highWord << 16 | lowWord;
  	LMark;

		// now convert NTP time into everyday time:
		// LPrint("Unix time = ");
		// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
		const unsigned long seventyYears = 2208988800UL;
   	LMark;
		// subtract seventy years:
		epoch = secsSince1900 - seventyYears;
  	LMark;

		print_module_label();
   	LMark;
		print_unix_time(epoch);
  	LMark;
	}
	else LPrint("Failed to parse UDP packet!\n");
  LMark;
	if (epoch > 4131551103UL) {
   	LMark;
		LPrint("Failed to parse UDP packet!\n");
   	LMark;
		return 0;
	}
	return epoch;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomInternetPlat::m_send_NTP_packet(UDP& udp_dev, byte packet_buffer[]) const
{
  LMark;
	// set all bytes in the buffer to 0
	memset(packet_buffer, 0, NTP_PACKET_SIZE);
  LMark;
	// Initialize values needed to form NTP request
	packet_buffer[0] = 0b11100011;	// LI, Version, Mode
  LMark;
	packet_buffer[1] = 0;			// Stratum, or type of clock
  LMark;
	packet_buffer[2] = 6;			// Polling Interval
  LMark;
	packet_buffer[3] = 0xEC;		// Peer Clock Precision
  LMark;
	// 8 bytes of zero for Root Delay & Root Dispersion
	packet_buffer[12] = 49;
  LMark;
	packet_buffer[13] = 0x4E;
  LMark;
	packet_buffer[14] = 49;
  LMark;
	packet_buffer[15] = 52;
  LMark;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	udp_dev.beginPacket(time_server, 123); // NTP requests are to port 123
  LMark;
	udp_dev.write(packet_buffer, NTP_PACKET_SIZE);
  LMark;
	udp_dev.endPacket();
 	LMark;
}
