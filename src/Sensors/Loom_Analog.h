#pragma once

#include "Loom_Sensor.h"


/// Battery pin
#define VBATPIN A7
/// Number of analog pins
#define ANALOG_COUNT 6


typedef struct {
	byte checksum;
	// bool valid;
	char string[50];
	int  number;
} AnalogConfig;


/// Different types of conversions from
/// analog to specified unit
enum class AnalogConversion {
	NONE, 			///< No conversion applied
	VOLTAGE, 		///< Analog to voltage
	THERMISTOR,		///< Thermistor
	PH,				///< pH value
	TURBIDITY, 		///< Turbidity
	EC, 			///< Electrical Conductivity
	TDS,			///< Total Dissolved Solids
	SALINITY		///< Salinity
	// Soil moisture
};



// FlashStorage(analog_flash_config, AnalogConfig);



// ### (LoomSensor) | dependencies: [] | conflicts: []
/// Analog pin manager
// ###
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


	/// Analog manager module constructor
	///
	/// \param[in]	module_name			String | <"Analog"> | null | Analog manager module name
	/// \param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	/// \param[in]	read_resolution		Int | <12> | [8-12] | How many bits to read analog values at
	/// \param[in]	enableA0			Bool | <true> | {true, false} | Enable pin A0 for managing
	/// \param[in]	enableA1			Bool | <true> | {true, false} | Enable pin A1 for managing
	/// \param[in]	enableA2			Bool | <true> | {true, false} | Enable pin A2 for managing
	/// \param[in]	enableA3			Bool | <true> | {true, false} | Enable pin A3 for managing
	/// \param[in]	enableA4			Bool | <true> | {true, false} | Enable pin A4 for managing
	/// \param[in]	enableA5			Bool | <true> | {true, false} | Enable pin A5 for managing
	/// \param[in]	convertA0			Set(AnalogConversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A0
	/// \param[in]	convertA1			Set(AnalogConversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A1
	/// \param[in]	convertA2			Set(AnalogConversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A2
	/// \param[in]	convertA3			Set(AnalogConversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A3
	/// \param[in]	convertA4			Set(AnalogConversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A4
	/// \param[in]	convertA5			Set(AnalogConversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A5
	Loom_Analog(
			const char*			module_name			= "Analog",
			uint8_t				num_samples			= 8,
			uint8_t				read_resolution		= 12,
			bool				enableA0			= true,
			bool				enableA1			= true,
			bool				enableA2			= true,
			bool				enableA3			= true,
			bool				enableA4			= true,
			bool				enableA5			= true,
			AnalogConversion	convertA0			= AnalogConversion::NONE,
			AnalogConversion	convertA1			= AnalogConversion::NONE,
			AnalogConversion	convertA2			= AnalogConversion::NONE,
			AnalogConversion	convertA3			= AnalogConversion::NONE,
			AnalogConversion	convertA4			= AnalogConversion::NONE,
			AnalogConversion	convertA5			= AnalogConversion::NONE
		);

	Loom_Analog(JsonVariant p);

	/// Destructor
	virtual ~Loom_Analog();

	// Inherited Methods
	void		print_config() override;
	void		print_measurements() override;
	void		measure() override;
	void 		package(JsonObject json);


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


	const char* conversion_name(AnalogConversion conversion);


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

	/// Convert analog to salinity (total dissolved solids)
	/// \param[in]	analog	Analog value to convert
	/// \return Converted value
	float		convert_salinity(uint16_t analog);


};


