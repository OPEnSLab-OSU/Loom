///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Analog.h
/// @brief		File for Loom_Analog definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#pragma once

#include "Sensor.h"

#define VBATPIN A7			///< Battery pin
#define ANALOG_COUNT 6		///< Number of analog pins


///////////////////////////////////////////////////////////////////////////////
///
/// Analog pin manager module.
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___analog.html)
///
///////////////////////////////////////////////////////////////////////////////
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

	/// Which resolution to read at (generally use 12 or 10)
	uint8_t		read_resolution;	

	/// Whether pins A0-A5 are enabled for analog reading
	bool		pin_enabled[ANALOG_COUNT];

	/// Last known analog readings for pin A0-A5
	uint16_t	analog_vals[ANALOG_COUNT];

	/// Battery voltage
	float		battery;

	/// Enable or disable all conversions	
	bool		enable_conversions;

	/// Conversion (if any) to apply to analog value when printing / packaging
	Conversion	conversions[ANALOG_COUNT];

	/// Temperature to use in conversions
	float		temperature;					

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Analog manager module constructor
	///
	/// @param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	/// @param[in]	read_resolution		Int | <12> | [8-12] | How many bits to read analog values at
	/// @param[in]	enableA0			Bool | <true> | {true, false} | Enable pin A0 for managing
	/// @param[in]	enableA1			Bool | <true> | {true, false} | Enable pin A1 for managing
	/// @param[in]	enableA2			Bool | <true> | {true, false} | Enable pin A2 for managing
	/// @param[in]	enableA3			Bool | <true> | {true, false} | Enable pin A3 for managing
	/// @param[in]	enableA4			Bool | <true> | {true, false} | Enable pin A4 for managing
	/// @param[in]	enableA5			Bool | <true> | {true, false} | Enable pin A5 for managing
	/// @param[in]	convertA0			Set(Conversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A0
	/// @param[in]	convertA1			Set(Conversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A1
	/// @param[in]	convertA2			Set(Conversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A2
	/// @param[in]	convertA3			Set(Conversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A3
	/// @param[in]	convertA4			Set(Conversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A4
	/// @param[in]	convertA5			Set(Conversion) | <0> | {0("No conversion"), 1("Analog to voltage"), 2("Thermistor" ), 3("pH"), 4("Turbidity"), 5("Electrical Conductivity"), 6("Total Dissolved Solids"), 7("Salinity")} | Conversion to apply to analog readings on pin A5
	/// @param[in]	temperature			float | <25.0> | [0.-100.] | Temperature to using in conversions
	Loom_Analog(
			LoomManager* manager,
			const uint8_t		num_samples			= 8,
			const uint8_t		read_resolution		= 12,
			const bool			enableA0			= true,
			const bool			enableA1			= true,
			const bool			enableA2			= true,
			const bool			enableA3			= true,
			const bool			enableA4			= true,
			const bool			enableA5			= true,
			const Conversion	convertA0			= Conversion::NONE,
			const Conversion	convertA1			= Conversion::NONE,
			const Conversion	convertA2			= Conversion::NONE,
			const Conversion	convertA3			= Conversion::NONE,
			const Conversion	convertA4			= Conversion::NONE,
			const Conversion	convertA5			= Conversion::NONE,
			const float			temperature			= 25.0
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_Analog(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_Analog() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;
	void		add_config(JsonObject json) override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_measurements() const override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the current analog read resolution
	/// @return		Read resolution
	uint8_t		get_analog_resolution() const { return read_resolution; }

	/// Get the analog value of specified pin
	/// @param[in]	pin		Pin to read
	/// @return		The analog value
	int			get_analog_val(const uint8_t pin) const;

	/// Get the battery voltage of the device
	/// @return		The battery voltage
	float		get_battery() const { return battery; }

	/// Get if a pin is enabled in manager
	/// @param[in]	pin		Pin to get enable state of
	/// @return		The enabled state of pin
	bool		get_pin_enabled(const uint8_t pin) const { return pin_enabled[pin]; }

	/// Get the current conversion associated with a pin
	/// @param[in]	pin		The pin to get conversion for
	/// @return		The current Conversion setting
	Conversion	get_conversion(const uint8_t pin) const { return conversions[pin]; }


//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set the analog read resolution
	/// @param[in]	res		Resolution to read at (12 bit max)
	void		set_analog_resolution(const uint8_t res) { analogReadResolution(read_resolution = res); }

	/// Set the enable state of a pin
	/// @param[in]	pin		The pin to set enable state of
	/// @param[in]	e		Enable state
	void		set_pin_enabled(const uint8_t pin, const bool e);

	/// Set the current conversion associated with a pin
	/// @param[in]	pin		The pin to set conversion for
	/// @param[in]	c		The Conversion to use
	void  		set_conversion(const uint8_t pin, const Conversion c) { conversions[pin] = c; }

	/// Enable or disable all conversions
	/// @param[in]	e		Enable state
	void  		set_enable_conversions(const bool e) { enable_conversions = e; }

	/// Set temperature to use in conversions that require temperature compensation
	/// @param[in]	temp	Temperature to use
	void		set_temperature(const float temp) { temperature = ((temp > 40.0) && (temp < 85.)) ? temp : 25.0; }

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

	/// Get string of name associated with conversion enum
	/// @return String of conversion
	static const char*	conversion_name(const Conversion conversion);

private:

	/// Read analog value of pin
	/// @param[in]	chnl	The channel/pin to read
	/// @return		The analog value
	uint16_t	read_analog(const uint8_t chnl) const;

	/// Apply conversion (if any) to analog value based on associated conversion setting
	/// @param[in]	pin		Pin the analog value is associated with
	/// @param[in]	analog	Analog value to convert
	/// @return		The converted value
	float 		convert(const uint8_t pin, const uint16_t analog) const;

	/// Convert analog to voltage
	/// @param[in]	analog	Analog value to convert
	/// @return Converted value
	float		convert_voltage(const uint16_t analog) const;

	/// Convert analog to temperature of thermistor
	/// @param[in]	analog	Analog value to convert
	/// @return Converted value
	float		convert_thermistor(const uint16_t analog) const;

	/// Convert analog to pH
	/// @param[in]	analog	Analog value to convert
	/// @return Converted value
	float		convert_pH(const uint16_t analog) const;

	/// Convert analog to turbidity
	/// @param[in]	analog	Analog value to convert
	/// @return Converted value
	float		convert_turbidity(const uint16_t analog) const;

	/// Convert analog to electrical conductivity
	/// @param[in]	analog	Analog value to convert
	/// @return Converted value
	float		convert_EC(const uint16_t analog) const;

	/// Convert analog to total dissolved solids
	/// @param[in]	analog	Analog value to convert
	/// @return Converted value
	float		convert_TDS(const uint16_t analog) const;

	/// Convert analog to salinity (total dissolved solids)
	/// @param[in]	analog	Analog value to convert
	/// @return Converted value
	float		convert_salinity(const uint16_t analog) const;

};


