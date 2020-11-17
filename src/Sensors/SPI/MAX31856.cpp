///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_MAX31856.cpp
/// @brief		File for Loom_MAX31856 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "MAX31856.h"

///////////////////////////////////////////////////////////////////////////////
Loom_MAX31856::Loom_MAX31856(
		LoomManager* manager,
		const char* module_name,
		const uint8_t	num_samples,
		const uint8_t	CS_pin,
		const uint8_t	SPI_a,
		const uint8_t	SPI_b,
		const uint8_t	SPI_c,
		const uint8_t	SPI_d
	) : LoomSPISensor(manager, module_name, Type::MAX31856, num_samples )
	, inst_max( Adafruit_MAX31856(SPI_a, SPI_b, SPI_c, SPI_d) )
{
  LMark;
	// Hardware Serial
	// inst_max = new Adafruit_MAX31856(CS_pin);

	// Software Serial
	// inst_max = new Adafruit_MAX31856(SPI_a, SPI_b, SPI_c, SPI_d);

	inst_max.begin();
  LMark;
	inst_max.setThermocoupleType(MAX31856_TCTYPE_K);
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_MAX31856::Loom_MAX31856(LoomManager* manager, JsonArrayConst p)
	: Loom_MAX31856(manager, EXPAND_ARRAY(p, 7) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31856::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("\tTemp: ", temperature, " C");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31856::measure()
{
  LMark;
	// cj_temp = inst_max.readCJTemperature();
	// temperature = inst_max.readThermocoupleTemperature();

	int i = num_samples;
  LMark;
	float cj = 0, temp = 0;
  LMark;

	while (i--) {
   	LMark;
		cj += inst_max.readCJTemperature();
   	LMark;
		temp += inst_max.readThermocoupleTemperature();
  	LMark;

		// Check and print any faults
		uint8_t fault = inst_max.readFault();
   	LMark;
		if ( (fault) && (print_verbosity == Verbosity::V_HIGH) ) {
    	LMark;
			if (fault & MAX31856_FAULT_CJRANGE) LPrintln("Cold Junction Range Fault");
    	LMark;
			if (fault & MAX31856_FAULT_TCRANGE) LPrintln("Thermocouple Range Fault");
    	LMark;
			if (fault & MAX31856_FAULT_CJHIGH)  LPrintln("Cold Junction High Fault");
    	LMark;
			if (fault & MAX31856_FAULT_CJLOW)   LPrintln("Cold Junction Low Fault");
    	LMark;
			if (fault & MAX31856_FAULT_TCHIGH)  LPrintln("Thermocouple High Fault");
    	LMark;
			if (fault & MAX31856_FAULT_TCLOW)   LPrintln("Thermocouple Low Fault");
    	LMark;
			if (fault & MAX31856_FAULT_OVUV)    LPrintln("Over/Under Voltage Fault");
    	LMark;
			if (fault & MAX31856_FAULT_OPEN)    LPrintln("Thermocouple Open Fault");
    	LMark;
			break;
		}
	}

	cj_temp = cj / num_samples;
  LMark;
	temperature   = temp / num_samples;
  LMark;

}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31856::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;
	data["temp"] = temperature;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31856::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
