///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_FXOS8700.cpp
/// @brief		File for Loom_FXOS8700 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "FXOS8700.h"

#include <Adafruit_Sensor.h>


///////////////////////////////////////////////////////////////////////////////
Loom_FXOS8700::Loom_FXOS8700(
LoomManager* manager,
const byte i2c_address,
		const uint8_t		mux_port
	)
	: LoomI2CSensor(manager, "FXOS8700", Type::FXOS8700, i2c_address, mux_port )
	, inst_FXOS8700(Adafruit_FXOS8700(0x8700A, 0x8700B))
{
  LMark;
	bool setup = inst_FXOS8700.begin(ACCEL_RANGE_4G);
  LMark;

	if (!setup) active = false;
  LMark;

	print_module_label();
  LMark;
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_FXOS8700::Loom_FXOS8700(LoomManager* manager, JsonArrayConst p)
	: Loom_FXOS8700(manager, EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXOS8700::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Measurements:");
  LMark;
	LPrintln("\tAX: ", accel[0], " m/s^2");
  LMark;
	LPrintln("\tAY: ", accel[1], " m/s^2");
  LMark;
	LPrintln("\tAZ: ", accel[2], " m/s^2");
  LMark;
	LPrintln("\tMX: ", mag[0], " uT");
  LMark;
	LPrintln("\tMY: ", mag[1], " uT");
  LMark;
	LPrintln("\tMZ: ", mag[2], " uT");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXOS8700::measure()
{
  LMark;
	sensors_event_t aevent, mevent;
  LMark;
	inst_FXOS8700.getEvent(&aevent, &mevent);
  LMark;

	accel[0] = aevent.acceleration.x;
  LMark;
	accel[1] = aevent.acceleration.y;
  LMark;
	accel[2] = aevent.acceleration.z;
  LMark;

	mag[0] = mevent.magnetic.x;
  LMark;
	mag[1] = mevent.magnetic.y;
  LMark;
	mag[2] = mevent.magnetic.z;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXOS8700::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;

	data["ax"] = accel[0];
  LMark;
	data["ay"] = accel[1];
  LMark;
	data["az"] = accel[2];
  LMark;

	data["mx"] = mag[0];
  LMark;
	data["my"] = mag[1];
  LMark;
	data["mz"] = mag[2];
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXOS8700::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
