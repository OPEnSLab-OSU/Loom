// ================================================================ 
// ===                          NOTES                           === 
// ================================================================

// MPU6050 not supported with multiplexer currently (fix in progress)

// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#ifdef is_32u4
	extern "C" { 
		#include "utility/twi.h"  // from Wire library, so we can do bus scanning
	}
#endif


		// measure_tca9548a();
		// if (millis()-state_tca9548a.last_update_time > state_tca9548a.mux_update_period){
		// 	update_sensors();
		// 	LOOM_DEBUG_Println("Update MuxShield Sensorlist");
		// 	state_tca9548a.last_update_time = millis();
		// }


// ================================================================
// ===                       DEFINITIONS                        ===
// ================================================================

// Build an array of supported / in use I2C addresses to even both checking to increase efficiency


byte possible_addresses[] = {0x00
	#if is_tsl2591 == 1
		, 0x29
	#endif
	#if is_tsl2561 == 1 && is_tsl2591 != 1
		, 0x29
	#endif
	#if is_tsl2561 == 1
		, 0x39, 0x49
	#endif
	#if is_fxos8700 == 1
		, 0x1C, 0x1D, 0x1E, 0x1F
	#endif
	#if is_fxas21002 == 1
		, 0x20, 0x21
	#endif
	#if is_zxgesturesensor == 1
		, 0x10, 0x11
	#endif
	#if is_sht31d == 1
		, 0x44, 0x45
	#endif
	#if is_mb1232 == 1
		, 0x70
	#endif
	// #if is_mpu6050 == 1
		// , 0x68, 0x69 
	// #endif
	#if is_lis3dh == 1
		, 0x19
	#endif
	#if is_ms5803 == 1
		, 0x76, 0x77
	#endif
	#if is_as726X == 1
		, 0x49
	#endif
	#if is_as7265X == 1
		, 0x49
	#endif
}; 




//The i2c address of the tca9548a is 0x70, but can be physically changed to 0x71
//It was necessary to do this because the i2c address of the mb1232 is also
//0x70, and i2c addresses must be unique.
#define i2c_addr_tca9548a 0x71 


#define MUX_PORTS 8 // Number of JST ports for I2C on the multiplexer shield

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 
struct config_tca9548a_t {

};

struct state_tca9548a_t {
	int     measurement_count; // does not appear to be used
	long    last_update_time;
	long    mux_update_period;
	uint8_t devices[MUX_PORTS];
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_tca9548a_t config_tca9548a;
struct state_tca9548a_t state_tca9548a;

uint8_t num_addresses;
uint8_t previous_devices[MUX_PORTS];

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_tca9548a();
void package_sensor_data(uint8_t,OSCBundle *, char[], uint8_t);
void measure_tca9548a();
void tcaseselect(uint8_t);
void send_sensors(OSCBundle *, char[]);
void update_sensors();
// void measure_tca9548a();
void package_tca9548a(OSCBundle *, char[]);
void send_sensor_list(OSCMessage &);
void setup_mux_sensors();

	
// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any TCA9548a multiplexer setup and initialization
//
// @return  Whether or not initialization was successful
//
bool setup_tca9548a() 
{
	num_addresses = sizeof(possible_addresses)/sizeof(possible_addresses[0]);


	delay(2000);
	Wire.begin();
	LOOM_DEBUG_Println("Initialized TCA9548A (multiplexer)");
	state_tca9548a.mux_update_period = UPDATE_PERIOD;
	update_sensors();

	// Send current sensor list
	OSCBundle send_bndl;

	// send_sensors(&send_bndl, packet_header_string);
	send_sensors(&send_bndl, global_packet_header_string);

	#if is_wifi
		wifi_send_bundle(&send_bndl);
		send_bndl.empty();
	#endif

	state_tca9548a.last_update_time = millis();
}



// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- TCASESELECT --- 
//
// Set port to begin reading from
// 
// @param port_num  Port to read from
//
void tcaseselect(uint8_t port_num) 
{
	if (port_num < 8) {
		Wire.beginTransmission(i2c_addr_tca9548a);
		Wire.write(1 << port_num);
		Wire.endTransmission();  
	}
}



// --- MEASURE SENSOR DATA ---
//
// Called by package_tca9548a(), which calls this in a loop through i2c addresses
// Checks for specified sensor (via i2c address) 
// Calls measure and package for that sensor
//
// @param i2c_addr              I2C address specifying which sensor to get and send readings from
// @param bndl                  The OSC bundle to be added to (forwarded to sensor package functions)
// @param packet_header_string  The device-identifying string to prepend to OSC messages  (forwarded to sensor package functions)
// @param port                  Which port the sensor being checked would be plugged into
//
void measure_sensor_data(uint8_t i2c_addr)
{
	// LOOM_DEBUG_Println2("Attempting to measure data from sensor with address: ", i2c_addr);
	
	switch (i2c_addr) {
		#if is_tsl2561 == 1 && is_tsl2591 != 1
			case 0x29: 
		#endif
		#if is_tsl2561 == 1
			case 0x39: case 0x49:   // Note that the 0x29 conflicts with the tsl2591 below
				measure_tsl2561();
				return;
		#endif
		#if is_tsl2591 == 1
			case 0x29: 
				measure_tsl2591();
				return;
		#endif
		#if is_fxos8700 == 1
			case 0x1C: case 0x1D: case 0x1E: case 0x1F:
				measure_fxos8700();
				return;
		#endif
		#if is_fxas21002 == 1	
			case 0x20: case 0x21:
				measure_fxas21002();
				return;
		#endif
		#if is_zxgesturesensor == 1
			case 0x10: case 0x11:
				measure_zxgesturesensor();
				return;
		#endif
		#if is_sht31d == 1		
			case 0x44: case 0x45:
				measure_sht31d();
				return;
		#endif
		#if is_mb1232 == 1		
			case 0x70:
				measure_mb1232();
				return;
		#endif
		// #if is_mpu6050 == 1	
		
		// 		case 0x68: // Only check for MPU // 	#endif
		// 	case
		
		// 			measure_mpu6050(/ 		} 
		// 		return;
		// #endif
		#if is_ms5803 == 1		
			case 0x76: case 0x77:
				measure_ms5803();
				return;
		#endif
		#if is_lis3dh == 1		
			case 0x19:
				measure_lis3dh();
				return;
		#endif
		#if is_as726X == 1		
			case 0x49:
				measure_as726X();
				return;
		#endif
		#if is_as7265X == 1		
			case 0x49:
				measure_as7265X();
				return;
		#endif

		case 0x00: default:
			Serial.println("This sensor is not currently supported by the Project LOOM sytem");
	}
}



// --- PACKAGE SENSOR DATA ---
//
// Called by package_tca9548a(), which calls this in a loop through i2c addresses
// Checks for specified sensor (via i2c address) 
// Calls package for that sensor
//
// @param i2c_addr              I2C address specifying which sensor to get and send readings from
// @param bndl                  The OSC bundle to be added to (forwarded to sensor package functions)
// @param packet_header_string  The device-identifying string to prepend to OSC messages  (forwarded to sensor package functions)
// @param port                  Which port the sensor being checked would be plugged into
//
void package_sensor_data(uint8_t i2c_addr, OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
	// LOOM_DEBUG_Println2("Update period: ", state_tca9548a.mux_update_period);

	// if (millis()-state_tca9548a.last_update_time > state_tca9548a.mux_update_period){
	// 	update_sensors();
	// 	LOOM_DEBUG_Println("Update MuxShield Sensorlist");
	// 	state_tca9548a.last_update_time = millis();
	// }

	// LOOM_DEBUG_Println("TCA PACKAGE SENSOR");
	// print_bundle(bndl);


	LOOM_DEBUG_Print3("Attempting to measure data from sensor with address: ", i2c_addr, " (0x");
	#if LOOM_DEBUG == 1
		Serial.print(i2c_addr, HEX);
	#endif
	LOOM_DEBUG_Println(")");

	switch (i2c_addr) {
		#if is_tsl2561 == 1 && is_tsl2591 != 1
			case 0x29: 
		#endif
		#if is_tsl2561 == 1
			case 0x39: case 0x49:   // Note that the 0x29 conflicts with the tsl2591 below
				package_tsl2561(bndl, packet_header_string, port);
				return;
		#endif
		#if is_tsl2591 == 1
			case 0x29: 
				package_tsl2591(bndl, packet_header_string, port);
				return;
		#endif
		#if is_fxos8700 == 1
			case 0x1C: case 0x1D: case 0x1E: case 0x1F:
				package_fxos8700(bndl, packet_header_string, port);
				return;
		#endif
		#if is_fxas21002 == 1	
			case 0x20: case 0x21:
				package_fxas21002(bndl, packet_header_string, port);
				return;
		#endif
		#if is_zxgesturesensor == 1
			case 0x10: case 0x11:
				package_zxgesturesensor(bndl, packet_header_string, port);
				return;
		#endif
		#if is_sht31d == 1		
			case 0x44: case 0x45:
				package_sht31d(bndl, packet_header_string, port);
				return;
		#endif
		#if is_mb1232 == 1		
			case 0x70:
				package_mb1232(bndl, packet_header_string, port);
				return;
		#endif
		// #if is_mpu6050 == 1
		
		// 		case 0x68: // Only check for MPU // 	#endif
		// 	case
		
		// 			package_mpu6050(bndl, packet_header_string, por// 		} 
		// 		return;
		// #endif
		#if is_ms5803 == 1		
			case 0x76: case 0x77:
				package_ms5803(bndl, packet_header_string, port);
				return;
		#endif
		#if is_lis3dh == 1		
			case 0x19:
				package_lis3dh(bndl, packet_header_string, port);
				return;
		#endif
		#if is_as726X == 1		
			case 0x49:
				package_as726X(bndl, packet_header_string, port);
				return;
		#endif
		#if is_as7265X == 1		
			case 0x49:
				package_as7265X(bndl, packet_header_string, port);
				return;
		#endif

		case 0x00: default:
			Serial.println("This sensor is not currently supported by the Project LOOM sytem");
	}
}



// --- SEND SENSOR LIST ---
//
// Message router parsed command to reply with current multiplexer sensors
// 
// @param msg  OSC message requesting the list of sensors currently plugged 
//               be transmitted
//
void send_sensor_list(OSCMessage &msg) 
{
	OSCBundle send_bndl;
	char header_string[255];
	msg.getString(0, header_string, 255);

	update_sensors();
	send_sensors(&send_bndl, header_string);

	#if is_wifi
		wifi_send_bundle(&send_bndl);
		send_bndl.empty();
	#endif
}



// --- UPDATE SENSORS --- 
//
// Updates state with devices that are currently plugged in
// and to which port
//
void update_sensors() 
{
	for (uint8_t i = 0; i < 8; i++) {

		previous_devices[i] = state_tca9548a.devices[i];

		state_tca9548a.devices[i] = 0x00;
		tcaseselect(i);

		// First element is skipped intentionally (see creation of the 'possible_addresses' array to see why)
		for (int j = 1; j < num_addresses; j++) {
			uint8_t i2c_addr = possible_addresses[j];

			#if is_rtc == 1
				if (i2c_addr == 0x68) { // 0x68 is RTC, dont read. will need to physically change MPU to 0x69 to use that 
					continue;
				}
			#endif

			#ifdef is_32u4
				uint8_t data;
				if (! twi_writeTo(i2c_addr, &data, 0, 1, 1)) {
			#endif
			#ifdef is_m0
				Wire.beginTransmission(i2c_addr);
				byte error = Wire.endTransmission();

				if (error == 0) {
			#endif

						state_tca9548a.devices[i] = i2c_addr;
				} 			
		}
	}

	LOOM_DEBUG_Println("Multiplexer Sensors:");
	for (int i = 0; i < MUX_PORTS; i++) {
		LOOM_DEBUG_Print4("[", i, "] = ", state_tca9548a.devices[i]);
		LOOM_DEBUG_Print(" (0x");
		#if LOOM_DEBUG == 1
			Serial.print(state_tca9548a.devices[i], HEX);
		#endif
		LOOM_DEBUG_Println(")");
	}

	setup_mux_sensors();

}



// --- GET SENSORS ---
//
// Fills OSC bundle with list of sensors currently plugged into multiplexer
// Calls update_sensors()
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//


void send_sensors(OSCBundle *bndl, char packet_header_string[]) 
{
	char addressString[255];
	sprintf(addressString, "%s%s", packet_header_string, "/sensors");
	OSCMessage msg = OSCMessage(addressString);

	for (int i = 0; i < MUX_PORTS; i++) {

		if (state_tca9548a.devices[i] != 0) {

			// Add the port number
			msg.add( (int32_t)i );

			// Add the sensor name
			switch (state_tca9548a.devices[i]) {
				#if is_tsl2561 == 1 && is_tsl2591 != 1
					case 0x29:  				
						msg.add("tsl2561"); 		break;
				#endif
				#if is_tsl25691 == 1
					case 0x29: case 0x39: case 0x49: 
						msg.add("tsl2591"); 		break;
				#endif
				#if is_fxos8700 == 1
					case 0x1C: case 0x1D: case 0x1E: case 0x1F:
						msg.add("fxos8700"); 		break;
				#endif
				#if is_fxas21002 == 1
					case 0x20: case 0x21:
						msg.add("fxas21002"); 		break;
				#endif
				#if is_zxgesturesensor == 1
					case 0x10: case 0x11:
						msg.add("zxgesturesensor"); break;
				#endif
				#if is_sht31d == 1
					case 0x44: case 0x45:
						msg.add("sht31d"); 			break;
				#endif
				#if is_mb1232 == 1
					case 0x70:
						msg.add("mb1232"); 			break;
				#endif
				// #if is_rtc != 1
				// 	case 0x68: // only check for MPU6050 on 0x68 if no RTC
				// #endif
				// 	case 0x69:
				// 		msg.add("mpu6050"); 		break;
				#if is_ms5803 == 1
					case 0x76: case 0x77:
						msg.add("ms5803"); 			break;
				#endif
				#if is_lis3dh == 1
					case 0x19:
						msg.add("lis3dh"); 			break;
				#endif
				#if is_as726X == 1		
					case 0x49:
						msg.add("as726X");			break;
				#endif
				#if is_as7265X == 1		
					case 0x49:
						msg.add("as7265X");			break;
				#endif

				default:
					msg.add("unknown"); 			break;
			} // of switch
		} // of if
	}

	bndl->add(msg);
}



// --- MEASURE TCA9548a ---
// 
// Calls measure_<sensor> of any sensors currently plugged in
//
// void measure_tca9548a()
// {
// 	LOOM_DEBUG_Println("Measuring data from devices connected to tca9548a (Multiplexer).");

// 	for (int i = 0; i < MUX_PORTS; i++) {
// 		if (state_tca9548a.devices[i] != 0) {
// 			tcaseselect(i);
// 			measure_sensor_data(state_tca9548a.devices[i]);
// 		}
// 	}

// }



// --- PACKAGE TCA9548A ---
//
// Calls package functions of any devices plugged into to multiplexer
// Adds each devices msgs to single bundle of mulitplexer sensor readings
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//
void package_tca9548a(OSCBundle *bndl, char packet_header_string[])
{
	// LOOM_DEBUG_Println("Packaging data from devices connected to tca9548a (Multiplexer).");
	
	// LOOM_DEBUG_Println2("Update period: ", state_tca9548a.mux_update_period);
	

	if (  (state_tca9548a.mux_update_period > 0) && (millis()-state_tca9548a.last_update_time > state_tca9548a.mux_update_period)  ){
		update_sensors();
		LOOM_DEBUG_Println("Update MuxShield Sensorlist");
		state_tca9548a.last_update_time = millis();
	}	

	for (int i = 0; i < MUX_PORTS; i++) {
		if (state_tca9548a.devices[i] != 0) {
			tcaseselect(i);
			measure_sensor_data(state_tca9548a.devices[i]);
			package_sensor_data(state_tca9548a.devices[i], bndl, packet_header_string, i);
		}
	}
}




void setup_mux_sensors()
{
	LOOM_DEBUG_Println("Setting up sensors");
		
	for (int i = 0; i < MUX_PORTS; i++) {
		
		if ((state_tca9548a.devices[i] == 0) || (previous_devices[i] == state_tca9548a.devices[i])) {
			continue; // No need to setup a device that hasn't been added / changed ports
		}


		tcaseselect(i);

		switch (state_tca9548a.devices[i]) {
			#if is_tsl2561 == 1 && is_tsl2591 != 1
				case 0x29: 
			#endif
			#if is_tsl2561 == 1
				case 0x39: case 0x49:   // Note that the 0x29 conflicts with the tsl2591 below
					setup_tsl2561();
					break;
			#endif
			#if is_tsl2591 == 1
				case 0x29: 
					setup_tsl2591();
					break;
			#endif
			#if is_fxos8700 == 1
				case 0x1C: case 0x1D: case 0x1E: case 0x1F:
					setup_fxos8700();
					break;
			#endif
			#if is_fxas21002 == 1	
				case 0x20: case 0x21:
					setup_fxas21002();
					break;
			#endif
			#if is_zxgesturesensor == 1
				case 0x10: case 0x11:
					setup_zxgesturesensor();
					break;
			#endif
			#if is_sht31d == 1		
				case 0x44: case 0x45:
					setup_sht31d();
					break;
			#endif
			#if is_mb1232 == 1		
				case 0x70:
					setup_mb1232();
					break;
			#endif
			#if is_ms5803 == 1		
				case 0x76: case 0x77:
					setup_ms5803();
					break;
			#endif
			#if is_lis3dh == 1		
				case 0x19:
					setup_lis3dh();
					break;
			#endif
			#if is_as726X == 1		
				case 0x49:
					setup_as726X();
					break;
			#endif
			#if is_as7265X == 1		
				case 0x49:
					setup_as7265X();
					break;
			#endif
		} // of switch
	} // of for
}







