#pragma once

#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>


#include "Loom_CommPlat.h"

// ### (LoomCommPlat) | dependencies: [] | conflicts: []
/// Bluetooth communication platform module
// ###
class Loom_Bluetooth : public LoomCommPlat
{

protected:

	Adafruit_BluefruitLE_SPI* BLE;


public:

	/// Bluetooth module constructor
	///
	/// \param[in]	module_name					String | <"Bluetooth"> | LoRa module name
	Loom_Bluetooth(
			const char*		module_name			= "Bluetooth",
			int 			max_message_length	= 120,

			uint			spi_CS				= 8,
			uint			spi_IRQ				= 7,
			uint			spi_RST				= 4
		);

	Loom_Bluetooth(JsonVariant p);

	/// Destructor
	virtual ~Loom_Bluetooth();




	void		print_config() override;


	void	 	connect(uint16_t connect_timeout);
	uint8_t		receiveCommand();
	int8_t		getCommand( uint16_t max_timeout );


	bool		test_send(const uint8_t val);
	bool		test_send_str(const char* string);

	void		set_address(uint addr) {}
	uint		get_address() {}


	bool		receive(JsonObject json) {}
	bool		send(JsonObject json, uint16_t destination) {}
	bool		send(JsonObject json) {}


private:


};

