//
//  Loom_SpoolPublish.hpp
//  
//
//  Created by Eli Winkelman on 8/7/19.
//

#include "Loom_PublishPlat.h"
#include "ArduinoJson.h"

class Loom_SpoolPublish : public LoomPublishPlat {
    
public:
    
    /// Loom Spool publishing platform
    Loom_SpoolPublish(
                    const char* module_name,
                    const LoomModule::Type	internet_type,
                    const char* spool_domain,
                    const char* device_data_endpoint,
                    const char* device_id
                    );
    
    Loom_SpoolPublish( JsonArrayConst p );
    
    /// Destructor
    ~Loom_SpoolPublish() = default;
    
    void print_config() override;
    
    
protected:
    
    // remember to close the socket!
    bool send_to_internet(const JsonObject json, LoomInternetPlat* plat) override;
    
    
private:
    // the spool url
    const String m_spool_domain;
    const String m_device_data_endpoint;
    const String m_device_id;
};
