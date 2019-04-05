
#ifndef LOOM_RELAY_h
#define LOOM_RELAY_h


#include "Loom_Actuator.h"


/// Relay actuator module
class Loom_Relay : public LoomActuator
{

protected:

	/// The digital pin the relay is connect to
	byte		pin;

	/// Whether the relay is on or off
	bool		on;	

public:

	// --- CONSTRUCTOR ---
	Loom_Relay( 
			char*		module_name		=  "Relay",  // RELAY_Module_Name,

			byte		pin				= 10  // RELAY_Pin 
	   );

	// --- DESTRUCTOR ---
	virtual ~Loom_Relay();

	// Inherited (Overriding) Methods
	void		print_config();
	void		print_state();
	void		package(OSCBundle& bndl, char* suffix="");
	bool		message_route(OSCMessage& msg, int address_offset);

	// --- Actuator Control --- 

	/// Set relay state
	/// \param[in]	state	The state to set relay to (True=HIGH, False=LOW)
	void		set_relay(bool state);

	/// Sest relay state.
	/// Settings enclosed in message, forwards to set_relay with bool arg
	/// \param[in]	msg		The message to parse
	void		set_relay(OSCMessage& msg);

private:

};


#endif // of LOOM_RELAY_h
