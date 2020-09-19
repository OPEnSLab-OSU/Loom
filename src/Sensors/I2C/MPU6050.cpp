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

#include <MPU6050_tockn.h>

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

::MPU6050 mpu_inst(Wire);

///////////////////////////////////////////////////////////////////////////////
Loom::MPU6050::MPU6050(
		const byte		i2c_address, 
		const uint8_t	mux_port,
		const bool		calibrate
	)
	: I2CSensor("MPU6050", i2c_address, mux_port )
{
	Wire.begin();
	mpu_inst.begin();

	if (calibrate) {
		print_module_label();
		mpu_inst.calcGyroOffsets(true);
		LPrintln();
	}
}

///////////////////////////////////////////////////////////////////////////////
Loom::MPU6050::MPU6050(JsonArrayConst p)
	: MPU6050(EXPAND_ARRAY(p, 3)) {}

///////////////////////////////////////////////////////////////////////////////
void Loom::MPU6050::print_state() const
{
	I2CSensor::print_state();

	LPrintln("\tgyroXoffset : ", mpu_inst.getGyroXoffset() );
	LPrintln("\tgyroYoffset : ", mpu_inst.getGyroYoffset() );
	LPrintln("\tgyroZoffset : ", mpu_inst.getGyroZoffset() );
}

///////////////////////////////////////////////////////////////////////////////
void Loom::MPU6050::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");

	LPrintln("\taccX   : ", acc[0]);
	LPrintln("\taccY   : ", acc[1]);
	LPrintln("\taccZ   : ", acc[2]);

	LPrintln("\tgyroX  : ", gyro[0]);
	LPrintln("\tgyroY  : ", gyro[1]);
	LPrintln("\tgyroZ  : ", gyro[2]);

	LPrintln("\troll  : ", angle[0]);
	LPrintln("\tpitch : ", angle[1]);
	LPrintln("\tyaw   : ", angle[2]);

	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln("\ttemp       : ", temp);

		LPrintln("\taccAngleX  : ", accAngle[0]);
		LPrintln("\taccAngleY  : ", accAngle[1]);

		LPrintln("\tgyroAngleX : ", gyroAngle[0]);
		LPrintln("\tgyroAngleY : ", gyroAngle[1]);
		LPrintln("\tgyroAngleZ : ", gyroAngle[2]);
	}

}

///////////////////////////////////////////////////////////////////////////////
void Loom::MPU6050::measure()
{
	mpu_inst.update();

	temp = mpu_inst.getTemp();

	acc[0] = mpu_inst.getAccX();
	acc[1] = mpu_inst.getAccY();
	acc[2] = mpu_inst.getAccZ();

	gyro[0] = mpu_inst.getGyroX();
	gyro[1] = mpu_inst.getGyroY();
	gyro[2] = mpu_inst.getGyroZ();

	accAngle[0] = mpu_inst.getAccAngleX();
	accAngle[1] = mpu_inst.getAccAngleY();

	gyroAngle[0] = mpu_inst.getGyroAngleX();
	gyroAngle[1] = mpu_inst.getGyroAngleY();
	gyroAngle[2] = mpu_inst.getGyroAngleZ();

	angle[0] = mpu_inst.getAngleX();
	angle[1] = mpu_inst.getAngleY();
	angle[2] = mpu_inst.getAngleZ();
}

///////////////////////////////////////////////////////////////////////////////
void Loom::MPU6050::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["ax"] = acc[0];
	data["ay"] = acc[1];
	data["az"] = acc[2];

	data["gx"] = gyro[0];
	data["gy"] = gyro[1];
	data["gz"] = gyro[2];

	data["roll"]  = angle[0];
	data["pitch"] = angle[1];
	data["yaw"]   = angle[2];

	if (print_verbosity == Verbosity::V_HIGH) {
		data["temp"]		= 	temp;

		data["accAngleX"]	= 	accAngle[0];
		data["accAngleY"]	= 	accAngle[1];

		data["gyroAngleX"]	= 	gyroAngle[0];
		data["gyroAngleY"]	= 	gyroAngle[1];
		data["gyroAngleZ"]	=	gyroAngle[2];
	}

}

///////////////////////////////////////////////////////////////////////////////
void Loom::MPU6050::calibrate()
{
	print_module_label();
	mpu_inst.calcGyroOffsets(true);
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS




