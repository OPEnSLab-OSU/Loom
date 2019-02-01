
#ifndef LOOM_MULTIPLEXER_h
#define LOOM_MULTIPLEXER_h


#include "Loom_Module.h"
#include "Loom_I2C_Sensor.h"


// I2C Sensor files
#include "Loom_SHT31D.h"
#include "Loom_TSL2591.h"


// // I2C Address Conflict Selection

// // enum to specify selection?
enum I2C_Selection { L_TSL2561, L_TSL2591, L_AS726X, L_AS7265X };

#define i2c_0x29 L_TSL2591 	// TSL2561 / TSL2591
#define i2c_0x49 L_AS7265X	// TSL2561 / AS726X / AS7265X







class Loom_Multiplexer : public LoomModule
{
private:

	const static byte known_addresses[];

protected:

	// --- PROTECTED MEMBERS ---

	LoomI2CSensor**   sensors;

	byte          i2c_address;		// The device's I2C address

	uint          num_ports;		// The number of ports
	bool          dynamic_list;		// Whether or not sensor list is dynamic	
	uint          update_period;	// Interval to update sensor list at
	
	unsigned long last_update_time; // When the sensor list was last updated


public:
	// --- PUBLIC MEMBERS ---


	// Arary of current LoomI2CSensor objects of num_ports size
// Public so other classes can easily access the devices


	// --- CONSTRUCTOR ---
	Loom_Multiplexer(	char* 	module_name 	= "Multiplexer",

						byte	i2c_address 	= 0x71,
						uint 	num_ports 		= 8,
						bool	dynamic_list 	= true,
						uint 	update_period 	= 5000 // find way to make this call interrupt class to manage timing
					);


	// --- DESTRUCTOR ---
	~Loom_Multiplexer();


	// --- PUBLIC METHODS ---

	void print_config();

	// Override LoomModule implementation
	void print_state(); // print connected sensors and update period

	void measure(); // maybe (as current implementation joins measure and package)
	void print_measurements();

	void package(OSCBundle* bndl);
	// void package(OSCBundle* bndl) { LoomModule::package(bndl); }


//  : other

	bool message_route(OSCMessage* msg, int address_offset);


	void get_sensor_list(OSCBundle* bndl); // populate an OSC bundle

	void set_is_dynamic(bool dynamic);
	bool get_is_dynamic();

	void set_update_period(int period);
	int  get_update_period();

	// Update sensor list 
	void refresh_sensors();

private:

	// Return sensor at index 'port'
	LoomI2CSensor* get_sensor(uint8_t port);

	// Select communication with sensor at index 'port'
	void tca_select(uint8_t port);

	// Create appropriate instance to manage sensor 
	static LoomI2CSensor* generate_sensor_object(byte i2c_address);

	byte get_i2c_on_port(uint8_t port);
};

#endif