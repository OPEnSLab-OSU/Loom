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
	BLE.begin();
	BLE.setMode(BLUEFRUIT_MODE_DATA); // set to simple UART back and forth comms
}

///////////////////////////////////////////////////////////////////////////////
Loom_Bluetooth::Loom_Bluetooth(LoomManager* manager, JsonArrayConst p)
	: Loom_Bluetooth(manager, EXPAND_ARRAY(p, 4) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_Bluetooth::add_config(JsonObject json)
{
	// add_config_aux(json, module_name,
	// 	module_name, 
	// 	max_message_len, spi_CS, spi_IRQ, spi_RST
	// );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Bluetooth::print_config() const
{
	LoomCommPlat::print_config();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_Bluetooth::connect(const uint16_t connect_timeout)
{
	uint32_t timeout = millis();

	LPrintln("[BLE_C] timeout: ", timeout );

	while ( !BLE.isConnected() ) {
		if ( ( millis() - timeout ) > connect_timeout ){
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
int8_t Loom_Bluetooth::getCommand(const uint16_t max_timeout )
{
	// printCommands();
	BLE.println("Enter Command.");
	char cmd_buffer [32] = {0};
	uint32_t timeout = millis();

	while ( !BLE.available() ) {
		if ( millis() - timeout > max_timeout ) {
			BLE.println("timed out");
			return -1;
		}
	}

	uint8_t index = 0;

	timeout = millis();
	while ( BLE.available() && index < sizeof(cmd_buffer) - 1) {
		cmd_buffer[index] = BLE.read();
		index++;
	}
	cmd_buffer[index] = 0;
	BLE.println( cmd_buffer );
	
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
	return -2;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Bluetooth::test_send(const uint8_t val)
{
	if ( BLE.isConnected() ) {
		BLE.print(val);
		return true;
	} else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_Bluetooth::test_send_str(const char* string)
{
	if ( BLE.isConnected() ) {
		BLE.print(string);
		return true;
	} else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
