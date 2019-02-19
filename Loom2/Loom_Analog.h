
#ifndef LOOM_ANALOG_h
#define LOOM_ANALOG_h

#include "Loom_Sensor.h"



#define VBATPIN A7
#define ANALOG_COUNT 6

typedef struct {
	byte checksum;  
	// bool valid;  
	char string[50];
	int  number;
} AnalogConfig;



enum AnalogConversion { NO_CONVERT, CONVERT_VOLTAGE, CONVERT_THERMISTOR, CONVERT_PH, CONVERT_TURBIDITY, CONVERT_EC, CONVERT_TDS }; 



// FlashStorage(analog_flash_config, AnalogConfig);




class Loom_Analog : public LoomSensor
{

protected:
	uint8_t  read_resolution;

	// Pins A0-A5
	bool     pin_enabled[ANALOG_COUNT];
	uint16_t analog_vals[ANALOG_COUNT];
	AnalogConversion conversions[ANALOG_COUNT];

	float    battery;  

	bool	enable_conversions;

	AnalogConfig configuration;

public:
	// --- CONSTRUCTOR ---
																	// The default parameters will be controlled by config file
	Loom_Analog(	char*   module_name 			= "Analog", 
					char*   sensor_description 		= "Analog Values",
					uint8_t num_samples 			= 8, 

					uint8_t read_resolution 		= 12,
					bool    enableA0 				= true,
					bool    enableA1 				= true,
					bool    enableA2 				= true,
					bool    enableA3 				= true,
					bool    enableA4 				= true,
					bool    enableA5 				= true,

					AnalogConversion convertA0		= NO_CONVERT,
					AnalogConversion convertA1		= NO_CONVERT,
					AnalogConversion convertA2		= NO_CONVERT,
					AnalogConversion convertA3		= NO_CONVERT,
					AnalogConversion convertA4		= NO_CONVERT,
					AnalogConversion convertA5		= NO_CONVERT
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_Analog();

	void print_config();
	void print_measurements();
	void measure();
	void package(OSCBundle& bndl, char* suffix="");		 // This might be where analog conversions are applied
	bool message_route(OSCMessage& msg, int address_offset);

	void 	set_analog_resolution(uint8_t res);
	uint8_t get_analog_resolution();

	int get_analog_val(uint8_t i);


	AnalogConversion get_conversion(uint8_t pin);
	void set_conversion(uint8_t pin, AnalogConversion c);

	void set_enable_conversions(bool e);

	float convert(uint8_t pin, uint16_t analog);


	// Save a FlashStorage struct
	void save_config();
	// Load a FlashStorage struct, true if valid
	bool load_config();
	void print_config_struct();


private:

	uint16_t read_analog(uint8_t chnl);

	float convert_voltage(uint16_t analog);
	float convert_thermistor(uint16_t analog);
	float convert_pH(uint16_t analog);
	float convert_turbidity(uint16_t analog);	
	float convert_EC(uint16_t analog);
	float convert_TDS(uint16_t analog);

};



#endif
