// ///////////////////////////////////////////////////////////////////////////////
// ///
// /// @file		Temperature_Sync.cpp
// /// @brief		File for TempSync implementation.
// /// @author		Luke Goertzen
// /// @date		2019
// /// @copyright	GNU General Public License v3.0
// ///
// ///////////////////////////////////////////////////////////////////////////////

// #ifdef LOOM_INCLUDE_SENSORS

// #include "TemperatureSync.h"
// #include "Manager.h"
// #include "Sensors/Analog.h"
// #include "Sensors/I2C/MS5803.h"
// #include "Module_Factory.h"

// using namespace Loom;

// ///////////////////////////////////////////////////////////////////////////////
// TempSync::TempSync(   
// 		const Module::Type		source,
// 		const Module::Type		dependant
// 	) 
// 	: Module("TempSync")
// 	, source_type(source)
// 	, dependant_type(dependant)
// {}

// ///////////////////////////////////////////////////////////////////////////////
// TempSync::TempSync(JsonArrayConst p)
// 	: TempSync( (Module::Type)(int)p[0], (Module::Type)(int)p[1] ) {}

// /////////////////////////////////////////////////////////////////////////////
// void TempSync::second_stage_ctor() 
// {
// 	bool setup = true;

// 	// Get source and dependant from manager
// 	if (device_manager != nullptr) {
// 		// source
// 		Module* tmp = device_manager->find_module(source_type);
// 		if (tmp != nullptr) { 
// 			print_module_label();
// 			LPrintln("Found source (", (int)source_type, ")");
// 			source = tmp;
// 		} else {
// 			setup = false;
// 		}
// 		// dependant
// 		tmp = device_manager->find_module(dependant_type);
// 		if (tmp != nullptr) { 
// 			print_module_label();
// 			LPrintln("Found dependant (", (int)dependant_type, ")");
// 			dependant = tmp;
// 		} else {
// 			setup = false;
// 		}
// 	}

// 	print_module_label();
// 	LPrintln("Setup: ", setup);

// 	// If one or more does not exist, delete self and remove pointer from manager
// 	if (!setup) active = false;
// }

// ///////////////////////////////////////////////////////////////////////////////
// void TempSync::print_config() const
// {
// 	Module::print_config();
// 	LPrintln("\tSource Type: ", (int)source_type); 
// 	LPrintln("\tDependant Type: ", (int)dependant_type); 
// }

// ///////////////////////////////////////////////////////////////////////////////
// void TempSync::sync_temp() 
// {
// 	if (source && source->get_active() && dependant && dependant->get_active() ) {
// 		float temp = 25.0;

// 		if (source_type == Module::Type::MS5803) {
// 			temp = ( (MS5803*)source )->get_temperature();
// 		}

// 		if ( dependant_type == Module::Type::Analog ) {
// 			( (Analog*)dependant )->set_temperature(temp);
// 		}
// 	}
// }

// ///////////////////////////////////////////////////////////////////////////////

// #endif // ifdef LOOM_INCLUDE_SENSORS
