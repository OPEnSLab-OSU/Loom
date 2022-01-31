///////////////////////////////////////////////////////////////////////////////
///
/// @file		MQTT.h
/// @brief		File for MQTT definition.
/// @author		Will Richards
/// @date		2022
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))
#pragma once

#include "PublishPlat.h"

#include <ArduinoMqttClient.h>

namespace Loom{
class MQTTPub : public PublishPlat{

public:
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

    /// Loom MQTT Platform module constructor.
    /// @param[in] broker       String | Address where the broker is located
    /// @param[in] port         Integer | Port the broker is listening on at the address
    /// @param[in] databaseName String | Which database to store the logged data in, usually a location name
    MQTTPub(
        const char* broker,         // Broker address
        const int port,             // Broker port
        const char* databaseName,   // Database were the information will be stored, typically a location name
        const char* username = "",  // Broker username
        const char* password = ""   // Broker password
    );

    /// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]  p     The array of constuctor args to expand
    MQTTPub(JsonArrayConst p);

    /// Destructor
    ~MQTTPub() = default;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void print_config() const override;

protected:

    /// Send the the JSON data over MQTT to the specific broker
    /// @param[in] json The JSON data, formatted according to publish(); in PublishPlat
    /// @param[in] plat A pointer to the internet platform that is being used
    /// @return True if successful publish, false if failed
    bool send_to_internet(const JsonObject json, InternetPlat* plat) override;

private:
    const String m_brokerAddress;               // Address the broker is running at
    const String m_databaseName;                // Database to store the logged data
    const String m_brokerUsername;              // Username to connect to the broker with
    const String m_brokerPassword;              // Password to connect to the broker with

    const int m_brokerPort;                     // Port to connect to on the MQTT broker

    char m_deviceNameBuf[20];                   // Name of the device
    int m_deviceInstanceNum;                    // Instance number of the device

    String m_broadcastTopic;                    // Constructed topic to broad cast the data over

    String m_stringifiedJSONData = "";          // Holds a stringified version of the JSON data to be sent of MQTT

    MqttClient* m_mqttClient = nullptr;         // Client pointer so we can make a new instance at a later point

    bool connect_to_broker(InternetPlat::ClientSession* client);                   // Utility method to clean up the send_to_internet method, connects to the MQTT broker returns true or false based on succsess
};

///////////////////////////////////////////////////////////////////////////////
REGISTER_NODEFAULT(Module, MQTTPub, "MQTTPub");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))