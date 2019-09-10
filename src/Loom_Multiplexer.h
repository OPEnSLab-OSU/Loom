///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Multiplexer.h
/// @brief		File for Loom_Multiplexer definition, supporting enums, and 
///				I2C address selection for conflicts.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Loom_Module.h"
#include "Sensors/I2C/Loom_I2C_Sensor.h"


// // I2C Address Conflict Selection
enum class I2C_Selection {
	L_TSL2561,		///< TSL2561
	L_TSL2591,		///< TSL2591
	L_AS7262,		///< AS7262
	L_AS7263,		///< AS7263
	L_AS7265X,		///< AS7265X
	L_MMA8451,		///< MMA8451
	L_FXOS8700		///< FXOS8700
};


// move to configuration?
const I2C_Selection i2c_0x1C = I2C_Selection::L_MMA8451;	///< TSL2561 / TSL2591
const I2C_Selection i2c_0x1D = I2C_Selection::L_MMA8451;	///< TSL2561 / TSL2591
const I2C_Selection i2c_0x29 = I2C_Selection::L_TSL2591;	///< TSL2561 / TSL2591
const I2C_Selection i2c_0x49 = I2C_Selection::L_AS7265X;	///< TSL2561 / AS7262 / AS7263 / AS7265X


// Maybe query DeviceManager if RTC is in use, if not, query 0x68 


///////////////////////////////////////////////////////////////////////////////


// ### (LoomModule) | dependencies: [] | conflicts: []
/// Multiplexer manager for interfacing with
/// multiple I2C sensors
// ###
class Loom_Multiplexer : public LoomModule
{
private:

	/// List of known I2C addresses used by Loom.
	/// Used to avoid checking addresses that no sensors in Loom use
	const static byte known_addresses[];

protected:

	LoomI2CSensor**	sensors;			///< Array of I2C sensor objects

	byte			i2c_address;		///< The multiplexer's I2C address

	uint8_t			num_ports;			///< The number of ports on the multiplexer
	bool			dynamic_list;		///< Whether or not sensor list is dynamic (refresh sensor list periodically)
	uint			update_period;		///< Interval to update sensor list at

	unsigned long	last_update_time;	///< When the sensor list was last updated

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Multiplexer module constructor.
	///
	/// \param[in]	i2c_address				Set(Int) | <0x71> | {0x71} | I2C address
	/// \param[in]	num_ports				Int | <8> | [1-8] | Number of ports available
	/// \param[in]	dynamic_list			Bool | <true> | {true, false} | Whether or not to automatically check for new sensors
	/// \param[in]	update_period			Int | <5000> | [500-30000] | The time between sensor list updates (if dynamic_list enabled)
	Loom_Multiplexer(
			byte			i2c_address			= 0x71,
			uint8_t			num_ports			= 8,
			bool			dynamic_list		= true,
			uint			update_period		= 5000
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_Multiplexer(JsonArrayConst p);

	/// Destructor
	~Loom_Multiplexer();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure();
	void 		package(JsonObject json) override;
	bool		dispatch(JsonObject) override {}

	/// Populate a bundle with a list of sensors currently attached
	/// \param[out]	json	Json object to populate with sensor list
	void		get_sensor_list(JsonObject json);

	/// Update sensor list.
	/// Polls all ports of multiplexer getting sensor on port (if any)
	void		refresh_sensors();

	/// Get the sensor object for sensor on provided port
	/// \param[port]	port	The port of the multiplexer to get sensor object for
	/// \return			The pointer to LoomI2CSensor on port, Null if no sensor
	LoomI2CSensor*	get_sensor(uint8_t port) { return sensors[port]; }

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;
	void		print_state() override;

	/// Prints measurements of all connected sensors.
	/// Calls implementations of LoomI2CSensor::print_measurements()
	void		print_measurements();

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get whether or not sensors are updated dynamically
	/// \return	True if dynamic
	bool		get_is_dynamic() { return dynamic_list; }

	/// Get the sensor list update period.
	/// \return	The update period
	int			get_update_period() { return update_period; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set whether or not to periodically update list of attached sensors
	/// \param[in]	dynamic		The setting to set
	void		set_is_dynamic(bool dynamic) { dynamic_list = dynamic; }

	/// Set the sensor list update period.
	/// Requires dynamic_list to be enabled
	/// \param[in]	period	New update period
	void		set_update_period(uint period) { update_period = period; }

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

private:

	/// Select communication with sensor at index port
	/// \param[in]	port	The port to open I2C communication on
	void			tca_select(uint8_t port);

	/// Create appropriate instance to manage sensor.
	/// Compares I2C address to known sensors and generates corresponding sensor instance
	/// \param[in]	i2c_address		The I2C address to match to sensor class
	/// \return		Pointer to the generated I2C sensor object, Null if no match for that address
	LoomI2CSensor*	generate_sensor_object(byte i2c_address, uint8_t port);

	/// Determine the I2C address of the sensor (if any) on port.
	/// \param[in]	port	The port to get sensor address of
	/// \return		The I2C address of sensor, 0x00 if no sensor found
	byte			get_i2c_on_port(uint8_t port);

};





