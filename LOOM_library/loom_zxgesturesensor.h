// ================================================================ 
// ===                ACCESSSING SENSOR STRUCTS                 === 
// ================================================================

// state_zxgesturesensor_0x10.pos[0] (px) 
// state_zxgesturesensor_0x10.pos[1] (pz) 

// state_zxgesturesensor_0x11.pos[0] (px) 
// state_zxgesturesensor_0x11.pos[1] (pz) 


// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <ZX_Sensor.h>

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
// #define i2c_addr_zxgesturesensor 0x10 		 //0x10, 0x11

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 

struct state_zxgesturesensor_t {
	//Cannot declare a ZX_Sensor and not initialize
	GestureType gesture;
	String gesture_type;
	uint8_t gesture_speed;
	int pos[2];
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
#if i2c_addr_zxgesturesensor_0x10 == 1
	ZX_Sensor inst_zxgesturesensor_0x10 = ZX_Sensor(0x10);
	struct state_zxgesturesensor_t state_zxgesturesensor_0x10;
#endif
#if i2c_addr_zxgesturesensor_0x11 == 1
	ZX_Sensor inst_zxgesturesensor_0x11 = ZX_Sensor(0x11);
	struct state_zxgesturesensor_t state_zxgesturesensor_0x11;
#endif

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_zxgesturesensor();
void package_zxgesturesensor(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_zxgesturesensor(OSCBundle *bndl, char packet_header_string[]);
void measure_zxgesturesensor();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any ZXgesturesensor setup and initialization
// 
// @return Whether or not setup was successful
//
bool setup_zxgesturesensor() 
{
	bool is_setup;
	uint8_t ver;
	

	#if i2c_addr_zxgesturesensor_0x10 == 1
		if (inst_zxgesturesensor_0x10.init()) {
			is_setup = true;
			LOOM_DEBUG_Println("Initialized zxgesturesensor");
		} else {
			is_setup = false;
			LOOM_DEBUG_Println("Failed to initialize zxgesturesensor");
		}
		
		ver = inst_zxgesturesensor_0x10.getModelVersion();
		if (ver != ZX_MODEL_VER) {
			is_setup = false;
			LOOM_DEBUG_Println("Incorrect Model Version or unable to read Model Version.");
		} else {
			LOOM_DEBUG_Println2("Model Version: ", ver);
		}
		
		// Read the register map version and ensure the library will work
		ver = inst_zxgesturesensor_0x10.getRegMapVersion();
		if (ver != ZX_REG_MAP_VER) {
			is_setup = false;
			LOOM_DEBUG_Println("Incorrect Register Map Version or unable to read Register Map Version.");
		} else {
			LOOM_DEBUG_Println2("Register Map Version: ", ver);
		}
	#endif

	#if i2c_addr_zxgesturesensor_0x11 == 1
		if (inst_zxgesturesensor_0x11.init()) {
			is_setup = true;
			LOOM_DEBUG_Println("Initialized zxgesturesensor");
		} else {
			is_setup = false;
			LOOM_DEBUG_Println("Failed to initialize zxgesturesensor");
		}
		
		ver = inst_zxgesturesensor_0x11.getModelVersion();
		if (ver != ZX_MODEL_VER) {
			is_setup = false;
			LOOM_DEBUG_Println("Incorrect Model Version or unable to read Model Version.");
		} else {
			LOOM_DEBUG_Println2("Model Version: ", ver);
		}
		
		// Read the register map version and ensure the library will work
		ver = inst_zxgesturesensor_0x11.getRegMapVersion();
		if (ver != ZX_REG_MAP_VER) {
			is_setup = false;
			LOOM_DEBUG_Println("Incorrect Register Map Version or unable to read Register Map Version.");
		} else {
			LOOM_DEBUG_Println2("Register Map Version: ", ver);
		}
	#endif

	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE ZXGESTURESENSOR ---
//
// Adds OSC Message of last read ZXGESTURESENSOR sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_zxgesturesensor(OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/zxgesturesensor/data");
	
	OSCMessage msg = OSCMessage(address_string);
	#if i2c_addr_zxgesturesensor_0x10 == 1
		msg.add("px").add((int32_t)state_zxgesturesensor_0x10.pos[0]);
		msg.add("pz").add((int32_t)state_zxgesturesensor_0x10.pos[1]);

		// msg.add("type" ).add(state_zxgesturesensor_0x10.gesture_type.c_str());
		// msg.add("speed").add((int32_t)state_zxgesturesensor_0x10.gesture_speed);
	#endif

	#if i2c_addr_zxgesturesensor_0x11 == 1
		msg.add("px").add((int32_t)state_zxgesturesensor_0x11.pos[0]);
		msg.add("pz").add((int32_t)state_zxgesturesensor_0x11.pos[1]);
	#endif
	// msg.add("type" ).add(state_zxgesturesensor_0x10.gesture_type.c_str());
	// msg.add("speed").add((int32_t)state_zxgesturesensor_0x10.gesture_speed);
	
	bndl->add(msg);
}

#if is_multiplexer != 1
void package_zxgesturesensor(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	#if i2c_addr_zxgesturesensor_0x10 == 1
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", zxgesturesensor_0x10_name, "_px");
		bndl->add(address_string).add((int32_t)state_zxgesturesensor_0x10.pos[0]);
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", zxgesturesensor_0x10_name, "_pz");
		bndl->add(address_string).add((int32_t)state_zxgesturesensor_0x10.pos[1]);
	#endif


	#if i2c_addr_zxgesturesensor_0x11 == 1
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", zxgesturesensor_0x11_name, "_px");
		bndl->add(address_string).add((int32_t)state_zxgesturesensor_0x11.pos[0]);
		sprintf(address_string, "%s%s%s%s", packet_header_string, "/", zxgesturesensor_0x11_name, "_pz");
		bndl->add(address_string).add((int32_t)state_zxgesturesensor_0x11.pos[1]);
	#endif

	// sprintf(address_string, "%s%s", packet_header_string, "/zxgesture_type");
	// bndl->add(address_string).add(state_zxgesturesensor_0x10.gesture_type.c_str());
	// sprintf(address_string, "%s%s", packet_header_string, "/zxgesture_speed");
	// bndl->add(address_string).add((int32_t)state_zxgesturesensor_0x10.gesture_speed);

}
#endif


// --- MEASURE ZXGESTURESENSOR ---
//
// Gets the current sensor readings of the ZXGESTURESENSOR and stores into its state struct
// 
void measure_zxgesturesensor() 
{
	uint8_t x, z;
	
	#if i2c_addr_zxgesturesensor_0x10 == 1

		if (inst_zxgesturesensor_0x10.positionAvailable()) {
			x = inst_zxgesturesensor_0x10.readX();
			z = inst_zxgesturesensor_0x10.readZ();
			
			if((x != ZX_ERROR) && (z != ZX_ERROR)) {
				state_zxgesturesensor_0x10.pos[0] = x;
				state_zxgesturesensor_0x10.pos[1] = z;
				LOOM_DEBUG_Println2("zxgesturesensor X: ", state_zxgesturesensor_0x10.pos[0]);
				LOOM_DEBUG_Println2("zxgesturesensor Z: ", state_zxgesturesensor_0x10.pos[1]);
			} else {
				LOOM_DEBUG_Println("Error occurred while reading position data");
			}	
		} else {
			LOOM_DEBUG_Println("Position data unavailable for zxgesturesensor");
			
			// Send 255 to indicate that no object is detected
			state_zxgesturesensor_0x10.pos[0] = 255; 
			state_zxgesturesensor_0x10.pos[1] = 255;
		}
	#endif

	#if i2c_addr_zxgesturesensor_0x11 == 1

		if (inst_zxgesturesensor_0x11.positionAvailable()) {
			x = inst_zxgesturesensor_0x11.readX();
			z = inst_zxgesturesensor_0x11.readZ();
			
			if((x != ZX_ERROR) && (z != ZX_ERROR)) {
				state_zxgesturesensor_0x11.pos[0] = x;
				state_zxgesturesensor_0x11.pos[1] = z;
				LOOM_DEBUG_Println2("zxgesturesensor X: ", state_zxgesturesensor_0x11.pos[0]);
				LOOM_DEBUG_Println2("zxgesturesensor Z: ", state_zxgesturesensor_0x11.pos[1]);
			} else {
				LOOM_DEBUG_Println("Error occurred while reading position data");
			}	
		} else {
			LOOM_DEBUG_Println("Position data unavailable for zxgesturesensor");
			
			// Send 255 to indicate that no object is detected
			state_zxgesturesensor_0x11.pos[0] = 255; 
			state_zxgesturesensor_0x11.pos[1] = 255;
		}
	#endif


// Commented out because type and speed are can only be used if px and pz are not used
// Gesture data is harder to get anyways, as too long of delay between polling the sensor
// will miss the gesture 

	// if (inst_zxgesturesensor_0x10.gestureAvailable()) {
	// 	state_zxgesturesensor_0x10.gesture       = inst_zxgesturesensor_0x10.readGesture();
	// 	state_zxgesturesensor_0x10.gesture_speed = inst_zxgesturesensor_0x10.readGestureSpeed();
		
	// 	switch (state_zxgesturesensor_0x10.gesture) {
	// 		case RIGHT_SWIPE:
	// 			state_zxgesturesensor_0x10.gesture_type = "Right Swipe";
	// 			break;
	// 		case LEFT_SWIPE:
	// 			state_zxgesturesensor_0x10.gesture_type = "Left Swipe";
	// 			break;
	// 		case UP_SWIPE:
	// 			state_zxgesturesensor_0x10.gesture_type = "Up Swipe";
	// 		case NO_GESTURE: default:
	// 			state_zxgesturesensor_0x10.gesture_type = "No Gesture";
	// 			break;
	// 	}

	// 	#if LOOM_DEBUG == 1
	// 		LOOM_DEBUG_Println2("Gesture type: ", state_zxgesturesensor_0x10.gesture_type);
	// 		LOOM_DEBUG_Print("Gesture speed: ");
	// 		Serial.println(state_zxgesturesensor_0x10.gesture_speed, DEC);
	// 	#endif

	// } else {
	// 	LOOM_DEBUG_Println("Gesture data unavailable for zxgesturesensor");
	// 	state_zxgesturesensor_0x10.gesture_type  = "No Gesture";
	// 	state_zxgesturesensor_0x10.gesture_speed = 0;
	// }



}






