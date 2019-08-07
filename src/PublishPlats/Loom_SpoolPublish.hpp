//
//  Loom_SpoolPublish.hpp
//  
//
//  Created by Eli Winkelman on 8/7/19.
//

#include "Loom_PublishPlat.h"
#include "ArduinoJson.h"

class Loom_SpoolPublish : public LoomPublishPlat {
    
protected:
    
public:
    
    /// Loom Google Sheets Platform module constructor.
    /// Check out http://www.open-sensing.org/evaporometerblog/datalog for more information on these parameters.
    /// \param[in]    module_name        String | <"Internet-Plat"> | null | Publish platform module name.
    /// \param[in]  internet_index  Int | <0> | [0-5] | Index from zero of of the desired internet platform based on the JSON configuration.
    /// \param[in]  script_url[in]  String | <""> | null | URL where the google sheets script is hosted, without domain (ex. /macros/s/AKfycby<more letters here>_k0n/exec).
    /// \param[in]  sheet_id        String | <""> | null |ID Of the spreadsheet to put data in (script must have edit access).
    /// \param[in]  tab_id          String | <"Data"> | null | Name of the tab that the device will put data into.
    /// \param[in]  device_id       String | <"Mine"> | null | Identifier to use for the device in the spreadsheet.
    Loom_SpoolPublish(
                      const char* module_name,
                      const uint  internet_index,
                      const char* spool_url,
                      const char* device_data_endpoint,
                      const char* device_id,
                      );
    
    Loom_SpoolPublish( JsonVariant p );
    
    /// Destructor
    ~Loom_SpoolPublish() = default;
    
    void print_config() override;
    
    // remember to close the socket!
    bool send_to_internet(const JsonObject json, LoomInternetPlat* plat) override;
    
private:
    // Get the number of bytes left in buffer from a given pointer in buffer
    size_t m_buffer_left(const char* buffer_index) const {
        const char* end_buffer = &(m_buffer[sizeof(m_buffer) - 1]);
        if (buffer_index >= end_buffer) return 0;
        return end_buffer - buffer_index;
    }
    
    // storage for all those strings we're going to need above
    // main string buffer, the front of which stores our URL
    // do not edit this directly, use m_url_end instead.
    char m_buffer[768];
    // store a pointer to where we can actually add data
    char* m_data_start;
    
    // the url to
    char* spool_url;
};


#endif
