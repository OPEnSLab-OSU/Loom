// ================================================================ 
// ===                ACCESSSING SENSOR STRUCTS                 === 
// ================================================================

// state_tsl2591.vis
// state_tsl2591.ir
// state_tsl2591.full


// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"

// ================================================================ 
// ===                        DEFINITIONS                       === 
// ================================================================ 
// #define i2c_addr_tsl2591 0x29

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 

struct config_tsl2591_t {
	uint8_t gain_level;
	uint8_t timing_level;	
};

struct state_tsl2591_t {
	Adafruit_TSL2591 inst_tsl2591;
	uint16_t vis;
	uint16_t ir;
	uint16_t full;
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_tsl2591_t config_tsl2591;
struct state_tsl2591_t  state_tsl2591;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_tsl2591();
void package_data_tsl2591(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_tsl2591(OSCBundle *bndl, char packet_header_string[]);
void measure_tsl2591();
void configure_tsl2591(uint8_t, uint8_t);
#if LOOM_DEBUG == 1
	void details_tsl2591();
#endif

// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
// 
// Runs any TSL2591 setup and initialization
//
// @return Whether or not setup was successful
//
bool setup_tsl2591() 
{
	bool is_setup;
	state_tsl2591.inst_tsl2591 = Adafruit_TSL2591(2591);
	if(state_tsl2591.inst_tsl2591.begin()) {
		is_setup = true;
		LOOM_DEBUG_Println("Initialized tsl2591");
		configure_tsl2591(1, 0); //Medium gain and timing
	} else {
		is_setup = false;
		LOOM_DEBUG_Println("Failed to initialize tsl2591");
	}

	return is_setup;
}

// ================================================================ 
// ===                        FUNCTION DECLARATIONS             === 
// ================================================================


// --- PACKAGE TSL2591 ---
//
// Adds an OSC Message of last read TSL2591 sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_tsl2591(OSCBundle *bndl, char packet_header_string[], uint8_t port) 
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/tsl2591/data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("vis" ).add((int32_t)state_tsl2591.vis);
	msg.add("ir"  ).add((int32_t)state_tsl2591.ir);
	msg.add("full").add((int32_t)state_tsl2591.full);
	
	bndl->add(msg);
}

#if is_multiplexer != 1
void package_tsl2591(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	sprintf(address_string, "%s%s%s%s", packet_header_string, "/", tsl2591_0x29_name, "_vis");
	bndl->add(address_string).add((int32_t)state_tsl2591.vis);
	sprintf(address_string, "%s%s%s%s", packet_header_string, "/", tsl2591_0x29_name, "_ir");
	bndl->add(address_string).add((int32_t)state_tsl2591.ir);
	sprintf(address_string, "%s%s%s%s", packet_header_string, "/", tsl2591_0x29_name, "_full");
	bndl->add(address_string).add((int32_t)state_tsl2591.full);
}
#endif

// --- MEASURE TSL2591 ---
//
// Gets the current sensor readings of the Tsl2591 and stores into its state struct
//
void measure_tsl2591() 
{
	state_tsl2591.ir   = state_tsl2591.inst_tsl2591.getLuminosity(TSL2591_INFRARED);
	state_tsl2591.full = state_tsl2591.inst_tsl2591.getLuminosity(TSL2591_FULLSPECTRUM);
	state_tsl2591.vis  =  state_tsl2591.inst_tsl2591.getLuminosity(TSL2591_VISIBLE);
	
	#if LOOM_DEBUG ==1 
		Serial.print(F("[ "));			Serial.print(millis());				Serial.print(F(" ms ] "));
		Serial.print(F("IR: "));		Serial.print(state_tsl2591.ir);		Serial.print(F("  "));
		Serial.print(F("Full: "));		Serial.print(state_tsl2591.full);	Serial.print(F("  "));
		Serial.print(F("Visible: "));	Serial.print(state_tsl2591.vis);	Serial.println(F("  "));
	#endif
}



// --- CONFIGURE TSL2591 ---
//
// 
//
// @param gain_level    
// @param timing_level  
//
void configure_tsl2591(uint8_t gain_level, uint8_t timing_level) 
{
	switch(gain_level) {
		case 0:
			state_tsl2591.inst_tsl2591.setGain(TSL2591_GAIN_LOW);  break;   // 1x gain (bright light)
		case 1:
			state_tsl2591.inst_tsl2591.setGain(TSL2591_GAIN_MED);  break;   // 25x gain
		case 2:
			state_tsl2591.inst_tsl2591.setGain(TSL2591_GAIN_HIGH); break;  // 428x gain
		case 3:
			state_tsl2591.inst_tsl2591.setGain(TSL2591_GAIN_MAX);  break;  // 9876x gain
		default:
			LOOM_DEBUG_Println("Invalid gain level."); break;
	}
  
	switch(timing_level) {
		case 0:
			state_tsl2591.inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_100MS); break; // shortest integration time (bright light)
		case 1:
			state_tsl2591.inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_200MS); break;
		case 2:
			state_tsl2591.inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_300MS); break;
		case 3:
			state_tsl2591.inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_400MS); break;
		case 4:
			state_tsl2591.inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_500MS); break;
		case 5:
			state_tsl2591.inst_tsl2591.setTiming(TSL2591_INTEGRATIONTIME_600MS);  // longest integration time (dim light) break;
		default:
			LOOM_DEBUG_Println("Invalid timing level"); break;
	}
	
	#if LOOM_DEBUG == 1
		/* Display the gain and integration time for reference sake */  
		Serial.println(F("------------------------------------"));
		Serial.print  (F("Gain:         "));
		tsl2591Gain_t gain = state_tsl2591.inst_tsl2591.getGain();
		switch(gain)
		{
			case TSL2591_GAIN_LOW:
				Serial.println(F("1x (Low)")); 		break;
			case TSL2591_GAIN_MED:
				Serial.println(F("25x (Medium)")); 	break;
			case TSL2591_GAIN_HIGH:
				Serial.println(F("428x (High)")); 	break;
			case TSL2591_GAIN_MAX:
				Serial.println(F("9876x (Max)")); 	break;
		}
		Serial.print  (F("Timing:       "));
		Serial.print((state_tsl2591.inst_tsl2591.getTiming() + 1) * 100, DEC); 
		Serial.println(F(" ms"));
		Serial.println(F("------------------------------------"));
		Serial.println(F(""));
	#endif //if LOOM_DEBUG == 1
}



// --- DETAILS TSL2591 ---
//
// With Loom debug enabled, allows for the printing of the TSl2591 details to be 
// printed to the Serial monitor
// 
#if LOOM_DEBUG == 1
void details_tsl2591() 
{
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





