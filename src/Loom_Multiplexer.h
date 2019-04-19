
#ifndef LOOM_MULTIPLEXER_h
#define LOOM_MULTIPLEXER_h


#include "Loom_Module.h"
#include "Sensors/I2C/Loom_I2C_Sensor.h"


// // I2C Sensor files
// #include "Sensors/I2C/Loom_AS7262.h"
// #include "Sensors/I2C/Loom_AS7263.h"
// #include "Sensors/I2C/Loom_AS7265X.h"
// #include "Sensors/I2C/Loom_FXAS21002.h"
// #include "Sensors/I2C/Loom_FXOS8700.h"
// #include "Sensors/I2C/Loom_LIS3DH.h"
// #include "Sensors/I2C/Loom_MB1232.h"
// #include "Sensors/I2C/Loom_MPU6050.h"
// #include "Sensors/I2C/Loom_MS5803.h"
// #include "Sensors/I2C/Loom_SHT31D.h"
// #include "Sensors/I2C/Loom_TSL2561.h"
// #include "Sensors/I2C/Loom_TSL2591.h"
// #include "Sensors/I2C/Loom_ZXGesture.h"


// class Loom_AS7262;    // dont remember what this was doing here


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




// # (LoomModule) | dependencies: [] | conflicts: []
/// Multiplexer manager for interfacing with 
/// multiple I2C sensors 
// #
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


	/// Multiplexer module constructor.
	/// 
	/// \param[in]	module_name			String | <"Multiplexer"> | Multiplexer module name
	/// \param[in]	i2c_address			Set(Int) | <0x71> | {0x71} | I2C address
	/// \param[in]	min_filter_delay	Int | <8> | [1-8] | Minimum update delay, if enable_rate_filter enabled
	/// \param[in]	dynamic_list		Bool | <true> | Whether or not to automatically check for new sensors
	/// \param[in]	update_period		Int | <5000> | [500-30000] | The time between sensor list updates (if dynamic_list enabled)
	Loom_Multiplexer(	
			const char*		module_name			= "Multiplexer",
			byte			i2c_address			= 0x71,
			uint			num_ports			= 8,
			bool			dynamic_list		= true,
			uint			update_period		= 5000
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
LoomI2CSensor* generate_sensor_object(byte i2c_address);
/////////////////////////////////////////////////////////////////////
// static LoomI2CSensor* generate_sensor_object(byte i2c_address)
// {
// 	switch (i2c_address) {
// 		case 0x10 : return new Loom_ZXGesture(i2c_address=0x10);	// ZXGesture
// 		case 0x11 : return new Loom_ZXGesture(i2c_address=0x11);	// ZXGesture
// 		case 0x19 : return new Loom_LIS3DH(i2c_address=0x19);		// LIS3DH
// 		case 0x1C : return new Loom_FXOS8700(i2c_address=0x1C);		// FXOS8700
// 		case 0x1D : return new Loom_FXOS8700(i2c_address=0x1D);		// FXOS8700
// 		case 0x1E : return new Loom_FXOS8700(i2c_address=0x1E);		// FXOS8700
// 		case 0x1F : return new Loom_FXOS8700(i2c_address=0x1F);		// FXOS8700
// 		case 0x20 : return new Loom_FXAS21002(i2c_address=0x20);	// FXAS21002
// 		case 0x21 : return new Loom_FXAS21002(i2c_address=0x21);	// FXAS21002

// 		case 0x29 : // TSL2561 / TSL2591
// 			if (i2c_0x29 == I2C_Selection::L_TSL2561) return new Loom_TSL2561(i2c_address=0x29);	// TSL2561
// 			if (i2c_0x29 == I2C_Selection::L_TSL2591) return new Loom_TSL2591(i2c_address=0x29);	// TSL2591

// 		case 0x39 : return new Loom_TSL2561(i2c_address=0x39);	// TSL2561
// 		case 0x44 : return new Loom_SHT31D(i2c_address=0x44);	// SHT31D
// 		case 0x45 : return new Loom_SHT31D(i2c_address=0x45);	// SHT31D

// 		case 0x49 : // TSL2561 / AS726X / AS7265X
// 			if (i2c_0x49 == I2C_Selection::L_TSL2561) return new Loom_TSL2561(i2c_address=0x49);	// TSL2561
// 			if (i2c_0x49 == I2C_Selection::L_AS7262 ) return new Loom_AS7262(i2c_address=0x49);	// AS7262
// 			if (i2c_0x49 == I2C_Selection::L_AS7263 ) return new Loom_AS7263(i2c_address=0x49);	// AS7263
// 			if (i2c_0x49 == I2C_Selection::L_AS7265X) return new Loom_AS7265X(i2c_address=0x49);	// AS7265X

// 		case 0x68 : return NULL; // new Loom_MPU6050();
// 		case 0x69 : return NULL; // new Loom_MPU6050();
// 		case 0x70 : return new Loom_MB1232(i2c_address=0x70); // MB1232
// 		case 0x76 : return new Loom_MS5803(i2c_address=0x76); // MS5803
// 		case 0x77 : return new Loom_MS5803(i2c_address=0x77); // MS5803

// 		default : return NULL;
// 	}
// }



	/// Determine the I2C address of the sensor (if any) on port.
	/// \param[in]	port	The port to get sensor address of
	/// \return		The I2C address of sensor, 0x00 if no sensor found
	byte			get_i2c_on_port(uint8_t port);

};





// /////////////////////////////////////////////////////////////////////
// LoomI2CSensor* generate_sensor_object(byte i2c_address)
// {
// 	switch (i2c_address) {
// 		case 0x10 : return new Loom_ZXGesture(i2c_address=0x10);	// ZXGesture
// 		case 0x11 : return new Loom_ZXGesture(i2c_address=0x11);	// ZXGesture
// 		case 0x19 : return new Loom_LIS3DH(i2c_address=0x19);		// LIS3DH
// 		case 0x1C : return new Loom_FXOS8700(i2c_address=0x1C);		// FXOS8700
// 		case 0x1D : return new Loom_FXOS8700(i2c_address=0x1D);		// FXOS8700
// 		case 0x1E : return new Loom_FXOS8700(i2c_address=0x1E);		// FXOS8700
// 		case 0x1F : return new Loom_FXOS8700(i2c_address=0x1F);		// FXOS8700
// 		case 0x20 : return new Loom_FXAS21002(i2c_address=0x20);	// FXAS21002
// 		case 0x21 : return new Loom_FXAS21002(i2c_address=0x21);	// FXAS21002

// 		case 0x29 : // TSL2561 / TSL2591
// 			if (i2c_0x29 == I2C_Selection::L_TSL2561) return new Loom_TSL2561(i2c_address=0x29);	// TSL2561
// 			if (i2c_0x29 == I2C_Selection::L_TSL2591) return new Loom_TSL2591(i2c_address=0x29);	// TSL2591

// 		case 0x39 : return new Loom_TSL2561(i2c_address=0x39);	// TSL2561
// 		case 0x44 : return new Loom_SHT31D(i2c_address=0x44);	// SHT31D
// 		case 0x45 : return new Loom_SHT31D(i2c_address=0x45);	// SHT31D

// 		case 0x49 : // TSL2561 / AS726X / AS7265X
// 			if (i2c_0x49 == I2C_Selection::L_TSL2561) return new Loom_TSL2561(i2c_address=0x49);	// TSL2561
// 			if (i2c_0x49 == I2C_Selection::L_AS7262 ) return new Loom_AS7262(i2c_address=0x49);	// AS7262
// 			if (i2c_0x49 == I2C_Selection::L_AS7263 ) return new Loom_AS7263(i2c_address=0x49);	// AS7263
// 			if (i2c_0x49 == I2C_Selection::L_AS7265X) return new Loom_AS7265X(i2c_address=0x49);	// AS7265X

// 		case 0x68 : return NULL; // new Loom_MPU6050();
// 		case 0x69 : return NULL; // new Loom_MPU6050();
// 		case 0x70 : return new Loom_MB1232(i2c_address=0x70); // MB1232
// 		case 0x76 : return new Loom_MS5803(i2c_address=0x76); // MS5803
// 		case 0x77 : return new Loom_MS5803(i2c_address=0x77); // MS5803

// 		default : return NULL;
// 	}
// }



#endif
