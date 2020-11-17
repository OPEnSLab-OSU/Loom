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
  LMark;
	bool setup = true;
  LMark;

	// Get source and dependant from manager
	if (device_manager != nullptr) {
   	LMark;
		// source
		LoomModule* tmp = device_manager->find_module(source_type);
   	LMark;
		if (tmp != nullptr) {
    	LMark;
			print_module_label();
    	LMark;
			LPrintln("Found source (", (int)source_type, ")");
    	LMark;
			source = tmp;
   		LMark;
		} else {
			setup = false;
   		LMark;
		}
		// dependant
		tmp = device_manager->find_module(dependant_type);
   	LMark;
		if (tmp != nullptr) {
    	LMark;
			print_module_label();
    	LMark;
			LPrintln("Found dependant (", (int)dependant_type, ")");
    	LMark;
			dependant = tmp;
   		LMark;
		} else {
			setup = false;
   		LMark;
		}
	}

	print_module_label();
  LMark;
	LPrintln("Setup: ", setup);
  LMark;

	// If one or more does not exist, delete self and remove pointer from manager
	if (!setup) active = false;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomTempSync::print_config() const
{
  LMark;
	LoomModule::print_config();
  LMark;
	LPrintln("\tSource Type: ", (int)source_type);
  LMark;
	LPrintln("\tDependant Type: ", (int)dependant_type);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void LoomTempSync::sync_temp()
{
  LMark;
	if (source && source->get_active() && dependant && dependant->get_active() ) {
   	LMark;
		float temp = 25.0;
  	LMark;

		if (source_type == LoomModule::Type::MS5803) {
    	LMark;
			temp = ( (Loom_MS5803*)source )->get_temperature();
   		LMark;
		}

		if ( dependant_type == LoomModule::Type::Analog ) {
    		LMark;
			( (Loom_Analog*)dependant )->set_temperature(temp);
   			LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
