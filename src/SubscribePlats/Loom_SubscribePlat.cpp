
#include "Loom_SubscribePlat.h"
#include "Loom_Manager.h"


///////////////////////////////////////////////////////////////////////////////
LoomSubscribePlat::LoomSubscribePlat(	
		const char*			module_name,
		LoomModule::Type	module_type
	// 	uint8_t				internet_index
	) 
	: LoomModule( module_name, module_type )
	, internetPlat( nullptr )
	, internet_index( internet_index ) 
{}

///////////////////////////////////////////////////////////////////////////////
void LoomSubscribePlat::second_stage_ctor() 
{

}

///////////////////////////////////////////////////////////////////////////////
bool LoomSubscribePlat::subscribe()
{
	// call normal subscribe (implemented by derived classes)
	if (device_manager != nullptr) {
		return subscribe( device_manager->internal_json(true) );  // true to clear json
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
bool LoomSubscribePlat::msgpack_buffer_to_json(const char* buffer, JsonObject json)
{
	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("Received: ", (const char*)buffer);
		print_module_label();
		LPrintln("Len: ", strlen(buffer));
	}

	messageJson.clear();

	if (deserializeMsgPack(messageJson, buffer) != DeserializationError::Ok ) {
		print_module_label();
		LPrintln("Failed to parse MsgPack");
		return false;
	}

	bool status = json.set(messageJson.as<JsonObject>());
	if (!status) return false;

	if (print_verbosity == Verbosity::V_HIGH) {
		print_module_label();
		LPrintln("\nInternal messageJson:");
		serializeJsonPretty(messageJson, Serial);

		// LPrintln("\nJson passed in:");
		// serializeJsonPretty(json, Serial);
		LPrintln();
	}
}

///////////////////////////////////////////////////////////////////////////////


