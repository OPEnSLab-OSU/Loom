///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_GoogleSheets.cpp
/// @brief		File for Loom_GoogleSheets implementation.
/// @author		Noah Koontz
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "GoogleSheets.h"
#include "../Manager.h"


///////////////////////////////////////////////////////////////////////////////
Loom_GoogleSheets::Loom_GoogleSheets(
		LoomManager* manager,
		const char*				module_name,
		const LoomModule::Type	internet_type,
		const char*				script_url,
		const char*				sheet_id,
		const bool				tab_matches_dev_id,
		const char*				tab_id
	)   
	: LoomPublishPlat(manager, module_name, Type::GoogleSheets, internet_type )
	, m_script_url(script_url)
	, m_sheet_id(sheet_id)
	, tab_matches_dev_id(tab_matches_dev_id)
	, m_tab_id(tab_id)
{   
	// Build the begining of the Google Sheets URL with all of the provided parameters
	print_module_label();
	LPrint("Google sheets ready with url: ", m_script_url, '\n');
} 

///////////////////////////////////////////////////////////////////////////////
Loom_GoogleSheets::Loom_GoogleSheets(LoomManager* manager, JsonArrayConst p)
	: Loom_GoogleSheets(manager, p[0], (LoomModule::Type)(int)p[1], p[2], p[3], p[4], p[5] ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_GoogleSheets::print_config() const
{
	LoomPublishPlat::print_config();
	LPrint("\t URL: ", m_script_url, "\n");
	LPrint("\t Sheet ID: ", m_sheet_id, "\n");
	LPrint("\t Tab ID: ", m_tab_id, "\n");
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_GoogleSheets::send_to_internet(const JsonObject json, LoomInternetPlat* plat) 
{
	// connect to script.google.com
	auto network = plat->connect_to_domain("script.google.com");
	// check if we connected
	if (!network) {
		print_module_label();
		LPrintln("Could not connect to script.google.com");
		return false;
	}
	// start writing data to the network
	// print the initial http request
	network->print("GET ");
	// construct the URL from a bunch of different segments
	// start with the sheet metadata base, referenced from the following snprintf statement:
	/* const int printed = snprintf(m_buffer, sizeof(m_buffer), "%s?key0=sheetID&val0=%s&key1=tabID&val1=%s&key2=deviceID&val2=%s&key3=full_data&val3=", 
		script_url,				// URL of the script
		sheet_id,				// Spreadsheet ID
		tab_id,					// Tab to write to
		device_id);				// The bundle source's device ID */

	network->print(m_script_url);
	network->print("?key0=sheetID&val0=");
	network->print(m_sheet_id);
	network->print("&key1=tabID&val1=");

	if (tab_matches_dev_id && device_manager) {
		char buf[20];
		device_manager->get_device_name(buf);
		snprintf(buf, 20, "%s%d", buf, device_manager->get_instance_num());
		network->print(buf);

	} else {
		network->print(m_tab_id);
	}

	network->print("&key2=deviceID&val2=");

	// Get device ID from manager
	if (device_manager) {
		char buf[20];
		device_manager->get_device_name(buf);
		snprintf(buf, 20, "%s%d", buf, device_manager->get_instance_num());
		network->print(buf);
	} else {
		network->print("Unknown");
	}

	network->print("&key3=full_data&val3=");
	// next print the body data, converted in real time
	m_serialize_internet_impl(json, *network);
	// that should finish off the URL, so print the rest of the HTTP request
	network->print(" HTTP/1.1\r\nUser-Agent: LoomOverSSLClient\r\nHost: script.google.com\r\nConnection: close\r\n\r\n");
	// all ready to go!
	if (!network->connected()) {
		print_module_label();
		LPrintln("Internet disconnected during transmission!");
		return false;
	}
	// flush all that
	network->flush();
	// all done!
	print_module_label();
	LPrint("Published successfully!\n");

	return true;
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_GoogleSheets::m_serialize_internet_impl(const JsonObject json, Print& write) 
{
	// step one: package timestamp
	const JsonObject time_obj = json["timestamp"];
	for (const JsonPair i : time_obj) {
		write.print(i.key().c_str());
		write.print('~');
		write.print(i.value().as<const char*>());
		write.print('~');
	}
	// step two: package data
	const JsonArrayConst data_ray = json["contents"];
	for (JsonArrayConst::iterator i = data_ray.begin(); i != data_ray.end();) {
		JsonObjectConst obj = i->as<JsonObjectConst>();
		// store the module name
		const char* name = obj["module"].as<const char*>();
		// iterate over the data, pushing it into the buffer
		const JsonObjectConst data_vals = obj["data"];
		// increment i, and let the loop below know if it's the very last cycle
		const bool end = (++i == data_ray.end());
		for (JsonObjectConst::iterator d = data_vals.begin();;) {
			// serialize the key
			write.print(name);
			write.print('-');
			write.print(d->key().c_str());
			write.print('~');
			// serialize the value
			const auto data_tmp = d->value();
			serializeJson(data_tmp, write);
			// add the trailing tilde, only if this is not the last element
			if (++d == data_vals.end()) {
				if (!end) write.print('~');
				break;
			}
			write.print('~');
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
