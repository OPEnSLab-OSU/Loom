#ifndef LOOM_SLIPSERIAL_H
#define LOOM_SLIPSERIAL_H

#include "Loom_CommPlat.h"

#include <SLIPEncodedSerial.h>

/// Pin used for SlipSerial RX
#define SERIAL3_RX	A5	
/// Pin used for SlipSerial TX
#define SERIAL3_TX	6	

/// Baud rate used for communication
#define BAUD_RATE	115200	
/// Maximum size a message can be (OSCBundle)
#define SERIAL_MAX_MESSAGE_LEN	15 



/// SlipSerial communication platform module
class Loom_SlipSerial : public LoomCommPlat
{

protected:

	/// Baud rate
	uint baud_rate = BAUD_RATE;

public:
	
	/// Constructor
	Loom_SlipSerial(
			char*	module_name			= "SlipSerial",

			uint	max_message_len		= SERIAL_MAX_MESSAGE_LEN,
			bool	compress_messages	= true
		); 

	/// Destructor	
	~Loom_SlipSerial();

	// Inherited (Overriding) Methods
	void print_config();
	bool receive_bundle(OSCBundle& bndl); 
	bool send_bundle(OSCBundle& bndl);
	void broadcast_bundle(OSCBundle& bndl);
	void set_address(uint addr);
	uint get_address();

private:	

};

#endif
