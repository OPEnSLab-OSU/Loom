///////////////////////////////////////////////////////////////////////////////
///
/// @file		ZXGesture.cpp
/// @brief		File for ZXGesture implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "ZXGesture.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
ZXGesture::ZXGesture(
		const byte		i2c_address,
		const uint8_t	mux_port,
		const Mode		mode
	)
	: I2CSensor("ZXGesture", i2c_address, mux_port)
	, mode(mode)
	, inst_ZX( ZX_Sensor(i2c_address) )
{
  LMark;
	bool setup = inst_ZX.init();
	uint8_t ver;
  LMark;


	ver = inst_ZX.getModelVersion();
	print_module_label();
	if (ver != ZX_MODEL_VER) {
   	LMark;
		setup = false;
   	LMark;
		LPrintln("Incorrect Model Version");
	} else {
		LPrintln("Model Version: ", ver);
	}

	// Read the register map version and ensure the library will work
	ver = inst_ZX.getRegMapVersion();
	print_module_label();
  LMark;
	if (ver != ZX_REG_MAP_VER) {
		setup = false;
		LPrintln("Incorrect Register Map Version");
	} else {
		LPrintln("Register Map Version: ", ver);
	}

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
ZXGesture::ZXGesture(JsonArrayConst p)
	: ZXGesture(p[0], p[1], (Mode)(int)p[2]) {}

///////////////////////////////////////////////////////////////////////////////
void ZXGesture::print_config() const
{
	I2CSensor::print_config();
  LMark;
	LPrintln("\tMode             : ", (mode == Mode::POS) ? "Position" : "Gesture" );
}

///////////////////////////////////////////////////////////////////////////////
void ZXGesture::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
  LMark;

	switch (mode) {
		case Mode::POS :
    	LMark;
			LPrintln("\tZX: ", pos[0]);
    	LMark;
			LPrintln("\tZY: ", pos[1]);
    	LMark;
			break;
		case Mode::GEST :
    	LMark;
			LPrintln("\tGesture type : ", gesture_type.c_str());
    	LMark;
			LPrintln("\tGesture speed: ", gesture_speed);
    	LMark;
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
void ZXGesture::measure()
{
  LMark;
	uint8_t x, z;
  LMark;

	switch (mode) {
		case Mode::POS :
    	LMark;
			if ( inst_ZX.positionAvailable() ) {
     		LMark;
				x = inst_ZX.readX();
     		LMark;
				z = inst_ZX.readZ();

				if ( (x != ZX_ERROR) && (z != ZX_ERROR) ) {
      		LMark;
					pos[0] = x;
      		LMark;
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
			if ( inst_ZX.gestureAvailable() ) {
	     	LMark;
				gesture       = inst_ZX.readGesture();
     		LMark;
				gesture_speed = inst_ZX.readGestureSpeed();
      	LMark;

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
void ZXGesture::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;

	switch (mode) {
		case Mode::POS :
    	LMark;
			data["zx"] = pos[0];
    	LMark;
			data["zy"] = pos[1];
    	LMark;
			break;
		case Mode::GEST :
    	LMark;
			data["type"]  = gesture_type.c_str();
    	LMark;
			data["speed"] = (int)gesture_speed;
    	LMark;
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
