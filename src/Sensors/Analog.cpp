///////////////////////////////////////////////////////////////////////////////
///
/// @file		Analog.cpp
/// @brief		File for Analog implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Analog.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
Analog::Analog(
		const uint8_t			num_samples,
		const uint8_t			read_resolution,

		const bool				enableA0,
		const bool				enableA1,
		const bool				enableA2,
		const bool				enableA3,
		const bool				enableA4,
		const bool				enableA5,

		const Conversion		convertA0,
		const Conversion		convertA1,
		const Conversion		convertA2,
		const Conversion		convertA3,
		const Conversion		convertA4,
		const Conversion		convertA5,

		const float				temperature
	)
	: Sensor("Analog", num_samples)
	, read_resolution(read_resolution)
	, enable_conversions(true)
	, analog_vals{0}
	, battery(0.)
	, temperature(temperature)
{
  LMark;
	// Set Analog Read Resolution
	analogReadResolution(read_resolution);
  LMark;

	// Set enabled pins
	pin_enabled[0] = enableA0;
  LMark;
	pin_enabled[1] = enableA1;
  LMark;
	pin_enabled[2] = enableA2;
  LMark;
	pin_enabled[3] = enableA3;
  LMark;
	pin_enabled[4] = enableA4;
  LMark;
	pin_enabled[5] = enableA5;


	for (auto i = 0; i < ANALOG_COUNT; i++) {
   LMark;
		if (pin_enabled[i]) {
    	LMark;
			pinMode(14+i, INPUT);
		}
	}

	// Set conversions
	conversions[0] = convertA0;
  LMark;
	conversions[1] = convertA1;
  LMark;
	conversions[2] = convertA2;
  LMark;
	conversions[3] = convertA3;
  LMark;
	conversions[4] = convertA4;
  LMark;
	conversions[5] = convertA5;
}

///////////////////////////////////////////////////////////////////////////////
Analog::Analog(JsonArrayConst p)
	: Analog(p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7],
		(Conversion)(int)p[8], (Conversion)(int)p[9], (Conversion)(int)p[10],
		(Conversion)(int)p[11], (Conversion)(int)p[12], (Conversion)(int)p[13],
		p[14]) {}

///////////////////////////////////////////////////////////////////////////////
void Analog::add_config(JsonObject json)
{
  LMark;
	JsonArray params = add_config_temp(json, module_name);
  LMark;
	params.add(module_name);
  LMark;
	params.add(num_samples);
  LMark;
	params.add(read_resolution);
  LMark;
	for (auto i = 0; i < 6; i++) {
   	LMark;
		params.add(pin_enabled[i]);
	}
	for (auto i = 0; i < 6; i++) {
   	LMark;
		params.add((int)conversions[i]);
	}
}

///////////////////////////////////////////////////////////////////////////////
float Analog::convert(const uint8_t pin, const uint16_t analog) const
{
  LMark;
	switch(conversions[pin]) {
		case Conversion::VOLTAGE 		: return convert_voltage(analog);
		case Conversion::THERMISTOR 	: return convert_thermistor(analog);
		case Conversion::PH 			: return convert_pH(analog);
		case Conversion::TURBIDITY 		: return convert_turbidity(analog);
		case Conversion::EC 			: return convert_EC(analog);
		case Conversion::TDS 			: return convert_TDS(analog);
		case Conversion::SALINITY 		: return convert_salinity(analog);
		default							: return (float)analog;
	}
}

///////////////////////////////////////////////////////////////////////////////
const char* Analog::conversion_name(const Conversion conversion)
{
  LMark;
	switch(conversion) {
		case Conversion::VOLTAGE 		: return "voltage";
		case Conversion::THERMISTOR 	: return "thermistor";
		case Conversion::PH 			: return "pH";
		case Conversion::TURBIDITY 		: return "turbidity";
		case Conversion::EC 			: return "EC";
		case Conversion::TDS 			: return "TDS";
		case Conversion::SALINITY 		: return "salinity";
		default							: return "analog";
	}
}

///////////////////////////////////////////////////////////////////////////////
void Analog::print_config() const
{
  LMark;
	Sensor::print_config();
  LMark;

	LPrintln("\tAnalog Resolution : ", read_resolution);
  LMark;
	LPrint("\tEnabled Pins        : ");
  LMark;
	for (auto i = 0; i < ANALOG_COUNT; i++) {
   LMark;
		if (pin_enabled[i]) {
    LMark;
			LPrint("A", i, ", ");
   LMark;
		}
	}
	LPrintln("\n\tTemperature        : ", temperature);
 LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Analog::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
  LMark;
	LPrintln("\tBattery = ", battery);
  LMark;
	for (auto i = 0; i < ANALOG_COUNT; i++) {
   	LMark;
		if ( (!enable_conversions) || (conversions[i] == Conversion::NONE) ) {
    	LMark;
			LPrintln("\tA", i, " = ", analog_vals[i]);
   		LMark;
		} else {
			LMark;
			LPrint("\tA", i, " = ", analog_vals[i]);
    	LMark;
			LPrintln( "  [ ", convert(i, analog_vals[i]), " (converted) ]" );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Analog::measure()
{
  LMark;
	battery = read_analog(VBATPIN) * 2 * 3.3 / (float)pow(2, read_resolution);
	// battery = read_analog(VBATPIN) * 2 * 3.3 ;/// (float)pow(2, read_resolution);

	for (auto i = 0; i < 6; i++) {
		if (pin_enabled[i]) {
    	LMark;
			analog_vals[i] = read_analog(i);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
void Analog::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;
	data["Vbat"] = battery;
  LMark;
	char buf[12];
  LMark;
	for (auto i = 0; i < ANALOG_COUNT; i++) {
   LMark;
		if (pin_enabled[i]) {
			if (!enable_conversions || conversions[i] == Conversion::NONE) {
     		LMark;
				sprintf(buf, "%s[%d]", "A", i);
			} else {
				sprintf(buf, "%s[%d]", conversion_name(get_conversion(i)), i);
			}
			LMark;
			data[buf] = (!enable_conversions || conversions[i] == Conversion::NONE)
						 ? analog_vals[i]
						 : convert(i, analog_vals[i]);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
int Analog::get_analog_val(const uint8_t pin) const
{
  LMark;
	return ( (pin >= 0) && (pin < ANALOG_COUNT) ) ? analog_vals[pin] : -1;
}

///////////////////////////////////////////////////////////////////////////////
void Analog::set_pin_enabled(const uint8_t pin, const bool e)
{
  LMark;
	pin_enabled[pin] = e;
  LMark;
	if (pin_enabled[pin]) {
   	LMark;
		pinMode(14+pin, INPUT);
	}
}

///////////////////////////////////////////////////////////////////////////////
uint16_t Analog::read_analog(const uint8_t chnl) const
{
	int i = num_samples;
	int reading = 0;

	while (i--) {
   	LMark;
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

///////////////////////////////////////////////////////////////////////////////
float Analog::convert_voltage(const uint16_t analog) const
{
  LMark;
	return analog*3.3/pow(2, read_resolution);
}

///////////////////////////////////////////////////////////////////////////////
#define reverse_connect 1
#define THERMISTORNOMINAL 	10000   // resistance at 25 degrees C
#define TEMPERATURENOMINAL 	25   	// temp. for nominal resistance (almost always 25 C)
#define BCOEFFICIENT 		3950  	// The beta coefficient of the thermistor (usually 3000-4000)
// #define SERIESRESISTOR 	10000
#define SERIESRESISTOR 		29330  	// the value of the 'other' resistor
#define range_resol  		4095
float Analog::convert_thermistor(const uint16_t analog) const
{
  LMark;
	float average = analog;
  LMark;

	#if reverse_connect == 0
   	LMark;
		average = range_resol / average - 1;
   	LMark;
		average = SERIESRESISTOR / average;
  	LMark;
	#endif
	#if reverse_connect == 1
   	LMark;
		average = range_resol / average - 1;
   	LMark;
		average = SERIESRESISTOR * average;
	#endif
	// Serial.print("Thermistor resistance ");
	// Serial.println(average);

	float steinhart;
  LMark;
	steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  LMark;
	steinhart = log(steinhart);                  // ln(R/Ro)
  LMark;
	steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  LMark;
	steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  LMark;
	steinhart = 1.0 / steinhart;                 // Invert
  LMark;
	steinhart -= 273.15;                         // convert to C
  LMark;

	return steinhart;
}

///////////////////////////////////////////////////////////////////////////////
#define PH_Offset 0.0

float Analog::convert_pH(const uint16_t analog) const
{
	// float voltage = convert_voltage(analog);
	// return pH_range*voltage + pH_offset;

	// return 1000.*convert_voltage(analog); // return millivolts

	float voltage = convert_voltage(analog);
  LMark;

	float pHValue = 3.5*voltage + PH_Offset;
  LMark;

	return pHValue;
}


// float DFRobot_PH::readPH(uint16_t voltage)
// {
// 	float slope = (7.0-4.0)/((this->_neutralVoltage-1500.0)/3.0 - (this->_acidVoltage-1500.0)/3.0);  // two point: (_neutralVoltage,7.0),(_acidVoltage,4.0)
// 	float intercept = 7.0 - slope*(this->_neutralVoltage-1500.0)/3.0;
// 	//Serial.print("slope:");
// 	//Serial.print(slope);
// 	//Serial.print(",intercept:");
// 	//Serial.println(intercept);
// 	return slope*(voltage-1500.0)/3.0 + intercept;  //y = mx + b
// }


///////////////////////////////////////////////////////////////////////////////
float Analog::convert_turbidity(const uint16_t analog) const
{
	// float voltage = convert_voltage(analog);
	// LPrintln("turbidity voltage: ", voltage);
	// return -1120.4 * (voltage * voltage) + (5742.3 * voltage) - 4352.9;


	return analog; // turbidity values are fairly qualitative, just returning the analog value for now
}

///////////////////////////////////////////////////////////////////////////////
// #define EC_TEMP 25

float Analog::convert_EC(const uint16_t analog) const
{
  LMark;
	// float temperature = 25.0;
	float voltage = convert_voltage(analog);
  LMark;

	//old block of code don't use for now
	//float compensation_coefficient = 1.0 + 0.02 * (EC_TEMP - 25.0); // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
	//float compensation_coefficient = 1.0 + 0.02 * (temperature - 25.0); // temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0));
	//float comp_volt = voltage / compensation_coefficient;  			// temperature compensation

	//newer block of code
	float EC = ( 133.42 * voltage * voltage * voltage - 255.86 * voltage * voltage + 857.39 * voltage ); //convert voltage value to EC value
  LMark;
	float compensation_coefficient = 1.0 + 0.02 * (temperature - 25.0);
  LMark;
	float compensationEC = EC/compensation_coefficient;
  LMark;

	return compensationEC;
}

///////////////////////////////////////////////////////////////////////////////
float Analog::convert_TDS(const uint16_t analog) const
{
	LMark;
	return convert_EC(analog)/2.;
}

///////////////////////////////////////////////////////////////////////////////
float Analog::convert_salinity(const uint16_t analog) const
{
	// Probably doesn't actually give a value of any worth right now...
	LMark;
	return (analog-76) / .0928;
}
