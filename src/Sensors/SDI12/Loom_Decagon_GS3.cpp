
#include "Loom_Decagon_GS3.h"


///////////////////////////////////////////////////////////////////////////////
Loom_DecagonGS3::Loom_DecagonGS3(	
		uint8_t			num_samples 
	) 
	: LoomSDI12Sensor( "GS3", Type::DecagonGS3, num_samples ) 
{

}

///////////////////////////////////////////////////////////////////////////////
Loom_DecagonGS3::Loom_DecagonGS3(JsonArrayConst p)
	: Loom_DecagonGS3( (uint8_t)p[0] ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_DecagonGS3::print_config()
{
	LoomSDI12Sensor::print_config();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_DecagonGS3::print_measurements()
{

}

///////////////////////////////////////////////////////////////////////////////
void Loom_DecagonGS3::measure()
{

}

///////////////////////////////////////////////////////////////////////////////
void Loom_DecagonGS3::package(JsonObject json)
{
	// package_json(json, module_name, "Temp", temperature);
}

///////////////////////////////////////////////////////////////////////////////


