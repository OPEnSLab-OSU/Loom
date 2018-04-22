// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

// ================================================================ 
// ===                        DEFINITIONS                       === 
// ================================================================ 
#define i2c_addr_tsl2591 0x29

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 

struct config_tsl2591_t {
	uint8_t gain_level;
	uint8_t timing_level;	
};

struct state_tsl2591_t {
	Adafruit_TSL2591 inst_tsl2591;
	float lux;
	uint32_t vis;
	uint16_t ir;
	uint16_t full;
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_tsl2591_t config_tsl2591;
struct state_tsl2591_t state_tsl2591;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_tsl2591();
void package_data_tsl2591(OSCBundle *, char[]);
void measure_tsl2591();
void configure_tsl2591(uint8_t, uint8_t);

#if DEBUG == 1
	void details_tsl2591();
#endif

// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
bool setup_tsl2591() {
	bool is_setup;
	state_tsl2591.inst_tsl2591 = Adafruit_TSL2591(2591);
	if(state_tsl2591.inst_tsl2591.begin()) {
		is_setup = true;
		#if DEBUG == 1
			Serial.println("Initialized tsl2591");
		#endif
		configure_tsl2591(1, 2); //Medium gain and timing
	}
	else {
		is_setup = false;
		#if DEBUG == 1
			Serial.println("Failed to initialize tsl2591");
		#endif
	}
	
	return is_setup;
}

// ================================================================ 
// ===                        FUNCTION DECLARATIONS             === 
// ================================================================
void package_data_tsl2591(OSCBundle *bndl, char packet_header_string[]) {
	//Create a message and fill it here, then add it to the bndl
}

void measure_tsl2591() {
	uint32_t lum = state_tsl2591.inst_tsl2591.getFullLuminosity();
	state_tsl2591.ir = lum >> 16;
	state_tsl2591.full = lum & 0xFFFF;
	state_tsl2591.vis = state_tsl2591.full - state_tsl2591.ir;
	state_tsl2591.lux = state_tsl2591.inst_tsl2591.calculateLux(state_tsl2591.full, state_tsl2591.ir);
	
	#if DEBUG ==1 
		Serial.print(F("[ ")); Serial.print(millis()); Serial.print(F(" ms ] "));
		Serial.print(F("IR: ")); Serial.print(state_tsl2591.ir);  Serial.print(F("  "));
		Serial.print(F("Full: ")); Serial.print(state_tsl2591.full); Serial.print(F("  "));
		Serial.print(F("Visible: ")); Serial.print(state_tsl2591.vis); Serial.print(F("  "));
		Serial.print(F("Lux: ")); Serial.println(state_tsl2591.lux, 6);
	#endif
}

void configure_tsl2591(uint8_t gain_level, uint8_t timing_level) {
	switch(gain_level) {
		case 0:
			state_tsl2591.inst_tsl2591.setGain(TSL2591_GAIN_LOW);    // 1x gain (bright light)
			break;
		case 1:
			state_tsl2591.inst_tsl2591.setGain(TSL2591_GAIN_MED);    // 25x gain
			break;
		case 2:
			state_tsl2591.inst_tsl2591.setGain(TSL2591_GAIN_HIGH);   // 428x gain
			break;
		case 3:
			state_tsl2591.inst_tsl2591.setGain(TSL2591_GAIN_MAX);   // 9876x gain
			break;
		default:
			#if DEBUG == 1
				Serial.println("Invalid gain level.");
			#endif
			break;
	}
  
	switch(timing_level) {
		case 0:
			state_tsl2591.inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_100MS);  // shortest integration time (bright light)
			break;
		case 1:
			state_tsl2591.inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_200MS);
			break;
		case 2:
			state_tsl2591.inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_300MS);
			break;
		case 3:
			state_tsl2591.inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_400MS);
			break;
		case 4:
			state_tsl2591.inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_500MS);
			break;
		case 5:
			state_tsl2591.inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light)
			break;
		default:
			#if DEBUG == 1
				Serial.println("Invalid timing level");
			#endif
			break;
	}
	
	#if DEBUG == 1
		/* Display the gain and integration time for reference sake */  
		Serial.println(F("------------------------------------"));
		Serial.print  (F("Gain:         "));
		tsl2591Gain_t gain = state_tsl2591.inst_tsl2591.getGain();
		switch(gain)
		{
			case TSL2591_GAIN_LOW:
				Serial.println(F("1x (Low)"));
				break;
			case TSL2591_GAIN_MED:
				Serial.println(F("25x (Medium)"));
				break;
			case TSL2591_GAIN_HIGH:
				Serial.println(F("428x (High)"));
				break;
			case TSL2591_GAIN_MAX:
				Serial.println(F("9876x (Max)"));
				break;
		}
		Serial.print  (F("Timing:       "));
		Serial.print((state_tsl2591.inst_tsl2591.getTiming() + 1) * 100, DEC); 
		Serial.println(F(" ms"));
		Serial.println(F("------------------------------------"));
		Serial.println(F(""));
	#endif //if DEBUG == 1
}

#if DEBUG == 1
	void details_tsl2591() {
		sensor_t sensor;
		state_tsl2591.inst_tsl2591.getSensor(&sensor);
		Serial.println(F("------------------------------------"));
		Serial.print  (F("Sensor:       ")); Serial.println(sensor.name);
		Serial.print  (F("Driver Ver:   ")); Serial.println(sensor.version);
		Serial.print  (F("Unique ID:    ")); Serial.println(sensor.sensor_id);
		Serial.print  (F("Max Value:    ")); Serial.print(sensor.max_value); Serial.println(F(" lux"));
		Serial.print  (F("Min Value:    ")); Serial.print(sensor.min_value); Serial.println(F(" lux"));
		Serial.print  (F("Resolution:   ")); Serial.print(sensor.resolution, 4); Serial.println(F(" lux"));  
		Serial.println(F("------------------------------------"));
		Serial.println(F(""));
		delay(500);
	}
#endif