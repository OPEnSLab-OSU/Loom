///////////////////////////////////////////////////////////////////////////////
///
/// @file		MPU6050.h
/// @brief		File for MPU6050 definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS
#pragma once

#include "I2C_Sensor.h"

///////////////////////////////////////////////////////////////////////////////
///
/// MPU6050 6 axis gyroscope and accelerometer  sensor module.
///
/// @attention	Need to modify line 7 of MPU6050_tockn.h to have I2C address of 0x69
///
/// @par Resources
/// - [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___m_p_u6050.html)
/// - [Product Page: Amazon –Gy-521 MPU-6050 MPU6050 Module 3 Axis Analog Gyro Sensors+ 3 Axis Accelerometer Module](https://www.amazon.com/MPU-6050-MPU6050-Accelerometer-Gyroscope-Converter/dp/B008BOPN40/)
/// - [Dependency: MPU6050_tockn](https://github.com/tockn/MPU6050_tockn)
/// - [Previous Dependency: MPU6050](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050)
/// - [Datasheet: MPU6050](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf)
///	- [Hardware Support](https://github.com/OPEnSLab-OSU/Loom/wiki/Hardware-Support#mpu6050-accelerometer--gyroscope)
///
///////////////////////////////////////////////////////////////////////////////
class L_MPU6050 : public I2CSensor
{
protected:

	float temp;				///< Temperature. Units: °C

	float acc[3];			///< Acceleration values. (x, y, z) Units: g.
	float gyro[3];			///< Gyro values. (x, y, z) Units: °/s.
	float accAngle[2];		///< X-axis acceleration angle (x, y)
	float gyroAngle[3];		///< Acceleration angles (x, y, z).
	float angle[3];			///< X-axis angle. (x, y, z)

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// @param[in]	i2c_address				Set(Int) | <0x69> | {0x68, 0x69} | I2C address
	/// @param[in]	mux_port				Int | <255> | [0-16] | Port on multiplexer
	/// @param[in]	calibrate				Bool | <true> | {true, false} | Whether or not to calibrate at start
	MPU6050(
			const byte		i2c_address	= 0x69,
			const uint8_t	mux_port	= 255,
			const bool		calibrate	= true
		);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// @param[in]	p		The array of constuctor args to expand
	MPU6050(JsonArrayConst p);
	
	/// Destructor
	virtual ~MPU6050() = default;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure() override;
	void		package(JsonObject json) override;
	void		calibrate() override;

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_state() const override;
	void		print_measurements() const override;

private:

};

#endif // ifdef LOOM_INCLUDE_SENSORS


