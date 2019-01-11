

#ifndef LOOM_MULTIPLEXER_h
#define LOOM_MULTIPLEXER_h



class LoomMultiplexer : public LoomModule
{

protected:
	// --- PROTECTED MEMBERS ---
	byte i2c_address;				// The device's I2C address


	uint          num_ports;		// The number of ports

	bool          dynamic_list;		// Whether or not sensor list is dynamic	
	uint          update_period;	// Interval to update sensor list at
	unsigned long last_update_time; // When the sensor list was last updated





public:
	// --- PUBLIC MEMBERS ---


	// Arary of current I2CSensor objects of num_ports size
// Public so other classes can easily access the devices


	// --- CONSTRUCTOR ---
	LoomMultiplexer();


	// --- DESTRUCTOR ---
	~LoomMultiplexer();


	// --- PUBLIC METHODS ---

	void print_config();

	// Override LoomModule implementation
	void print_state(); // print connected sensors and update period


	void measure(); // maybe (as current implementation joins measure and package)

	void package(OSCBundle* bndl, char* id_prefix);

	virtual bool message_route(OSCMessage* msg, int address_offset) {}




	void get_sensor_list(); // populate an OSC bundle

	void set_is_dynamic(bool dynamic);
	bool get_is_dynamic();

	void set_update_period(int time);
	int  get_update_period();



private:

// --- PRIVATE MEMBERS ---

// --- PRIVATE METHODS --- 
	void update_sensors();



};



#endif