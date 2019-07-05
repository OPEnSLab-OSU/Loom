#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>


///////////////////////////////////////////////////////////////////////////////
/// @private (hide from Doxygen)
/// Invalid package argument count
void package_json(JsonObject json, const char* module_name); 

/// @private (hide from Doxygen)
/// Invalid package argument count
template<typename Arg1>
void package_json(JsonObject json, const char* module_name, const Arg1 arg1) 
{
	Serial.print("Not enough arguments to package");
}

/// @private (hide from Doxygen)
/// Termination version no args
void package_json_aux(JsonObject data); 

/// @private (hide from Doxygen)
/// Termination version 1 arg
template<typename Arg1>
void package_json_aux(JsonObject data, const Arg1 arg1) {} 

/// @private (hide from Doxygen)
/// Add 1 key value pair to a compenent.
/// Auxilliary recursive function for package_json
template<typename Arg1, typename Arg2, typename... Args>
void package_json_aux(JsonObject data, const Arg1 arg1, const Arg2 arg2, const Args... args)
{
	// JsonObject tmp = data.createNestedObject();
	data[arg1] = arg2;
	package_json_aux(data, args...); 
}

/// Add an list of alternating keys and values to a json data object
/// under the specified module
/// \param[out]		json			Object to add data to
/// \param[in]		module_name		Name of module calling package
/// \param[in]		args			Series of alternating keys and values 
///									keys: const* char
///									vals: any
template<typename... Args>
void package_json(JsonObject json, const char* module_name, const Args... args)
{
	// Get / Create array for module data blocks
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
	if (compenent.isNull()) {
		compenent = contents.createNestedObject();
		compenent["module"] = module_name;
		compenent.createNestedObject("data"); 
	}

	// Add key value pairs to module's data array
	package_json_aux(compenent["data"], args...); 
}

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


