//
//  Loom_SDS011.cpp
//  
//
//  Created by Eli Winkelman on 7/5/19.
//

#include "Loom_SDS011.h"

Loom_SDS011::Loom_SDS011(
                         const char*    module_name,
                         int            num_samples
                         )
: LoomSensor(module_name, num_samples)
{
    
    this->module_type = ModuleType::SDS011;
    

    Serial1.begin(9600);
    // initialize the underlying sensor manager
    inst_SDS011 = new SDS011(Serial1);

    print_module_label();
    LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

Loom_SDS011::Loom_SDS011(JsonVariant p)
    : Loom_SDS011( EXPAND_ARRAY(p, 2) )
{}

Loom_SDS011::~Loom_SDS011() {
    delete inst_SDS011;
}

void Loom_SDS011::measure() {
    inst_SDS011 -> read(&p25, &p10);
}

void Loom_SDS011::print_measurements() {
    print_module_label();
    LPrintln("Measurements:");
    
    LPrintln("p10: ", p10);
    LPrintln("p25: ", p25);
}

void Loom_SDS011::package(JsonObject json)
{
    package_json(json, module_name,
                 "P10", p10,
                 "P25", p25);
}
