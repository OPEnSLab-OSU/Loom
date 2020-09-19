///////////////////////////////////////////////////////////////////////////////
///
/// @file		Serial_Sensor.cpp
/// @brief		File for Serial Sensor implementation. 
/// @author		Eli Winkelman and Kenneth Kang
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "Serial_Sensor.h"

using namespace Loom;

////////////////////////////////////////////////////////////////////////////
SerialSensor::SerialSensor(
        const char*            module_name,
        const Module::Type module_type,
        const uint8_t          num_samples
    )
    : Sensor(module_name, module_type, num_samples) {}

////////////////////////////////////////////////////////////////////////////
SerialSensor::~SerialSensor() {
    delete sensor_serial;
}

////////////////////////////////////////////////////////////////////////////
void SerialSensor::print_config() const
{
    Sensor::print_config();
    LPrint('\t', "Serial Set         : ");
    LPrintln(sensor_serial != nullptr);
}

void SerialSensor::set_serial(Stream *stream) {
    sensor_serial = stream;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS