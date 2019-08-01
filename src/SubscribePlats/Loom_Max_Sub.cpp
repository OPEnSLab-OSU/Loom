
#include "Loom_Max_Sub.h"

///////////////////////////////////////////////////////////////////////////////
Loom_MaxSub::Loom_MaxSub(
		LoomModule::Type	internet_type,
		uint16_t	port
	)   
	: LoomSubscribePlat( "MaxSub", Type::MaxSub, internet_type )
	, UDP_port(port)
{} 

///////////////////////////////////////////////////////////////////////////////
Loom_MaxSub::Loom_MaxSub(JsonArrayConst p) 
	: Loom_MaxSub( (LoomModule::Type)(int)p[0], p[1] ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxSub::second_stage_ctor() 
{
	LoomSubscribePlat::second_stage_ctor();

	// Get new UDP pointer	
	if (m_internet != nullptr) {
		UDP_Inst = m_internet->open_socket(UDP_port);
	} else {
		print_module_label();
		LPrintln("No internet platform, could not get UDP object");
	}
}


///////////////////////////////////////////////////////////////////////////////
bool Loom_MaxSub::subscribe(JsonObject json)
{
	if (!UDP_Inst) {
		// Try to get UDP 
		UDP_Inst = (m_internet != nullptr) ? m_internet->open_socket(UDP_port) : nullptr;
		// Check if still null
		if (!UDP_Inst) {
			LPrintln("UDP_Inst is still null");
			return false;
		}
	}

	if ( UDP_Inst->parsePacket() ) {

		messageJson.clear();
		if ( deserializeJson(messageJson, (*UDP_Inst) ) != DeserializationError::Ok ) {
			print_module_label();
			LPrintln("Failed to parse MsgPack");
			return false;
		}

		bool status = json.set(messageJson.as<JsonObject>());
		if (!status) {
			LPrintln("Json set error");	
			return false;
		}

		if (print_verbosity == Verbosity::V_HIGH) {
			print_module_label();
			LPrintln("Internal messageJson:");
			serializeJsonPretty(messageJson, Serial);
			LPrintln();
		}

		return true;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxSub::print_config()
{
	LoomSubscribePlat::print_config();
	LPrintln("\tUDP Port : ", UDP_port);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MaxSub::set_port(uint16_t port)
{
	UDP_port = port;

	if (m_internet != nullptr) {
		UDP_Inst = m_internet->open_socket(UDP_port);
	} else {
		print_module_label();
		LPrintln("No internet platform, could not get UDP object");
	}
}

///////////////////////////////////////////////////////////////////////////////

