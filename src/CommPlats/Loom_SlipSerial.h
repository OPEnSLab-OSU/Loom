#pragma once

#include "Loom_CommPlat.h"

#include <SLIPEncodedSerial.h>


#define SERIAL3_RX	A5				/// Pin used for SlipSerial RX
#define SERIAL3_TX	6				/// Pin used for SlipSerial TX

#define BAUD_RATE	115200			/// Baud rate used for communication
#define SERIAL_MAX_MESSAGE_LEN	15	/// Maximum size a message can be


///////////////////////////////////////////////////////////////////////////////


// ### (LoomLogPlat) | dependencies: [] | conflicts: []
/// SlipSerial communication platform module
// ###
class Loom_SlipSerial : public LoomCommPlat
{

protected:
	
	uint baud_rate = BAUD_RATE;		/// Baud rate

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// SlipSerial module constructor.
	///
	/// \param[in]	max_message_len			Set(Int) | <15> | {15("Max length")} | The maximum possible message length
	Loom_SlipSerial(
			uint16_t		max_message_len		= SERIAL_MAX_MESSAGE_LEN
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_SlipSerial(JsonArrayConst p);

	//// Destructor
	~Loom_SlipSerial() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	bool		receive(JsonObject json) override {}
	bool		send(JsonObject json, uint8_t destination) override {}
	void		add_config(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void 		print_config() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	uint8_t		get_address() override;

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	void 		set_address(uint8_t addr) override;

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

private:

};

