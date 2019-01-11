
#ifndef LOOM_SENSOR_I2C_h
#define LOOM_SENSOR_I2C_h


#include "loom_sensor_class.h"



class I2CSensor : public LoomSensor
{

protected:
	// --- PROTECTED MEMBERS ---
	byte i2c_address;	// The sensor's I2C address

	// should is setup be a member here?

public:

	// --- PUBLIC MEMBERS ---


	// --- CONSTRUCTOR ---
	I2CSensor( 	char* module_name, 
				char* sensor_description, 

				byte  i2c_address 

			);

	// maybe a constructor that specifies the i2c address (use a default otherwise)

	// --- DESTRUCTOR ---
	~I2CSensor();

	// --- PUBLIC METHODS ---

	virtual void print_config();


	// Get the sensor's I2C address
	byte get_i2c_address(); 	

// these dont actually need to be re-added here
	// virtual void measure() = 0;
	// virtual void package() = 0;


private:

};


#endif