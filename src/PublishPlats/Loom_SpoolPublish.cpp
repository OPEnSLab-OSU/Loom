
#include "Loom_SpoolPublish.h"

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR --
Loom_SpoolPublish::Loom_SpoolPublish(
                                     const char* module_name,
                                     const LoomModule::Type	internet_type,
                                     const char* spool_domain,
                                     const char* device_data_endpoint,
                                     const char* device_id
                                     ) 
    : LoomPublishPlat(module_name, LoomModule::Type::SpoolPub, internet_type)
    , m_spool_domain(spool_domain)
    , m_device_data_endpoint(device_data_endpoint)
    , m_device_id(device_id) {

    print_module_label();
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR --
Loom_SpoolPublish::Loom_SpoolPublish(JsonArrayConst p)
: Loom_SpoolPublish( p[0], (LoomModule::Type)(int)p[1], p[2], p[3], p[4] ) {}

/////////////////////////////////////////////////////////////////////
void Loom_SpoolPublish::print_config() {
    LoomPublishPlat::print_config();
    print_module_label();
    LPrint("Domain: ", m_spool_domain, '\n');
    print_module_label();
    LPrint("URL: ", m_device_data_endpoint, '\n');
    print_module_label();
    LPrint("Device ID: ", m_device_id, '\n');
}

/////////////////////////////////////////////////////////////////////
bool Loom_SpoolPublish::send_to_internet(const JsonObject json, LoomInternetPlat* plat) {
    print_module_label();
    // serialize the data, checking for an error
    // not sure if this is the right way to check if there is a overflow
    
    auto network = plat->connect_to_ip(m_spool_domain, 443);

    if (!network) {
        print_module_label();
        LPrintln("Could not connect to Spool instance.");
        return false;
    }
    
    // POST DEVICE_DATA_ENDPOINT
    network -> print("POST ");
    network -> print(m_device_data_endpoint);
    network -> println(" HTTP/1.1");
    
    //Headers
    network -> println("Content-Type: application/json");
    network -> print("Content-Length: ");
    // Equivalent to:
    // strlen("{\"device_id\":\"") + m_device_id + strlen("\",\"data\":") + json + strlen("}")
    network -> println(14 + m_device_id.length() + 9 + measureJson(json) + 1);
    network -> print("Host: ");
    network -> println(m_spool_domain);

    //print the body
    network -> println();
    network -> print("{\"device_id\":\"");
    network -> print(m_device_id);
    network -> print("\",\"data\":");
    
    serializeJson(json, *network);

    network -> println("}");

    if (!network->connected()) {
        print_module_label();
        LPrintln("Internet disconnected during transmission!");
        return false;
    }

    network -> flush();

    // all done!
    print_module_label();
    LPrint("Published successfully!\n");
    return true;
}
