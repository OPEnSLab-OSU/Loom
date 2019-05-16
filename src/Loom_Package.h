#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>


/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

// Termination version
void package_json(JsonObject json, const char* module_name); 

template<typename Arg1>
void package_json(JsonObject json, const char* module_name, const Arg1 arg1) 
{
	Serial.print("Not enough arguments to package");
}

// Termination version no args
void package_json_aux(JsonArray data); 

// Termination version 1 arg
template<typename Arg1>
void package_json_aux(JsonArray data, const Arg1 arg1) {} 


// Add 1 key value pair to a compenent
template<typename Arg1, typename Arg2, typename... Args>
void package_json_aux(JsonArray data, const Arg1 arg1, const Arg2 arg2, const Args... args)
{
	JsonObject tmp = data.createNestedObject();
	tmp[arg1] = arg2;
	package_json_aux(data, args...); 
}


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
		compenent.createNestedArray("data"); 
	}

	// Add key value pairs to module's data array
	package_json_aux(compenent["data"], args...); 
}











// void func();// {} // termination version

// template<typename Arg1, typename... Args>
// void func(const Arg1& arg1, const Args&&... args)
// {
//     Serial.println( arg1 );
//     func(args...); // note: arg1 does not appear here!
// }


// Recursively extract the first two args at a time


void func();// {} // termination version

template<typename Arg1>
void func(const Arg1 arg1)
{
	Serial.println( arg1 );
}



template<typename Arg1, typename Arg2, typename... Args>
void func(const Arg1 arg1, const Arg2 arg2, const Args... args)
{
	Serial.print( arg1 );
	Serial.print(" ");
	Serial.println( arg2 );
	func(args...); // note: arg1 does not appear here!
}








// This might be useful::
// void package_json_flat(JsonObject json, const char* module_name, const Args... args)





// template<typename Arg1, typename Arg2, typename... Args>
// void test_json(JsonObject json, const char* module_name, const Arg1 arg1, const Arg2 arg2, const Args... args)
// {





// 	Serial.print( arg1 );
// 	Serial.print(" ");
// 	Serial.println( arg2 );
// 	test_json_aux(json, args...); // note: arg1 does not appear here!
// }








// template<typename... Types> 
// void package_json(JsonObject json, Types... vars)
// // void package_json_data(int json)
// {

// 	// for(const auto v : {1, 2, 3}) {
// 	// 	Serial.println(v);
// 	// }

// 	// for(const auto v : {1, 2, 3}) {
// 	// 	Serial.println(v);
// 	// }
// 	LPrintln("START", vars...);
// 	serializeJson(json, Serial);


// 	JsonArray contents = json["contents"];
// 	if (contents.isNull()) {
// 		// LPrintln("Array is null, creating contents array", vars);
// 		contents = json.createNestedArray("contents");
// 	}

// 	JsonObject compenent = contents.createNestedObject();
// 	compenent["name"] = "module_name";

// 	JsonArray data = compenent.createNestedArray("data"); 
// 	JsonObject tmp;

// 	tmp = data.createNestedObject();
// 	tmp["Vbat"] = 4.37;




// 	// LPrintln("END", vars);
// 	serializeJson(json, Serial);

// }
// 




/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////


