
#include "loom_multiplexer_class.h"
// #include "loom_device_class.h"

#include "loom_macros.h"
// #include "loom_misc.h"
// #include "loom_translator.h"

#include <Wire.h>


LoomMultiplexer::LoomMultiplexer(	char* 	module_name,

						byte	i2c_address,
						uint	num_ports,
						bool	dynamic_list,	
						uint 	update_period

					  ) : LoomModule( module_name ) 
{
	// LOOM_DEBUG_Println("LoomMultiplexer Constructor 1");
	this->i2c_address 	= i2c_address; 
	this->num_ports 	= num_ports;
	this->update_period	= update_period;

	sensors = new I2CSensor*[num_ports];

	for (int i = 0; i < num_ports; i++) {
		sensors[i] = NULL;
	}



}



// --- DESTRUCTOR ---
LoomMultiplexer::~LoomMultiplexer() {}


// --- PUBLIC METHODS ---

void LoomMultiplexer::print_config()
{
	LoomModule::print_config();

	// LOOM_DEBUG_Print3('\t', "I2C Address         : ", i2c_address);
	// LOOM_DEBUG_Print(" (0x");				// Not sure if this print statement will work
	// #if LOOM_DEBUG == 1
	// 	Serial.print(i2c_address, HEX);
	// #endif
	// LOOM_DEBUG_Println(")");
	// LOOM_DEBUG_Println3('\t', "Num Ports           : ", num_ports);
	// LOOM_DEBUG_Println3('\t', "Update Period (ms)  : ", update_period);
}


void LoomMultiplexer::print_state() // print connected sensors and update period
{
	print_module_label();
	LOOM_DEBUG_Println("Attached Sensors:");

	for (int i = 0; i < num_ports; i++) {
		LOOM_DEBUG_Print3("\tPort ", i, ": ");
		if (sensors[i]) {
			LOOM_DEBUG_Println3( sensors[i]->get_i2c_address_str(), " - ", sensors[i]->get_module_name() );
		} else {
			LOOM_DEBUG_Println(" -");
		}
	}
}

void LoomMultiplexer::measure() // maybe (as current implementation joins measure and package)
{
	for (int i = 0; i < num_ports; i++) {
		if (sensors[i]) {
			port_select(i);
			sensors[i]->measure();
		} 
	}
}


void LoomMultiplexer::package(OSCBundle* bndl, char* id_prefix)
{
	// if (  (state_tca9548a.mux_update_period > 0) && (millis()-state_tca9548a.last_update_time > state_tca9548a.mux_update_period)  ){
	// 	update_sensors();
	// 	LOOM_DEBUG_Println("Update MuxShield Sensorlist");
	// 	state_tca9548a.last_update_time = millis();
	// }	

	// for (int i = 0; i < MUX_PORTS; i++) {
	// 	if (state_tca9548a.devices[i] != 0) {
	// 		port_select(i);
	// 		measure_sensor_data(state_tca9548a.devices[i]);
	// 		package_sensor_data(state_tca9548a.devices[i], bndl, packet_header_string, i);
	// 	}
	// }


	// call measure ?

	// char new_prefix[30];

	// char address_string[80];
	// sprintf(address_string, "%s%s", id_prefix, "/Multiplexer/Sensors");
	// OSCMessage msg = OSCMessage(address_string);

	for (int i = 0; i < num_ports; i++) {
		if (sensors[i]) {
			port_select(i);

			// maybe measure

			// sprintf(new_prefix, "%s/Port/%d", id_prefix, i);
			sensors[i]->package_mux(bndl, id_prefix, i);
		} 
	}

	// bndl->add(msg);

	// char new_prefix[30];

	// for (int i = 0; i < num_ports; i++) {
	// 	if (sensors[i]) {
	// 		sprintf(new_prefix, "%s/Port/%d", id_prefix, i);
	// 		sensors[i]->package(bndl, new_prefix);
	// 	} 
	// }
}


bool LoomMultiplexer::message_route(OSCMessage* msg, int address_offset)
{
	// Mux routing 

	// Sensor routing?
}


// not sure if this should include the empty ports or not - probably not
// could alternatively use a list in just a single message
void LoomMultiplexer::get_sensor_list(OSCBundle* bndl, char* id_prefix) // populate an OSC bundle
{
	// append_to_bundle_key_value(bndl, id_prefix, "/Sensors", humid);
	
	char address_string[80];
	sprintf(address_string, "%s%s", id_prefix, "/Multiplexer/Sensors");
	OSCMessage msg = OSCMessage(address_string);

	for (int i = 0; i < num_ports; i++) {
		if (sensors[i]) {
			// Add the port number
			msg.add( (int32_t)i );
			// Add sensor name
			msg.add( sensors[i]->get_module_name() );
		} 
	}	

	bndl->add(msg);

}

void LoomMultiplexer::set_is_dynamic(bool dynamic)
{
	dynamic_list = dynamic;
}

bool LoomMultiplexer::get_is_dynamic()
{
	return dynamic_list;
}


void LoomMultiplexer::set_update_period(int period)
{
	update_period = period;
}

int  LoomMultiplexer::get_update_period()
{
	return update_period;
}





void LoomMultiplexer::refresh_sensors()
{




}


I2CSensor* LoomMultiplexer::get_sensor(uint8_t port)
{
	return sensors[port];
}


void LoomMultiplexer::port_select(uint8_t port) 
{
	if (port < num_ports) {
		Wire.beginTransmission(i2c_address);
		Wire.write(1 << port);
		Wire.endTransmission();  
	}
}

