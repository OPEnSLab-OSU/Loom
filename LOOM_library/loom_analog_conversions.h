// ================================================================ 
// ===                          NOTES                           === 
// ================================================================
// This file contains functions that are used ot convert analog
// readings from the raw values before they are stored into the 
// analog state struct

// Conversions are selected in the config based on the index of the 
// desired conversion

// All conversion functions should take int and return int

// Defines and constants should be placed immediately above their
// associated function, rather than all at the top

// Functions should be named analog_convert_X



// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
typedef int (*convertRetFuncPtr)(int); 



// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
int analog_convert_triple(int analog);
int analog_convert_thermistor(int analog);



// ================================================================ 
// ===                     CONVERSION ARRAY                     === 
// ================================================================

static convertRetFuncPtr conversion_list[] = 
{
	analog_convert_triple,
	analog_convert_thermistor
};



// ================================================================ 
// ===                         Double                           === 
// ================================================================

// mostly just an example
int analog_convert_triple(int analog)
{
	return 3*analog;
}


// ================================================================ 
// ===                ANALOG CONVERT THERMISTOR                 === 
// ================================================================


#define THERMISTORNOMINAL 	10000   // resistance at 25 degrees C
#define TEMPERATURENOMINAL 	25   	// temp. for nominal resistance (almost always 25 C)
#define BCOEFFICIENT 		3950  	// The beta coefficient of the thermistor (usually 3000-4000)
//#define SERIESRESISTOR 10000    
 #define SERIESRESISTOR 	29330  	// the value of the 'other' resistor
 #define range_resol  		4095

int analog_convert_thermistor(int analog)
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










