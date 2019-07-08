//
//  Loom_K30.hpp
//  
//
//  Created by Eli Winkelman on 7/8/19.
//

#pragma once

#ifndef Loom_K30_hpp
#define Loom_K30_hpp

#include "Loom_Sensor.h"
#include "Arduino.h"

class Loom_K30 : public LoomSensor
{
    
protected:
    float C02_levels;
    //Command packet to read Co2 (see app note)
    byte read_C02[7] = {0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25};  //Command packet to read Co2 (see app note)
    //create an array to store the response
    byte sensor_response[7] = {0, 0, 0, 0, 0, 0, 0};
    
    //
    
    //Using pin 12 for Rx and pin 11 for Tx
    Uart* K30_Serial = nullptr; //Sets up a virtual serial port
    
    void sendSensorRequest(byte request[7]);
    int readSensorResponse(byte packet[7]);
    
    
public:
    /// Constructor
    /// \param[in]      module_name
    Loom_K30(
        const char* module_name = "K30",
        int         num_samples = 1
             );
    
    Loom_K30(JsonVariant p);
    
    /// Destructor
    virtual ~Loom_K30();
    
    /// Inherited Methods
    void print_measurements() override;
    void measure() override;
    void package(JsonObject json) override;
    
    /// Sensor Specific Methods
    void set_Uart(Uart* serial);
};

#endif /* Loom_K30_hpp */
