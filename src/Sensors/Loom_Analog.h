
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
	// Soil moisture
}; 



// FlashStorage(analog_flash_config, AnalogConfig);




class Loom_Analog : public LoomSensor
{

protected:

	/// Which resolution to read at (generally use 12 or 10)
	uint8_t		read_resolution;

	/// Whether pins A0-A5 are enabled for analog reading
	bool		pin_enabled[ANALOG_COUNT];
	/// Last known analog readings for pin A0-A5
	uint16_t	analog_vals[ANALOG_COUNT];
	/// Conversion (if any) to apply to analog value when printing / packaging
	AnalogConversion conversions[ANALOG_COUNT];

	/// Battery voltage
	float		battery;  

	/// Enable or disable all conversions
	bool		enable_conversions;

	AnalogConfig configuration;

public:
	// --- CONSTRUCTOR ---
																	// The default parameters will be controlled by config file
	Loom_Analog(	
			char*			module_name				= ANALOG_Module_Name, // "Analog", 
			char*			sensor_description		= ANALOG_Sensor_Description, // "Analog Values",
			uint8_t			num_samples				= ANALOG_Num_Samples, // 8, 

			uint8_t			read_resolution			= ANALOG_Read_Resolution, // 12,
			bool			enableA0				= ANALOG_EnableA0, // true,
			bool			enableA1				= ANALOG_EnableA1, // true,
			bool			enableA2				= ANALOG_EnableA2, // true,
			bool			enableA3				= ANALOG_EnableA3, // true,
			bool			enableA4				= ANALOG_EnableA4, // true,
			bool			enableA5				= ANALOG_EnableA5, // true,

			AnalogConversion convertA0				= ANALOG_ConvertA0, // AnalogConversion::NONE,
			AnalogConversion convertA1				= ANALOG_ConvertA1, // AnalogConversion::NONE,
			AnalogConversion convertA2				= ANALOG_ConvertA2, // AnalogConversion::NONE,
			AnalogConversion convertA3				= ANALOG_ConvertA3, // AnalogConversion::NONE,
			AnalogConversion convertA4				= ANALOG_ConvertA4, // AnalogConversion::NONE,
			AnalogConversion convertA5				= ANALOG_ConvertA5  // AnalogConversion::NONE
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_Analog();

	// Inherited Methods
	void		print_config();
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");		 // This might be where analog conversions are applied
	bool		message_route(OSCMessage& msg, int address_offset);


	/// Set the analog read resolution
	/// \param[in]	res		Resolution to read at (12 bit max)
	void		set_analog_resolution(uint8_t res);
	
	/// Get the current analog read resolution
	/// \return		Read resolution
	uint8_t		get_analog_resolution();

	/// Get the analog value of specified pin
	/// \param[in]	pin		Pin to read
	/// \return		The analog value
	int			get_analog_val(uint8_t pin);


	/// Get the battery voltage of the device
	/// \return		The battery voltage
	float		get_battery();

	/// Get if a pin is enabled in manager
	/// \param[in]	pin		Pin to get enable state of
	/// \return		The enabled state of pin		
	bool		get_pin_enabled(uint8_t pin);
	/// Set the enable state of a pin
	/// \param[in]	pin		The pin to set enable state of
	/// \param[in]	e		Enable state
	void		set_pin_enabled(uint8_t pin, bool e);

	/// Get the current conversion associated with a pin
	/// \param[in]	pin		The pin to get conversion for 
	/// \return		The current AnalogConversion setting
	AnalogConversion get_conversion(uint8_t pin);

	/// Set the current conversion associated with a pin
	/// \param[in]	pin		The pin to set conversion for 
	/// \param[in]	c		The AnalogConversion to use
	void  		set_conversion(uint8_t pin, AnalogConversion c);

	/// Enable or disable all conversions
	/// \param[in]	e		Enable state
	void  		set_enable_conversions(bool e);

	/// Apply conversion (if any) to analog value based on associated conversion setting
	/// \param[in]	pin		Pin the analog value is associated with
	/// \param[in]	analog	Analog value to convert
	/// \return		The converted value
	float 		convert(uint8_t pin, uint16_t analog);




	// Save a FlashStorage struct
	void save_config();
	// Load a FlashStorage struct, true if valid
	bool load_config();
	void print_config_struct();


private:

	/// Read analog value of pin
	/// \param[in]	chnl	The channel/pin to read
	/// \return		The analog value
	uint16_t	read_analog(uint8_t chnl);

	/// Convert analog to voltage
	/// \param[in]	analog	Analog value to convert
	/// \return Converted value
	float		convert_voltage(uint16_t analog);

	/// Convert analog to temperature of thermistor
	/// \param[in]	analog	Analog value to convert
	/// \return Converted value
	float		convert_thermistor(uint16_t analog);

	/// Convert analog to pH
	/// \param[in]	analog	Analog value to convert
	/// \return Converted value
	float		convert_pH(uint16_t analog);

	/// Convert analog to turbidity
	/// \param[in]	analog	Analog value to convert
	/// \return Converted value
	float		convert_turbidity(uint16_t analog);	

	/// Convert analog to electrical conductivity
	/// \param[in]	analog	Analog value to convert
	/// \return Converted value
	float		convert_EC(uint16_t analog);

	/// Convert analog to total dissolved solids
	/// \param[in]	analog	Analog value to convert
	/// \return Converted value
	float		convert_TDS(uint16_t analog);

};



#endif
