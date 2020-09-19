
#ifdef LOOM_INCLUDE_SENSORS

#include "STEMMA.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

REGISTER(Module, STEMMA, "STEMMA");

///////////////////////////////////////////////////////////////////////////////
STEMMA::STEMMA(
        const byte i2c_address,
        const uint8_t  mux_port
    )
    : I2CSensor("STEMMA", Type::STEMMA, i2c_address, mux_port) 
{
    ss = Adafruit_seesaw();
    
    LPrintln(i2c_address);
    // Try to setup sensor
    bool setup = ss.begin(i2c_address);
        
    if (!setup) active = false;

    print_module_label();
    LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

STEMMA::STEMMA(JsonArrayConst p)
: STEMMA( EXPAND_ARRAY(p, 2) ) {}

void STEMMA::print_measurements() const
{
    print_module_label();
    LPrintln("Measurements:");
    LPrintln("\tTemperature: ", temperature);
    LPrintln("\tCapacitive: ", capacitive);
}

void STEMMA::measure() {
    temperature = ss.getTemp();
    capacitive = ss.touchRead(0);
}

void STEMMA::package(JsonObject json) {
    JsonObject data = get_module_data_object(json, module_name);
    
    data["temperature"] = temperature;
    data["capactive"] = capacitive;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
