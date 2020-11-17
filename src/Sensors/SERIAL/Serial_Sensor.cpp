///////////////////////////////////////////////////////////////////////////////
///
/// @file		Serial_Sensor.cpp
/// @brief		File for Serial Sensor implementation.
/// @author		Eli Winkelman and Kenneth Kang
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Serial_Sensor.h"

////////////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
LoomSerialSensor::LoomSerialSensor(
        LoomManager* Manager,
        const char*        module_name,
        const LoomModule::Type module_type,
        const uint8_t       num_samples
    )
    : LoomSensor(Manager, module_name, module_type, num_samples ) {}

////////////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
LoomSerialSensor::~LoomSerialSensor() {
    LMark;
    delete sensor_serial;
    LMark;
}

////////////////////////////////////////////////////////////////////////////
// -- PUBLIC METHODS --
void LoomSerialSensor::print_config() const
{
    LMark;
    LoomSensor::print_config();
    LMark;
    LPrint('\t', "Serial Set         : ");
    LMark;
    LPrintln(sensor_serial != nullptr);
    LMark;
}

void LoomSerialSensor::set_serial(Stream *stream) {
    LMark;
    sensor_serial = stream;
    LMark;
}
