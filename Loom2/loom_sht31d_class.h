
#ifndef LOOM_SHT31D_h
#define LOOM_SHT31D_h

#include "loom_sensor_i2c_class.h"

// #include <Wire.h>
#include "Adafruit_SHT31.h"



class Loom_SHT31D : public I2CSensor
{

protected:
	// --- PROTECTED MEMBERS ---
	Adafruit_SHT31 inst_sht31d;

	float temp;
	float humid;

public:

	// --- PUBLIC MEMBERS ---


	// --- CONSTRUCTOR ---
	Loom_SHT31D(	char* module_name 			= "SHT31D",
	 
					char* sensor_description 	= "Temperature/Humidty", 
					byte i2c_address 			= 0x44 

				);

	// maybe a constructor that specifies the i2c address (use a default otherwise)

	// --- DESTRUCTOR ---
	~Loom_SHT31D();

	// --- PUBLIC METHODS ---

	// void print_config()
	// {
	// 	I2CSensor::print_config();
	// }

	void print_measurements();

	void measure();

	void package(OSCBundle* bndl, char* id_prefix);


private:

};


#endif