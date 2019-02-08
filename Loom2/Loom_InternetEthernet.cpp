
#include "Loom_InternetEthernet.h"

// int test = 0;




// --- CONSTRUCTOR ---
Loom_Ethernet_I::Loom_Ethernet_I(	char* module_name,

									byte 	mac[6],
									byte 	ip[4]

				) : LoomInternetPlat( module_name )
{
	client = new EthernetClient();
	UDP    = new EthernetUDP();

	for (int i = 0; i < 6; i++)
		this->mac[i] = mac[i];
	this->ip = IPAddress(ip[0], ip[1], ip[2], ip[3]); 

	connect();

	// if (client->connect("www.google.com", 80)) {
	// 	LOOM_DEBUG_Println("Successfully connected to internet");
	// 	client->stop();
	// } else {
	// 	LOOM_DEBUG_Println("Failed to connect to internet");
	// }
		// UDP->begin(8888);


	// LOOM_DEBUG_Println2("Connected : ", is_connected());
	// LOOM_DEBUG_Println2("Time      : ", get_time());
	// LOOM_DEBUG_Println2("Time      : ", get_time());


}

// --- DESTRUCTOR ---
Loom_Ethernet_I::~Loom_Ethernet_I()
{
	delete client;
	delete UDP;
}


void Loom_Ethernet_I::print_config()
{
	LoomInternetPlat::print_config();
	LOOM_DEBUG_Print2('\t', "MAC:                : [");
	for (int i = 0; i < 6; i++) {
		LOOM_DEBUG_Print_Hex(mac[i]);
		if (i < 5) LOOM_DEBUG_Print(",");
		else 	   LOOM_DEBUG_Println("]");
	}

	LOOM_DEBUG_Print2('\t', "IP:                 : " );
	LOOM_DEBUG_Print4(  ip[0], ".", ip[1], ".");
	LOOM_DEBUG_Println3(ip[2], ".", ip[3]);
	
}


void Loom_Ethernet_I::print_state()
{
	LoomInternetPlat::print_state();	
	// LOOM_DEBUG_Println3('\t', "Connected:          : ", (is_connected()) ? "True" : "False" );
}


bool Loom_Ethernet_I::connect()
{
	digitalWrite(8, HIGH);	 // if is LoRa board	

	if (Ethernet.begin(mac) == 0) {
		print_module_label();
		LOOM_DEBUG_Println("Failed to configure Ethernet using DHCP");
		// try to congifure using IP address instead of DHCP:
		Ethernet.begin(mac, ip);
	}
	
	bool status = is_connected();
	print_module_label();
	LOOM_DEBUG_Println2("Connect to internet ", (status) ? "successful" : "failed" );

	if (!status) client->stop();
	
	return status;
}


bool Loom_Ethernet_I::is_connected()
{

	bool status = client->connect("www.google.com", 80);
		LOOM_DEBUG_Println2("CONNECTED STATUS: ", client->connected() );

	// print_module_label();
	// LOOM_DEBUG_Println2("Connect to internet ", (status) ? "successful" : "failed" );
	return status;
}





const unsigned int localPort = 8888;		// Local port to listen for UDP packets on

const char time_server[] = "time.nist.gov"; 	// time.nist.gov NTP server
const int NTP_PACKET_SIZE = 48; 			// NTP time stamp is in the first 48 bytes of the message
// byte packet_buffer[NTP_PACKET_SIZE]; 		//buffer to hold incoming and outgoing packets



void print_unix_time(unsigned long epoch) 
{
	// print Unix time:
	LOOM_DEBUG_Println2("Epoch:", epoch);


	// print the hour, minute and second:
	// LOOM_DEBUG_Print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
	LOOM_DEBUG_Print2((epoch  % 86400L) / 3600, ":"); // print the hour (86400 equals secs per day)
	if (((epoch % 3600) / 60) < 10) {
		// In the first 10 minutes of each hour, we'll want a leading '0'
		LOOM_DEBUG_Print('0');
	}
	LOOM_DEBUG_Print2((epoch  % 3600) / 60, ":"); // print the minute (3600 equals secs per minute)
	if ((epoch % 60) < 10) {
		// In the first 10 seconds of each minute, we'll want a leading '0'
		LOOM_DEBUG_Print('0');
	}
	LOOM_DEBUG_Println(epoch % 60); // print the second
}


uint32_t Loom_Ethernet_I::get_time()
{
	UDP->begin(localPort);

	// connect();

	if (!is_connected()) return 0;


	byte packet_buffer[NTP_PACKET_SIZE]; 		//buffer to hold incoming and outgoing packets

	send_NTP_packet(packet_buffer); // send an NTP packet to a time server

	unsigned long epoch = 0;

	// wait to see if a reply is available
	delay(1000);
	if (UDP->parsePacket()) {
		// We've received a packet, read the data from it
		UDP->read(packet_buffer, NTP_PACKET_SIZE); // read the packet into the buffer

		// the timestamp starts at byte 40 of the received packet and is four bytes,
		// or two words, long. First, extract the two words:
		unsigned long highWord = word(packet_buffer[40], packet_buffer[41]);
		unsigned long lowWord  = word(packet_buffer[42], packet_buffer[43]);
		// combine the four bytes (two words) into a long integer
		// this is NTP time (seconds since Jan 1 1900):
		unsigned long secsSince1900 = highWord << 16 | lowWord;
		// LOOM_DEBUG_Print("Seconds since Jan 1 1900 = ");
		LOOM_DEBUG_Println(secsSince1900);

		// now convert NTP time into everyday time:
		// LOOM_DEBUG_Print("Unix time = ");
		// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
		const unsigned long seventyYears = 2208988800UL;
		// subtract seventy years:
		epoch = secsSince1900 - seventyYears;
		
		print_unix_time(epoch);
	}

	// wait ten seconds before asking for the time again
	// delay(10000);
	// Ethernet.maintain();

	UDP->stop();

	return epoch;

}


void Loom_Ethernet_I::send_NTP_packet(byte packet_buffer[])
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
	UDP->beginPacket(time_server, 123); // NTP requests are to port 123
	UDP->write(packet_buffer, NTP_PACKET_SIZE);
	UDP->endPacket();
}


// bool Loom_Ethernet_I::log_to_pushingbox(OSCBundle* bndl)
// {
	
// }
