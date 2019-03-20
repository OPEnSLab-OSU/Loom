
#ifndef LOOM_SHT31D_h
#define LOOM_SHT31D_h

#include "Loom_I2C_Sensor.h"

#include "Adafruit_SHT31.h"



class Loom_SHT31D : public LoomI2CSensor
{

protected:

	/// Underlying SHT31D sensor manager instance
	Adafruit_SHT31 inst_sht31d;

	/// Measured temperature (C)
	float		temp;
	/// Measured humidity (%)
	float		humid;

public:

	// --- CONSTRUCTOR ---
	Loom_SHT31D(	
			byte		i2c_address				= SHT31D_I2C_Address, // 0x44, // 0x44, 0x45
			
			char*		module_name				= SHT31D_Module_Name, // "SHT31D",
			char*		sensor_description		= SHT31D_Sensor_Description // "Temperature/Humidty" 
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_SHT31D();

	// Inherited (Overriding) Methods
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");

private:

};


#endif
