
#ifndef LOOM_MULTIPLEXER_h
#define LOOM_MULTIPLEXER_h


#include "loom_abstract_module_class.h"
#include "loom_sensor_i2c_class.h"







// switch (addr) {

// 	case 0x10 : // ZXGESTURESENSOR
// 	case 0x11 : // ZXGESTURESENSOR

// 	case 0x19 : // LIS3DH

// 	case 0x1C : // FXOS8700
// 	case 0x1D : // FXOS8700
// 	case 0x1E : // FXOS8700
// 	case 0x1F : // FXOS8700

// 	case 0x20 : // FXAS21002
// 	case 0x21 : // FXAS21002

// 	case 0x29 : // TSL2561 / TSL2591
// 	case 0x39 : // TSL2561

// 	case 0x44 : // SHT31D
// 	case 0x45 : // SHT31D

// 	case 0x49 : // TSL2561 / AS726X / AS7265X

// 	case 0x68 : // MPU6050
// 	case 0x69 : // MPU6050

// 	case 0x76 : // MS5803
// 	case 0x77 : // MS5803

// 	case 0x70 : // MB1232



// }








class LoomMultiplexer : public LoomModule
{

protected:
	// --- PROTECTED MEMBERS ---

	I2CSensor**   sensors;

	byte          i2c_address;		// The device's I2C address

	uint          num_ports;		// The number of ports
	bool          dynamic_list;		// Whether or not sensor list is dynamic	
	uint          update_period;	// Interval to update sensor list at
	
	unsigned long last_update_time; // When the sensor list was last updated


public:
	// --- PUBLIC MEMBERS ---


	// Arary of current I2CSensor objects of num_ports size
// Public so other classes can easily access the devices


	// --- CONSTRUCTOR ---
	LoomMultiplexer(	char* 	module_name 	= "Multiplexer",

						byte	i2c_address 	= 0x71,
						uint 	num_ports 		= 8,
						bool	dynamic_list 	= true,
						uint 	update_period 	= 5000 // find way to make this call interrupt class to manage timing
					);


	// --- DESTRUCTOR ---
	~LoomMultiplexer();


	// --- PUBLIC METHODS ---

	void print_config();

	// Override LoomModule implementation
	void print_state(); // print connected sensors and update period


	void measure(); // maybe (as current implementation joins measure and package)

	void package(OSCBundle* bndl, char* id_prefix);

	bool message_route(OSCMessage* msg, int address_offset);




	void get_sensor_list(OSCBundle* bndl, char* id_prefix); // populate an OSC bundle

	void set_is_dynamic(bool dynamic);
	bool get_is_dynamic();

	void set_update_period(int period);
	int  get_update_period();



private:

// --- PRIVATE MEMBERS ---

// --- PRIVATE METHODS --- 
	void refresh_sensors();

	I2CSensor* get_sensor(uint8_t port);

	void port_select(uint8_t port);

};

#endif