///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_MPU6050.h
/// @brief		File for Loom_MPU6050 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Loom_I2C_Sensor.h"

#include <MPU6050_tockn.h>	
// Need to modify line 7 of MPU6050_tockn.h to have I2C address of 0x69


///////////////////////////////////////////////////////////////////////////////


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// MPU6050 6 axis gyroscope and accelerometer
// ###
class Loom_MPU6050 : public LoomI2CSensor
{

protected:

	float temp;				///< Temperature

	float accX;				///< X-axis acceleration value
	float accY;				///< Y-axis acceleration value
	float accZ;				///< Z-axis acceleration value

	float gyroX;			///< X-axis gyro value
	float gyroY;			///< Y-axis gyro value
	float gyroZ;			///< Z-axis gyro value

	float accAngleX;		///< X-axis acceleration angle
	float accAngleY;		///< Y-axis acceleration angle

	float gyroAngleX;		///< X-axis acceleration angle
	float gyroAngleY;		///< Y-axis acceleration angle
	float gyroAngleZ; 		///< Z-axis acceleration angle

	float angleX;			///< X-axis angle
	float angleY;			///< Y-axis angle
	float angleZ;			///< Z-axis angle

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x69> | {0x68, 0x69} | I2C address
	/// \param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	/// \param[in]	calibrate				Bool | <true> | {true, false} | Whether or not to calibrate at start
	Loom_MPU6050(
			byte			i2c_address		= 0x69,
			uint8_t			mux_port		= 255,
			bool			calibrate		= true
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	Loom_MPU6050(JsonArrayConst p);
	
	/// Destructor
	virtual ~Loom_MPU6050() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void 		package(JsonObject json) override;
	void		calibrate() override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_state() override;
	void		print_measurements() override;

//=============================================================================
///@name	GETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name	SETTERS
/*@{*/ //======================================================================


//=============================================================================
///@name	MISCELLANEOUS
/*@{*/ //======================================================================


private:

};




