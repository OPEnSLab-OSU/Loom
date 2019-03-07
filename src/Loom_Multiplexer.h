
#ifndef LOOM_MULTIPLEXER_h
#define LOOM_MULTIPLEXER_h


#include "Loom_Module.h"
#include "Sensors/I2C/Loom_I2C_Sensor.h"


// I2C Sensor files
#include "Sensors/I2C/Loom_AS7262.h"
#include "Sensors/I2C/Loom_AS7263.h"
#include "Sensors/I2C/Loom_AS7265X.h"
#include "Sensors/I2C/Loom_FXAS21002.h"
#include "Sensors/I2C/Loom_FXOS8700.h"
#include "Sensors/I2C/Loom_LIS3DH.h"
#include "Sensors/I2C/Loom_MB1232.h"
#include "Sensors/I2C/Loom_MPU6050.h"
#include "Sensors/I2C/Loom_MS5803.h"
#include "Sensors/I2C/Loom_SHT31D.h"
#include "Sensors/I2C/Loom_TSL2561.h"
#include "Sensors/I2C/Loom_TSL2591.h"
#include "Sensors/I2C/Loom_ZXGesture.h"



// // I2C Address Conflict Selection

// // enum to specify selection?
enum class I2C_Selection { L_TSL2561, L_TSL2591, L_AS7262, L_AS7263, L_AS7265X };

#define i2c_0x29 I2C_Selection::L_TSL2591 	// TSL2561 / TSL2591
#define i2c_0x49 I2C_Selection::L_AS7265X	// TSL2561 / AS7262 / AS7263 / AS7265X


// Maybe query DeviceManager if RTC is in use, if not, query 0x68 s




class Loom_Multiplexer : public LoomModule
{
private:

	const static byte known_addresses[];

protected:

	LoomI2CSensor**   sensors;

	byte          i2c_address;		// The device's I2C address

	uint          num_ports;		// The number of ports
	bool          dynamic_list;		// Whether or not sensor list is dynamic	
	uint          update_period;	// Interval to update sensor list at
	
	unsigned long last_update_time; // When the sensor list was last updated

public:

	// --- CONSTRUCTOR ---
	Loom_Multiplexer(	char* 	module_name 	= "Multiplexer",

						byte	i2c_address 	= 0x71,
						uint 	num_ports 		= 8,
						bool	dynamic_list 	= true,
						uint 	update_period 	= 5000 // find way to make this call interrupt class to manage timing
					);


	// --- DESTRUCTOR ---
	virtual ~Loom_Multiplexer();

	// General
	void print_config();
	void print_state(); 
	void measure();
	void package(OSCBundle& bndl, char* suffix="");
	bool message_route(OSCMessage& msg, int address_offset);
	void print_measurements();

	
	void get_sensor_list(OSCBundle& bndl); // populate an OSC bundle

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
