
#include "Loom_Bluetooth.h"


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_Bluetooth::Loom_Bluetooth( 	
		const char*		module_name,
		int 			max_message_length,
		uint			spi_CS,
		uint			spi_IRQ,
		uint			spi_RST

	)
	: LoomCommPlat( module_name, max_message_length )

{ 
	this->module_type = ModuleType::Bluetooth;


	BLE = new Adafruit_BluefruitLE_SPI(spi_CS, spi_IRQ, spi_RST);

	BLE->begin();
	BLE->setMode(BLUEFRUIT_MODE_DATA); // set to simple UART back and forth comms
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_Bluetooth::Loom_Bluetooth(JsonVariant p)
	// : Loom_Bluetooth(p[0], p[1], p[2], p[3], p[4])
	: Loom_Bluetooth( ARR_EXPAND(p, 5) )
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_Bluetooth::~Loom_Bluetooth() 
{
}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void Loom_Bluetooth::print_config() 
{
	LoomCommPlat::print_config();

}



/////////////////////////////////////////////////////////////////////
void Loom_Bluetooth::connect(uint16_t connect_timeout)
{
	uint32_t timeout = millis();

	LPrintln("[BLE_C] timeout: ", timeout );

	while ( !BLE->isConnected() ) {
		if ( ( millis() - timeout ) > connect_timeout ){
			return;
		}
	}
}





/*
Get Command

Gets a command from the ble interface, or times out. This is used in combination with
parseCommand if the return value is valid.

Returns:
-1: user never wrote a command, timed out
-2: user entered an invalid command
0 or positive integer: index in COMMAND_TABLE that matches the user-entered command

*/

int8_t Loom_Bluetooth::getCommand( uint16_t max_timeout )
{
	// printCommands();
	BLE->println("Enter Command.");
	char cmd_buffer [32] = {0};
	uint32_t timeout = millis();

	while ( !BLE->available() ) {
		if ( millis() - timeout > max_timeout ) {
			BLE->println("timed out");
			return -1;
		}
	}

	uint8_t index = 0;

	timeout = millis();
	while ( BLE->available() && index < sizeof(cmd_buffer) - 1) {
		cmd_buffer[index] = BLE->read();
		index++;
	}
	cmd_buffer[index] = 0;
	BLE->println( cmd_buffer );
	
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





/////////////////////////////////////////////////////////////////////

bool Loom_Bluetooth::test_send(const uint8_t val)
{
	if ( BLE->isConnected() ) {
		BLE->print(val);
		return true;
	} else {
		return false;
	}
}


bool Loom_Bluetooth::test_send_str(const char* string)
{
	if ( BLE->isConnected() ) {
		BLE->print(string);
		return true;
	} else {
		return false;
	}
}



/////////////////////////////////////////////////////////////////////
void Loom_Bluetooth::set_address(uint addr)    // Need to test this
{ 

}

/////////////////////////////////////////////////////////////////////
uint Loom_Bluetooth::get_address() 
{ 
}


