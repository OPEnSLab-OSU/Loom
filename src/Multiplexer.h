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

#include "Module.h"
// #include "Sensors/I2C/I2C_Sensor.h"

#undef min
#undef max
#include <vector>
#include <array>

class LoomI2CSensor;

/// I2C Address Conflict Selection
enum class I2C_Selection {
	L_TSL2561,		///< TSL2561
	L_TSL2591,		///< TSL2591
	L_AS7262,		///< AS7262
	L_AS7263,		///< AS7263
	L_AS7265X,		///< AS7265X
	L_MMA8451,		///< MMA8451
	L_FXOS8700,		///< FXOS8700
    L_STEMMA
};


// move to configuration?
const I2C_Selection i2c_0x1C = I2C_Selection::L_MMA8451;	///< TSL2561 / TSL2591
const I2C_Selection i2c_0x1D = I2C_Selection::L_MMA8451;	///< TSL2561 / TSL2591
const I2C_Selection i2c_0x29 = I2C_Selection::L_TSL2591;	///< TSL2561 / TSL2591
const I2C_Selection i2c_0x49 = I2C_Selection::L_AS7265X;	///< TSL2561 / AS7262 / AS7263 / AS7265X
const I2C_Selection i2c_0x36 = I2C_Selection::L_STEMMA;

// Maybe query DeviceManager if RTC is in use, if not, query 0x68 


///////////////////////////////////////////////////////////////////////////////
///
/// Multiplexer manager for interfacing with multiple I2C sensors
///
/// @par Resources
/// - [Documentation](https://openslab-osu.github.io/Loom/html/class_loom___multiplexer.html)
///
///////////////////////////////////////////////////////////////////////////////
class Loom_Multiplexer : public LoomModule
{
private:

	/// List of known I2C addresses used by Loom.
	/// Used to avoid checking addresses that no sensors in Loom use
	const static std::array<byte, 21> known_addresses;

protected:

	LoomI2CSensor**	sensors;			///< Array of I2C sensor objects

	byte			i2c_address;		///< The multiplexer's I2C address

	uint8_t			num_ports;			///< The number of ports on the multiplexer
	bool			dynamic_list;		///< Whether or not sensor list is dynamic (refresh sensor list periodically)
	uint			update_period;		///< Interval to update sensor list at

	uint8_t control_port;			//< Mux port to be used as a control
	byte 		control_address;	//< Address at control_port to be ignored by refresh

	unsigned long	last_update_time;	///< When the sensor list was last updated
    std::vector<byte> i2c_conflicts; ///< List of I2C address conflicts
    
	const static std::array<byte, 9> alt_addresses;

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Multiplexer module constructor.
	///
	/// @param[in]	i2c_address				Set(Int) | <0x71> | {0x71} | I2C address
	/// @param[in]	num_ports				Int | <8> | [1-8] | Number of ports available
	/// @param[in]	dynamic_list			Bool | <true> | {true, false} | Whether or not to automatically check for new sensors
	/// @param[in]	update_period			Int | <5000> | [500-30000] | The time between sensor list updates (if dynamic_list enabled)
	Loom_Multiplexer(
			LoomManager* manager,
			const byte			i2c_address			= 0x71,
			const uint8_t		num_ports			= 8,
			const bool			dynamic_list		= true,
			const uint			update_period		= 5000
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	Loom_Multiplexer(LoomManager* manager, JsonArrayConst p);

	/// Destructor
	~Loom_Multiplexer();

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Call measure on all connected sensors
	void		measure();
	
	void		package(JsonObject json) override;
	bool		dispatch(JsonObject) override {}

	/// Populate a bundle with a list of sensors currently attached
	/// @param[out]	json	Json object to populate with sensor list
	void		get_sensor_list(JsonObject json);

	/// Update sensor list.
	/// Polls all ports of multiplexer getting sensor on port (if any)
	void		refresh_sensors();

	/// Get the sensor object for sensor on provided port
	/// @param[port]	port	The port of the multiplexer to get sensor object for
	/// @return			The pointer to LoomI2CSensor on port, Null if no sensor
	LoomI2CSensor*	get_sensor(uint8_t port) const { return sensors[port]; }

	void power_up() override;

	void power_down() override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() const override;
	void		print_state() const override;

	/// Prints measurements of all connected sensors.
	/// Calls implementations of LoomI2CSensor::print_measurements() const
	void		print_measurements() const;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get whether or not sensors are updated dynamically
	/// @return	True if dynamic
	bool		get_is_dynamic() const { return dynamic_list; }

	/// Get the sensor list update period.
	/// @return	The update period
	int			get_update_period() const { return update_period; }

//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================

	/// Set whether or not to periodically update list of attached sensors
	/// @param[in]	dynamic		The setting to set
	void		set_is_dynamic(const bool dynamic) { dynamic_list = dynamic; }

	/// Set the sensor list update period.
	/// Requires dynamic_list to be enabled
	/// @param[in]	period	New update period
	void		set_update_period(const uint period) { update_period = period; }

//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================

private:

	/// Select communication with sensor at index port
	/// @param[in]	port	The port to open I2C communication on
	void			tca_select(const uint8_t port) const;

	void			tca_deselect() const;

	/// Create appropriate instance to manage sensor.
	/// Compares I2C address to known sensors and generates corresponding sensor instance
	/// @param[in]	i2c_address		The I2C address to match to sensor class
	/// @return		Pointer to the generated I2C sensor object, Null if no match for that address
	LoomI2CSensor*	generate_sensor_object(const byte i2c_address, const uint8_t port);

	/// Determine the I2C address of the sensor (if any) on port.
	/// @param[in]	port	The port to get sensor address of
	/// @return		The I2C address of sensor, 0x00 if no sensor found
	byte			get_i2c_on_port(const uint8_t port) const;
    
    /// Checks for an I2C conflict with this address
    /// @param[in] address              The I2C address to check for conflicts
    /// @return     True if there is a conflict, false otherwise
    bool i2c_conflict(byte address) const;

    /// Find all I2C conflicts
    /// Loops through known addresses and checks if they are being used outside of the multiplexer
    /// @return     A vector of conflicting I2C addresses
    std::vector<byte> find_i2c_conflicts();
};





