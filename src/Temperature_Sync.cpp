///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Temperature_Sync.cpp
/// @brief		File for LoomTempSync implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Temperature_Sync.h"
#include "Manager.h"
#include "Sensors/Analog.h"

#include "Sensors/I2C/MS5803.h"

///////////////////////////////////////////////////////////////////////////////
LoomTempSync::LoomTempSync(   
		LoomManager* 				manager,
		const LoomModule::Type		source,
		const LoomModule::Type		dependant
	) 
	: LoomModule(manager, "TempSync", Type::TempSync )
	, source_type(source)
	, dependant_type(dependant)
	{}

///////////////////////////////////////////////////////////////////////////////
LoomTempSync::LoomTempSync(LoomManager* manager, JsonArrayConst p)
	: LoomTempSync(manager, (LoomModule::Type)(int)p[0], (LoomModule::Type)(int)p[1] ) {}

/////////////////////////////////////////////////////////////////////////////
void LoomTempSync::second_stage_ctor() 
{
	bool setup = true;

	// Get source and dependant from manager
	if (device_manager != nullptr) {
		// source
		LoomModule* tmp = device_manager->find_module(source_type);
		if (tmp != nullptr) { 
			print_module_label();
			LPrintln("Found source (", (int)source_type, ")");
			source = tmp;
		} else {
			setup = false;
		}
		// dependant
		tmp = device_manager->find_module(dependant_type);
		if (tmp != nullptr) { 
			print_module_label();
			LPrintln("Found dependant (", (int)dependant_type, ")");
			dependant = tmp;
		} else {
			setup = false;
		}
	}

	print_module_label();
	LPrintln("Setup: ", setup);

	// If one or more does not exist, delete self and remove pointer from manager
	if (!setup) active = false;
}

///////////////////////////////////////////////////////////////////////////////
void LoomTempSync::print_config() const
{
	LoomModule::print_config();
	LPrintln("\tSource Type: ", (int)source_type); 
	LPrintln("\tDependant Type: ", (int)dependant_type); 
}

///////////////////////////////////////////////////////////////////////////////
void LoomTempSync::sync_temp() 
{
	if (source && source->get_active() && dependant && dependant->get_active() ) {
		float temp = 25.0;

		if (source_type == LoomModule::Type::MS5803) {
			temp = ( (Loom_MS5803*)source )->get_temperature();
		}

		if ( dependant_type == LoomModule::Type::Analog ) {
			( (Loom_Analog*)dependant )->set_temperature(temp);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
