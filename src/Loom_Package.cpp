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