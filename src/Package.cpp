///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Package.cpp
/// @brief		File for implementation of packaging functions.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Package.h"


///////////////////////////////////////////////////////////////////////////////
/// @param[out]		json			Object to add data to
/// @param[in]		module_name		Name of module calling package
JsonObject get_module_data_object(JsonObject json, const char* module_name)
{
	JsonArray contents = json["contents"];
	if (contents.isNull()) {
		contents = json.createNestedArray("contents");
	}

	// Find module object if it exists
	JsonObject compenent;
	for (auto module_obj : contents) {
		if ( strcmp(module_obj["module"], module_name) == 0 ) {
			compenent = module_obj;
			break;
		}
	}

	// If module object does not exist yet
	// create object, specify module name, 
	// and create data array
	JsonObject data;
	if (compenent.isNull()) {
		compenent = contents.createNestedObject();
		compenent["module"] = module_name;
		data = compenent.createNestedObject("data"); 
	}

	return data;
}

///////////////////////////////////////////////////////////////////////////////
/// @param[out]		json	Object to add timestamp to
/// @param[in]		date 	Time string of timestamp
/// @param[in]		time	Date string of timestamp
void package_json_timestamp(JsonObject json, const char* date, const char* time)
{
	// Note that if the keys are the same as the ones added by the Google script 
	// for received time, only one value will be kept
	JsonObject timestamp = json.createNestedObject("timestamp");
	timestamp["date"] = date;
	timestamp["time"] = time;
}

///////////////////////////////////////////////////////////////////////////////
/// @param[out]		json	Object to flatten data of
void flatten_json_data_object(JsonObject json)
{
	// Get / Create array for module data blocks
	JsonArray contents = json["contents"];
	if (contents.isNull()) return; 

	JsonObject flatData = json.createNestedObject("flatObj");

	// Iterate over modules
	JsonObject data;
	for (auto module_obj : contents) {
		for (JsonPair kv : module_obj["data"].as<JsonObject>()) {
			flatData[kv.key()] = kv.value();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// @param[out]		json	Object to flatten data of
void flatten_json_data_array(JsonObject json)
{
	// Get / Create array for module data blocks
	JsonArray contents = json["contents"];
	if (contents.isNull()) return; 

	JsonArray flatData = json.createNestedArray("flatArray");

	// Iterate over modules
	JsonObject data;
	for (auto module_obj : contents) {
		for (JsonPair kv : module_obj["data"].as<JsonObject>()) {
			flatData.add(kv.key());
			flatData.add(kv.value());
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
JsonArray add_config_temp(JsonObject json, const char* module_name)
{
	JsonArray components = json["components"];
	if (components.isNull()) {
		components = json.createNestedArray("components");
	}

	JsonObject config_info = components.createNestedObject();
	config_info["name"] = module_name;
	return config_info.createNestedArray("params");
} 

///////////////////////////////////////////////////////////////////////////////



