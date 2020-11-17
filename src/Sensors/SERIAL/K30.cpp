///////////////////////////////////////////////////////////////////////////////
///
/// @file		K30.cpp
/// @brief		File for K30 CO2 Sensor implementation.
/// @author		Eli Winkelman and Kenneth Kang
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "K30.h"

Loom_K30::Loom_K30(LoomManager* manager,
                   const char* module_name,
                   int num_samples)
: LoomSerialSensor(manager, module_name, Type::K30, num_samples)
{
    LMark;
    print_module_label();
    LMark;
    LPrintln("Initializing K30");
    LMark;

    print_module_label();
    LMark;
    LPrintln("Finished Initialize K30");
    LMark;

}

///////////////////////////////////////////////////////////////////////////////

Loom_K30::Loom_K30(LoomManager* manager, JsonArrayConst p)
: Loom_K30(manager, EXPAND_ARRAY(p, 2))
{}

///////////////////////////////////////////////////////////////////////////////

void Loom_K30::sendSensorRequest(byte request[7]) {
  LMark;

    if (sensor_serial == nullptr) {
        LMark;
        LPrintln("Serial is not set, cannot send request");
        LMark;
        return;
    }

    LPrintln("Sending Request to K30 Sensor");
    LMark;
    delay(100);
    LMark;
    while (!sensor_serial -> available()) //keep sending request until we start to get a response
    {
        LMark;
        LPrintln("Sensor_Serial Available, Retrieving Response");  // TODO:This doesn't make sense: in line 41 and line 49
        LMark;
        sensor_serial -> write(request, 7);
        LMark;
        delay(50);
        LMark;
    }

    int timeout = 0; //set a timeoute counter
    LMark;
    while (sensor_serial -> available() < 7 ) //Wait to get a 7 byte response
    {
        LMark;
        LPrintln("Sensor_Serial Unavailable, Trying to send request again");
        LMark;
        timeout++;
        LMark;
        if (timeout > 10)   //if it takes to long there was probably an error
        {
            LMark;
            while (sensor_serial -> available()) //flush whatever we have
                sensor_serial -> read();
            LMark;
            break;                        //exit and try again
        }
        delay(50);
        LMark;
    }
    for (int i = 0; i < 7; i++)
    {
        LMark;
        sensor_response[i] = sensor_serial -> read();
        LMark;
    }
    LPrintln("Finished Sending Request to K30 sensor");
    LMark;
}

///////////////////////////////////////////////////////////////////////////////

int Loom_K30::readSensorResponse(byte packet[7]){
    LMark;
    int high = packet[3];                        //high byte for value is 4th byte in packet in the packet
    LMark;
    int low = packet[4];                         //low byte for value is 5th byte in the packet
    LMark;
    unsigned long val = high * 256 + low;        //Combine high byte and low byte with this formula to get value
    LMark;
    return val;

}

///////////////////////////////////////////////////////////////////////////////

void Loom_K30::measure() {
    LMark;
    sendSensorRequest(read_CO2);
    LMark;
    CO2_levels = readSensorResponse(sensor_response);
    LMark;
}

///////////////////////////////////////////////////////////////////////////////

void Loom_K30::print_measurements() const {
    LMark;
    print_module_label();
    LMark;
    LPrintln("Measurements:");
    LMark;
    LPrintln("CO2 Levels: ", CO2_levels);
    LMark;
}

///////////////////////////////////////////////////////////////////////////////

void Loom_K30::package(JsonObject json) {
    LMark;
    JsonObject data = get_module_data_object(json, module_name);
    LMark;

    data["CO2"] = CO2_levels;
    LMark;

}

///////////////////////////////////////////////////////////////////////////////
void Loom_K30::diagnose(bool& result){
  LMark;
	// implement here
}
///////////////////////////////////////////////////////////////////////////////
