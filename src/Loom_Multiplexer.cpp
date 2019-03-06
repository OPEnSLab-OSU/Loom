
#include "Loom_Multiplexer.h"


#include <Wire.h>




// Maybe these sensor lists go in own file


// Used to optimize searching for sensors:
// search addresses in array rather than 0-127 
const byte Loom_Multiplexer::known_addresses[] = 
{
	0x10, // ZXGESTURESENSOR
	0x11, // ZXGESTURESENSOR
	0x19, // LIS3DH
	0x1C, // FXOS8700
	0x1D, // FXOS8700
	0x1E, // FXOS8700
	0x1F, // FXOS8700
	0x20, // FXAS21002
	0x21, // FXAS21002
	0x29, // TSL2561 / TSL2591
	0x39, // TSL2561
	0x44, // SHT31D
	0x45, // SHT31D
	0x49, // TSL2561 / AS726X / AS7265X
	0x68, // MPU6050
	0x69, // MPU6050
	0x70, // MB1232
	0x76, // MS5803
	0x77  // MS5803
};


LoomI2CSensor* Loom_Multiplexer::generate_sensor_object(byte i2c_address)
{
	switch (i2c_address) {
		case 0x10 : return new Loom_ZXGesture(i2c_address=0x10);	// ZXGesture
		case 0x11 : return new Loom_ZXGesture(i2c_address=0x11);	// ZXGesture
		case 0x19 : return new Loom_LIS3DH(i2c_address=0x19);		// LIS3DH
		case 0x1C : return new Loom_FXOS8700(i2c_address=0x1C);		// FXOS8700
		case 0x1D : return new Loom_FXOS8700(i2c_address=0x1D);		// FXOS8700
		case 0x1E : return new Loom_FXOS8700(i2c_address=0x1E);		// FXOS8700
		case 0x1F : return new Loom_FXOS8700(i2c_address=0x1F);		// FXOS8700
		case 0x20 : return new Loom_FXAS21002(i2c_address=0x20);	// FXAS21002
		case 0x21 : return new Loom_FXAS21002(i2c_address=0x21);	// FXAS21002

		case 0x29 : // TSL2561 / TSL2591
			if (i2c_0x29 == L_TSL2561) return new Loom_TSL2561(i2c_address=0x29);	// TSL2561
			if (i2c_0x29 == L_TSL2591) return new Loom_TSL2591(i2c_address=0x29);	// TSL2591

		case 0x39 : return new Loom_TSL2561(i2c_address=0x39);	// TSL2561
		case 0x44 : return new Loom_SHT31D(i2c_address=0x44);	// SHT31D
		case 0x45 : return new Loom_SHT31D(i2c_address=0x45);	// SHT31D

		case 0x49 : // TSL2561 / AS726X / AS7265X
			if (i2c_0x49 == L_TSL2561) return new Loom_TSL2561(i2c_address=0x49);	// TSL2561
			if (i2c_0x49 == L_AS7262 ) return new Loom_AS7262(i2c_address=0x49);	// AS7262
			if (i2c_0x49 == L_AS7263 ) return new Loom_AS7263(i2c_address=0x49);	// AS7263
			if (i2c_0x49 == L_AS7265X) return new Loom_AS7265X(i2c_address=0x49);	// AS7265X

		case 0x68 : return NULL; // new Loom_MPU6050();
		case 0x69 : return NULL; // new Loom_MPU6050();
		case 0x70 : return new Loom_MB1232(i2c_address=0x70); // MB1232
		case 0x76 : return new Loom_MS5803(i2c_address=0x76); // MS5803
		case 0x77 : return new Loom_MS5803(i2c_address=0x77); // MS5803

		default : return NULL;
	}
}






Loom_Multiplexer::Loom_Multiplexer(	char* 	module_name,

									byte	i2c_address,
									uint	num_ports,
									bool	dynamic_list,	
									uint 	update_period

								) : LoomModule( module_name ) 
{
	// Println("Loom_Multiplexer Constructor 1");
	this->i2c_address 	= i2c_address; 
	this->num_ports 	= num_ports;
	this->update_period	= update_period;

	// Begin I2C 
	Wire.begin();

	// Create sensor array of 'num_ports' size
	sensors = new LoomI2CSensor*[num_ports];

	// Initialize array of sensors to Null pointrs
	for (uint8_t i = 0; i < num_ports; i++) {
		sensors[i] = NULL;
	}

	// Update sensor list and display   -- currently removed because Mux should be linked to DeviceManager before polling sensors
	// refresh_sensors();
	// print_state();

	print_module_label(); 
	Println("Setup Complete");
}



// --- DESTRUCTOR ---
Loom_Multiplexer::~Loom_Multiplexer() 
{
	// Free any sensors
	for (uint8_t i = 0; i < num_ports; i++) {
		if (sensors[i] != NULL) {
			delete sensors[i];
		}
	}
}


// --- PUBLIC METHODS ---


void Loom_Multiplexer::print_config()
{
	LoomModule::print_config();

	Print2('\t', "I2C Address         : ");
	Println_Hex(i2c_address);
	Println3('\t', "Num Ports           : ", num_ports);
	Println3('\t', "Update Period (ms)  : ", update_period);
}


void Loom_Multiplexer::print_state()
{
	print_module_label();
	Println("Attached Sensors:");

	for (uint8_t i = 0; i < num_ports; i++) {
		Print3("\tPort ", i, ": ");
		if (sensors[i] != NULL) {
			Print_Dec_Hex(sensors[i]->get_i2c_address());
			Println2(" - ", sensors[i]->get_module_name() );
		} else {
			Println(" -");
		}
	}
	Println();
}


void Loom_Multiplexer::measure()
{
	refresh_sensors();

	for (uint8_t i = 0; i < num_ports; i++) {
		if (sensors[i] != NULL) {
			tca_select(i);
			sensors[i]->measure();
		} 
	}
}


void Loom_Multiplexer::print_measurements()
{
	for (uint8_t i = 0; i < num_ports; i++) {
		if (sensors[i] != NULL) {
			tca_select(i);
			sensors[i]->print_measurements();
		} 
	}
}

void Loom_Multiplexer::package(OSCBundle& bndl, char* suffix)
{
	for (uint8_t i = 0; i < num_ports; i++) {
		if (sensors[i] != NULL) {
			tca_select(i);
			char tmp[4];
			itoa(i, tmp, 10);
			sensors[i]->package(bndl, tmp);
		} 
	}
}




bool Loom_Multiplexer::message_route(OSCMessage& msg, int address_offset)
{
	// Mux routing 

	// Sensor routing?
}


void Loom_Multiplexer::get_sensor_list(OSCBundle& bndl) // populate an OSC bundle
{
	refresh_sensors();

	char id_prefix[40];
	resolve_bundle_address(id_prefix, "Sensors");

	bool found_first = false;

	for (uint8_t i = 0; i < num_ports; i++) {
		if (sensors[i] != NULL) {
			// First sensor found should start a new message
			if (!found_first) {
				append_to_bundle(bndl, id_prefix, i, sensors[i]->get_module_name(), NEW_MSG);
				found_first = true;
			} else {
				append_to_bundle(bndl, id_prefix, i, sensors[i]->get_module_name());
			}
		} 
	}	
}


void Loom_Multiplexer::set_is_dynamic(bool dynamic)
{
	dynamic_list = dynamic;
}


bool Loom_Multiplexer::get_is_dynamic()
{
	return dynamic_list;
}


void Loom_Multiplexer::set_update_period(int period)
{
	update_period = period;
}


int  Loom_Multiplexer::get_update_period()
{
	return update_period;
}


void Loom_Multiplexer::refresh_sensors()
{
	byte previous, current;

	for (uint8_t i = 0; i < num_ports; i++) {

		// Println2("TCA Port: ", i);
		// tca_select(i);

		previous = (sensors[i] != NULL) ? sensors[i]->get_i2c_address() : 0x00;
		// Print("\tPrevious: 0x");
		// Println_Hex(previous);

		current = get_i2c_on_port(i);


		// Print("\tNew     : 0x");
		// Println_Hex(current);


		// Cases:
		// No change (prev = current)
			// Do nothing
		// Sensor removed (prev ≠ 0, current = 0)
			// Free removed sensor object memory
		// Sensor added (prev = 0, currect ≠ 0)
			// Create object
		// Sensor switched (prev ≠ current)
			// Delete old object
			// Create new object

		// Sensor removed, added, or swapped
		if (previous != current) {

			if ( (previous != 0) || (current == 0) ) {
				// Free object
				print_module_label();
				Println2("Free Memory of ", sensors[i]->get_module_name() );
				delete sensors[i];
			}

			// Create new sensor object and setup (in constructor)
			sensors[i] = generate_sensor_object(current);

			if (sensors[i] != NULL) {
				// Make sure sensor is also linked to DeviceManager
				sensors[i]->link_device_manager(device_manager);
				// device_manager->add_module(sensors[i]);


				print_module_label();
				Println2("Added ", sensors[i]->get_module_name() );
			}
		} 

		// Otherwise still no sensor or sensor did not 
		// change. In which case, do nothing.
	}
}


byte Loom_Multiplexer::get_i2c_on_port(uint8_t port)
{
	tca_select(port);
	byte addr;

	// Iterate through known addresses try to get confirmation from sensor
	// for (uint8_t addr = 1; addr <= 127; addr++) {
	for (uint8_t j = 0; j < sizeof(known_addresses)/sizeof(known_addresses[0]); j++) {
		
		addr = known_addresses[j];
		
		if (addr == this->i2c_address) continue;

		Wire.beginTransmission(addr);
		byte error = Wire.endTransmission();

		if (error == 0) return addr;
	}

	return 0x00; // No sensor found
}


LoomI2CSensor* Loom_Multiplexer::get_sensor(uint8_t port)
{
	return sensors[port];
}


void Loom_Multiplexer::tca_select(uint8_t port) 
{
	if (port < num_ports) {
		Wire.beginTransmission(i2c_address);
		Wire.write(1 << port);
		Wire.endTransmission();  
	}
}

