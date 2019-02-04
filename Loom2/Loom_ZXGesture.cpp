
#include "Loom_ZXGesture.h"






// --- CONSTRUCTOR ---
Loom_ZXGesture::Loom_ZXGesture(byte i2c_address, char* module_name, char* sensor_description, ZXMode mode)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
	this->mode = mode;


	inst_ZX = new ZX_Sensor(i2c_address);

	bool setup = inst_ZX->init();
	uint8_t ver;


	ver = inst_ZX->getModelVersion();
	print_module_label();
	if (ver != ZX_MODEL_VER) {
		is_setup = false;
		LOOM_DEBUG_Println("Incorrect Model Version or unable to read Model Version.");
	} else {
		LOOM_DEBUG_Println2("Model Version: ", ver);
	}
	
	// Read the register map version and ensure the library will work
	ver = inst_ZX->getRegMapVersion();
	print_module_label();
	if (ver != ZX_REG_MAP_VER) {
		is_setup = false;
		LOOM_DEBUG_Println("Incorrect Register Map Version or unable to read Register Map Version.");
	} else {
		LOOM_DEBUG_Println2("Register Map Version: ", ver);
	}


	print_module_label();
	LOOM_DEBUG_Println2("\tInitialize ", (setup) ? "sucessful" : "failed");
}



// --- DESTRUCTOR ---
Loom_ZXGesture::~Loom_ZXGesture() 
{

}


void Loom_ZXGesture::print_config()
{
	LoomI2CSensor::print_config();
	LOOM_DEBUG_Println3('\t', "Mode                : ", (mode == ZX_POS) ? "Position" : "Gesture" );
}


void Loom_ZXGesture::print_measurements()
{
	print_module_label();
	LOOM_DEBUG_Println("Measurements:");

	switch (mode) {
		case ZX_POS : 
			LOOM_DEBUG_Println3("\t", "ZX: ", pos[0]);
			LOOM_DEBUG_Println3("\t", "ZY: ", pos[1]);
			break;
		case ZX_GEST : 
			LOOM_DEBUG_Println3("\t", "Gesture type : ", gesture_type);
			LOOM_DEBUG_Println3("\t", "Gesture speed: ", gesture_speed);
			break; 
	}
}


void Loom_ZXGesture::measure()
{
	switch (mode) {
		case ZX_POS : 
			if ( inst_ZX.positionAvailable() ) {
				x = inst_ZX.readX();
				z = inst_ZX.readZ();
				
				if ( (x != ZX_ERROR) && (z != ZX_ERROR) ) {
					pos[0] = x;
					pos[1] = z;
					// LOOM_DEBUG_Println2("zxgesturesensor X: ", pos[0]);
					// LOOM_DEBUG_Println2("zxgesturesensor Z: ", pos[1]);
				} else {
					print_module_label()
					LOOM_DEBUG_Println("Error occurred while reading position data");
				}	
			} else {
				// LOOM_DEBUG_Println("Position data unavailable for zxgesturesensor");
				
				// Send 255 to indicate that no object is detected
				pos[0] = 255; 
				pos[1] = 255;
			}			
			break;
		

		case ZX_GEST :
			if ( inst_ZX.gestureAvailable() ) {
				gesture       = inst_ZX.readGesture();
				gesture_speed = inst_ZX.readGestureSpeed();
				
				switch (gesture) {
					case RIGHT_SWIPE : gesture_type = "Right Swipe"; break;
					case LEFT_SWIPE  : gesture_type = "Left Swipe";  break;
					case UP_SWIPE    : gesture_type = "Up Swipe";    break;
					
					case NO_GESTURE  : 
					default: gesture_type = "No Gesture"; break;
				}

			} else {
				// LOOM_DEBUG_Println("Gesture data unavailable for zxgesturesensor");
				gesture_type  = "No Gesture";
				gesture_speed = 0;
			}
			break; 
	}
}


void Loom_ZXGesture::package(OSCBundle* bndl)
{
	char id_prefix[30]; 
	resolve_package_prefix(id_prefix);

	switch (mode) {
		case ZX_POS : 
			append_to_bundle_key_value(bndl, id_prefix, "zx", pos[0]);
			append_to_bundle_key_value(bndl, id_prefix, "zy", pos[1]);
			break;
		case ZX_GEST : 
			append_to_bundle_key_value(bndl, id_prefix, "type", gesture_type);
			append_to_bundle_key_value(bndl, id_prefix, "speed", gesture_speed);
			break; 
	}
}


void Loom_ZXGesture::package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port)
{
	LoomI2CSensor::package_mux(bndl, id_prefix, port);

	switch (mode) {
		case ZX_POS : 
			append_to_bundle_key_value(bndl, id_prefix, "zx", pos[0]);
			append_to_bundle_key_value(bndl, id_prefix, "zy", pos[1]);
			break;
		case ZX_GEST : 
			append_to_bundle_key_value(bndl, id_prefix, "type", gesture_type);
			append_to_bundle_key_value(bndl, id_prefix, "speed", gesture_speed);
			break; 
	}
}
