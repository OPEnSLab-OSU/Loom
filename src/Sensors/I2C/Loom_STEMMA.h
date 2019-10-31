#pragma once

#include "Loom_I2C_Sensor.h"
#include "Adafruit_seesaw.h"

///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// Class description
// ###
class Loom_STEMMA : public LoomI2CSensor
{

protected:

    // Protected members here (are inherited)
    Adafruit_seesaw ss;
    float temperature;
    uint16_t capacitive;
    
public:

//=============================================================================
///@name    CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

    /// Constructor
    ///
    /// @param[in] i2c_address              Set(Int) | <0x36> | {0x36} | I2C address
    Loom_STEMMA(
            byte                i2c_address        = 0x36,
            const uint8_t       mux_port           = 255
        );

    /// Constructor that takes Json Array, extracts args
    /// and delegates to regular constructor
    /// \param[in]    p        The array of constuctor args to expand
    Loom_STEMMA(JsonArrayConst p);
    
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

//=============================================================================
///@name    GETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name    SETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name    MISCELLANEOUS
/*@{*/ //======================================================================



private:

    // Private members (are not inherited)

    // Auxiliary functions

};




