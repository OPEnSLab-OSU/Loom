
#ifndef LOOM_MAX31856_h
#define LOOM_MAX31856_h

#include "Loom_SPI_Sensor.h"

#include <Adafruit_MAX31856.h>



class Loom_MAX31856 : public LoomSPISensor
{

protected:

	/// Underlying MAX31856 sensor manager instance
	Adafruit_MAX31856* inst_max;

	/// Raw value
	// float voltage;

	/// Measured temperature		
	float temperature;
	/// Measured cold junction temperature
	float cj_temp;

public:

	Loom_MAX31856(	
			char*		module_name			= MAX31856_Module_Name, // "MAX31856",
			char*		sensor_description	= MAX31856_Sensor_Description, // "Thermocouple", 
			uint8_t		num_samples			= MAX31856_Num_Damples, // 1,

			uint8_t		CS_pin				= MAX31856_CS_Pin, // 10,

			uint8_t		SPI_a				= MAX31856_SPI_A, // 10,
			uint8_t		SPI_b				= MAX31856_SPI_B, // 11,
			uint8_t		SPI_c				= MAX31856_SPI_C, // 12,
			uint8_t		SPI_d				= MAX31856_SPI_D // 13

			// type 
		);


	// --- DESTRUCTOR ---
	virtual ~Loom_MAX31856();

	// Inherited Methods
	void		print_config();
	void		print_measurements();
	void		calibrate() {}
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");
	bool		message_route(OSCMessage& msg, int address_offset) {}

private:


};



#endif // of #ifndef LOOM_MAX31856_h
