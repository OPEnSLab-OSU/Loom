///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Module.cpp
/// @brief		File for Module implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Module.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
Module::Module(
	const char* 				module_name
	)
	: module_name_base(module_name)
	, module_name(module_name_base.c_str())
	, active(true)
	, print_verbosity(Verbosity::V_LOW)
	, package_verbosity(Verbosity::V_LOW)
	, device_manager(nullptr)
{}

///////////////////////////////////////////////////////////////////////////////
void Module::link_device_manager(Manager* LM)
{
  LMark;
	device_manager = LM;
}

///////////////////////////////////////////////////////////////////////////////
void Module::print_module_label() const
{
  LMark;
	LPrint("[", module_name, "] ");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Module::print_config() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Config:");
  LMark;
	LPrintln("\tModule Active    : ", (active)      ? "Enabled" : "Disabled" );
  LMark;
	LPrintln("\tPrint Verbosity  : ", enum_verbosity_string(print_verbosity) );
  LMark;
	LPrintln("\tPackage Verbosity: ", enum_verbosity_string(package_verbosity) );
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Module::print_state() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("State:");
}

///////////////////////////////////////////////////////////////////////////////
void Module::get_module_name(char* buf) const
{
  LMark;
	module_name_base.toCharArray(buf, 20);
}

///////////////////////////////////////////////////////////////////////////////
void Module::set_print_verbosity(const Verbosity v)
{
  LMark;
	print_verbosity = v;
  LMark;
	if (print_verbosity == Verbosity::V_HIGH) {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("Set print verbosity to: ", enum_verbosity_string(v));
	}
}

///////////////////////////////////////////////////////////////////////////////
void Module::set_package_verbosity(const Verbosity v)
{
  LMark;
	package_verbosity = v;
  LMark;
	if (print_verbosity == Verbosity::V_HIGH) {
   	LMark;
		print_module_label();
   	LMark;
		LPrintln("Set package verbosity verbosity to: ", enum_verbosity_string(v));
	}
}

///////////////////////////////////////////////////////////////////////////////
const char* Module::enum_verbosity_string(const Verbosity v)
{
	switch(v) {
		case Verbosity::V_OFF  : return "Off";
		case Verbosity::V_LOW  : return "Low";
		case Verbosity::V_HIGH : return "High";
	}
}

///////////////////////////////////////////////////////////////////////////////
// Module::Category Module::category() const
// {
//   LMark;
// 	switch( (int)module_type / 1000 ) {
// 		case 1 : return Category::Other;		// Other
// 		case 2 : return Category::Sensor;		// Sensors
// 		case 3 : return Category::L_RTC;		// RTC
// 		case 4 : return Category::Actuator;		// Actuators
// 		case 5 : return Category::LogPlat;		// LogPlats
// 		case 6 : return Category::CommPlat;		// CommPlats
// 		case 7 : return Category::InternetPlat;	// InternetPlats
// 		case 8 : return Category::PublishPlat;	// PublishPlats
// 		case 9 : return Category::SubscribePlat;// SubscribePlats
// 		default: return Category::Unknown;		// Unknown
// 	}
// }

///////////////////////////////////////////////////////////////////////////////
// const char* Module::enum_category_string(const Category c)
// {
//   LMark;
// 	switch ( (int)c ) {
// 		case 1 : return "Other";		// Other
// 		case 2 : return "Sensor";		// Sensors
// 		case 3 : return "RTC";			// RTC
// 		case 4 : return "Actuator";		// Actuators
// 		case 5 : return "LogPlat";		// LogPlats
// 		case 6 : return "CommPlat";		// CommPlats
// 		case 7 : return "InternetPlat";	// InternetPlats
// 		case 8 : return "PublishPlat";	// PublishPlats
// 		case 9 : return "SubscribePlat";// SubscribePlats
// 		default: return "Unknown";		// Unknown
// 	}
// }

///////////////////////////////////////////////////////////////////////////////
