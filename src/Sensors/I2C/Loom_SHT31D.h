
#ifndef LOOM_SHT31D_h
#define LOOM_SHT31D_h

#include "Loom_I2C_Sensor.h"

#include "Adafruit_SHT31.h"


/// SHT31D Temperature / Humidty sensor module
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
			byte		i2c_address				=  0x44, // 0x44, 0x45  // SHT31D_I2C_Address,
			
			char*		module_name				=  "SHT31D",  // SHT31D_Module_Name,
			char*		sensor_description		= "Temperature/Humidty"   // SHT31D_Sensor_Description 
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
