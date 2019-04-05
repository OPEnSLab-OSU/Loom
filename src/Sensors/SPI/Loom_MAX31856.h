
#ifndef LOOM_MAX31856_h
#define LOOM_MAX31856_h

#include "Loom_SPI_Sensor.h"

#include <Adafruit_MAX31856.h>


/// MAX31856 thermocouple module
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
			char*		module_name			=  "MAX31856",  // MAX31856_Module_Name,
			char*		sensor_description	=  "Thermocouple",   // MAX31856_Sensor_Description,
			uint8_t		num_samples			=  1,  // MAX31856_Num_Damples,

			uint8_t		CS_pin				=  10,  // MAX31856_CS_Pin,

			uint8_t		SPI_a				=  10,  // MAX31856_SPI_A,
			uint8_t		SPI_b				=  11,  // MAX31856_SPI_B,
			uint8_t		SPI_c				=  12,  // MAX31856_SPI_C,
			uint8_t		SPI_d				=  13  // MAX31856_SPI_D 

			// type 
		);


	// --- DESTRUCTOR ---
	virtual ~Loom_MAX31856();

	// Inherited Methods
	void		print_config() override;
	void		print_measurements() override;
	void		calibrate() {}
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;
	bool		message_route(OSCMessage& msg, int address_offset) {}

private:


};



#endif // of #ifndef LOOM_MAX31856_h
