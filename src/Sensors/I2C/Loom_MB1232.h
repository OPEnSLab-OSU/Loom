
#ifndef LOOM_MB1232_h
#define LOOM_MB1232_h


#include "Loom_I2C_Sensor.h"


#define RangeCommand    		0x51	//The Sensor ranging command has a value of 0x51
#define ChangeAddressCommand1 	0xAA	//These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand2 	0xA5


class Loom_MB1232 : public LoomI2CSensor
{

protected:

	/// Measure distance (cm)
	int range;

public:

	// --- CONSTRUCTOR ---
	Loom_MB1232(	
			byte		i2c_address				= MB1232_I2C_Address, // 0x70, // 0x70
			
			char*		module_name				= MB1232_Module_Name, // "MB1232",
			char*		sensor_description		= MB1232_Sensor_Description // "Sonar" 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_MB1232();

	// Inherited (Overriding) Methods
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");

private:

};


#endif
