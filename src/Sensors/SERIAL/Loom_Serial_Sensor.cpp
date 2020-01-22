//
// Created by Eli Winkelman on 1/7/20.
//

#include "Loom_Serial_Sensor.h"

////////////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
LoomSerialSensor::LoomSerialSensor(
        const char*        module_name,
        const LoomModule::Type module_type,
        const uint8_t       num_samples
    )
    : LoomSensor( module_name, module_type, num_samples ) {}

////////////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
LoomSerialSensor::~LoomSerialSensor() {
    delete sensor_serial;
}

////////////////////////////////////////////////////////////////////////////
// -- PUBLIC METHODS --
void LoomSerialSensor::print_config() const
{
    LoomSensor::print_config();
    LPrint('\t', "Serial Set         : ");
    LPrintln(sensor_serial != nullptr);
}

void LoomSerialSensor::set_serial(Stream *stream) {
    sensor_serial = stream;
}
