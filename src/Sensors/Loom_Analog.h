#pragma once

#include "Loom_Sensor.h"

#define VBATPIN A7			/// Battery pin
#define ANALOG_COUNT 6		/// Number of analog pins


///////////////////////////////////////////////////////////////////////////////


// ### (LoomSensor) | dependencies: [] | conflicts: []
/// Analog pin manager
// ###
class Loom_Analog : public LoomSensor
{
	
public:

	/// Different types of conversions from
	/// analog to specified unit
	enum class Conversion {
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

protected:

	uint8_t		read_resolution;				/// Which resolution to read at (generally use 12 or 10)
	bool		pin_enabled[ANALOG_COUNT];		/// Whether pins A0-A5 are enabled for analog reading
	uint16_t	analog_vals[ANALOG_COUNT];		/// Last known analog readings for pin A0-A5
	float		battery;						/// Battery voltage
	bool		enable_conversions;				/// Enable or disable all conversions

	Conversion	conversions[ANALOG_COUNT];		/// Conversion (if any) to apply to analog value when printing / packaging

	// the following will likely get a different implementation
	float		temperature;					/// Temperature to use in conversions
	float		pH_offset;						/// Offset to use in pH conversion
	float		pH_range;						/// Range to use in pH conversion

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Analog manager module constructor
	///
	/// \param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	/// \param[in]	read_resolution		Int | <12> | [8-12] | How many bits to read analog values at
	/// \param[in]	enableA0			Bool | <true> | {true, false} | Enable pin A0 for managing
	/// \param[in]	enableA1			Bool | <true> | {true, false} | Enable pin A1 for managing
	/// \param[in]	enableA2			Bool | <true> | {true, false} | Enable pin A2 for managing
	/// \param[in]	enableA3			Bool | <true> | {true, false} | Enable pin A3 for managing
	/// \param[in]	enableA4			Bool | <true> | {true, false} | Enable pin A4 for managing
	/// \param[in]	enableA5			Bool | <true> | {true, false} | Enable pin A5 for managing
	/// \param[in]	convertA0			Set(Conversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A0
	/// \param[in]	convertA1			Set(Conversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A1
	/// \param[in]	convertA2			Set(Conversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A2
	/// \param[in]	convertA3			Set(Conversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A3
	/// \param[in]	convertA4			Set(Conversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A4
	/// \param[in]	convertA5			Set(Conversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A5
	/// \param[in]	temperature			float | <25.0> | [0.-100.] | Temperature to using in conversions
	/// \param[in]	pH_offset			float | <0.0> | [0.-14.] | Offset in pH calculation (b value in mx+b)
	/// \param[in]	pH_range			float | <3.5> | [0.-10.] | Slope in pH calculation (x value in mx+b)
	Loom_Analog(
			uint8_t			num_samples			= 8,
			uint8_t			read_resolution		= 12,
			bool			enableA0			= true,
			bool			enableA1			= true,
			bool			enableA2			= true,
			bool			enableA3			= true,
			bool			enableA4			= true,
			bool			enableA5			= true,
			Conversion		convertA0			= Conversion::NONE,
			Conversion		convertA1			= Conversion::NONE,
			Conversion		convertA2			= Conversion::NONE,
			Conversion		convertA3			= Conversion::NONE,
			Conversion		convertA4			= Conversion::NONE,
			Conversion		convertA5			= Conversion::NONE
			// uint16_t		pH_neutralAnalog	= 3000,
			// uint16_t		pH_acidAnalog		= 3.5,
			// float			temperature			= 25.0,
			// float			pH_offset			= 0.0,
			// float			pH_range			= 3.5
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Analog(JsonArrayConst p);

	/// Destructor
	~Loom_Analog() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;
	void		add_config(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_measurements() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the current analog read resolution
	/// \return		Read resolution
	uint8_t		get_analog_resolution() { return read_resolution; }

	/// Get the analog value of specified pin
	/// \param[in]	pin		Pin to read
	/// \return		The analog value
	int			get_analog_val(uint8_t pin);

	/// Get the battery voltage of the device
	/// \return		The battery voltage
	float		get_battery() { return battery; };

	/// Get if a pin is enabled in manager
	/// \param[in]	pin		Pin to get enable state of
	/// \return		The enabled state of pin
	bool		get_pin_enabled(uint8_t pin) { return pin_enabled[pin]; }

	/// Get the current conversion associated with a pin
	/// \param[in]	pin		The pin to get conversion for
	/// \return		The current Conversion setting
	Conversion	get_conversion(uint8_t pin) { return conversions[pin]; }


//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set the analog read resolution
	/// \param[in]	res		Resolution to read at (12 bit max)
	void		set_analog_resolution(uint8_t res) { analogReadResolution(read_resolution = res); }

	/// Set the enable state of a pin
	/// \param[in]	pin		The pin to set enable state of
	/// \param[in]	e		Enable state
	void		set_pin_enabled(uint8_t pin, bool e);

	/// Set the current conversion associated with a pin
	/// \param[in]	pin		The pin to set conversion for
	/// \param[in]	c		The Conversion to use
	void  		set_conversion(uint8_t pin, Conversion c) { conversions[pin] = c; }

	/// Enable or disable all conversions
	/// \param[in]	e		Enable state
	void  		set_enable_conversions(bool e) { enable_conversions = e; }

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Get string of name associated with conversion enum
	/// \return String of conversion
	const char* conversion_name(Conversion conversion);

private:

	/// Read analog value of pin
	/// \param[in]	chnl	The channel/pin to read
	/// \return		The analog value
	uint16_t	read_analog(uint8_t chnl);

	/// Apply conversion (if any) to analog value based on associated conversion setting
	/// \param[in]	pin		Pin the analog value is associated with
	/// \param[in]	analog	Analog value to convert
	/// \return		The converted value
	float 		convert(uint8_t pin, uint16_t analog);

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


