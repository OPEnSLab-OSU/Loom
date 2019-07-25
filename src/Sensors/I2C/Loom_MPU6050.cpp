
#include "Loom_MPU6050.h"

// Unfortunately cannot be inside Loom_MPU6050 class
// For some reason Wire is not recognized like that
MPU6050 mpu6050(Wire);


///////////////////////////////////////////////////////////////////////////////
Loom_MPU6050::Loom_MPU6050(
		byte			i2c_address, 
		uint8_t			mux_port,
		const char*		module_name,
		bool			calibrate
	) 
	: LoomI2CSensor( module_name, i2c_address, mux_port )
{
	this->module_type = LoomModule::Type::MPU6050;

	Wire.begin();
	mpu6050.begin();

	if (calibrate) {
		print_module_label();
		mpu6050.calcGyroOffsets(true);
		LPrintln();
	}
}

///////////////////////////////////////////////////////////////////////////////
Loom_MPU6050::Loom_MPU6050(JsonArrayConst p)
	: Loom_MPU6050( EXPAND_ARRAY(p, 4) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::print_state()
{
	LoomI2CSensor::print_state();

	LPrintln("\t", "gyroXoffset : ", mpu6050.getGyroXoffset() );
	LPrintln("\t", "gyroYoffset : ", mpu6050.getGyroYoffset() );
	LPrintln("\t", "gyroZoffset : ", mpu6050.getGyroZoffset() );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");

	LPrintln("\t", "accX       : ", accX);
	LPrintln("\t", "accY       : ", accY);
	LPrintln("\t", "accZ       : ", accZ);

	LPrintln("\t", "gyroX      : ", gyroX);
	LPrintln("\t", "gyroY      : ", gyroY);
	LPrintln("\t", "gyroZ      : ", gyroZ);

	LPrintln("\t", "angleX     : ", angleX);
	LPrintln("\t", "angleY     : ", angleY);
	LPrintln("\t", "angleZ     : ", angleZ);

	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln("\t", "temp       : ", temp);

		LPrintln("\t", "accAngleX  : ", accAngleX);
		LPrintln("\t", "accAngleY  : ", accAngleY);

		LPrintln("\t", "gyroAngleX : ", gyroAngleX);
		LPrintln("\t", "gyroAngleY : ", gyroAngleY);
		LPrintln("\t", "gyroAngleZ : ", gyroAngleZ);
	}

}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::measure()
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
void Loom_MPU6050::package(JsonObject json)
{
	if (print_verbosity == Verbosity::V_LOW) {
		package_json(json, module_name, 
			"accX", 		accX,
			"accY", 		accY,
			"accZ", 		accZ,

			"gyroX", 		gyroX,
			"gyroY", 		gyroY,
			"gyroZ", 		gyroZ,

			"angleX", 		angleX,
			"angleY", 		angleY,
			"angleZ", 		angleZ
		);
	}

	if (print_verbosity == Verbosity::V_HIGH) {
		package_json(json, module_name, 
			"accX", 		accX,
			"accY", 		accY,
			"accZ", 		accZ,

			"gyroX", 		gyroX,
			"gyroY", 		gyroY,
			"gyroZ", 		gyroZ,

			"angleX", 		angleX,
			"angleY", 		angleY,
			"angleZ", 		angleZ,

			"temp", 		temp,

			"accAngleX", 	accAngleX,
			"accAngleY", 	accAngleY,

			"gyroAngleX", 	gyroAngleX,
			"gyroAngleY", 	gyroAngleY,
			"gyroAngleZ",	gyroAngleZ
		);
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::calibrate()
{
	print_module_label();
	mpu6050.calcGyroOffsets(true);
	LPrintln();
}

///////////////////////////////////////////////////////////////////////////////





