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


void flatten_json_data(JsonObject json)
{
	// Get / Create array for module data blocks
	JsonArray contents = json["contents"];
	if (contents.isNull()) return; 

	JsonObject flatData = json.createNestedObject("flat");

	// Iterate over modules
	JsonObject compenent;
	JsonObject data;
	for (auto module_obj : contents) {
		
		data = module_obj["data"];
		// if (data.isNull()) continue;

		for (JsonPair kv : data) {
			// Serial.println(kv.key().c_str());
			// Serial.println(kv.value().as<char*>());
			flatData[kv.key()] = kv.value();

		}



	}

}
