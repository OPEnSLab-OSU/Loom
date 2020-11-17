///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Bluetooth.cpp
/// @brief		File for Loom_Bluetooth implementation. Needs more work.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Bluetooth.h"

///////////////////////////////////////////////////////////////////////////////
Loom_Bluetooth::Loom_Bluetooth(
		LoomManager* manager,
		const uint16_t 		max_message_len,
		const uint8_t		spi_CS,
		const uint8_t		spi_IRQ,
		const uint8_t		spi_RST

	)
	: LoomCommPlat(manager, "Bluetooth", Type::Bluetooth, max_message_len )
	, spi_CS(spi_CS)
	, spi_IRQ(spi_IRQ)
	, spi_RST(spi_RST)
	, BLE( Adafruit_BluefruitLE_SPI(spi_CS, spi_IRQ, spi_RST) )

{
  LMark;
	BLE.begin();
  LMark;
	BLE.setMode(BLUEFRUIT_MODE_DATA); // set to simple UART back and forth comms
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_Bluetooth::Loom_Bluetooth(LoomManager* manager, JsonArrayConst p)
	: Loom_Bluetooth(manager, EXPAND_ARRAY(p, 4) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Bluetooth::add_config(JsonObject json)
{
  LMark;
	// add_config_aux(json, module_name,
	// 	module_name,
	// 	max_message_len, spi_CS, spi_IRQ, spi_RST
	// );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Bluetooth::print_config() const
{
  LMark;
	LoomCommPlat::print_config();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Bluetooth::connect(const uint16_t connect_timeout)
{
  LMark;
	uint32_t timeout = millis();
  LMark;

	LPrintln("[BLE_C] timeout: ", timeout );
  LMark;

	while ( !BLE.isConnected() ) {
   	LMark;
		if ( ( millis() - timeout ) > connect_timeout ){
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
int8_t Loom_Bluetooth::getCommand(const uint16_t max_timeout )
{
  LMark;
	// printCommands();
	BLE.println("Enter Command.");
  LMark;
	char cmd_buffer [32] = {0};
	uint32_t timeout = millis();
  LMark;

	while ( !BLE.available() ) {
   	LMark;
		if ( millis() - timeout > max_timeout ) {
    	LMark;
			BLE.println("timed out");
    	LMark;
			return -1;
		}
	}

	uint8_t index = 0;
  LMark;

	timeout = millis();
  LMark;
	while ( BLE.available() && index < sizeof(cmd_buffer) - 1) {
   	LMark;
		cmd_buffer[index] = BLE.read();
   	LMark;
		index++;
  	LMark;
	}
	cmd_buffer[index] = 0;
  LMark;
	BLE.println( cmd_buffer );
 	LMark;

	// for( int i = 0; i< NUMCOMMANDS; i++)
	// {
	// 	int cmp = 0;
	// 	cmp = strncmp_P(  cmd_buffer, (char*)pgm_read_word( &(COMMAND_TABLE[i]) ),
	// 		strlen(cmd_buffer) - 1 );
	// 	if ( cmp == 0 )
	// 	{
	// 		if ( i == CMD_QUIT2 )
	// 		{
	// 			i = CMD_QUIT;
	// 		}
	// 		return i;
	// 	}

	// }
	delay(5000);
  LMark;
	return -2;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Bluetooth::test_send(const uint8_t val)
{
  LMark;
	if ( BLE.isConnected() ) {
   	LMark;
		BLE.print(val);
   	LMark;
		return true;
	} else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Bluetooth::test_send_str(const char* string)
{
  LMark;
	if ( BLE.isConnected() ) {
   	LMark;
		BLE.print(string);
   	LMark;
		return true;
	} else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
