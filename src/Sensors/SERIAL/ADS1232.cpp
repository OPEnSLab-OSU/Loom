#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "ADS1232.h"
#include "Module_Factory.h"

using namespace Loom;


ADS1232::ADS1232(
                 const char*            module_name,
                 int         num_samples) 
            : SerialSensor(module_name, num_samples){

}

ADS1232::ADS1232(JsonArrayConst p)
: ADS1232(EXPAND_ARRAY(p, 2)){
    
}

void ADS1232::print_measurements() const
{
	
}

///////////////////////////////////////////////////////////////////////////////
void ADS1232::measure()
{
	
}

///////////////////////////////////////////////////////////////////////////////
void ADS1232::package(JsonObject json)
{
   	
}


#endif