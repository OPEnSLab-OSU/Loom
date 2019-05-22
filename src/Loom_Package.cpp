#include "Loom_Package.h"

// termination version
void func() {} 

// termination version
void package_json(JsonObject json, const char* module_name) {} 

// termination version
void package_json_aux(JsonObject data) {} 


void package_json_timestamp(JsonObject json, const char* date, const char* time)
{
	JsonObject timestamp = json.createNestedObject("timestamp");
	timestamp["Date"] = date;
	timestamp["Time"] = time;
}


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






