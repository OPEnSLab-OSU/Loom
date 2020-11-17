///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_MPU6050.cpp
/// @brief		File for Loom_MPU6050 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "MPU6050.h"

// Unfortunately cannot be inside Loom_MPU6050 class
// For some reason Wire is not recognized like that
MPU6050 mpu6050(Wire);


///////////////////////////////////////////////////////////////////////////////
Loom_MPU6050::Loom_MPU6050(
LoomManager* manager,
const byte i2c_address,
		const uint8_t		mux_port,
		const bool			calibrate
	)
	: LoomI2CSensor(manager, "MPU6050", Type::MPU6050, i2c_address, mux_port )
{
  LMark;
	Wire.begin();
  LMark;
	mpu6050.begin();
  LMark;

	if (calibrate) {
   	LMark;
		print_module_label();
   	LMark;
		mpu6050.calcGyroOffsets(true);
   	LMark;
		LPrintln();
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
Loom_MPU6050::Loom_MPU6050(LoomManager* manager, JsonArrayConst p)
	: Loom_MPU6050(manager, EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::print_state() const
{
  LMark;
	LoomI2CSensor::print_state();
  LMark;

	LPrintln("\tgyroXoffset : ", mpu6050.getGyroXoffset() );
  LMark;
	LPrintln("\tgyroYoffset : ", mpu6050.getGyroYoffset() );
  LMark;
	LPrintln("\tgyroZoffset : ", mpu6050.getGyroZoffset() );
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Measurements:");
  LMark;

	LPrintln("\taccX   : ", accX);
  LMark;
	LPrintln("\taccY   : ", accY);
  LMark;
	LPrintln("\taccZ   : ", accZ);
  LMark;

	LPrintln("\tgyroX  : ", gyroX);
  LMark;
	LPrintln("\tgyroY  : ", gyroY);
  LMark;
	LPrintln("\tgyroZ  : ", gyroZ);
  LMark;

	LPrintln("\troll  : ", angleX);
  LMark;
	LPrintln("\tpitch : ", angleY);
  LMark;
	LPrintln("\tyaw   : ", angleZ);
  LMark;

	if (print_verbosity == Verbosity::V_HIGH) {
   	LMark;
		LPrintln("\ttemp       : ", temp);
  	LMark;

		LPrintln("\taccAngleX  : ", accAngleX);
   	LMark;
		LPrintln("\taccAngleY  : ", accAngleY);
  	LMark;

		LPrintln("\tgyroAngleX : ", gyroAngleX);
   	LMark;
		LPrintln("\tgyroAngleY : ", gyroAngleY);
   	LMark;
		LPrintln("\tgyroAngleZ : ", gyroAngleZ);
  	LMark;
	}

}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::measure()
{
  LMark;
	mpu6050.update();
  LMark;

	temp = mpu6050.getTemp();
  LMark;

	accX = mpu6050.getAccX();
  LMark;
	accY = mpu6050.getAccY();
  LMark;
	accZ = mpu6050.getAccZ();
  LMark;

	gyroX = mpu6050.getGyroX();
  LMark;
	gyroY = mpu6050.getGyroY();
  LMark;
	gyroZ = mpu6050.getGyroZ();
  LMark;

	accAngleX = mpu6050.getAccAngleX();
  LMark;
	accAngleY = mpu6050.getAccAngleY();
  LMark;

	gyroAngleX = mpu6050.getGyroAngleX();
  LMark;
	gyroAngleY = mpu6050.getGyroAngleY();
  LMark;
	gyroAngleZ = mpu6050.getGyroAngleZ();
  LMark;

	angleX = mpu6050.getAngleX();
  LMark;
	angleY = mpu6050.getAngleY();
  LMark;
	angleZ = mpu6050.getAngleZ();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;

	data["ax"] = accX;
  LMark;
	data["ay"] = accY;
  LMark;
	data["az"] = accZ;
  LMark;

	data["gx"] = gyroX;
  LMark;
	data["gy"] = gyroY;
  LMark;
	data["gz"] = gyroZ;
  LMark;

	data["roll"] = angleX;
  LMark;
	data["pitch"] = angleY;
  LMark;
	data["yaw"] = angleZ;
  LMark;

	if (print_verbosity == Verbosity::V_HIGH) {
   	LMark;
		data["temp"]		= 	temp;
  	LMark;

		data["accAngleX"]	= 	accAngleX;
   	LMark;
		data["accAngleY"]	= 	accAngleY;
  	LMark;

		data["gyroAngleX"]	= 	gyroAngleX;
   	LMark;
		data["gyroAngleY"]	= 	gyroAngleY;
   	LMark;
		data["gyroAngleZ"]	=	gyroAngleZ;
  	LMark;
	}

}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::calibrate()
{
  LMark;
	print_module_label();
  LMark;
	mpu6050.calcGyroOffsets(true);
  LMark;
	LPrintln();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
