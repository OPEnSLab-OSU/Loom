#if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))

#include "MQTT_Pub.h"
#include "../Manager.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

MQTTPub::MQTTPub(
    const char* broker, 
    const int port, 
    const char* databaseName,
    const char* username,
    const char* password
    ) 
    : PublishPlat("MQTT")
    , m_brokerAddress(broker)
    , m_databaseName(databaseName)
    , m_brokerPort(port) 
    , m_brokerUsername(username)
    , m_brokerPassword(password)
{
    print_module_label();
    LPrintln("Publishing Platform Ready!");
}

///////////////////////////////////////////////////////////////////////////////

MQTTPub::MQTTPub(JsonArrayConst p) : MQTTPub(p[0], p[1], p[2], p[3], p[4]) {}

///////////////////////////////////////////////////////////////////////////////

void MQTTPub::print_config() const
{
    PublishPlat::print_config();
    LPrint("\t Broker Address: ", m_brokerAddress, "\n");
    LPrint("\t Broker port: ", m_brokerPort, "\n");
    if(m_brokerPassword.length() > 0){
        LPrint("\t Broker Username: ", m_brokerUsername, "\n");
        LPrint("\t Broker Password: ", m_brokerPassword, "\n");
    }
    LPrint("\t Database Name: ", m_databaseName, "\n");

    
}

///////////////////////////////////////////////////////////////////////////////

bool MQTTPub::send_to_internet(const JsonObject json, InternetPlat* plat)
{

    // Get the instance number and device name from the Manager
    if(device_manager){
        device_manager->get_device_name(m_deviceNameBuf);
        m_deviceInstanceNum = device_manager->get_instance_num();
        m_broadcastTopic = ( String(m_databaseName) + "/" + String(m_deviceNameBuf) + String(m_deviceInstanceNum) );
    }

    // Connect to the domain on the specified port
    auto clientSession = plat->get_client_session();

    // If a network session wasn't successfully opened return false
    if(!clientSession){
        print_module_label();
        LPrintln("Could not get SSL Client instance");
        return false;
    }

    // Connect to the broker passing in the reference to the clientSession
    if(!connect_to_broker(&clientSession)){
        return false;
    }

    // Clear before writing new data, cause serializeJson will append
    m_stringifiedJSONData = "";

    // Serialize the JSON object into a string to be sent over MQTT
    serializeJson(json, m_stringifiedJSONData);

    // Print that we connected to the broker
    print_module_label();
    LPrintln("Connected To MQTT Broker!");

    // Make sure we stay connected to the broker
    m_mqttClient->poll();

    // Open a new message to send over a topic consisting of the databaseName followed by the device name with instance number appended
    if(!m_mqttClient->beginMessage(m_broadcastTopic)){
        print_module_label();
        LPrintln("Failed to begin a message on topic: " + m_broadcastTopic);
        return false;
    }

    // Transmit the data over the MQTT connection
    m_mqttClient->print(m_stringifiedJSONData);

    // End the message and return true indicating a successful transmition
    if(!m_mqttClient->endMessage()){
        print_module_label();
        LPrintln("Failed to close message on topic: " + m_broadcastTopic);
        return false;
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////

bool MQTTPub::connect_to_broker(InternetPlat::ClientSession* client){
     // Check if the MQTT client is a nullptr if so create a new MqttClient at the current address
    if(m_mqttClient == nullptr){
        m_mqttClient = new MqttClient(**client);
    }

    // Check if we are not connected to the broker if not then establish a connection then transmit data
    if(!m_mqttClient->connected()){

        // Check if we are using a username and password for the broker
        if(m_brokerPassword.length() > 0)
            m_mqttClient->setUsernamePassword(m_brokerUsername, m_brokerPassword);

        // Attempt to connect if it fails return and print out an error
        if(!m_mqttClient->connect(m_brokerAddress.c_str(), m_brokerPort)){
            print_module_label();
            LPrintln("A Connection Error Occurred" + String(m_mqttClient->connectError()));
            return false;
        }
    }

    return true;
}

///////////////////////////////////////////////////////////////////////////////

#endif //if (defined(LOOM_INCLUDE_WIFI) || defined(LOOM_INCLUDE_ETHERNET) || defined(LOOM_INCLUDE_LTE))