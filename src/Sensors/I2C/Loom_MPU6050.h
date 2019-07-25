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

	// // /// Variables to save accel and gyro readings
	// int16_t ax, ay, az, gx, gy, gz;

	float temp;					/// Temperature

	float accX, accY, accZ;		/// Acceleration values
	float gyroX, gyroY, gyroZ;	/// Gyro values

	float accAngleX, accAngleY;	/// Acceleration angle values

	float gyroAngleX, gyroAngleY, gyroAngleZ; /// Gyro angle values

	float angleX, angleY, angleZ;	/// Angle values

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x69> | {0x68, 0x69} | I2C address
	/// \param[in]	mux_port				Int | <-1> | [0-16] | Port on multiplexer
	/// \param[in]	module_name				String | <"SHT31D"> | null | SHT31D module name
	/// \param[in]	calibrate				Bool | <true> | {true, false} | Whether or not to calibrate at start
	Loom_MPU6050(
			byte			i2c_address		= 0x69,
			uint8_t			mux_port		= -1,
			const char*		module_name		= "MPU6050",
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




