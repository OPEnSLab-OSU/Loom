///////////////////////////////////////////////////////////////////////////////
///
/// @file		Package.h
/// @brief		File for functions to package data into JsonObject, 
///				and extracting data from it
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
/// Add a list of alternating keys and values to a json data object
/// under the specified module
/// @param[out]		json			Object to add data to
/// @param[in]		module_name		Name of module calling package
JsonObject get_module_data_object(JsonObject json, const char* module_name);

///////////////////////////////////////////////////////////////////////////////
/// Add timestamp to a Json object
/// @param[out]		json	Object to add timestamp to
/// @param[in]		date 	Time string of timestamp
/// @param[in]		time	Date string of timestamp
void package_json_timestamp(JsonObject json, const char* date, const char* time);

///////////////////////////////////////////////////////////////////////////////
/// @param[out]		json	Object to flatten data of
/// Convert data in key values in arrays in ojects to 
/// keys and values in single object 'flatObj'
void flatten_json_data_object(JsonObject json);

///////////////////////////////////////////////////////////////////////////////
/// Convert data in key values in arrays in ojects to
/// alternating keys and values in array 'flatArray'
/// @param[out]		json	Object to flatten data of
void flatten_json_data_array(JsonObject json);

//////////////////////////////////////////////////////////////////////////////
JsonArray add_config_temp(JsonObject json, const char* module_name);

///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom
