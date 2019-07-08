//
//  Loom_K30.cpp
//  
//
//  Created by Eli Winkelman on 7/8/19.
//

#include "Loom_K30.h"

Loom_K30::Loom_K30(
                   const char* module_name,
                   int num_samples)
: LoomSensor(module_name, num_samples)
{
    LPrintln ("Initializing K30");
    this -> module_type = ModuleType::K30;
    
    LPrintln("K30 Initialization Finished");
}

Loom_K30::Loom_K30(JsonVariant p)
: Loom_K30(EXPAND_ARRAY(p, 2))
{}

Loom_K30::~Loom_K30() {
    delete K30_Serial;
}

void Loom_K30::sendSensorRequest(byte request[7]) {
    
    if (K30_Serial == nullptr) {
        LPrintln("Serial is not set, cannot send request");
        return;
    }
    
    LPrintln("Sending Request to K30 Sensor");
    while (!K30_Serial -> available()) //keep sending request until we start to get a response
    {
        LPrintln("K30_Serial Unavailable, Sending Request");
        K30_Serial -> write(request, 7);
        delay(50);
    }
    
    int timeout = 0; //set a timeoute counter
    while (K30_Serial -> available() < 7 ) //Wait to get a 7 byte response
    {
        LPrintln("K30_Serial Available, retrieving response");
        timeout++;
        if (timeout > 10)   //if it takes to long there was probably an error
        {
            while (K30_Serial -> available()) //flush whatever we have
                K30_Serial -> read();
            break;                        //exit and try again
        }
        delay(50);
    }
    for (int i = 0; i < 7; i++)
    {
        sensor_response[i] = K30_Serial -> read();
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

void Loom_K30::print_measurements() {
    print_module_label();
    LPrintln("Measurements:");
    LPrintln("C02 Levels: ", C02_levels);
}

void Loom_K30::package(JsonObject json) {
    package_json(json, module_name,
                 "C02", C02_levels);
}

void Loom_K30::set_Uart(Uart* serial) {
    K30_Serial = serial;
}
