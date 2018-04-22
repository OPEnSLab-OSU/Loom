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
// ===                        DEFINITIONS                       === 
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
struct config_tca9548a_t *config_tca9548a;
struct state_tca9548a_t *state_tca9548a;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_tca9548a();
void package_data_tca9548a(OSCBundle *, char[]);
void measure_sensor(uint8_t);
void measure_tca9548a();
void tcaseselect(uint8_t);

	
// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
bool setup_tca9548a() {
	#if DEBUG == 1
		Serial.println("Setting up tca9548a (multiplexer).");
	#endif
  delay(2000);
  Wire.begin();
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

void measure_sensor(uint8_t i2c_addr){
  if(i2c_addr == i2c_addr_tsl2591){
    if (setup_tsl2591()) {
			measure_tsl2591();
    } 
  }
	else {
		#if DEBUG == 1
			Serial.println("This sensor is not currently supported by the Project LOOM sytem");
		#endif
	}
}

void measure_tca9548a() {
  for (uint8_t t=0; t<8; t++) {
     tcaseselect(t);

		for (uint8_t i2c_addr = 0; i2c_addr<=127; i2c_addr++) {
			if (i2c_addr == i2c_addr_tca9548a) 
				continue;
			
			#ifdef is_32u4
				uint8_t data;
				if (! twi_writeTo(i2c_addr, &data, 0, 1, 1)) {
					measure_sensor(i2c_addr);
				}
			#endif //is_32u4
			
			#ifdef is_m0
				Wire.beginTransmission(i2c_addr);
				byte error = Wire.endTransmission();

				if (error == 0) {
					measure_sensor(i2c_addr);
				}
			#endif // is_m0
		}
	}
}

