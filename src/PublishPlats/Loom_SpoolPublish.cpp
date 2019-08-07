
#include "Loom_SpoolPublish.h"

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR --
Loom_SpoolPublish::Loom_SpoolPublish(
                                     const char* module_name,
                                     const uint  internet_index,
                                     const char* spool_url,
                                     const char* device_data_endpoint,
                                     const char* device_id,
                                     ) : LoomPublishPlat(module_name, internet_index)
, m_buffer{}
, m_data_start(nullptr), spool_url(spool_url) {
    
    m_buffer = "\nContent-Type: application/json\n\n";
    
    /// Build the begining of the Google Sheets URL with all of the provided parameters
    print_module_label();
   // if(printed < 0) LPrint("Buffer overflowed when constructing google sheets url\n");
   /* else {
        LPrint("Google sheets ready with url: ", m_buffer);
        m_data_start = &(m_buffer[printed]);
    }*/
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR --
Loom_SpoolPublish::Loom_SpoolPublish(JsonVariant p)
: Loom_SpoolPublish( EXPAND_ARRAY(p, 6) ) {}

/////////////////////////////////////////////////////////////////////
bool Loom_SpoolPublish::send_to_internet(const JsonObject json, LoomInternetPlat* plat) {
    print_module_label();
    // serialize the data, checking for an error
    // not sure if this is the right way to check if there is a overflow
    if (serializeJson(json, m_buffer) < 0) {
        LPrint("Buffer overflow during serialize!\n");
        return false;
    }
    
    else {
        LPrint(m_buffer, "\n");
        Client& m_cli = plat->http_post_request(spool_url, device_data_endpoint, m_buffer);
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
