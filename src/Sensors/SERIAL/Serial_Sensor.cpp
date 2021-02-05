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
        const uint8_t          num_samples
    )
    : Sensor(module_name, num_samples) {}

////////////////////////////////////////////////////////////////////////////
SerialSensor::~SerialSensor() {
     LMark;
    delete sensor_serial;
}

////////////////////////////////////////////////////////////////////////////
void SerialSensor::print_config() const
{
    LMark;
    Sensor::print_config();
    LMark;
    LPrint('\t', "Serial Set         : ");
    LMark;
    LPrintln(sensor_serial != nullptr);
}

void SerialSensor::set_serial(Stream *stream) {
    LMark;
    sensor_serial = stream;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
