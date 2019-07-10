
#include "Loom_GoogleSheets.h"

///////////////////////////////////////////////////////////////////////////////
Loom_GoogleSheets::Loom_GoogleSheets(
    const char* module_name,
    const uint	internet_index,
    const char* script_url,
    const char* sheet_id,
    const char* tab_id,
    const char* device_id
)   : LoomPublishPlat( module_name, internet_index )
    , m_buffer{}
    , m_data_start(nullptr) 
{
    const int printed = snprintf(m_buffer, sizeof(m_buffer), "%s?key0=sheetID&val0=%s&key1=tabID&val1=%s&key2=deviceID&val2=%s&key3=full_data&val3=", 
		script_url,                                 // URL of the script
        sheet_id,   								// Spreadsheet ID
		tab_id, 				                    // Tab to write to
		device_id);                           // The bundle source's device ID
    
    /// Build the begining of the Google Sheets URL with all of the provided parameters
    print_module_label();
	if(printed < 0) LPrint("Buffer overflowed when constructing google sheets url\n");
    else {
        LPrint("Google sheets ready with url: ", m_buffer);
        m_data_start = &(m_buffer[printed]);
    }
} 

///////////////////////////////////////////////////////////////////////////////
Loom_GoogleSheets::Loom_GoogleSheets(JsonArrayConst p)
	: Loom_GoogleSheets( EXPAND_ARRAY(p, 6) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_GoogleSheets::print_config() 
{
    LoomPublishPlat::print_config();
    // just in case
    m_data_start[0] = '\0';
    LPrint("\t URL: ", m_buffer, "\n");
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_GoogleSheets::send_to_internet(const JsonObject json, LoomInternetPlat* plat) 
{
    print_module_label();
    // serialize the data, checking for an error
    if (!m_serialize_internet_impl(json)) {
        LPrint("Buffer overflow during serialize!\n");
        return false;
    }
    else {
        LPrint(m_buffer, "\n");
        Client& m_cli = plat->http_get_request("script.google.com", m_buffer);
        if (!m_cli.connected()) return false;
        // discard all oncoming data
        const auto start = millis();
        while (m_cli.connected()) {
            const auto read = m_cli.available();
            if(read) m_cli.read(nullptr, read);
            // timeout in case connection doesn't close itself
            if (millis() - start > 5000) m_cli.stop();
        }
        // all done!
        print_module_label();
        LPrint("Published successfully!\n");
        return true;
    }
}

///////////////////////////////////////////////////////////////////////////////
bool Loom_GoogleSheets::m_serialize_internet_impl(const JsonObject json) 
{
    // step one: package timestamp
    char* m_data_cur = m_data_start;
    const JsonObject time_obj = json["timestamp"];
    for (const JsonPair i : time_obj) {
        const int printed = snprintf(m_data_cur, m_buffer_left(m_data_cur),
            "%s~%s~", i.key().c_str(), i.value().as<const char*>());
        if (printed < 0) return false;
        // move the pointer foward
        m_data_cur = &(m_data_cur[printed]);
    }
    // step two: package data
    const JsonArray data_ray = json["contents"];
    for (const auto i : data_ray) {
        // store the module name
        const char* name = i["module"].as<const char*>();
        // iterate over the data, pushing it into the buffer
        const JsonObject data_vals = i["data"];
        for (const auto d : data_vals) {
            // serialize the key
            const int printed = snprintf(m_data_cur, m_buffer_left(m_data_cur),
                "%s-%s~", name, d.key().c_str());
            if (printed < 0) return false;
            // move the pointer foward
            m_data_cur = &(m_data_cur[printed]);
            // serialize the value
            const auto data_tmp = d.value();
            const auto json_size = measureJson(data_tmp);
            const auto max_json_size = m_buffer_left(m_data_cur);
            // no overflow check
            if (json_size + 1 >= max_json_size) return false;
            serializeJson(data_tmp, m_data_cur, max_json_size);
            // add the trailing tilde
            m_data_cur[json_size] = '~';
            // move the pointer forward
            m_data_cur = &(m_data_cur[json_size + 1]);
        }
    }
    // step three: replace the last trailing tilde with a null
    m_data_cur--;
    m_data_cur[0] = '\0';
    return true;
}

///////////////////////////////////////////////////////////////////////////////