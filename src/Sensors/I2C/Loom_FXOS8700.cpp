
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
	: Loom_FXOS8700(p[0], p[1]) 
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
void Loom_FXOS8700::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle(bndl, id_prefix, "ax", accel[0], NEW_MSG);
	append_to_bundle(bndl, id_prefix, "ay", accel[1]);
	append_to_bundle(bndl, id_prefix, "az", accel[2]);

	append_to_bundle(bndl, id_prefix, "mx", mag[0]);
	append_to_bundle(bndl, id_prefix, "my", mag[1]);
	append_to_bundle(bndl, id_prefix, "mz", mag[2]);
}

