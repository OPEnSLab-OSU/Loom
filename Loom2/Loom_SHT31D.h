
#ifndef LOOM_SHT31D_h
#define LOOM_SHT31D_h

#include "Loom_I2C_Sensor.h"

#include "Adafruit_SHT31.h"



class Loom_SHT31D : public LoomI2CSensor
{

protected:

	Adafruit_SHT31 inst_sht31d;

	float temp;
	float humid;

public:

	// --- CONSTRUCTOR ---
	Loom_SHT31D(	byte i2c_address 			= 0x44, // 0x44, 0x45
					
					char* module_name 			= "SHT31D",
					char* sensor_description 	= "Temperature/Humidty" 
				);

	// --- DESTRUCTOR ---
	virtual ~Loom_SHT31D();

	void print_measurements();

	void measure();
	void package(OSCBundle& bndl, char* suffix="");

private:

};


#endif
