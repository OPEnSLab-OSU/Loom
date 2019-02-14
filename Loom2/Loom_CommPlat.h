
#ifndef LOOM_COMM_PLAT_h
#define LOOM_COMM_PLAT_h


#include "Loom_Module.h"

#include <OSCBundle.h>



enum CommPlatform { WIFI, LORA, NRF };
// enum CommScope { SCOPE_DEVICE, SCOPE_SUBNET, SCOPE_FAMILY, SCOPE_SUBNET_ONLY, SCOPE_FAMILY_ONLY, SCOPE_ANY };




class LoomCommPlat : public LoomModule
{

protected:
	uint16_t  max_message_len;		// The maximum message length
	// CommScope subnet_scope;			// The scope to accept messages on       Switch to be ENUM      Maybe move to LoomDevice instead?

	bool      compress_messages;		//
	int16_t   signal_strength; 		// Rssi for Lora (need to determine what the other platforms use)

	// Not sure if address will be in this scope of only the individual implementations

public:

	// --- CONSTRUCTOR ---
	LoomCommPlat(	char*     module_name, 

					uint      max_message_len, 
					// CommScope subnet_scope, 
					bool      compress_messages 
				);


	// --- DESTRUCTOR ---
	virtual ~LoomCommPlat();


	static char* enum_comm_plat_string(CommPlatform c);

	// static char* enum_subnet_scope_string(CommScope s);


	// --- PUBLIC METHODS ---

	virtual void print_config();

	// maybe don't define package and measure in .cpp because they are empty

	virtual void package(OSCBundle* bndl, char* suffix="");
	virtual void measure();
	virtual bool message_route(OSCMessage* msg, int address_offset);


	// Build OSC Bundle from packet if any exists
	virtual bool receive_bundle(OSCBundle *bndl) = 0;

	// Send an OSC bundle
	// Should overload or use default args to optionally allow default address
	virtual bool send_bundle(OSCBundle *bndl, uint16_t destination) = 0;
	virtual bool send_bundle(OSCBundle *bndl) = 0;

	// Broadcast an OSC bundle
	virtual void broadcast_bundle(OSCBundle *bndl) = 0;

	virtual void set_address(uint a) = 0;
	virtual uint get_address() = 0;

	// void set_subnet_scope(CommScope s);
	// CommScope get_subnet_scope();

	void set_compress_messages(bool c);
	bool get_compress_messages();


// move back to protected?

// can probably just return offset and check if -1 to remove need to return bool
	bool scope_filter(OSCBundle* bndl, int* offset);
	bool scope_filter(OSCBundle* bndl);


protected:


	// Fix   
	// returns false if filtered out

// might not actually empty bundle but just return a boolean
	// depends on whether filtering is done at routing or receiving

// need to actually check names
	// unless that is done via routing?

	// bool scope_filter(OSCBundle* bndl);


	void convert_string_to_bundle(char *osc_string, OSCBundle* bndl);

// break compression out to different function
	void convert_bundle_to_string(OSCBundle *bndl, char *osc_string);


private:

	// Conversion without compression
	void original_convert_string_to_bundle(char *osc_string, OSCBundle* bndl);

	// Conversion without decompression
	void original_convert_bundle_to_string(OSCBundle *bndl, char *osc_string);
	// Compress messages 
	void compress_message_string(char* osc_string);

	// Uncompress messages
	void uncompress_message_string(char* osc_string);



};




#endif