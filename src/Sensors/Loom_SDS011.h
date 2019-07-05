//
//  Loom_SDS001.hpp
//  
//
//  Created by Eli Winkelman on 7/5/19.
//
#pragma once

#ifndef Loom_SDS001_hpp
#define Loom_SDS001_hpp

#include "Loom_Sensor.h"
#include "SDS011-select-serial.h"
#include "Arduino.h"

class Loom_SDS011 : public LoomSensor
{
    
protected:
    //Underlying SDS011 sensor manager
    SDS011* inst_SDS011;

    float p10, p25; //particles less than 10 micrometers, 2.5 micrometers (PM10 and PM2.5 air quality standard)
    
public:
    /// Constructor
    /// \param[in]    module_name                String | <"SDS011"> | SDS011 module name

    Loom_SDS011(
        const char* module_name = "SDS011",
        int         num_samples = 3
    );
    Loom_SDS011(JsonVariant p);
    
    /// Destructor
    virtual ~Loom_SDS011();
    
    /// Inherited Methods
    void        print_measurements() override;
    void        measure() override;
    void        package(JsonObject json) override;
    
};

#endif /* Loom_SDS001_hpp */
