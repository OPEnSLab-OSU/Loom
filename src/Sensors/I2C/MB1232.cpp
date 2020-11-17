///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_MB1232.cpp
/// @brief		File for Loom_MB1232 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////


#include "MB1232.h"

#define RangeCommand    		0x51	///< The Sensor ranging command has a value of 0x51
#define ChangeAddressCommand1 	0xAA	///< These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand2 	0xA5	///< These are the two commands that need to be sent in sequence to change the sensor address


///////////////////////////////////////////////////////////////////////////////
Loom_MB1232::Loom_MB1232(
LoomManager* manager,
const byte i2c_address,
		const uint8_t		mux_port
	)
	: LoomI2CSensor(manager, "MB1232", Type::MB1232, i2c_address, mux_port )
{
  LMark;
	Wire.beginTransmission(i2c_address);
  LMark;

	Wire.write(RangeCommand);
  LMark;
	Wire.endTransmission();
  LMark;
	delay(100);
  LMark;

	Wire.requestFrom(i2c_address, byte(2));
  LMark;
	bool setup = (Wire.available() >= 2);
  LMark;

	if (!setup) active = false;
  LMark;

	print_module_label();
  LMark;
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_MB1232::Loom_MB1232(LoomManager* manager, JsonArrayConst p)
	: Loom_MB1232(manager, EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MB1232::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("Measurements:");
  LMark;
	LPrintln("\tRange: ", range, " cm");
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MB1232::measure()
{
  LMark;
	Wire.beginTransmission(i2c_address);
  LMark;

	Wire.write(RangeCommand);
  LMark;
	Wire.endTransmission();
  LMark;
	delay(100);
  LMark;

	Wire.requestFrom(0x70, byte(2));
  LMark;
	if (Wire.available() >= 2) {
   	LMark;
		// The sensor communicates two bytes, each a range. The
		// high byte is typically zero, in which case the low
		// byte is equal to the range, so only the range is transmitted.
		// The low byte will not be less than 20.
		byte high = Wire.read();
   	LMark;
		byte low  = Wire.read();
   	LMark;
		byte tmp  = Wire.read();
  	LMark;

		range = (high * 256) + low;
  	LMark;
	} else {
		print_module_label();
   	LMark;
		LPrintln("Error reading from mb1232 (range)");
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MB1232::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);
  LMark;

	data["range"] = range;
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MB1232::diagnose(bool& result){
  LMark;
	// implement here
}

///////////////////////////////////////////////////////////////////////////////
