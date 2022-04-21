///////////////////////////////////////////////////////////////////////////////
///
/// @file		ADS1232.h
/// @brief		File for ADS1232 definition.
/// @author		Jace Parks
/// @date		2022
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#define _dout 6
#define _sclk 7
#define _pdwn 8

#include "Serial_Sensor.h"
#include <Adafruit_ADS1232.h>

///////////////////////////////////////////////////////////////////////////////
///
/// ADS1232 
///
/// @par Resources
///
///
///////////////////////////////////////////////////////////////////////////////

namespace Loom {

class ADS1232 : public SerialSensor
{
protected:
	Adafruit_ADS1232 weight =  Adafruit_ADS1232(_pdwn,_sclk,_dout);

public:
    ADS1232(
		const char* module_name = "ADS1232",
        int         num_samples = 1
	);

    ADS1232(JsonArrayConst p);
    //ADS1232() = default;

    virtual ~ADS1232() = default;

    void		measure() override;
	void		package(JsonObject json) override;

    //void		print_config() const override;
	void		print_measurements() const override;

private:
    
};

///////////////////////////////////////////////////////////////////////////////
REGISTER(Module, ADS1232, "ADS1232");
///////////////////////////////////////////////////////////////////////////////    

};

#endif

