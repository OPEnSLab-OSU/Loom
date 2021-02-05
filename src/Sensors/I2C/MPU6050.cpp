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
  LMark;
	Wire.begin();
  LMark;
	mpu_inst.begin();

	if (calibrate) {
		print_module_label();
   	LMark;
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
  LMark;
	LPrintln("\tgyroYoffset : ", mpu_inst.getGyroYoffset() );
  LMark;
	LPrintln("\tgyroZoffset : ", mpu_inst.getGyroZoffset() );
}

///////////////////////////////////////////////////////////////////////////////
void Loom::MPU6050::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
  LMark;

	LPrintln("\taccX   : ", acc[0]);
  LMark;
	LPrintln("\taccY   : ", acc[1]);
  LMark;
	LPrintln("\taccZ   : ", acc[2]);
  LMark;

	LPrintln("\tgyroX  : ", gyro[0]);
  LMark;
	LPrintln("\tgyroY  : ", gyro[1]);
  LMark;
	LPrintln("\tgyroZ  : ", gyro[2]);
  LMark;

	LPrintln("\troll  : ", angle[0]);
  LMark;
	LPrintln("\tpitch : ", angle[1]);
  LMark;
	LPrintln("\tyaw   : ", angle[2]);
  LMark;

	if (print_verbosity == Verbosity::V_HIGH) {
   	LMark;
		LPrintln("\ttemp       : ", temp);
  	LMark;

		LPrintln("\taccAngleX  : ", accAngle[0]);
   	LMark;
		LPrintln("\taccAngleY  : ", accAngle[1]);
  	LMark;

		LPrintln("\tgyroAngleX : ", gyroAngle[0]);
   	LMark;
		LPrintln("\tgyroAngleY : ", gyroAngle[1]);
   	LMark;
		LPrintln("\tgyroAngleZ : ", gyroAngle[2]);
  	LMark;
	}

}

///////////////////////////////////////////////////////////////////////////////
void Loom::MPU6050::measure()
{
  LMark;
	mpu_inst.update();
  LMark;

	temp = mpu_inst.getTemp();
  LMark;

	acc[0] = mpu_inst.getAccX();
  LMark;
	acc[1] = mpu_inst.getAccY();
  LMark;
	acc[2] = mpu_inst.getAccZ();
  LMark;

	gyro[0] = mpu_inst.getGyroX();
  LMark;
	gyro[1] = mpu_inst.getGyroY();
  LMark;
	gyro[2] = mpu_inst.getGyroZ();
  LMark;

	accAngle[0] = mpu_inst.getAccAngleX();
  LMark;
	accAngle[1] = mpu_inst.getAccAngleY();
  LMark;

	gyroAngle[0] = mpu_inst.getGyroAngleX();
  LMark;
	gyroAngle[1] = mpu_inst.getGyroAngleY();
  LMark;
	gyroAngle[2] = mpu_inst.getGyroAngleZ();
  LMark;

	angle[0] = mpu_inst.getAngleX();
  LMark;
	angle[1] = mpu_inst.getAngleY();
  LMark;
	angle[2] = mpu_inst.getAngleZ();
}

///////////////////////////////////////////////////////////////////////////////
void Loom::MPU6050::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;

	data["ax"] = acc[0];
  LMark;
	data["ay"] = acc[1];
  LMark;
	data["az"] = acc[2];
  LMark;

	data["gx"] = gyro[0];
  LMark;
	data["gy"] = gyro[1];
  LMark;
	data["gz"] = gyro[2];
  LMark;

	data["roll"]  = angle[0];
  LMark;
	data["pitch"] = angle[1];
  LMark;
	data["yaw"]   = angle[2];
  LMark;

	if (print_verbosity == Verbosity::V_HIGH) {
   	LMark;
		data["temp"]		= 	temp;
  	LMark;

		data["accAngleX"]	= 	accAngle[0];
   	LMark;
		data["accAngleY"]	= 	accAngle[1];
  	LMark;

		data["gyroAngleX"]	= 	gyroAngle[0];
   	LMark;
		data["gyroAngleY"]	= 	gyroAngle[1];
   	LMark;
		data["gyroAngleZ"]	=	gyroAngle[2];
	}

}

///////////////////////////////////////////////////////////////////////////////
void Loom::MPU6050::calibrate()
{
	print_module_label();
  LMark;
	mpu_inst.calcGyroOffsets(true);
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
