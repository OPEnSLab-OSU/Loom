
#include "Loom_STEMMA.h"

Loom_STEMMA::Loom_STEMMA(
                         const byte     i2c_address,
                         const uint8_t  mux_port
        )
: LoomI2CSensor("STEMMA", Type::STEMMA, i2c_address, mux_port) {
    
    ss = Adafruit_seesaw();
    
    LPrintln(i2c_address);
    // Try to setup sensor
    bool setup = ss.begin(i2c_address);
        
    if (!setup) active = false;

    print_module_label();
    LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

Loom_STEMMA::Loom_STEMMA(JsonArrayConst p)
: Loom_STEMMA( EXPAND_ARRAY(p, 2) ) {}

void Loom_STEMMA::print_measurements() const
{
    print_module_label();
    LPrintln("Measurements:");
    LPrintln("\tTemperature: ", temperature);
    LPrintln("\tCapacitive: ", capacitive);
}

void Loom_STEMMA::measure() {
    temperature = ss.getTemp();
    capacitive = ss.touchRead(0);
}

void Loom_STEMMA::package(JsonObject json) {
    JsonObject data = get_module_data_object(json, module_name);
    
    data["temperature"] = temperature;
    data["capactive"] = capacitive;
}
