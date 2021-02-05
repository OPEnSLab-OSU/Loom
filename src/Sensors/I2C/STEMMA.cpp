///////////////////////////////////////////////////////////////////////////////
///
/// @file		STEMMA.cpp
/// @brief		File for STEMMA implementation.
/// @author		Eli Winkelman
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "STEMMA.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
STEMMA::STEMMA(
        const byte i2c_address,
        const uint8_t  mux_port
    )
    : I2CSensor("STEMMA", i2c_address, mux_port)
{
    LMark;
    ss = Adafruit_seesaw();

    LPrintln(i2c_address);
    LMark;
    // Try to setup sensor
    bool setup = ss.begin(i2c_address);

    if (!setup) active = false;

    print_module_label();
    LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
STEMMA::STEMMA(JsonArrayConst p)
: STEMMA( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void STEMMA::print_measurements() const
{
    print_module_label();
    LPrintln("Measurements:");
    LMark;
    LPrintln("\tTemperature: ", temperature);
    LMark;
    LPrintln("\tCapacitive: ", capacitive);
}

///////////////////////////////////////////////////////////////////////////////
void STEMMA::measure() {
    LMark;
    temperature = ss.getTemp();
    LMark;
    capacitive = ss.touchRead(0);
}

///////////////////////////////////////////////////////////////////////////////
void STEMMA::package(JsonObject json) {
    LMark;
    JsonObject data = get_module_data_object(json, module_name);
    LMark;

    data["temperature"] = temperature;
    LMark;
    data["capactive"] = capacitive;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
