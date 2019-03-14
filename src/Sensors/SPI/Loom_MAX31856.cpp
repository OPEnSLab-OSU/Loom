
#include "Loom_MAX31856.h"


/////////////////////////////////////////////////////////////////////
Loom_MAX31856::Loom_MAX31856(	
		char*		module_name,
		char*		sensor_description, 
		uint8_t		num_samples, 
		
		uint8_t		CS_pin,

		uint8_t		SPI_a,
		uint8_t		SPI_b,
		uint8_t		SPI_c,
		uint8_t		SPI_d

						  ) : LoomSPISensor( module_name, sensor_description, num_samples ) 
{

	// Hardware Serial
	// inst_max = new Adafruit_MAX31856(CS_pin);

	// Software Serial
	inst_max = new Adafruit_MAX31856(SPI_a, SPI_b, SPI_c, SPI_d);
	

	inst_max->begin();

	inst_max->setThermocoupleType(MAX31856_TCTYPE_K);
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_MAX31856::~Loom_MAX31856() 
{
	delete inst_max;
}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void Loom_MAX31856::print_config()
{
	LoomSPISensor::print_config();
	
	Print("Thermocouple type: ");
	switch ( inst_max->getThermocoupleType() ) {
		case MAX31856_TCTYPE_B  : Println("B Type"); break;
		case MAX31856_TCTYPE_E  : Println("E Type"); break;
		case MAX31856_TCTYPE_J  : Println("J Type"); break;
		case MAX31856_TCTYPE_K  : Println("K Type"); break;
		case MAX31856_TCTYPE_N  : Println("N Type"); break;
		case MAX31856_TCTYPE_R  : Println("R Type"); break;
		case MAX31856_TCTYPE_S  : Println("S Type"); break;
		case MAX31856_TCTYPE_T  : Println("T Type"); break;
		case MAX31856_VMODE_G8  : Println("Voltage x8 Gain mode"); break;
		case MAX31856_VMODE_G32 : Println("Voltage x8 Gain mode"); break;
		default: Println("Unknown"); break;
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_MAX31856::print_measurements() 
{
	print_module_label();
	Println4("\t", "Temperature: ", temperature, " C");
}

/////////////////////////////////////////////////////////////////////
void Loom_MAX31856::measure() 
{
	cj_temp = inst_max->readCJTemperature();
	temperature = inst_max->readThermocoupleTemperature();

	if (print_verbosity == Verbosity::V_HIGH) {
		// Check and print any faults
		uint8_t fault = inst_max->readFault();
		if (fault) {
			if (fault & MAX31856_FAULT_CJRANGE) Println("Cold Junction Range Fault");
			if (fault & MAX31856_FAULT_TCRANGE) Println("Thermocouple Range Fault");
			if (fault & MAX31856_FAULT_CJHIGH)  Println("Cold Junction High Fault");
			if (fault & MAX31856_FAULT_CJLOW)   Println("Cold Junction Low Fault");
			if (fault & MAX31856_FAULT_TCHIGH)  Println("Thermocouple High Fault");
			if (fault & MAX31856_FAULT_TCLOW)   Println("Thermocouple Low Fault");
			if (fault & MAX31856_FAULT_OVUV)    Println("Over/Under Voltage Fault");
			if (fault & MAX31856_FAULT_OPEN)    Println("Thermocouple Open Fault");
		}
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_MAX31856::package(OSCBundle& bndl, char* suffix) 
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle(bndl, id_prefix, "Temp", temperature, NEW_MSG);
}


