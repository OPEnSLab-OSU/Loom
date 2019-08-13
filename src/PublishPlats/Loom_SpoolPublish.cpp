
#include "Loom_SpoolPublish.h"

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR --
Loom_SpoolPublish::Loom_SpoolPublish(
                                     const char* module_name,
                                     const char* spool_url,
                                     const char* device_data_endpoint,
                                     const char* device_id,
                                     ) : LoomPublishPlat(module_name, internet_index), spool_url(spool_url), device_data_endpoint(device_data_endpoint), device_id(device_id) {

    print_module_label();
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR --
Loom_SpoolPublish::Loom_SpoolPublish(JsonVariant p)
: Loom_SpoolPublish( EXPAND_ARRAY(p, 4) ) {}

/////////////////////////////////////////////////////////////////////
bool Loom_SpoolPublish::send_to_internet(const JsonObject json, LoomInternetPlat* plat) {
    print_module_label();
    // serialize the data, checking for an error
    // not sure if this is the right way to check if there is a overflow
    
    auto network = plat->connect_to_domain(spool_url);
    
    if (!network) {
        print_module_label();
        LPrintln("Could not connect to Spool instance.");
        return false;
    }
    
    //write to the spool
    
    // POST DEVICE_DATA_ENDPOINT
    network -> print("POST ");
    network -> print(device_data_endpoint);
    
    //Headers
    network -> print("\r\nContent-Type: application/json);
    network -> print("\r\ncache-conrtol: no-cache")
    
    //print the body
    network -> print("{\"device_id\": ");
    network -> print(device_id);
    network -> print("\"data\": ")
    
    serializeJson(json, *network);
    
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
