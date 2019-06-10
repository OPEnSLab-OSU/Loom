
#include "Loom_FXOS8700.h"

#include <Adafruit_Sensor.h>


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_FXOS8700::Loom_FXOS8700(
		byte			i2c_address, 
		const char*		module_name
	)
	: LoomI2CSensor( module_name, i2c_address )
{
	this->module_type = ModuleType::FXOS8700;

	inst_FXOS8700 = new Adafruit_FXOS8700(0x8700A, 0x8700B);
	bool setup = inst_FXOS8700->begin(ACCEL_RANGE_4G);

	print_module_label();
	LPrintln("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}

/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_FXOS8700::Loom_FXOS8700(JsonVariant p)
	: Loom_FXOS8700( EXPAND_ARRAY(p, 2) ) 
{}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_FXOS8700::~Loom_FXOS8700() 
{
	delete inst_FXOS8700;
}

/////////////////////////////////////////////////////////////////////
void Loom_FXOS8700::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "AX: ", accel[0], " m/s^2");
	LPrintln("\t", "AY: ", accel[1], " m/s^2");
	LPrintln("\t", "AZ: ", accel[2], " m/s^2");
	LPrintln("\t", "MX: ", mag[0], " uT");
	LPrintln("\t", "MY: ", mag[1], " uT");
	LPrintln("\t", "MZ: ", mag[2], " uT");
}

/////////////////////////////////////////////////////////////////////
void Loom_FXOS8700::measure()
{
	sensors_event_t aevent, mevent;
	inst_FXOS8700->getEvent(&aevent, &mevent);

	accel[0] = aevent.acceleration.x;
	accel[1] = aevent.acceleration.y;
	accel[2] = aevent.acceleration.z;

	mag[0] = mevent.magnetic.x;
	mag[1] = mevent.magnetic.y;
	mag[2] = mevent.magnetic.z;
}


/////////////////////////////////////////////////////////////////////
void Loom_FXOS8700::package(JsonObject json)
{
	package_json(json, module_name, 
		// Acceleration
		"ax", accel[0],
		"ay", accel[1],
		"az", accel[2],

		// Magnetism
		"mx", mag[0],
		"my", mag[1],
		"mz", mag[2]
	);
}

/////////////////////////////////////////////////////////////////////


