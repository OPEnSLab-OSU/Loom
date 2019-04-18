
#ifndef LOOM_MAX31856_h
#define LOOM_MAX31856_h

#include "Loom_SPI_Sensor.h"

#include <Adafruit_MAX31856.h>


// # (LoomSPISensor) | dependencies: [] | conflicts: []
/// MAX31856 thermocouple module.
/// Measures temperature.
// #
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

	/// MAX31856 module constructor.
	///
	/// \param[in]	module_name			String | <"MAX31856"> | MAX31856 module name
	/// \param[in]	num_samples			Set(Int) | <8> | {1, 2, 4, 8, 16} | How many samples to take and average
	/// \param[in]	CS_pin				Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used for chip select
	/// \param[in]	SPI_a				Set(Int) | <10> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used for Software Serial a
	/// \param[in]	SPI_b				Set(Int) | <11> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used for Software Serial b
	/// \param[in]	SPI_c				Set(Int) | <12> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used for Software Serial c
	/// \param[in]	SPI_d				Set(Int) | <13> | {5, 6, 9, 10, 11, 12, 13, 14("A0"), 15("A1"), 16("A2"), 17("A3"), 18("A4"), 19("A5")} | Which pin should be used for Software Serial d
	Loom_MAX31856(	
			char*		module_name			= "MAX31856",
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
	void		print_config() override;
	void		print_measurements() override;
	void		calibrate() {}
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;
	bool		message_route(OSCMessage& msg, int address_offset) {}

private:


};



#endif // of #ifndef LOOM_MAX31856_h
