
#ifndef LOOM_SHT31D_h
#define LOOM_SHT31D_h


#include "Loom_I2C_Sensor.h"

#include "Adafruit_SHT31.h"



class Loom_SHT31D : public LoomI2CSensor
{

protected:
	// --- PROTECTED MEMBERS ---
	Adafruit_SHT31 inst_sht31d;

	float temp;
	float humid;

public:

	// --- PUBLIC MEMBERS ---


	// --- CONSTRUCTOR ---
	Loom_SHT31D(	byte i2c_address 			= 0x44,
					
					char* module_name 			= "SHT31D",
					char* sensor_description 	= "Temperature/Humidty" 

				);

	// maybe a constructor that specifies the i2c address (use a default otherwise)

	// --- DESTRUCTOR ---
	~Loom_SHT31D();

	void print_measurements();

	void measure();

	void package(OSCBundle* bndl);
	// void package(OSCBundle* bndl) { LoomModule::package(bndl); }

	void package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port);

private:

};


#endif