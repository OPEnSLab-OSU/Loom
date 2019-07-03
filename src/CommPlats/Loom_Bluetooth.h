#pragma once

#include "Loom_CommPlat.h"

#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_BluefruitLE_UART.h>


///////////////////////////////////////////////////////////////////////////////


// ### (LoomCommPlat) | dependencies: [] | conflicts: []
/// Bluetooth communication platform module
// ###
class Loom_Bluetooth : public LoomCommPlat
{

protected:

	Adafruit_BluefruitLE_SPI* BLE;		/// Bluetooth manager instance

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

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

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Bluetooth(JsonVariant p);

	/// Destructor
	~Loom_Bluetooth() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool		receive(JsonObject json) override {}
	bool		send(JsonObject json, uint16_t destination) override {}
	bool		send(JsonObject json) override {}

	/// Allow a phone to connect
	/// \param[in] connect_timeout		Max time to allow user to try to connect
	void	 	connect(uint16_t connect_timeout);

	/// Receive a command from user
	/// \return Status 
	// uint8_t		receiveCommand();


	/// Gets a command from the ble interface, or times out. 
	/// This is used in combination with parseCommand if the return value is valid.
	/// \return Status where:
	/// -1: user never wrote a command, timed out
	/// -2: user entered an invalid command
	/// 0 or positive integer: index in COMMAND_TABLE that matches the user-entered command
	int8_t		getCommand( uint16_t max_timeout );

//=============================================================================
///@name	TESTING METHODS
/*@{*/ //======================================================================

	bool		test_send(const uint8_t val);
	bool		test_send_str(const char* string);

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Bluetooth currently does not use addresses
	uint		get_address() override {}

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Bluetooth currently does not use addresses
	void		set_address(uint addr) override {}

private:

};

