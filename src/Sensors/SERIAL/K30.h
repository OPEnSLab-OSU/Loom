//
//  Loom_K30.hpp
//  
//
//  Created by Eli Winkelman on 1/21/20.
//

#pragma once

#include "Serial_Sensor.h"

class Loom_K30 : public LoomSerialSensor
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
    /// \param[in]      module_name The name of the module
    /// \param[in]      num_samples The number of samples to take and average
    Loom_K30(
        const char* module_name = "K30",
        int         num_samples = 1
             );
    
    Loom_K30(JsonArrayConst p);
    
    /// Destructor
    virtual ~Loom_K30() = default;
    
    /// Inherited Methods
    void print_measurements() const override;
    void measure() override;
    void package(JsonObject json) override;
    
};


