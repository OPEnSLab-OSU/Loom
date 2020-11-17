
#include "STEMMA.h"


///////////////////////////////////////////////////////////////////////////////
Loom_STEMMA::Loom_STEMMA(
LoomManager* manager,
const byte i2c_address,
                         const uint8_t  mux_port
        )
: LoomI2CSensor(manager, "STEMMA", Type::STEMMA, i2c_address, mux_port) {
  LMark;

    ss = Adafruit_seesaw();
    LMark;

    LPrintln(i2c_address);
    LMark;
    // Try to setup sensor
    bool setup = ss.begin(i2c_address);
    LMark;

    if (!setup) active = false;
    LMark;

    print_module_label();
    LMark;
    LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
    LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_STEMMA::Loom_STEMMA(LoomManager* manager, JsonArrayConst p)
: Loom_STEMMA(manager, EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_STEMMA::print_measurements() const
{
    LMark;
    print_module_label();
    LMark;
    LPrintln("Measurements:");
    LMark;
    LPrintln("\tTemperature: ", temperature);
    LMark;
    LPrintln("\tCapacitive: ", capacitive);
    LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_STEMMA::measure() {
    LMark;
    temperature = ss.getTemp();
    LMark;
    capacitive = ss.touchRead(0);
    LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_STEMMA::package(JsonObject json) {
    LMark;
    JsonObject data = get_module_data_object(json, module_name);
    LMark;

    data["temperature"] = temperature;
    LMark;
    data["capactive"] = capacitive;
    LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_STEMMA::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
