
#include "Loom_Actuator.h"
// #include "Loom_Device.h"
// #include "Loom_Macros.h"


// --- CONSTRUCTOR ---
LoomActuator::LoomActuator( char* module_name ) : LoomModule( module_name ) 
{
	// LOOM_DEBUG_Println("LoomActuator Constructor");
}

// --- DESTRUCTOR ---
LoomActuator::~LoomActuator() 
{

}


// --- PUBLIC METHODS ---

void LoomActuator::print_config() 
{
	LoomModule::print_config();
}


// void LoomActuator::measure() 
// {

// }




