///////////////////////////////////////////////////////////////////////////////
///
/// @file		Serial_Sensor.h
/// @brief		File for Serial Sensor definition. 
/// @author		Eli Winkelman and Kenneth Kang
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../Sensor.h"

///////////////////////////////////////////////////////////////////////////////
///
/// Abstract root for Serial/UART sensor modules.
/// @par Resources
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#serial-sensors)
///
///////////////////////////////////////////////////////////////////////////////
class LoomSerialSensor : public LoomSensor {

protected:

    // a serial stream for the sensor
    Stream* sensor_serial = nullptr;

public:
    
    //=============================================================================
    ///@name    CONSTRUCTORS / DESTRUCTOR
    /*@{*/ //======================================================================

        /// Constructor
        /// @param[in]    module_name        Name of the module (provided by derived classes)
        /// @param[in]    module_type        Type of the module (provided by derived classes)
        /// @param[in]    num_samples        The number of samples to take and average
    LoomSerialSensor(
            LoomManager* Manager,
            const char*        module_name,
            const LoomModule::Type module_type,
            const uint8_t       num_samples
    );
    
    /// Destructor
    virtual ~LoomSerialSensor();
    
    //=============================================================================
    ///@name    PRINT INFORMATION
    /*@{*/ //======================================================================
    virtual void print_config() const override;
    
    //=============================================================================
    ///@name    SETTERS
    /*@{*/ //======================================================================
    
    /// Set the sensor's serial stream
    /// @param[in]  stream      A serial stream to read sensor data from
    void set_serial(Stream* stream);
    
private:
};
