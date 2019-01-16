
#include "loom_sensor_i2c_class.h"

#include "loom_macros.h"



// --- CONSTRUCTOR ---
I2CSensor::I2CSensor( 	char* module_name, 
			char* sensor_description, 
			byte  i2c_address 
		) : LoomSensor( module_name, sensor_description )
{
	// LOOM_DEBUG_Println("I2C Constructor 1");
	this->i2c_address = i2c_address;
}



// --- DESTRUCTOR ---
I2CSensor::~I2CSensor() 
{

}

// --- PUBLIC METHODS ---

void I2CSensor::print_config()
{
	LoomSensor::print_config();

	LOOM_DEBUG_Print3('\t', "I2C Address         : ", get_i2c_address_str() );


	// LOOM_DEBUG_Print3('\t', "I2C Address         : ", i2c_address);
	

	// char buf[20];
	// get_i2c_address_str(buf);
	// LOOM_DEBUG_Println(buf);


	// LOOM_DEBUG_Print(" (0x");				// Not sure if this print statement will work
	// #if LOOM_DEBUG == 1
	// 	Serial.print(i2c_address, HEX);
	// #endif
	// LOOM_DEBUG_Println(")");
}


	// Get the sensor's I2C address
byte I2CSensor::get_i2c_address() 
{ 
	return i2c_address; 
}


void I2CSensor::get_i2c_address_str(char* buf)
{
	sprintf(buf, "%d (0x%02X)", i2c_address, i2c_address);
}

char* I2CSensor::get_i2c_address_str()
{
	char buf[20];
	get_i2c_address_str(buf);
	return (char*)buf;
}


void I2CSensor::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
{
	char address_string[80];
	sprintf(address_string, "%s/%s/%d", id_prefix, module_name, port);
	bndl->add(address_string);
}




// these dont actually need to be re-added here
	// virtual void measure() = 0;
	// virtual void package() = 0;


