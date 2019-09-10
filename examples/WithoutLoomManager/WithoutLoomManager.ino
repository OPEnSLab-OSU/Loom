///////////////////////////////////////////////////////////////////////////////

// This example demonstrates how you might make use of Loom features / modules
// but without using the LoomManager.

// We do not generally recommend this usage of Loom, and support may not be as
// complete as it would be if using the manager.
// However, it may give you more control if you do not want all of the automation
// of the LoomManager.

// See the wiki for more details about using Loom without the manager:
// https://github.com/OPEnSLab-OSU/Loom/wiki/Using-Loom-without-LoomManager

///////////////////////////////////////////////////////////////////////////////

#include <Loom.h>
#include <ArduinoJson.h>

Loom_Analog Analog{};			// Analog module with default settings
Loom_Relay  Relay("Relay", 10);	// Relay module with provided settings (i.e. relay on pin 10) 

StaticJsonDocument<400> doc;	// Json document for data


void setup() 
{ 
	Serial.begin(115200);
	while (!Serial);
	delay(1000);

	Analog.print_config();
	Relay.print_config();

	LPrintln("\n ** Setup Complete ** ");
}


void loop() 
{
	// Setup Json document for data
	doc.clear();
	doc["type"] = "data";
	JsonObject json = doc.as<JsonObject>();

	// Collect data
	Analog.measure();
	Analog.package(json);

	// Print data
	serializeJsonPretty(json, Serial);

	// Extract value from Json data object
	int A0_val = 0;
	JsonArray contents = json["contents"];
	if (!contents.isNull()) {
		// Find analog data block object
		JsonObject module;
		for (auto module_obj : contents) {
			if ( strcmp(module_obj["module"], "Analog") == 0 ) {
				module = module_obj;
				A0_val = module["data"]["A0"];
				break;
			}
		}
	}

	// Control relay based on A0 value
	bool toggle = A0_val > 2048 ? true : false;
	LPrintln("\nToggle: ", toggle);
	Relay.set(toggle);

	delay(2000);
}
