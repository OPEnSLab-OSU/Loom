#ifndef LOOM_SLIPSERIAL_H
#define LOOM_SLIPSERIAL_H

#include "Loom_CommPlat.h"

#include <SLIPEncodedSerial.h>


/// Slip Serial Pins
#define SERIAL3_RX	A5	///< Pin used for SlipSerial RX
#define SERIAL3_TX	6	///< Pin used for SlipSerial TX

#define BAUD_RATE	115200	///< Buad rate used for communication
#define SERIAL_MAX_MESSAGE_LEN	15 ///< Maximum size a message can be (OSCBundle)



class Loom_SlipSerial : public LoomCommPlat
{

protected:

	/// Baud rate
	uint baud_rate = BAUD_RATE;

public:
	
	/// Constructor
	Loom_SlipSerial(char* 	module_name 		= "SlipSerial",
					uint 	max_message_len 	= SERIAL_MAX_MESSAGE_LEN,
					bool 	compress_messages 	= true
					); 
	/// Destructor	
	~Loom_SlipSerial();

	// Inherited (Overriding) Methods
	void print_config(); ///< Outputs Configuration of CommPlat
	bool receive_bundle(OSCBundle& bndl); ///< Build OSC Bundle from packet if any exists
	bool send_bundle(OSCBundle& bndl); ///< Send an OSC Bundle
	void broadcast_bundle(OSCBundle& bndl); ///< Broadcast an OSC bundle
	void set_address(uint addr);
	uint get_address();

private:	

};

#endif
