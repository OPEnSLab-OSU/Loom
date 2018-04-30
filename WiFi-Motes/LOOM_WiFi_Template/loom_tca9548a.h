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
void package_data_tca9548a(OSCBundle *, char[]);
void get_sensor_data(uint8_t,OSCBundle *, char[], uint8_t);
void measure_tca9548a();
void tcaseselect(uint8_t);
void get_sensors(OSCBundle *, char[]);

	
// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
bool setup_tca9548a() {
  delay(2000);
  Wire.begin();
	#if LOOM_DEBUG == 1
		Serial.println("initialized tca9548a (multiplexer).");
	#endif
}



// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================

void tcaseselect(uint8_t port_num) {
  if (port_num < 8) {
		Wire.beginTransmission(i2c_addr_tca9548a);
		Wire.write(1 << port_num);
		Wire.endTransmission();  
	}
}

void package_data_tca9548a(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void get_sensor_data(uint8_t i2c_addr, OSCBundle *bndl, char packet_header_string[], uint8_t port){
  #if LOOM_DEBUG == 1
		Serial.print("Attempting to measure data from sensor with address: ");
		Serial.println(i2c_addr);
	#endif 
	
	#ifdef i2c_addr_tsl2591
		if(i2c_addr == 0x29){
			if (setup_tsl2591()) {
				measure_tsl2591();
        package_tsl2591(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif
	#ifdef i2c_addr_fxos8700
		if((i2c_addr == 0x1C) || (i2c_addr == 0x1D) || (i2c_addr == 0x1E) || (i2c_addr == 0x1F)){
			if (setup_fxos8700()) {
				measure_fxos8700();
        package_fxos8700(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif
	#ifdef i2c_addr_fxas21002
		if((i2c_addr == 0x20) || (i2c_addr == 0x21)){
			if (setup_fxas21002()) {
				measure_fxas21002();
        package_fxas21002(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif
	#ifdef i2c_addr_zxgesturesensor
		if((i2c_addr == 0x10) || (i2c_addr == 0x11)){
			if (setup_zxgesturesensor()) {
				measure_zxgesturesensor();
        package_zxgesturesensor(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif
	#ifdef i2c_addr_sht31d
		if((i2c_addr == 0x44) || (i2c_addr == 0x45)){
			if (setup_sht31d()) {
				measure_sht31d();
        package_sht31d(bndl,packet_header_string,port);
				return;
			} 
		}
	#endif
	#ifdef i2c_addr_mb1232
	if((i2c_addr == 0x70)){
		if (setup_mb1232()) {
			measure_mb1232();
      package_mb1232(bndl,packet_header_string,port);
			return;
		} 
	}
	#endif
	#if LOOM_DEBUG == 1
    if (i2c_addr != 0x00) //sht31d hardware bug
		Serial.println("This sensor is not currently supported by the Project LOOM sytem");
	#endif
}

void send_sensor_list(OSCMessage &msg){
  OSCBundle send_bndl;
  char header_string[255];
  msg.getString(0,header_string,255);
  get_sensors(&send_bndl,header_string);
  #if is_wifi
    wifi_send_bundle(&send_bndl);
    send_bndl.empty();
  #endif
}

void get_sensors(OSCBundle *bndl, char packet_header_string[])
{
  char addressString[255];
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
          if(i2c_addr == 0x29){
            sprintf(addressString,"%s%s%d%s",packet_header_string,"/port",t,"/tsl2591");
            bndl->add(addressString);
          }
          else if (i2c_addr == 0x1C || i2c_addr == 0x1D || i2c_addr == 0x1E || i2c_addr == 0x1F){
            sprintf(addressString,"%s%s%d%s",packet_header_string,"/port",t,"/fxos8700");
            bndl->add(addressString);
          }
          else if (i2c_addr == 0x20 || i2c_addr == 0x21){
            sprintf(addressString,"%s%s%d%s",packet_header_string,"/port",t,"/fxas21002");
            bndl->add(addressString);
          }
          else if (i2c_addr == 0x10 || i2c_addr == 0x11){
            sprintf(addressString,"%s%s%d%s",packet_header_string,"/port",t,"/zxgesturesensor");
            bndl->add(addressString);
          }
          else if (i2c_addr == 0x44 || i2c_addr == 0x45){
            sprintf(addressString,"%s%s%d%s",packet_header_string,"/port",t,"/sht31d");
            bndl->add(addressString);
          }
          else if (i2c_addr == 0x70){
            sprintf(addressString,"%s%s%d%s",packet_header_string,"/port",t,"/mb1232");
            bndl->add(addressString);
          }
          else if (i2c_addr != 0x00){
            sprintf(addressString,"%s%s%d%s",packet_header_string,"/port",t,"/unsupported");
            bndl->add(addressString);
          }
        }
    }
  }
}

void package_tca9548a(OSCBundle *bndl, char packet_header_string[]) {
	#if LOOM_DEBUG == 1
		Serial.println("Measuring data from devices connected to tca9548a (Multiplexer).");
	#endif
  for (uint8_t t=0; t<8; t++) {
     tcaseselect(t);

		for (uint8_t i2c_addr = 0; i2c_addr<=127; i2c_addr++) {
			if (i2c_addr == i2c_addr_tca9548a) 
				continue;
			
			#ifdef is_32u4
				uint8_t data;
				if (! twi_writeTo(i2c_addr, &data, 0, 1, 1)) {
					get_sensor_data(i2c_addr,bndl,packet_header_string,t);
				}
			#endif //is_32u4
			
			#ifdef is_m0
				Wire.beginTransmission(i2c_addr);
				byte error = Wire.endTransmission();

				if (error == 0) {
					get_sensor_data(i2c_addr,bndl,packet_header_string,t);
				}
			#endif // is_m0
		}
		//delay(3000);
	}
}

