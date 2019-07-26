#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>


///////////////////////////////////////////////////////////////////////////////
/// Add an list of alternating keys and values to a json data object
/// under the specified module
/// \param[out]		json			Object to add data to
/// \param[in]		module_name		Name of module calling package
JsonObject get_module_data_object(JsonObject json, const char* module_name);

///////////////////////////////////////////////////////////////////////////////
/// Add timestamp to a Json object
/// \param[out]		json	Object to add timestamp to
/// \param[in]		date 	Time string of timestamp
/// \param[in]		time	Date string of timestamp
void package_json_timestamp(JsonObject json, const char* date, const char* time);

///////////////////////////////////////////////////////////////////////////////
/// Convert data in key values in arrays in ojects to 
/// keys and values in single object 'flatObj'
/// \param[out]		json	Object to flatten data of
void flatten_json_data_object(JsonObject json);

///////////////////////////////////////////////////////////////////////////////
/// Convert data in key values in arrays in ojects to 
/// alternating keys and values in array 'flatArray'
/// \param[out]		json	Object to flatten data of
void flatten_json_data_array(JsonObject json);

///////////////////////////////////////////////////////////////////////////////


// /// @private (hide from Doxygen)
// /// Termination version no args
// void add_config_aux2(JsonArray parameters); 

// /// @private (hide from Doxygen)
// template<typename Arg1, typename... Args>
// void add_config_aux2(JsonArray parameters, const Arg1 arg1, const Args... args)
// {
// 	parameters.add(arg1);
// 	add_config_aux2(parameters, args...);
// }


// /// Use to help build configuration Json
// /// \param[in] json		JsonObject to add config info to
// template<typename... Args>
// void add_config_aux(JsonObject json, const char* module_name, const Args... args)
// {
// 	JsonArray components = json["components"];
// 	if (components.isNull()) {
// 		components = json.createNestedArray("components");
// 	}

// 	JsonObject config_info = components.createNestedObject();
// 	config_info["name"] = module_name;
// 	JsonArray parameters = config_info.createNestedArray("params");

// 	// Add parameters
// 	add_config_aux2(parameters, args...);
// }
///////////////////////////////////////////////////////////////////////////////

JsonArray add_config_temp(JsonObject json, const char* module_name);


