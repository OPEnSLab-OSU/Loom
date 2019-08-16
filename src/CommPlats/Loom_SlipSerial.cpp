
#include "Loom_SlipSerial.h"


///////////////////////////////////////////////////////////////////////////////
Loom_SlipSerial::Loom_SlipSerial(
		uint16_t		max_message_len
	) 
	: LoomCommPlat("SlipSerial", Type::SlipSerial, max_message_len )
{
	// this->module_type = LoomModule::Type::SlipSerial;

	///Begin Setup Telemetry				


	
//	Uart Serial3 (&sercom5, SERIAL3_RX, SERIAL3_TX, SERCOM_RX_PAD_0, UART_TX_PAD_2);
//	void SERCOM5_Handler()
//	{
//	  Serial3.IrqHandler();
//	}
//
//	SLIPEncodedSerial SLIPSerial(Serial3);
}

///////////////////////////////////////////////////////////////////////////////
Loom_SlipSerial::Loom_SlipSerial(JsonArrayConst p)
	: Loom_SlipSerial( (uint16_t)p[0] ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_SlipSerial::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name, max_message_len
	// );
}

///////////////////////////////////////////////////////////////////////////////