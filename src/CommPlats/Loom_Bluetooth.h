///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Bluetooth.h
/// @brief		File for the Loom_Bluetooth definition. Needs more work.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

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

	Adafruit_BluefruitLE_SPI BLE;	///< Bluetooth manager instance

	uint8_t spi_CS;		///< SPI chip select pin
	uint8_t spi_IRQ;	///< SPI IRQ pin
	uint8_t spi_RST;	///< SPI reset pin

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// \param[in] 	max_message_len		The maximum possible message length
	/// \param[in] 	spi_CS				SPI chip select pin
	/// \param[in] 	spi_IRQ				SPI IRQ pin
	/// \param[in] 	spi_RST				SPI reset pin
	Loom_Bluetooth(
			uint16_t 		max_message_len		= 120,
			uint8_t			spi_CS				= 8,
			uint8_t			spi_IRQ				= 7,
			uint8_t			spi_RST				= 4
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Bluetooth(JsonArrayConst p);

	/// Destructor
	~Loom_Bluetooth() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Currently no implementation
	bool		receive(JsonObject json) override {}

	/// Currently no implementation
	bool		send(JsonObject json, uint8_t destination) override {}
	void		add_config(JsonObject json) override;

	/// Allow a phone to connect
	/// \param[in] connect_timeout		Max time to allow user to try to connect
	void	 	connect(uint16_t connect_timeout);

	/// Receive a command from user
	/// \return Status 
	// uint8_t		receiveCommand();


	/// Gets a command from the ble interface, or times out. 
	/// This is used in combination with parseCommand if the return value is valid.
	/// \param[in]	max_timeout		Maximum number of milliseconds to wait for command
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

	/// Bluetooth currently does not use addresses.
	/// Override with empty body
	uint8_t		get_address() override {}

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Bluetooth currently does not use addresses.
	/// Override with empty body
	void		set_address(uint8_t addr) override {}

private:

};

