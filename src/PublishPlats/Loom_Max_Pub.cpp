
#include "Loom_Max_Pub.h"

///////////////////////////////////////////////////////////////////////////////
Loom_MaxPub::Loom_MaxPub(
		// uint8_t		internet_index,
		LoomModule::Type	internet_type,
		uint16_t	port
	)   
	// : LoomPublishPlat( "MaxPub", Type::MaxPub, internet_index )
	: LoomPublishPlat( "MaxPub", Type::MaxPub, internet_type )
	, UDP_port(port)
	// , UDP_Inst( (m_internet != nullptr) ? m_internet->open_socket(port) : nullptr )
{

}	

///////////////////////////////////////////////////////////////////////////////
// void Loom_MaxPub::second_stage_ctor() 
// {
	// superclass LoomPublishPlat will have obtained pointer to internet module if possible 

	// Get UDP object
	// if (m_internet != nullptr) {
	// 	UDP_Inst = m_internet->open_socket(UDP_port);
	// } else {
	// 	print_module_label();
	// 	LPrintln("Has no internet module to get UDP instance from");
	// }
// }

///////////////////////////////////////////////////////////////////////////////
Loom_MaxPub::Loom_MaxPub(JsonArrayConst p)
	// : Loom_MaxPub( EXPAND_ARRAY(p, 2) ) {}
	: Loom_MaxPub( (LoomModule::Type)(int)p[0], p[1] ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxPub::print_config()
{
	LoomPublishPlat::print_config();
	LPrintln("\tUDP Port : ", UDP_port);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxPub::print_state()
{
	LoomPublishPlat::print_state();

}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxPub::set_port(uint16_t port)
{
	UDP_port = port;

	// Get new UDP pointer	
	// if (m_internet != nullptr) {
	// 	UDP_Inst = m_internet->open_socket(UDP_port);
	// }
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_MaxPub::send_to_internet(const JsonObject json, LoomInternetPlat* plat)
{
	// Get UDP object
	auto UDP_Inst = (m_internet != nullptr) ? m_internet->open_socket(UDP_port) : nullptr;
	// if (m_internet != nullptr) {
	// 	LPrintln("Got UDP");
	// } else {
		// print_module_label();
		// LPrintln("Has no internet module to get UDP instance from");
		// return false;
	// }	

	delay(50);

	// Send Json
	// serializeJson(json, std::ostream& output);
	
	if (!UDP_Inst) {
		LPrintln("UDP_Inst was null");
		return false;
	}

	// serializeJson(json, buffer );

	UDP_Inst->beginPacket("255.255.255.255", UDP_port);
	// UDP_Inst->beginPacket("192.168.1.2", UDP_port);

	serializeJson(json, (*UDP_Inst) );
// 
	// UDP_Inst->write( (const uint8_t*)"testStringA", 12 );
	// UDP_Inst->write( (const uint8_t*)"'testStrin'", 12 );

	UDP_Inst->endPacket();		// Mark the end of the OSC Packet




	// bndl->send(UdpDevice);    	// Send the bytes to the SLIP stream



}


///////////////////////////////////////////////////////////////////////////////
