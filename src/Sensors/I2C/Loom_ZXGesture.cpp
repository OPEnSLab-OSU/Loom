
#include "Loom_ZXGesture.h"


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_ZXGesture::Loom_ZXGesture(
		byte			i2c_address, 
		const char*		module_name, 
		ZXMode			mode
	)
	: LoomI2CSensor( module_name, i2c_address )
{
	this->module_type = ModuleType::ZXGesture;

	this->mode = mode;

	inst_ZX = new ZX_Sensor(i2c_address);

	bool setup = inst_ZX->init();
	uint8_t ver;


	ver = inst_ZX->getModelVersion();
	print_module_label();
	if (ver != ZX_MODEL_VER) {
		setup = false;
		Println("Incorrect Model Version or unable to read Model Version.");
	} else {
		Println2("Model Version: ", ver);
	}
	
	// Read the register map version and ensure the library will work
	ver = inst_ZX->getRegMapVersion();
	print_module_label();
	if (ver != ZX_REG_MAP_VER) {
		setup = false;
		Println("Incorrect Register Map Version or unable to read Register Map Version.");
	} else {
		Println2("Register Map Version: ", ver);
	}

	print_module_label();
	Println2("\tInitialize ", (setup) ? "sucessful" : "failed");
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_ZXGesture::Loom_ZXGesture(JsonVariant p)
	: Loom_ZXGesture(p[0], p[1], (ZXMode)(int)p[2])
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_ZXGesture::~Loom_ZXGesture() 
{
	delete inst_ZX;
}

/////////////////////////////////////////////////////////////////////
void Loom_ZXGesture::print_config()
{
	LoomI2CSensor::print_config();
	Println3('\t', "Mode                : ", (mode == ZXMode::ZX_POS) ? "Position" : "Gesture" );
}

/////////////////////////////////////////////////////////////////////
void Loom_ZXGesture::print_measurements()
{
	print_module_label();
	Println("Measurements:");

	switch (mode) {
		case ZXMode::ZX_POS : 
			Println3("\t", "ZX: ", pos[0]);
			Println3("\t", "ZY: ", pos[1]);
			break;
		case ZXMode::ZX_GEST : 
			Println3("\t", "Gesture type : ", gesture_type.c_str());
			Println3("\t", "Gesture speed: ", gesture_speed);
			break; 
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_ZXGesture::measure()
{
	uint8_t x, z;

	switch (mode) {
		case ZXMode::ZX_POS : 
			if ( inst_ZX->positionAvailable() ) {
				x = inst_ZX->readX();
				z = inst_ZX->readZ();
				
				if ( (x != ZX_ERROR) && (z != ZX_ERROR) ) {
					pos[0] = x;
					pos[1] = z;
					// Println2("zxgesturesensor X: ", pos[0]);
					// Println2("zxgesturesensor Z: ", pos[1]);
				} else {
					print_module_label();
					Println("Error occurred while reading position data");
				}	
			} else {
				// Println("Position data unavailable for zxgesturesensor");
				
				// Send 255 to indicate that no object is detected
				pos[0] = 255; 
				pos[1] = 255;
			}			
			break;
		

		case ZXMode::ZX_GEST :
			if ( inst_ZX->gestureAvailable() ) {
				gesture       = inst_ZX->readGesture();
				gesture_speed = inst_ZX->readGestureSpeed();
				
				switch (gesture) {
					case RIGHT_SWIPE : gesture_type = "Right Swipe"; break;
					case LEFT_SWIPE  : gesture_type = "Left Swipe";  break;
					case UP_SWIPE    : gesture_type = "Up Swipe";    break;
					
					case NO_GESTURE  : 
					default: gesture_type = "No Gesture"; break;
				}

			} else {
				// Println("Gesture data unavailable for zxgesturesensor");
				gesture_type  = "No Gesture";
				gesture_speed = 0;
			}
			break; 
	}
}

/////////////////////////////////////////////////////////////////////
void Loom_ZXGesture::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	switch (mode) {
		case ZXMode::ZX_POS : 
			append_to_bundle(bndl, id_prefix, "zx", pos[0], NEW_MSG);
			append_to_bundle(bndl, id_prefix, "zy", pos[1]);
			break;
		case ZXMode::ZX_GEST : 
			append_to_bundle(bndl, id_prefix, "type" , gesture_type.c_str(), NEW_MSG);
			append_to_bundle(bndl, id_prefix, "speed", (int)gesture_speed);
			break; 
	}
}
