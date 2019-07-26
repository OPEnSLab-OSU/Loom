
#include "Loom_MAX31856.h"


///////////////////////////////////////////////////////////////////////////////
Loom_MAX31856::Loom_MAX31856(	
		const char*		module_name,
		uint8_t			num_samples, 
		
		uint8_t			CS_pin,

		uint8_t			SPI_a,
		uint8_t			SPI_b,
		uint8_t			SPI_c,
		uint8_t			SPI_d
	) : LoomSPISensor( module_name, Type::MAX31856, num_samples ) 
	, inst_max( Adafruit_MAX31856(SPI_a, SPI_b, SPI_c, SPI_d) )
{
	// Hardware Serial
	// inst_max = new Adafruit_MAX31856(CS_pin);

	// Software Serial
	// inst_max = new Adafruit_MAX31856(SPI_a, SPI_b, SPI_c, SPI_d);

	inst_max.begin();
	inst_max.setThermocoupleType(MAX31856_TCTYPE_K);
}

///////////////////////////////////////////////////////////////////////////////
Loom_MAX31856::Loom_MAX31856(JsonArrayConst p)
	: Loom_MAX31856( EXPAND_ARRAY(p, 7) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31856::print_config()
{
	LoomSPISensor::print_config();
	
	LPrint("Thermocouple type: ");
	switch ( inst_max.getThermocoupleType() ) {
		case MAX31856_TCTYPE_B  : LPrint("B"); break;
		case MAX31856_TCTYPE_E  : LPrint("E"); break;
		case MAX31856_TCTYPE_J  : LPrint("J"); break;
		case MAX31856_TCTYPE_K  : LPrint("K"); break;
		case MAX31856_TCTYPE_N  : LPrint("N"); break;
		case MAX31856_TCTYPE_R  : LPrint("R"); break;
		case MAX31856_TCTYPE_S  : LPrint("S"); break;
		case MAX31856_TCTYPE_T  : LPrint("T"); break;
		case MAX31856_VMODE_G8  : LPrint("Voltage x8 Gain mode"); break;
		case MAX31856_VMODE_G32 : LPrint("Voltage x8 Gain mode"); break;
		default: LPrint("Unknown"); break;
	}
	LPrintln(" Type");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31856::print_measurements() 
{
	print_module_label();
	LPrintln("\tTemp: ", temperature, " C");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31856::measure() 
{
	// cj_temp = inst_max.readCJTemperature();
	// temperature = inst_max.readThermocoupleTemperature();

	int i = num_samples;
	float cj = 0, temp = 0;

	while (i--) {
		cj += inst_max.readCJTemperature();
		temp += inst_max.readThermocoupleTemperature();

		// Check and print any faults
		uint8_t fault = inst_max.readFault();
		if ( (fault) && (print_verbosity == Verbosity::V_HIGH) ) {
			if (fault & MAX31856_FAULT_CJRANGE) LPrintln("Cold Junction Range Fault");
			if (fault & MAX31856_FAULT_TCRANGE) LPrintln("Thermocouple Range Fault");
			if (fault & MAX31856_FAULT_CJHIGH)  LPrintln("Cold Junction High Fault");
			if (fault & MAX31856_FAULT_CJLOW)   LPrintln("Cold Junction Low Fault");
			if (fault & MAX31856_FAULT_TCHIGH)  LPrintln("Thermocouple High Fault");
			if (fault & MAX31856_FAULT_TCLOW)   LPrintln("Thermocouple Low Fault");
			if (fault & MAX31856_FAULT_OVUV)    LPrintln("Over/Under Voltage Fault");
			if (fault & MAX31856_FAULT_OPEN)    LPrintln("Thermocouple Open Fault");
			break;
		} 
	}

	cj_temp = cj / num_samples;
	temperature   = temp / num_samples;

}

///////////////////////////////////////////////////////////////////////////////
void Loom_MAX31856::package(JsonObject json)
{
	package_json(json, module_name, 
		"Temp", temperature
	);
}

///////////////////////////////////////////////////////////////////////////////

