
#include "Loom_MB1232.h"


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_MB1232::Loom_MB1232(byte i2c_address, char* module_name, char* sensor_description)

	: LoomI2CSensor( module_name, sensor_description, i2c_address )
{	
	Wire.beginTransmission(i2c_address);

	Wire.write(RangeCommand);
	Wire.endTransmission();
	delay(100);
	
	Wire.requestFrom(i2c_address, byte(2));
	bool setup = (Wire.available() >= 2);

	print_module_label();
	Println3("\t", "Initialize ", (setup) ? "sucessful" : "failed");
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_MB1232::~Loom_MB1232() 
{

}

/////////////////////////////////////////////////////////////////////
void Loom_MB1232::print_measurements()
{
	print_module_label();
	Println("Measurements:");
	Println4("\t", "Range: ", range, " cm");
}

/////////////////////////////////////////////////////////////////////
void Loom_MB1232::measure()
{
	Wire.beginTransmission(i2c_address);

	Wire.write(RangeCommand);
	Wire.endTransmission();
	delay(100);
	
	Wire.requestFrom(0x70, byte(2));
	if (Wire.available() >= 2) {
		// The sensor communicates two bytes, each a range. The
		// high byte is typically zero, in which case the low
		// byte is equal to the range, so only the range is transmitted.
		// The low byte will not be less than 20.
		byte high = Wire.read();
		byte low  = Wire.read();
		byte tmp  = Wire.read();
		
		range = (high * 256) + low;
	} else {
		print_module_label();
		Println("Error reading from mb1232 (range)");
	}  
}

/////////////////////////////////////////////////////////////////////
void Loom_MB1232::package(OSCBundle& bndl, char* suffix)
{
	char id_prefix[30]; 
	resolve_bundle_address(id_prefix, suffix);

	append_to_bundle(bndl, id_prefix, "Range", range, NEW_MSG);
}


