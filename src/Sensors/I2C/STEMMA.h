///////////////////////////////////////////////////////////////////////////////
///
/// Adafruit Stemma soil moisture sensor.
///
/// @par Resources
/// - [Product Page] (https://learn.adafruit.com/adafruit-stemma-soil-sensor-i2c-capacitive-moisture-sensor/arduino-test)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#stemma-soil-sensor)
/// - [DataSheet](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-stemma-soil-sensor-i2c-capacitive-moisture-sensor.pdf?timestamp=1594765200)
///
//////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "I2C_Sensor.h"

#include <Adafruit_seesaw.h>

namespace Loom {

///////////////////////////////////////////////////////////////////////////////
class STEMMA : public I2CSensor
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
    STEMMA(
            byte           i2c_address = 0x36,
            const uint8_t  mux_port    = 255
        );

    /// Constructor that takes Json Array, extracts args
    /// and delegates to regular constructor
    /// \param[in]    p        The array of constuctor args to expand
    STEMMA(JsonArrayConst p);
    
    /// Destructor
    virtual ~STEMMA() = default;

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

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, STEMMA, "STEMMA");
///////////////////////////////////////////////////////////////////////////////

}; // namespace Loom

#endif // ifdef LOOM_INCLUDE_SENSORS


