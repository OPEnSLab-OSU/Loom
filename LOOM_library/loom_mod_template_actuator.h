// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
// #include <ExampleLibrary.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_<module>_t {
	// int  stored_int;
	// char stored_string[32];
};

struct state_<module>_t {
	// float some_value;
	// int   array_example[3];

};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_<module>_t config_<module>;
struct state_<module>_t  state_<module>;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_<module>();
void handle_<module>(OSCMessage &msg);


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any startup for <module> that should occur on device startup
// 
// @return  Whether or not sensor initialization was successful
//
bool setup_<module>() 
{

}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- HANDLE <MODULE> --- (Multiplexer Version)
//
// Takes OSC Message forwarded by the message router from another
// device or the Max interfaces
//
// @param msg  The message containing an actuator command
void handle_<module>(OSCMessage &msg) 
{
	// Example of what measure might look like:

	int module_num  = msg.getInt(0);
	int amount = msg.getInt(1);
	
	LOOM_DEBUG_Print("received message for <module ");
	LOOM_DEBUG_Print(module_num);
	LOOM_DEBUG_Print(" with degree ");
	LOOM_DEBUG_Println(amount);
	

	// Use the received value to set / control the actuator 
	//  in some way
	// This can be done here or, preferred, done via an auxiliary
	//  function that take individual value rather than an 
	//  OSC Message

	set_module(module_num, amount);

}







