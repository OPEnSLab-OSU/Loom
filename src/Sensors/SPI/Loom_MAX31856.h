
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
			char*		module_name			= "MAX31856",
			char*		sensor_description	= "Thermocouple", 
			uint8_t		num_samples			= 1,

			uint8_t		CS_pin				= 10,

			uint8_t		SPI_a				= 10,
			uint8_t		SPI_b				= 11,
			uint8_t		SPI_c				= 12,
			uint8_t		SPI_d				= 13

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
