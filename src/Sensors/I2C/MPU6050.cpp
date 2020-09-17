///////////////////////////////////////////////////////////////////////////////
///
/// @file		MPU6050.cpp
/// @brief		File for MPU6050 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "MPU6050.h"
#include "Module_Factory.h"

// Unfortunately cannot be inside MPU6050 class
// For some reason Wire is not recognized like that
MPU6050 mpu6050(Wire);

///////////////////////////////////////////////////////////////////////////////

REGISTER(LoomModule, MPU6050, "MPU6050");

///////////////////////////////////////////////////////////////////////////////
MPU6050::MPU6050(
		const byte		i2c_address, 
		const uint8_t	mux_port,
		const bool		calibrate
	)
	: I2CSensor("MPU6050", Type::MPU6050, i2c_address, mux_port )
{
	Wire.begin();
	mpu6050.begin();

	if (calibrate) {
		print_module_label();
		mpu6050.calcGyroOffsets(true);
		LPrintln();
	}
}

///////////////////////////////////////////////////////////////////////////////
MPU6050::MPU6050(JsonArrayConst p)
	: MPU6050(EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void MPU6050::print_state() const
{
	I2CSensor::print_state();

	LPrintln("\tgyroXoffset : ", mpu6050.getGyroXoffset() );
	LPrintln("\tgyroYoffset : ", mpu6050.getGyroYoffset() );
	LPrintln("\tgyroZoffset : ", mpu6050.getGyroZoffset() );
}

///////////////////////////////////////////////////////////////////////////////
void MPU6050::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");

	LPrintln("\taccX   : ", accX);
	LPrintln("\taccY   : ", accY);
	LPrintln("\taccZ   : ", accZ);

	LPrintln("\tgyroX  : ", gyroX);
	LPrintln("\tgyroY  : ", gyroY);
	LPrintln("\tgyroZ  : ", gyroZ);

	LPrintln("\troll  : ", angleX);
	LPrintln("\tpitch : ", angleY);
	LPrintln("\tyaw   : ", angleZ);

	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln("\ttemp       : ", temp);

		LPrintln("\taccAngleX  : ", accAngleX);
		LPrintln("\taccAngleY  : ", accAngleY);

		LPrintln("\tgyroAngleX : ", gyroAngleX);
		LPrintln("\tgyroAngleY : ", gyroAngleY);
		LPrintln("\tgyroAngleZ : ", gyroAngleZ);
	}

}

///////////////////////////////////////////////////////////////////////////////
void MPU6050::measure()
{
	mpu6050.update();

	temp = mpu6050.getTemp();

	accX = mpu6050.getAccX();
	accY = mpu6050.getAccY();
	accZ = mpu6050.getAccZ();

	gyroX = mpu6050.getGyroX();
	gyroY = mpu6050.getGyroY();
	gyroZ = mpu6050.getGyroZ();

	accAngleX = mpu6050.getAccAngleX();
	accAngleY = mpu6050.getAccAngleY();

	gyroAngleX = mpu6050.getGyroAngleX();
	gyroAngleY = mpu6050.getGyroAngleY();
	gyroAngleZ = mpu6050.getGyroAngleZ();

	angleX = mpu6050.getAngleX();
	angleY = mpu6050.getAngleY();
	angleZ = mpu6050.getAngleZ();
}

///////////////////////////////////////////////////////////////////////////////
void MPU6050::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["ax"] = accX;
	data["ay"] = accY;
	data["az"] = accZ;

	data["gx"] = gyroX;
	data["gy"] = gyroY;
	data["gz"] = gyroZ;

	data["roll"] = angleX;
	data["pitch"] = angleY;
	data["yaw"] = angleZ;

	if (print_verbosity == Verbosity::V_HIGH) {
		data["temp"]		= 	temp;

		data["accAngleX"]	= 	accAngleX;
		data["accAngleY"]	= 	accAngleY;

		data["gyroAngleX"]	= 	gyroAngleX;
		data["gyroAngleY"]	= 	gyroAngleY;
		data["gyroAngleZ"]	=	gyroAngleZ;
	}

}

///////////////////////////////////////////////////////////////////////////////
void MPU6050::calibrate()
{
	print_module_label();
	mpu6050.calcGyroOffsets(true);
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS




