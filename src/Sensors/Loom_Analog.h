
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



enum class AnalogConversion { 
	NONE, 			///< No conversion applied
	VOLTAGE, 		///< Analog to voltage
	THERMISTOR,		///< Thermistor 
	PH,				///< pH value
	TURBIDITY, 		///< Turbidity
	EC, 			///< Electrical Conductivity
	TDS				///< Total Dissolved Solids
}; 



// FlashStorage(analog_flash_config, AnalogConfig);




class Loom_Analog : public LoomSensor
{

protected:

	/// Which resolution to read at (generally use 12 or 10)
	uint8_t  read_resolution;

	/// Whether pins A0-A5 are enabled for analog reading
	bool     pin_enabled[ANALOG_COUNT];
	/// Last known analog readings for pin A0-A5
	uint16_t analog_vals[ANALOG_COUNT];
	/// Conversion (if any) to apply to analog value when printing / packaging
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

					AnalogConversion convertA0		= AnalogConversion::NONE,
					AnalogConversion convertA1		= AnalogConversion::NONE,
					AnalogConversion convertA2		= AnalogConversion::NONE,
					AnalogConversion convertA3		= AnalogConversion::NONE,
					AnalogConversion convertA4		= AnalogConversion::NONE,
					AnalogConversion convertA5		= AnalogConversion::NONE
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_Analog();

	// Inherited Methods
	void		print_config();
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");		 // This might be where analog conversions are applied
	bool		message_route(OSCMessage& msg, int address_offset);



	void		set_analog_resolution(uint8_t res);
	uint8_t		get_analog_resolution();

	int			get_analog_val(uint8_t pin);


	bool		get_pin_enabled(uint8_t pin);
	void		set_pin_enabled(uint8_t pin, bool e);

	AnalogConversion get_conversion(uint8_t pin);
	void  		set_conversion(uint8_t pin, AnalogConversion c);
	void  		set_enable_conversions(bool e);
	float 		convert(uint8_t pin, uint16_t analog);




	// Save a FlashStorage struct
	void save_config();
	// Load a FlashStorage struct, true if valid
	bool load_config();
	void print_config_struct();


private:

	uint16_t	read_analog(uint8_t chnl);

	float		convert_voltage(uint16_t analog);
	float		convert_thermistor(uint16_t analog);
	float		convert_pH(uint16_t analog);
	float		convert_turbidity(uint16_t analog);	
	float		convert_EC(uint16_t analog);
	float		convert_TDS(uint16_t analog);

};



#endif
