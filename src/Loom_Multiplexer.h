
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


enum class I2C_Selection { 
	L_TSL2561,		///< TSL2561
	L_TSL2591,		///< TSL2591
	L_AS7262,		///< AS7262 
	L_AS7263,		///< AS7263
	L_AS7265X		///< AS72625X
};


// move to config

#define i2c_0x29 I2C_Selection::L_TSL2591	// TSL2561 / TSL2591
#define i2c_0x49 I2C_Selection::L_AS7265X	// TSL2561 / AS7262 / AS7263 / AS7265X


// Maybe query DeviceManager if RTC is in use, if not, query 0x68 s




class Loom_Multiplexer : public LoomModule
{
private:

	/// List of known I2C addresses used by Loom.
	/// Used to avoid checking addresses that no sensors in Loom use
	const static byte known_addresses[19];


protected:

	/// Array of I2C sensor objects
	LoomI2CSensor**	sensors;

	/// The multiplexer's I2C address
	byte			i2c_address;		

	/// The number of ports on the multiplexer
	uint			num_ports;
	/// Whether or not sensor list is dynamic (refresh sensor list periodically)
	bool			dynamic_list;
	/// Interval to update sensor list at
	uint			update_period;	
	
	/// When the sensor list was last updated
	unsigned long	last_update_time; 

public:

	// --- CONSTRUCTOR ---
	Loom_Multiplexer(	
			char*		module_name			= "Multiplexer",

			byte		i2c_address			= 0x71,
			uint		num_ports			= 8,
			bool		dynamic_list		= true,
			uint		update_period		= 5000 // find way to make this call interrupt class to manage timing
		);


	// --- DESTRUCTOR ---
	virtual ~Loom_Multiplexer();

	// General
	void		print_config();
	void		print_state(); 
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");
	bool		message_route(OSCMessage& msg, int address_offset);
	void		print_measurements();

	/// Populate a bundle with a list of sensors currently attached  
	/// \param[out]	bndl	Bundle to populate with sensor list
	void		get_sensor_list(OSCBundle& bndl);

	/// Set whether or not to periodically update list of attached sensors
	/// \param[in]	dynamic		The setting to set 
	void		set_is_dynamic(bool dynamic);
	/// Get whether or not sensors are updated dynamically
	/// \return	True if dynamic
	bool		get_is_dynamic();

	/// Set the sensor list update period.
	/// Requires dynamic_list to be enabled
	/// \param[in]	New update period
	void		set_update_period(int period);
	/// Get the sensor list update period.
	/// \return		The update period
	int			get_update_period();

	/// Update sensor list.
	/// Polls all ports of multiplexer getting sensor on port (if any)
	void		refresh_sensors();

private:

	/// Get the sensor object for sensor on provided port
	/// \param[port]	port	The port of the multiplexer to get sensor object for
	/// \return			The pointer to LoomI2CSensor on port, Null if no sensor
	LoomI2CSensor*	get_sensor(uint8_t port);

	/// Select communication with sensor at index port
	/// \param[in]	port	The port to open I2C communication on
	void			tca_select(uint8_t port);

	/// Create appropriate instance to manage sensor.
	/// Compares I2C address to known sensors and generates corresponding sensor instance
	/// \param[in]	i2c_address		The I2C address to match to sensor class
	/// \return		Pointer to the generated I2C sensor object, Null if no match for that address
	static LoomI2CSensor* generate_sensor_object(byte i2c_address);

	/// Determine the I2C address of the sensor (if any) on port.
	/// \param[in]	port	The port to get sensor address of
	/// \return		The I2C address of sensor, 0x00 if no sensor found
	byte			get_i2c_on_port(uint8_t port);

};

#endif
