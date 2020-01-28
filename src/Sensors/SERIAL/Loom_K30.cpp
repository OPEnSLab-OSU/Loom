//
//  Loom_K30.cpp
//  
//
//  Created by Eli Winkelman on 1/21/20.
//

#include "Loom_K30.h"

Loom_K30::Loom_K30(
                   const char* module_name,
                   int num_samples)
: LoomSerialSensor(module_name, Type::K30, num_samples)
{
    LPrintln ("Initializing K30");
    LPrintln("K30 Initialization Finished");
}

Loom_K30::Loom_K30(JsonArrayConst p)
: Loom_K30(EXPAND_ARRAY(p, 2))
{}

void Loom_K30::sendSensorRequest(byte request[7]) {
    
    if (sensor_serial == nullptr) {
        LPrintln("Serial is not set, cannot send request");
        return;
    }
    
    LPrintln("Sending Request to K30 Sensor");
    while (!sensor_serial -> available()) //keep sending request until we start to get a response
    {
        LPrintln("sensor_serial Unavailable, Sending Request");
        sensor_serial -> write(request, 7);
        delay(50);
    }
    
    int timeout = 0; //set a timeoute counter
    while (sensor_serial -> available() < 7 ) //Wait to get a 7 byte response
    {
        LPrintln("sensor_serial Available, retrieving response");
        timeout++;
        if (timeout > 10)   //if it takes to long there was probably an error
        {
            while (sensor_serial -> available()) //flush whatever we have
                sensor_serial -> read();
            break;                        //exit and try again
        }
        delay(50);
    }
    for (int i = 0; i < 7; i++)
    {
        sensor_response[i] = sensor_serial -> read();
    }
    LPrintln("Finished Sending Request to K30 sensor");
}

int Loom_K30::readSensorResponse(byte packet[7]){
    LPrintln("Reading sensor response");
    int high = packet[3];                        //high byte for value is 4th byte in packet in the packet
    int low = packet[4];                         //low byte for value is 5th byte in the packet
    unsigned long val = high * 256 + low;        //Combine high byte and low byte with this formula to get value
    LPrintln("Finished Reading Sensor response");
    return val;
    
}

void Loom_K30::measure() {
    LPrintln("Measuring...");
    sendSensorRequest(read_C02);
    C02_levels = readSensorResponse(sensor_response);
    LPrintln("Finished Measuring.");
}

void Loom_K30::print_measurements() const {
    print_module_label();
    LPrintln("Measurements:");
    LPrintln("C02 Levels: ", C02_levels);
}

void Loom_K30::package(JsonObject json) {
    JsonObject data = get_module_data_object(json, module_name);
    
    data["C02"] = C02_levels;
    
}
