///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_I2C_Sensor.h
/// @brief		File for LoomI2CSensor definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "../Loom_Sensor.h"
#include <Wire.h>


///////////////////////////////////////////////////////////////////////////////


/// Abstract root for I2C sensor modules
class LoomI2CSensor : public LoomSensor
{

protected:
	
	/// The sensor's I2C address.
	/// If the sesnor supports mutliple, make sure this matches
	/// the current configuration of the i2c address 
	byte	i2c_address;	

	/// Used with multiplexer, keep track of port it is on
	uint8_t	port_num;		

public:
	
//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	/// \param[in]	module_name		Name of the module (provided by derived classes)
	/// \param[in]	module_type		Type of the module (provided by derived classes)
	/// \param[in]	i2c_address		The i2c address of the sensor 
	/// \param[in]	mux_portx		The port of the sensor if used with a multiplexer, 255 if not 
	/// \param[in]	num_samples		The number of samples to take and average
	LoomI2CSensor(
			const char*			module_name, 
			LoomModule::Type	module_type,
			byte				i2c_address,
			uint8_t				mux_port	= 255,	// Default to 255 (not on mux)
			uint8_t				num_samples	= 1		 
		);

	/// Destructor
	virtual ~LoomI2CSensor() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================


//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	virtual void	print_config() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================

	/// Get the sensor's I2C address
	/// \return	The sensor's I2C address
	byte			get_i2c_address() { return i2c_address; }; 	

private:

	/// Allow Multiplexer to access private methods of I2C sensor modules
	friend class Loom_Multiplexer;

};


