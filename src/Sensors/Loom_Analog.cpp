
#include "Loom_Analog.h"

// #include "Loom_Package.h"

// FlashStorage(analog_flash_config, AnalogConfig);



// Make a static class for analog conversions?
// Is this going to be a member of the Loom_Analog class?
	// this might contain the information about which conversion is being use - selecting conversion should be dynamic
// Point is to contain location of updates adding new conversions

// Should support taking an extra float for use in conversion
	// Like temperature for electrical conductivity 

/////////////////////////////////////////////////////////////////////
float Loom_Analog::convert(uint8_t pin, uint16_t analog)
{
	switch(conversions[pin]) {
		case AnalogConversion::VOLTAGE 		: return convert_voltage(analog);
		case AnalogConversion::THERMISTOR 	: return convert_thermistor(analog);
		case AnalogConversion::PH 			: return convert_pH(analog);
		case AnalogConversion::TURBIDITY 	: return convert_turbidity(analog);
		case AnalogConversion::EC 			: return convert_EC(analog);
		case AnalogConversion::TDS 			: return convert_TDS(analog);
		case AnalogConversion::SALINITY 	: return convert_salinity(analog);
		default								: return (float)analog;  
	}
}


const char* Loom_Analog::conversion_name(AnalogConversion conversion)
{
	switch(conversion) {
		case AnalogConversion::VOLTAGE 		: return "voltage";
		case AnalogConversion::THERMISTOR 	: return "thermistor";
		case AnalogConversion::PH 			: return "pH";
		case AnalogConversion::TURBIDITY 	: return "turbidity";
		case AnalogConversion::EC 			: return "EC";
		case AnalogConversion::TDS 			: return "TDS";
		case AnalogConversion::SALINITY 	: return "salinity";
		default								: return "analog";  
	}
}


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_Analog::Loom_Analog(	
		const char*			module_name,
		uint8_t				num_samples,

		uint8_t				read_resolution,
		bool				enableA0,
		bool				enableA1,
		bool				enableA2,
		bool				enableA3,
		bool				enableA4,
		bool				enableA5,

		AnalogConversion	convertA0,
		AnalogConversion	convertA1,
		AnalogConversion	convertA2,
		AnalogConversion	convertA3,
		AnalogConversion	convertA4,
		AnalogConversion	convertA5	
	) 
	: LoomSensor( module_name, num_samples )
{
	this->module_type = ModuleType::Analog;

	// Set Analog Read Resolution
	this->read_resolution = read_resolution;
	analogReadResolution(this->read_resolution);

	// Zero out array of measurements 
	for (int i = 0; i < ANALOG_COUNT; i++) { 
		analog_vals[i] = 0; 
	}
	battery = 0.;

	// Set enabled pins
	pin_enabled[0] = enableA0;
	pin_enabled[1] = enableA1;
	pin_enabled[2] = enableA2;
	pin_enabled[3] = enableA3;
	pin_enabled[4] = enableA4;
	pin_enabled[5] = enableA5;


	for (int i = 0; i < ANALOG_COUNT; i++) {
		if (pin_enabled[i]) {
			pinMode(14+i, INPUT);
		}
	}

	// Set conversions
	this->enable_conversions = true;
	conversions[0] = convertA0;
	conversions[1] = convertA1;
	conversions[2] = convertA2;
	conversions[3] = convertA3;
	conversions[4] = convertA4;
	conversions[5] = convertA5;


	// print_config_struct();
	// load_config();

}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_Analog::Loom_Analog(JsonVariant p)
	: Loom_Analog(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], (AnalogConversion)(int)p[9], (AnalogConversion)(int)p[10], (AnalogConversion)(int)p[11], (AnalogConversion)(int)p[12], (AnalogConversion)(int)p[13], (AnalogConversion)(int)p[14])
{

}


/////////////////////////////////////////////////////////////////////
/// Destructor
Loom_Analog::~Loom_Analog() 
{
	LPrintln("DESTRUCTOR");
}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void Loom_Analog::print_config()
{
	LoomSensor::print_config();

	LPrintln("\tAnalog Resolution   : ", read_resolution);
	LPrint("\tEnabled Pins        : ");
	for (int i = 0; i < ANALOG_COUNT; i++) { 
		if (pin_enabled[i]) {
			LPrint("A", i, ", ");
		}
	}
	LPrintln();

	// print_config_struct();
}

/////////////////////////////////////////////////////////////////////
void Loom_Analog::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tBattery = ", battery);
	for (int i = 0; i < ANALOG_COUNT; i++) {
		if ( (!enable_conversions) || (conversions[i] == AnalogConversion::NONE) ) {
			LPrintln("\tA", i, " = ", analog_vals[i]);
		} else {
			LPrint("\tA", i, " = ", analog_vals[i]);
			LPrintln( "  [ ", convert(i, analog_vals[i]), " (converted) ]" );
		}
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Analog::measure()
{
	battery = read_analog(VBATPIN) * 2 * 3.3 / (float)pow(2, read_resolution);
	// battery = read_analog(VBATPIN) * 2 * 3.3 ;/// (float)pow(2, read_resolution);

	for (int i = 0; i < 6; i++) {
		if (pin_enabled[i]) {
			analog_vals[i] = read_analog(i);
		}
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_Analog::package(JsonObject json)
{
	package_json(json, module_name, "Vbat", battery);

	char buf[20];
	for (int i = 0; i < ANALOG_COUNT; i++) {
		if (pin_enabled[i]) {

			if ( (!enable_conversions) || (conversions[i] == AnalogConversion::NONE) ) {
				sprintf(buf, "%s%d", "A", i);
				package_json(json, module_name, buf, analog_vals[i]);
			} else {
				sprintf(buf, "%s%d(%s)", "A", i, conversion_name(conversions[i]));
				package_json(json, module_name, buf, convert(i, analog_vals[i]) );
			}		
		}
	}	
}

/////////////////////////////////////////////////////////////////////
void Loom_Analog::set_analog_resolution(uint8_t res) 
{ 
	analogReadResolution(read_resolution = res); 
}

/////////////////////////////////////////////////////////////////////
uint8_t Loom_Analog::get_analog_resolution() 
{ 
	return read_resolution; 
}

/////////////////////////////////////////////////////////////////////
int Loom_Analog::get_analog_val(uint8_t pin) 
{
	return ( (pin >= 0) && (pin < ANALOG_COUNT) ) ? analog_vals[pin] : -1;
}

/////////////////////////////////////////////////////////////////////
float Loom_Analog::get_battery()
{
	return battery;
}

/////////////////////////////////////////////////////////////////////
bool Loom_Analog::get_pin_enabled(uint8_t pin)
{
	return pin_enabled[pin];
}

/////////////////////////////////////////////////////////////////////
void Loom_Analog::set_pin_enabled(uint8_t pin, bool e)
{
	pin_enabled[pin] = e;
	if (pin_enabled[pin]) {
		pinMode(14+pin, INPUT);
	}
}

/////////////////////////////////////////////////////////////////////
AnalogConversion Loom_Analog::get_conversion(uint8_t pin)
{
	return conversions[pin];
}

/////////////////////////////////////////////////////////////////////
void Loom_Analog::set_conversion(uint8_t pin, AnalogConversion c)
{
	conversions[pin] = c;
}

/////////////////////////////////////////////////////////////////////
void Loom_Analog::set_enable_conversions(bool e)
{
	enable_conversions = e;
}

/////////////////////////////////////////////////////////////////////
uint16_t Loom_Analog::read_analog(uint8_t chnl)
{
	int i = num_samples;
	int reading = 0;

	while (i--) {
		reading += analogRead(chnl); 
	}

	switch (num_samples) {
		case 1 : return (reading);
		case 2 : return (reading >> 1); // Divide by 2
		case 4 : return (reading >> 2); // Divide by 4
		case 8 : return (reading >> 3); // Divide by 8
		case 16: return (reading >> 4); // Divide by 16
		default: return (reading);
	}
} 

/////////////////////////////////////////////////////////////////////
float Loom_Analog::convert_voltage(uint16_t analog)
{
	return analog*3.3/pow(2, read_resolution);
}

/////////////////////////////////////////////////////////////////////
#define reverse_connect 1
#define THERMISTORNOMINAL 	10000   // resistance at 25 degrees C
#define TEMPERATURENOMINAL 	25   	// temp. for nominal resistance (almost always 25 C)
#define BCOEFFICIENT 		3950  	// The beta coefficient of the thermistor (usually 3000-4000)
// #define SERIESRESISTOR 	10000    
#define SERIESRESISTOR 		29330  	// the value of the 'other' resistor
#define range_resol  		4095
float Loom_Analog::convert_thermistor(uint16_t analog)
{
	float average = analog;

	#if reverse_connect == 0
		average = range_resol / average - 1;
		average = SERIESRESISTOR / average;
	#endif
	#if reverse_connect == 1
		average = range_resol / average - 1; 
		average = SERIESRESISTOR * average;
	#endif
	// Serial.print("Thermistor resistance "); 
	// Serial.println(average);
 
	float steinhart;
	steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
	steinhart = log(steinhart);                  // ln(R/Ro)
	steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
	steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
	steinhart = 1.0 / steinhart;                 // Invert
	steinhart -= 273.15;                         // convert to C

	return steinhart;
}

/////////////////////////////////////////////////////////////////////
#define PH_Offset 0.0

float Loom_Analog::convert_pH(uint16_t analog)
{
	float voltage = convert_voltage(analog);

	return 3.5*voltage + PH_Offset;
}

/////////////////////////////////////////////////////////////////////
float Loom_Analog::convert_turbidity(uint16_t analog)
{
	float voltage = convert_voltage(analog);

	// LPrintln("turbidity voltage: ", voltage);

	return -1120.4 * (voltage * voltage) + (5742.3 * voltage) - 4352.9;
}	

/////////////////////////////////////////////////////////////////////
#define EC_TEMP 25

float Loom_Analog::convert_EC(uint16_t analog)
{
	float voltage = convert_voltage(analog);

	float compensation_coefficient = 1.0 + 0.02 * (EC_TEMP - 25.0); // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
	float comp_volt = voltage / compensation_coefficient;  			// temperature compensation
	float EC = ( 133.42 * comp_volt * comp_volt * comp_volt - 255.86 * comp_volt * comp_volt + 857.39 * comp_volt ); //convert voltage value to EC value
	
	return EC;
}

/////////////////////////////////////////////////////////////////////
float Loom_Analog::convert_TDS(uint16_t analog)
{
	return convert_EC(analog)/2.;
}


/////////////////////////////////////////////////////////////////////
float Loom_Analog::convert_salinity(uint16_t analog)
{
	// Probably doesn't actually give a value of any worth right now...
	return (analog-76) / .0928;
}
















// Save a FlashStorage struct
void Loom_Analog::save_config() 
{
	// Probably set valid to true here
	LPrintln("Save Analog Config");
// analog_flash_config.write(configuration);
}


// Load a FlashStorage struct, true if valid
bool Loom_Analog::load_config() 
{
	LPrintln("Load Analog Config");

// configuration = analog_flash_config.read();


// 		LPrint("B: ");
// 		print_config_struct();

// 		if (configuration.checksum != 42) {
// 			// configuration.valid = true;
// 			configuration.checksum = 42;
// 			sprintf( configuration.string, "%s", "Data" );
// 			configuration.number = 1;	
// 		}

// 		LPrint("C: ");
// 		print_config_struct();

// 		LPrintln("Incrementing number");
// 		configuration.number++;

// 		LPrint("D: ");
// 		print_config_struct();
}



void Loom_Analog::print_config_struct() 
{
	LPrintln("Analog Flash Struct:");
	// LPrintln("\tValid : ", configuration.valid);
	LPrintln("\tChecksum : ", configuration.checksum);

	LPrintln("\tString   : ", configuration.string);
	LPrintln("\tNumber   : ", configuration.number);

}








