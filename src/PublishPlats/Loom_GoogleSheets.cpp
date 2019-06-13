
#include "Loom_GoogleSheets.h"

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR --
Loom_GoogleSheets::Loom_GoogleSheets(
    const char* module_name,
    const uint	internet_index,
    const char* script_url,
    const char* sheet_id,
    const char* tab_id,
    const char* device_id
)   : LoomPublishPlat(module_name, internet_index)
    , m_buffer{}
    , m_data_start(nullptr) {
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

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR --
Loom_GoogleSheets::Loom_GoogleSheets(JsonVariant p)
	: Loom_GoogleSheets( EXPAND_ARRAY(p, 6) ) {}

/////////////////////////////////////////////////////////////////////
void Loom_GoogleSheets::print_config() {
    LoomPublishPlat::print_config();
    // just in case
    m_data_start[0] = '\0';
    LPrint("\t URL: ", m_buffer, "\n");
}

/////////////////////////////////////////////////////////////////////
bool Loom_GoogleSheets::send_to_internet(const JsonObject json, LoomInternetPlat* plat) {
    print_module_label();
    // step one: package timestamp
    char* m_data_cur = m_data_start;
    const JsonObject time_obj = json["timestamp"];
    for (const JsonPair i : time_obj) {
        const int printed = snprintf(m_data_cur, &(m_buffer[sizeof(m_buffer) - 1]) - m_data_cur,
            "%s~%s~", i.key().c_str(), i.value().as<const char*>());
        if (printed < 0) {
            LPrint("Buffer overflow during timestamp packaging");
            return false;
        }
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
            const int printed = snprintf(m_data_cur, &(m_buffer[sizeof(m_buffer) - 1]) - m_data_cur,
            "%s-%s~", name, d.key().c_str());
            if (printed < 0) {
                LPrint("Buffer overflow during data packaging");
                return false;
            }
            // move the pointer foward
            m_data_cur = &(m_data_cur[printed]);
            // serialize the value
            const auto data_tmp = d.value();
            const auto size = measureJson(data_tmp);
            const auto max_size = &(m_buffer[sizeof(m_buffer) - 1]) - m_data_cur;
            // no overflow check
            if (size + 1 >= max_size) {
                LPrint("Buffer overflow during data packaging");
                return false;
            }
            serializeJson(data_tmp, m_data_cur, max_size);
            // add the trailing tilde
            m_data_cur[size] = '~';
            // move the pointer forward
            m_data_cur = &(m_data_cur[size + 1]);
        }
    }
    // step three: replace the last trailing tilde with a null
    m_data_cur--;
    m_data_cur[0] = '\0';
    // step four: send the data!
    LPrint("Google sheets data: ", m_buffer, "\n");
}
