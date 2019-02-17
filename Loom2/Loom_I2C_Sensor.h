
#ifndef LOOM_I2C_SENSOR_h
#define LOOM_I2C_SENSOR_h


#include "Loom_Sensor.h"



class LoomI2CSensor : public LoomSensor
{

protected:

	byte i2c_address;	// The sensor's I2C address

public:

	// --- CONSTRUCTOR ---
	LoomI2CSensor( 	char* module_name, 
					char* sensor_description, 

					byte  i2c_address 

			);

	// --- DESTRUCTOR ---
	virtual ~LoomI2CSensor();

	// --- PUBLIC METHODS ---

	virtual void print_config();

	virtual void measure() = 0;
	virtual void package(OSCBundle& bndl, char* suffix="") = 0;

	// Get the sensor's I2C address
	byte get_i2c_address(); 	

private:

};


#endif // of #ifndef LOOM_I2C_SENSOR_h
