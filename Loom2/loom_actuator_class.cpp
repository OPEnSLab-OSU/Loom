
#include "loom_actuator_class.h"
#include "loom_device_class.h"

#include "loom_macros.h"


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


void LoomActuator::measure() 
{

}




