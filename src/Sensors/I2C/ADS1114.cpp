///////////////////////////////////////////////////////////////////////////////
///
/// @file		ADS1114.cpp
/// @brief		File for ADS1114 implementation.
/// @author
/// @date
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "ADS1114.h"
#include "Module_Factory.h"

//ADS1114 ADS1114(i2c_address);

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
ADS1114::ADS1114(
		const 	byte		i2c_address,
		const	uint8_t 	mux_port,
		const 	bool		analog_0_enabled,
		const 	bool		analog_1_enabled,
		const 	bool		diff_enabled,
		const	Gain		gain
	)
	: I2CSensor("ADS1114", i2c_address, mux_port)
	, ads1114(i2c_address)
	, analog_enabled{ analog_0_enabled, analog_1_enabled }
	, diff_enabled{ diff_enabled }
	, analog_reads{}
	, diff_read()
{
  LMark;
	// Gain is an internal value in this driver, so this function
	// does not actually write to the I2C bus
	LPrintln("setting gain: ",(uint32_t)gain);
	ads1114.setGain(static_cast<adsGain_t>(static_cast<uint32_t>(gain)));
	ads1114.begin();
}

///////////////////////////////////////////////////////////////////////////////
ADS1114::ADS1114(JsonArrayConst p)
	: ADS1114(p[0], p[1], p[2], p[3], p[4], (Gain)(uint32_t)p[5] ) {}

///////////////////////////////////////////////////////////////////////////////
void ADS1114::print_config() const
{
	LPrintln('\t', "i2c_address: ", i2c_address);
	LPrintln('\t', "module_name: ", "ADS1114" );
	LPrintln('\t', "Using Analog 0: ", analog_enabled[0]);
	LPrintln('\t', "Using Analog 1 : ", analog_enabled[1]);
	LPrintln('\t', "Using differential Measurements A0 and A1 : ", diff_enabled );
}

///////////////////////////////////////////////////////////////////////////////
void ADS1114::print_measurements() const
{
	print_module_label();
	LPrintln("************************ \n");
	LPrintln("Measurements for simple: \n");
	if (analog_enabled[0]) LPrintln("\t", "A0  : ", analog_reads[0]);
	if (analog_enabled[1]) LPrintln("\t", "A1  : ", analog_reads[1]);
	if (diff_enabled) {
		LPrintln("Measurements for derivative: \n");
		if (diff_enabled) LPrintln("\t", "DA0  : ", diff_read);
	}
}

///////////////////////////////////////////////////////////////////////////////
void ADS1114::measure()
{
	// for (uint8_t i = 0; i < 2; i++) {
	// 	if (analog_enabled[i])
	// 		analog_reads[i] = ads1114.readADC_SingleEnded(i);
	// 		LPrintln("\nreading ADC , ", i, " ", analog_reads[i]);
	// }
	analog_reads[0] = ads1114.readADC_SingleEnded(0);
	analog_reads[1] = ads1114.readADC_SingleEnded(1);
	if (diff_enabled)
		diff_read = ads1114.readADC_Differential_0_1();
}

///////////////////////////////////////////////////////////////////////////////
void ADS1114::package(JsonObject json)
{
   	LMark;
		JsonObject data = get_module_data_object(json, module_name);
		if (analog_enabled[0]) data["analog0"] = analog_reads[0];
		if (analog_enabled[1]) data["analog1"] = analog_reads[1];
		if (diff_enabled) data["diff"] = diff_read;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
