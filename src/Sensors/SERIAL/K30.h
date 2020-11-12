///////////////////////////////////////////////////////////////////////////////
///
/// @file		K30.h
/// @brief		File for K30 CO2 Sensor definition. 
/// @author		Eli Winkelman and Kenneth Kang
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Serial_Sensor.h"

///////////////////////////////////////////////////////////////////////////////
///
/// K30 CO2 sensor module.
///
/// @par Resources
/// - [Module Documentation](https://github.com/OPEnSLab-OSU/OPEnS-Lab-Home/wiki/K30-CO2-Sensor-(SE-0018))
/// - [Product Page: CO2Meter K30 10,000ppm CO2 Sensor](https://www.co2meter.com/products/k-30-co2-sensor-module?variant=8463942)
/// - [Datasheet: K30](http://co2meters.com/Documentation/Datasheets/DS_SE_0118_CM_0024_Revised9%20(1).pdf)
/// - [Arduino UART Interface to K30 Sensor](http://co2meters.com/Documentation/Other/AN_SE_0018_AN_126_Revised8.pdf)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#k30-co2-sensor)
///
///////////////////////////////////////////////////////////////////////////////

class Loom_K30 : public LoomSerialSensor
{
    
protected:

    float CO2_levels;
    //Command packet to read Co2 (see app note)
    byte read_CO2[7] = {0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25};  //Command packet to read Co2 (see app note)
    //create an array to store the response
    byte sensor_response[7] = {0, 0, 0, 0, 0, 0, 0};
    
    //
    
    //Using pin 12 for Rx and pin 11 for Tx
    Uart* K30_Serial = nullptr; //Sets up a virtual serial port
    
    void sendSensorRequest(byte request[7]);
    int readSensorResponse(byte packet[7]);
    
    
public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

    /// Constructor
    /// \param[in]      module_name The name of the module
    /// \param[in]      num_samples The number of samples to take and average
    Loom_K30(LoomManager* manager, 
        const char* module_name = "K30",
        int         num_samples = 1
             );
    
    Loom_K30(LoomManager* manager, JsonArrayConst p);
    
    /// Destructor
    virtual ~Loom_K30() = default;
    
//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

    void        print_measurements()   const override;


//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

    void        measure() override;
    void        package(JsonObject json) override;
    
};


