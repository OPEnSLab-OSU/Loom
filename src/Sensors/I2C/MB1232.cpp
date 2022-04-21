///////////////////////////////////////////////////////////////////////////////
///
/// @file		MB1232.cpp
/// @brief		File for MB1232 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "MB1232.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

#define RangeCommand    		0x51	///< The Sensor ranging command has a value of 0x51
#define ChangeAddressCommand1 	0xAA	///< These are the two commands that need to be sent in sequence to change the sensor address
#define ChangeAddressCommand2 	0xA5	///< These are the two commands that need to be sent in sequence to change the sensor address

///////////////////////////////////////////////////////////////////////////////
MB1232::MB1232(
		const byte		i2c_address,
		const uint8_t	mux_port
	)
	: I2CSensor("MB1232", i2c_address, mux_port)
{
  LMark;
	Wire.beginTransmission(i2c_address);

	Wire.write(RangeCommand);
	Wire.endTransmission();
	delay(100);

	Wire.requestFrom(i2c_address, byte(2));
	bool setup = (Wire.available() >= 2);

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
MB1232::MB1232(JsonArrayConst p)
	: MB1232(EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void MB1232::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tRange: ", range, " cm");
}

///////////////////////////////////////////////////////////////////////////////
void MB1232::measure()
{
  LMark;
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
		LPrintln("Error reading from mb1232 (range)");
	}
}

///////////////////////////////////////////////////////////////////////////////
void MB1232::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);

	data["range"] = range;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
