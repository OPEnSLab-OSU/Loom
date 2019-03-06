
#include "Loom_FXOS8700.h"



// --- CONSTRUCTOR ---
Loom_FXOS8700::Loom_FXOS8700(byte i2c_address, char* module_name, char* sensor_description)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{
	inst_FXOS8700 = new Adafruit_FXOS8700(0x8700A, 0x8700B);
	bool setup = inst_FXOS8700->begin(ACCEL_RANGE_4G);

	print_module_label();
	Println3("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}


// --- DESTRUCTOR ---
Loom_FXOS8700::~Loom_FXOS8700() 
{
	delete inst_FXOS8700;
}


void Loom_FXOS8700::print_measurements()
{
	print_module_label();
	Println("Measurements:");
	Println4("\t", "AX: ", accel[0], " m/s^2");
	Println4("\t", "AY: ", accel[1], " m/s^2");
	Println4("\t", "AZ: ", accel[2], " m/s^2");
	Println4("\t", "MX: ", mag[0], " uT");
	Println4("\t", "MY: ", mag[1], " uT");
	Println4("\t", "MZ: ", mag[2], " uT");
}


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

