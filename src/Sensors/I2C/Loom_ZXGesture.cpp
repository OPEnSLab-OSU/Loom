
#include "Loom_ZXGesture.h"


///////////////////////////////////////////////////////////////////////////////
Loom_ZXGesture::Loom_ZXGesture(
		byte			i2c_address, 
		int				mux_port, 
		const char*		module_name,
		Mode			mode
	)
	: LoomI2CSensor( module_name, i2c_address, mux_port )
{
	this->module_type = LoomModule::Type::ZXGesture;

	this->mode = mode;

	inst_ZX = new ZX_Sensor(i2c_address);

	bool setup = inst_ZX->init();
	uint8_t ver;


	ver = inst_ZX->getModelVersion();
	print_module_label();
	if (ver != ZX_MODEL_VER) {
		setup = false;
		LPrintln("Incorrect Model Version or unable to read Model Version.");
	} else {
		LPrintln("Model Version: ", ver);
	}
	
	// Read the register map version and ensure the library will work
	ver = inst_ZX->getRegMapVersion();
	print_module_label();
	if (ver != ZX_REG_MAP_VER) {
		setup = false;
		LPrintln("Incorrect Register Map Version or unable to read Register Map Version.");
	} else {
		LPrintln("Register Map Version: ", ver);
	}

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_ZXGesture::Loom_ZXGesture(JsonArrayConst p)
	: Loom_ZXGesture(p[0], p[1], p[2], (Mode)(int)p[3])
{}

///////////////////////////////////////////////////////////////////////////////
Loom_ZXGesture::~Loom_ZXGesture() 
{
	delete inst_ZX;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_ZXGesture::print_config()
{
	LoomI2CSensor::print_config();
	LPrintln('\t', "Mode                : ", (mode == Mode::POS) ? "Position" : "Gesture" );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_ZXGesture::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");

	switch (mode) {
		case Mode::POS : 
			LPrintln("\t", "ZX: ", pos[0]);
			LPrintln("\t", "ZY: ", pos[1]);
			break;
		case Mode::GEST : 
			LPrintln("\t", "Gesture type : ", gesture_type.c_str());
			LPrintln("\t", "Gesture speed: ", gesture_speed);
			break; 
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_ZXGesture::measure()
{
	uint8_t x, z;

	switch (mode) {
		case Mode::POS : 
			if ( inst_ZX->positionAvailable() ) {
				x = inst_ZX->readX();
				z = inst_ZX->readZ();
				
				if ( (x != ZX_ERROR) && (z != ZX_ERROR) ) {
					pos[0] = x;
					pos[1] = z;
					// LPrintln("zxgesturesensor X: ", pos[0]);
					// LPrintln("zxgesturesensor Z: ", pos[1]);
				} else {
					print_module_label();
					LPrintln("Error occurred while reading position data");
				}	
			} else {
				// LPrintln("Position data unavailable for zxgesturesensor");
				
				// Send 255 to indicate that no object is detected
				pos[0] = 255; 
				pos[1] = 255;
			}			
			break;
		

		case Mode::GEST :
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
				// LPrintln("Gesture data unavailable for zxgesturesensor");
				gesture_type  = "No Gesture";
				gesture_speed = 0;
			}
			break; 
	}
}

///////////////////////////////////////////////////////////////////////////////

void Loom_ZXGesture::package(JsonObject json)
{
	switch (mode) {
		case Mode::POS : 		
			package_json(json, module_name, 
				"zx",	pos[0],
				"zy",	pos[1]
			);

			break;
		case Mode::GEST : 
			package_json(json, module_name, 
				"type",		gesture_type.c_str(),
				"speed",	(int)gesture_speed
			);
			break; 
	}

}

///////////////////////////////////////////////////////////////////////////////



