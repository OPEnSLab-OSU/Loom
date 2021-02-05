///////////////////////////////////////////////////////////////////////////////
///
/// @file		Package.cpp
/// @brief		File for implementation of packaging functions.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Package.h"

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
/// @param[out]		json			Object to add data to
/// @param[in]		module_name		Name of module calling package
JsonObject get_module_data_object(JsonObject json, const char* module_name)
{
  LMark;
	JsonArray contents = json["contents"];
  LMark;
	if (contents.isNull()) {
    LMark;
		contents = json.createNestedArray("contents");
    LMark;
	}

	// Find module object if it exists
	JsonObject compenent;
  LMark;
	for (auto module_obj : contents) {
    LMark;
		if ( strcmp(module_obj["module"], module_name) == 0 ) {
      LMark;
			compenent = module_obj;
      LMark;
			break;
		}
	}

	// If module object does not exist yet
	// create object, specify module name,
	// and create data array
	JsonObject data;
  LMark;
	if (compenent.isNull()) {
    LMark;
		compenent = contents.createNestedObject();
    LMark;
		compenent["module"] = module_name;
    LMark;
		data = compenent.createNestedObject("data");
    LMark;
	}

	return data;
}

///////////////////////////////////////////////////////////////////////////////
/// @param[out]		json	Object to add timestamp to
/// @param[in]		date 	Time string of timestamp
/// @param[in]		time	Date string of timestamp
void package_json_timestamp(JsonObject json, const char* date, const char* time)
{
  LMark;
	// Note that if the keys are the same as the ones added by the Google script
	// for received time, only one value will be kept
	JsonObject timestamp = json.createNestedObject("timestamp");
  LMark;
	timestamp["date"] = date;
  LMark;
	timestamp["time"] = time;
  LMark;
}

///////////////////////////////////////////////////////////////////////////////
/// @param[out]		json	Object to flatten data of
void flatten_json_data_object(JsonObject json)
{
  LMark;
	// Get / Create array for module data blocks
	JsonArray contents = json["contents"];
  LMark;
	if (contents.isNull()) return;
  LMark;

	JsonObject flatData = json.createNestedObject("flatObj");
  LMark;

	// Iterate over modules
	JsonObject data;
  LMark;
	for (auto module_obj : contents) {
    LMark;
		for (JsonPair kv : module_obj["data"].as<JsonObject>()) {
      LMark;
			flatData[kv.key()] = kv.value();
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
/// @param[out]		json	Object to flatten data of
void flatten_json_data_array(JsonObject json)
{
  LMark;
	// Get / Create array for module data blocks
	JsonArray contents = json["contents"];
  LMark;
	if (contents.isNull()) return;
  LMark;

	JsonArray flatData = json.createNestedArray("flatArray");
  LMark;

	// Iterate over modules
	JsonObject data;
  LMark;
	for (auto module_obj : contents) {
    LMark;
		for (JsonPair kv : module_obj["data"].as<JsonObject>()) {
      LMark;
			flatData.add(kv.key());
      LMark;
			flatData.add(kv.value());
      LMark;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
JsonArray add_config_temp(JsonObject json, const char* module_name)
{
  LMark;
	JsonArray components = json["components"];
  LMark;
	if (components.isNull()) {
    LMark;
		components = json.createNestedArray("components");
    LMark;
	}

	JsonObject config_info = components.createNestedObject();
  LMark;
	config_info["name"] = module_name;
  LMark;
	return config_info.createNestedArray("params");
}

///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom
