#pragma once

#include "I2C_Sensor.h"
#include "Adafruit_seesaw.h"

///////////////////////////////////////////////////////////////////////////////
///
/// Adafruit Stemma soil moisture sensor.
///
/// @par Resources
/// - [Product Page: https://learn.adafruit.com/adafruit-stemma-soil-sensor-i2c-capacitive-moisture-sensor/arduino-test]
///

class Loom_STEMMA : public LoomI2CSensor
{

protected:

    /// Adafruit Seesaw object.
    Adafruit_seesaw ss;
    
    /// Sensor data objects
    float temperature;
    uint16_t capacitive;
    
public:

//=============================================================================
///@name    CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

    /// Constructor
    ///
    /// @param[in] i2c_address              Set(Int) | <0x36> | {0x36} | I2C address
    /// @param[in] mux_port                     Int | <255> | [0-16] | Port on multiplexer
    Loom_STEMMA(
            LoomManager* manager,
            byte                i2c_address        = 0x36,
            const uint8_t       mux_port           = 255
        );

    /// Constructor that takes Json Array, extracts args
    /// and delegates to regular constructor
    /// \param[in]    p        The array of constuctor args to expand
    Loom_STEMMA(LoomManager* manager, JsonArrayConst p);
    
    /// Destructor
    virtual ~Loom_STEMMA() = default;

//=============================================================================
///@name    OPERATION
/*@{*/ //======================================================================

    void        measure() override;
    void        package(JsonObject json) override;

//=============================================================================
///@name    PRINT INFORMATION
/*@{*/ //======================================================================

    void        print_measurements() const override;

};




