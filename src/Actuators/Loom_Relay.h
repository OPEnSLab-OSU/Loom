
#ifndef LOOM_RELAY_h
#define LOOM_RELAY_h


#include "Loom_Actuator.h"


// ### (LoomActuator) | dependencies: [] | conflicts: []
/// Relay actuator module
// ###
class Loom_Relay : public LoomActuator
{

protected:

	/// The digital pin the relay is connect to
	byte		pin;

	/// Whether the relay is on or off
	bool		on;

public:

	/// Relay module constructor.
	///
	/// \param[in]	module_name		String | <"Relay"> | {true, false} | Relay module name
	/// \param[in]	pin						Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used to control the relay
	Loom_Relay(
			const char*		module_name		= "Relay",
			byte			pin				= 10
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_Relay();

	// Inherited (Overriding) Methods
	void		print_config() override;
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
