// ///////////////////////////////////////////////////////////////////////////////
// ///
// /// @file		Loom_Temperature_Sync.cpp
// /// @brief		File for TempSync implementation.
// /// @author		Luke Goertzen
// /// @date		2019
// /// @copyright	GNU General Public License v3.0
// ///
// ///////////////////////////////////////////////////////////////////////////////
//
// #ifdef LOOM_INCLUDE_SENSORS
//
// #include "TemperatureSync.h"
// #include "Manager.h"
// #include "Sensors/Analog.h"
// #include "Sensors/I2C/MS5803.h"
// #include "Module_Factory.h"
//
// using namespace Loom;
//
// ///////////////////////////////////////////////////////////////////////////////
// TempSync::TempSync(
// 		const Module::Type		source,
// 		const Module::Type		dependant
// 	)
// 	: Module("TempSync")
// 	, source_type(source)
// 	, dependant_type(dependant)
// 	{}
//
// ///////////////////////////////////////////////////////////////////////////////
// TempSync::TempSync(JsonArrayConst p)
// 	: TempSync((Module::Type)(int)p[0], (Module::Type)(int)p[1] ) {}
//
// /////////////////////////////////////////////////////////////////////////////
// void TempSync::second_stage_ctor()
// {
//   LMark;
// 	bool setup = true;
//   LMark;
//
// 	// Get source and dependant from manager
// 	if (device_manager != nullptr) {
//    	LMark;
// 		// source
// 		Module* tmp = device_manager->find_module(source_type);
//    	LMark;
// 		if (tmp != nullptr) {
//     	LMark;
// 			print_module_label();
//     	LMark;
// 			LPrintln("Found source (", (int)source_type, ")");
//     	LMark;
// 			source = tmp;
//    		LMark;
// 		} else {
// 			setup = false;
//    		LMark;
// 		}
// 		// dependant
// 		tmp = device_manager->find_module(dependant_type);
//    	LMark;
// 		if (tmp != nullptr) {
//     	LMark;
// 			print_module_label();
//     	LMark;
// 			LPrintln("Found dependant (", (int)dependant_type, ")");
//     	LMark;
// 			dependant = tmp;
//    		LMark;
// 		} else {
// 			setup = false;
//    		LMark;
// 		}
// 	}
//
// 	print_module_label();
//   LMark;
// 	LPrintln("Setup: ", setup);
//   LMark;
//
// 	// If one or more does not exist, delete self and remove pointer from manager
// 	if (!setup) active = false;
//  	LMark;
// }
//
// ///////////////////////////////////////////////////////////////////////////////
// void TempSync::print_config() const
// {
//   LMark;
// 	Module::print_config();
//   LMark;
// 	LPrintln("\tSource Type: ", (int)source_type);
//   LMark;
// 	LPrintln("\tDependant Type: ", (int)dependant_type);
//  	LMark;
// }
//
// ///////////////////////////////////////////////////////////////////////////////
// void TempSync::sync_temp()
// {
//   LMark;
// 	if (source && source->get_active() && dependant && dependant->get_active() ) {
//    	LMark;
// 		float temp = 25.0;
//   	LMark;
//
// 		if (source_type == Module::Type::MS5803) {
//     	LMark;
// 			temp = ( (MS5803*)source )->get_temperature();
//    		LMark;
// 		}
//
// 		if ( dependant_type == Module::Type::Analog ) {
//     		LMark;
// 			( (Analog*)dependant )->set_temperature(temp);
//    			LMark;
// 		}
// 	}
// }
//
// ///////////////////////////////////////////////////////////////////////////////
//
// #endif // ifdef LOOM_INCLUDE_SENSORS
