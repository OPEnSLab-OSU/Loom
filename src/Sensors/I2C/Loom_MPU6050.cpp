
#include "Loom_MPU6050.h"

// Unfortunately cannot be inside Loom_MPU6050 class
// For some reason Wire is not recognized like that
MPU6050 mpu6050(Wire);


///////////////////////////////////////////////////////////////////////////////
Loom_MPU6050::Loom_MPU6050(
		byte			i2c_address, 
		uint8_t			mux_port,
		bool			calibrate
	) 
	: LoomI2CSensor( "MPU6050", Type::MPU6050, i2c_address, mux_port )
{
	Wire.begin();
	mpu6050.begin();

	if (calibrate) {
		print_module_label();
		mpu6050.calcGyroOffsets(true);
		LPrintln();
	}
    
    for (int i = 0; i < 15; i++) {
        Values.push_back(var());
    }
}

///////////////////////////////////////////////////////////////////////////////
Loom_MPU6050::Loom_MPU6050(JsonArrayConst p)
	: Loom_MPU6050( EXPAND_ARRAY(p, 3) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::print_state()
{
	LoomI2CSensor::print_state();

	LPrintln("\tgyroXoffset : ", mpu6050.getGyroXoffset() );
	LPrintln("\tgyroYoffset : ", mpu6050.getGyroYoffset() );
	LPrintln("\tgyroZoffset : ", mpu6050.getGyroZoffset() );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");

	LPrintln("\taccX   : ", Values[1].retrieve<float>().value_or(0));
	LPrintln("\taccY   : ", Values[2].retrieve<float>().value_or(0));
	LPrintln("\taccZ   : ", Values[3].retrieve<float>().value_or(0));

	LPrintln("\tgyroX  : ", Values[4].retrieve<float>().value_or(0));
	LPrintln("\tgyroY  : ", Values[5].retrieve<float>().value_or(0));
	LPrintln("\tgyroZ  : ", Values[6].retrieve<float>().value_or(0));

	LPrintln("\troll  : ", Values[12].retrieve<float>().value_or(0));
	LPrintln("\tpitch : ", Values[13].retrieve<float>().value_or(0));
	LPrintln("\tyaw   : ", Values[14].retrieve<float>().value_or(0));
	// LPrintln("\tangleX : ", angleX);
	// LPrintln("\tangleY : ", angleY);
	// LPrintln("\tangleZ : ", angleZ);

	if (print_verbosity == Verbosity::V_HIGH) {
		LPrintln("\ttemp       : ", Values[0].retrieve<float>().value_or(0));

		LPrintln("\taccAngleX  : ", Values[7].retrieve<float>().value_or(0));
		LPrintln("\taccAngleY  : ", Values[8].retrieve<float>().value_or(0));

		LPrintln("\tgyroAngleX : ", Values[4].retrieve<float>().value_or(0));
		LPrintln("\tgyroAngleY : ", Values[5].retrieve<float>().value_or(0));
		LPrintln("\tgyroAngleZ : ", Values[6].retrieve<float>().value_or(0));
	}

}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::measure()
{
	mpu6050.update();

     Values[0] = mpu6050.getTemp();

     Values[1] = mpu6050.getAccX();
     Values[2] = mpu6050.getAccY();
     Values[3] = mpu6050.getAccZ();

     Values[4] = mpu6050.getGyroX();
     Values[5] = mpu6050.getGyroY();
     Values[6] = mpu6050.getGyroZ();

     Values[7] = mpu6050.getAccAngleX();
     Values[8] = mpu6050.getAccAngleY();

     Values[9] = mpu6050.getGyroAngleX();
     Values[10] = mpu6050.getGyroAngleY();
     Values[11] = mpu6050.getGyroAngleZ();

     Values[12] = mpu6050.getAngleX();
     Values[13] = mpu6050.getAngleY();
     Values[14] = mpu6050.getAngleZ();
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MPU6050::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["ax"] = Values[1].retrieve<float>().value_or(0); //< Print accX
	data["ay"] = Values[2].retrieve<float>().value_or(0); //< Print accY
	data["az"] = Values[3].retrieve<float>().value_or(0); //< Print accZ

	data["gx"] = Values[4].retrieve<float>().value_or(0); //< Print gyroX
	data["gy"] = Values[5].retrieve<float>().value_or(0); //< Print gyroY
	data["gz"] = Values[6].retrieve<float>().value_or(0); //< Print gyroZ

	data["roll" ] = Values[12].retrieve<float>().value_or(0); //< Print angleX
	data["pitch"] = Values[13].retrieve<float>().value_or(0); //< Print angleY
    data["yaw"  ] = Values[14].retrieve<float>().value_or(0); //< Print angleZ
	// data["angx"] = angleX;
	// data["angy"] = angleY;
	// data["angz"] = angleZ;

	if (print_verbosity == Verbosity::V_HIGH) {
		data["temp"]		= 	Values[0].retrieve<float>().value_or(0); //< Print temp

		data["accAngleX"]	= 	Values[7].retrieve<float>().value_or(0); //< Print accAngleX
		data["accAngleY"]	= 	Values[8].retrieve<float>().value_or(0); //< Print accAngleY

		data["gyroAngleX"]	= 	Values[9 ].retrieve<float>().value_or(0); //< Print gyroAngleX
		data["gyroAngleY"]	= 	Values[10].retrieve<float>().value_or(0); //< Print gyroAngleY
		data["gyroAngleZ"]	=	Values[11].retrieve<float>().value_or(0); //< Print gyroAngleZ
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





