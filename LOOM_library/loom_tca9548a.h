// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#ifdef is_32u4
	extern "C" { 
		#include "utility/twi.h"  // from Wire library, so we can do bus scanning
	}
#endif


// ================================================================
// ===                       DEFINITIONS                        ===
// ================================================================

//The i2c address of the tca9548a is 0x70, but can be physically changed to 0x71
//It was necessary to do this because the i2c address of the mb1232 is also
//0x70, and i2c addresses must be unique.
#define i2c_addr_tca9548a 0x71 

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 
struct config_tca9548a_t {

};

struct state_tca9548a_t {
	int measurement_count;
	long last_update_time;
	long mux_update_period;
	byte devices[16][2];
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_tca9548a_t config_tca9548a;
struct state_tca9548a_t state_tca9548a;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_tca9548a();
void package_sensor_data(uint8_t,OSCBundle *, char[], uint8_t);
void measure_tca9548a();
void tcaseselect(uint8_t);
void get_sensors(OSCBundle *, char[]);
void update_sensors();
void measure_tca9548a();
void package_tca9548a(OSCBundle *, char[]);
void send_sensor_list(OSCMessage &);

	
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
	delay(2000);
	Wire.begin();
	LOOM_DEBUG_Println("initialized tca9548a (multiplexer).");
	state_tca9548a.mux_update_period = UPDATE_PERIOD;
	update_sensors();
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
	LOOM_DEBUG_Println2("Attempting to measure data from sensor with address: ", i2c_addr);
	
	#ifdef i2c_addr_tsl2591
		if ( (i2c_addr == 0x29) && setup_tsl2591() ) {
			measure_tsl2591(); return;
		} 
	#endif
	#ifdef i2c_addr_fxos8700
		if ( ((i2c_addr == 0x1C) || (i2c_addr == 0x1D) || (i2c_addr == 0x1E) || (i2c_addr == 0x1F)) && setup_fxos8700() ) {
			measure_fxos8700(); return;
		}
	#endif
	#ifdef i2c_addr_fxas21002
		if (((i2c_addr == 0x20) || (i2c_addr == 0x21)) && setup_fxas21002()) {
			measure_fxas21002(); return;
		}
	#endif
	#ifdef i2c_addr_zxgesturesensor
		if (((i2c_addr == 0x10) || (i2c_addr == 0x11)) && setup_zxgesturesensor()) {
			measure_zxgesturesensor(); return;
		}
	#endif
	#ifdef i2c_addr_sht31d
		if (((i2c_addr == 0x44) || (i2c_addr == 0x45)) && setup_sht31d()) {
			measure_sht31d(); return;
		}
	#endif
	#ifdef i2c_addr_mb1232
		if (((i2c_addr == 0x70)) && setup_mb1232()) {
			measure_mb1232(); return;
		}
	#endif
	#ifdef i2c_addr_mpu6050
		if (((i2c_addr == 0x68) || (i2c_addr == 0x69)) && setup_mpu6050()) {
			measure_mpu6050(); return;
		}
	#endif
	
	#ifdef i2c_addr_ms5803
		if ((i2c_addr == 0x77)  && setup_ms5803()) {
			measure_ms5803(); return;
		}
	#endif

	#ifdef i2c_addr_lis3dh
		if ((i2c_addr == 0x19)  && setup_lis3dh()) {
			measure_lis3dh(); return;
		}
	#endif

	#if LOOM_DEBUG == 1
		if (i2c_addr != 0x00) //sht31d hardware bug
			Serial.println("This sensor is not currently supported by the Loom sytem");
	#endif
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
	LOOM_DEBUG_Println2("Attempting to measure data from sensor with address: ", i2c_addr);
	
	#ifdef i2c_addr_tsl2591
		if (i2c_addr == 0x29){
			if (setup_tsl2591()) {
				measure_tsl2591();
				package_tsl2591(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif
	#ifdef i2c_addr_fxos8700
		if ((i2c_addr == 0x1C) || (i2c_addr == 0x1D) || (i2c_addr == 0x1E) || (i2c_addr == 0x1F)){
			if (setup_fxos8700()) {
				measure_fxos8700();
				package_fxos8700(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif
	#ifdef i2c_addr_fxas21002
		if ((i2c_addr == 0x20) || (i2c_addr == 0x21)){
			if (setup_fxas21002()) {
				measure_fxas21002();
				package_fxas21002(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif
	#ifdef i2c_addr_zxgesturesensor
		if ((i2c_addr == 0x10) || (i2c_addr == 0x11)){
			if (setup_zxgesturesensor()) {
				measure_zxgesturesensor();
				package_zxgesturesensor(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif
	#ifdef i2c_addr_sht31d
		if ((i2c_addr == 0x44) || (i2c_addr == 0x45)){
			if (setup_sht31d()) {
				measure_sht31d();
				package_sht31d(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif
	#ifdef i2c_addr_mb1232
		if ((i2c_addr == 0x70)){
			if (setup_mb1232()) {
				measure_mb1232();
				package_mb1232(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif
	#ifdef i2c_addr_mpu6050
		if ((i2c_addr == 0x68 || i2c_addr == 0x69)){
			if (setup_mpu6050()) {
				measure_mpu6050();
				package_mpu6050(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif

	#ifdef i2c_addr_ms5803
		if (i2c_addr == 0x77){
			if (setup_ms5803()) {
				measure_ms5803();
				package_ms5803(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif

	#ifdef i2c_addr_lis3dh
		if (i2c_addr == 0x19){
			if (setup_lis3dh()) {
				measure_lis3dh();
				package_lis3dh(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif

	#if LOOM_DEBUG == 1
		if (i2c_addr != 0x00) //sht31d hardware bug
			Serial.println("This sensor is not currently supported by the Project LOOM sytem");
	#endif
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
	msg.getString(0,header_string,255);
	get_sensors(&send_bndl,header_string);

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
	uint8_t current_ind = 0;
	for (uint8_t t=0; t<8; t++){
		tcaseselect(t);
		for (uint8_t i2c_addr = 0; i2c_addr<=127; i2c_addr++) {
			if (i2c_addr == i2c_addr_tca9548a)
				continue;

			#ifdef is_32u4
				uint8_t data;
				if (! twi_writeTo(i2c_addr, &data, 0, 1, 1)) {
			#endif
			#ifdef is_m0
				Wire.beginTransmission(i2c_addr);
				byte error = Wire.endTransmission();

				if (error == 0) {
			#endif
					if (i2c_addr != 0x00){
						state_tca9548a.devices[current_ind][0] = t;
						state_tca9548a.devices[current_ind][1] = i2c_addr;
						current_ind++;
					}
				}
				
		}
		for (uint8_t ind = current_ind; ind < 16; ind++){
				state_tca9548a.devices[ind][0] = 8;
		}
	}
}



// --- GET SENSORS ---
//
// Fills OSC bundle with list of sensors currently plugged into multiplexer
// Calls update_sensors()
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//
void get_sensors(OSCBundle *bndl, char packet_header_string[]) 
{
	char addressString[255];
	update_sensors();

	for (int i = 0; i < 16; i++){
		uint8_t t = state_tca9548a.devices[i][0];
		if (t > 7)
			continue;
		uint8_t i2c_addr = state_tca9548a.devices[i][1];
		if(i2c_addr == 0x29){
			sprintf(addressString,"%s%s",packet_header_string,"/port");
			bndl->add(addressString).add((int32_t)t).add("tsl2591");
		}
		else if (i2c_addr == 0x1C || i2c_addr == 0x1D || i2c_addr == 0x1E || i2c_addr == 0x1F){
			sprintf(addressString,"%s%s%d%s",packet_header_string,"/port");
			bndl->add(addressString).add((int32_t)t).add("fxos8700");
		}
		else if (i2c_addr == 0x20 || i2c_addr == 0x21){
			sprintf(addressString,"%s%s%d%s",packet_header_string,"/port");
			bndl->add(addressString).add((int32_t)t).add("fxas21002");
		}
		else if (i2c_addr == 0x10 || i2c_addr == 0x11){
			sprintf(addressString,"%s%s%d%s",packet_header_string,"/port");
			bndl->add(addressString).add((int32_t)t).add("zxgesturesensor");
		}
		else if (i2c_addr == 0x44 || i2c_addr == 0x45){
			sprintf(addressString,"%s%s%d%s",packet_header_string,"/port");
			bndl->add(addressString).add((int32_t)t).add("sht31d");
		}
		else if (i2c_addr == 0x70){
			sprintf(addressString,"%s%s%d%s",packet_header_string,"/port");
			bndl->add(addressString).add((int32_t)t).add("mb1232");
		}
		else if (i2c_addr == 0x68 || i2c_addr == 0x69){
			sprintf(addressString,"%s%s%d%s",packet_header_string,"/port");
			bndl->add(addressString).add((int32_t)t).add("mpu6050");
		}
		else if (i2c_addr == 0x77){
			sprintf(addressString,"%s%s%d%s",packet_header_string,"/port");
			bndl->add(addressString).add((int32_t)t).add("ms5803");
		}
		else if (i2c_addr == 0x19){
			sprintf(addressString,"%s%s%d%s",packet_header_string,"/port");
			bndl->add(addressString).add((int32_t)t).add("lis3dh");
		}


		else if (i2c_addr != 0x00){
			sprintf(addressString,"%s%s%d%s",packet_header_string,"/port");
			bndl->add(addressString).add((int32_t)t).add("unsupported");;
		}
	}
}



// --- MEASURE TCA9548a ---
// 
// Calls measure_<sensor> of any sensors currently plugged in
//
void measure_tca9548a()
{
	LOOM_DEBUG_Println("Measuring data from devices connected to tca9548a (Multiplexer).");

	for (int device = 0; device < 16; device++) {
		if (state_tca9548a.devices[device][0] <= 7) {
			tcaseselect(state_tca9548a.devices[device][0]);
			measure_sensor_data(state_tca9548a.devices[device][1]);
		}
	}
}



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
	LOOM_DEBUG_Println("Packaging data from devices connected to tca9548a (Multiplexer).");
		
	for (int device = 0; device < 16; device++) {
		if (state_tca9548a.devices[device][0] <= 7) {
			tcaseselect(state_tca9548a.devices[device][0]);
			package_sensor_data(state_tca9548a.devices[device][1],bndl,packet_header_string,state_tca9548a.devices[device][0]);
		}
	}
}










