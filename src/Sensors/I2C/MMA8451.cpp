///////////////////////////////////////////////////////////////////////////////
///
/// @file		MMA8451.cpp
/// @brief		File for MMA8451 implementation.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_SENSORS

#include "MMA8451.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////
MMA8451::MMA8451(
		const byte				i2c_address,
		const uint8_t			mux_port,
		const mma8451_range_t	range
	)
	: I2CSensor("MMA8451", i2c_address, mux_port)
	, range{range}
{
  LMark;
	bool setup = MMA.begin(i2c_address);

	// Set range
	MMA.setRange(range);

	// Configure interrupts
	// configure_interrupts(); // not verified yet

	if (!setup) active = false;
	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
MMA8451::MMA8451(JsonArrayConst p)
	: MMA8451(p[0], p[1], (mma8451_range_t)(int)p[2]) {}

///////////////////////////////////////////////////////////////////////////////
void MMA8451::print_config() const
{
	I2CSensor::print_config();
	// LPrintln("\tRange               : ", 2 << MMA.getRange(), "G" );
}

///////////////////////////////////////////////////////////////////////////////
void MMA8451::print_measurements() const
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tAccel X     : ", accel[0], " m/s^2");
	LPrintln("\tAccel Y     : ", accel[1], " m/s^2");
	LPrintln("\tAccel Z     : ", accel[2], " m/s^2");

	LPrint("\tOrientation : ");
	switch (orientation) {
		case MMA8451_PL_PUF: LPrintln("Portrait Up Front");		break;
		case MMA8451_PL_PUB: LPrintln("Portrait Up Back");		break;
		case MMA8451_PL_PDF: LPrintln("Portrait Down Front");	break;
		case MMA8451_PL_PDB: LPrintln("Portrait Down Back");	break;
		case MMA8451_PL_LRF: LPrintln("Landscape Right Front");	break;
		case MMA8451_PL_LRB: LPrintln("Landscape Right Back");	break;
		case MMA8451_PL_LLF: LPrintln("Landscape Left Front");	break;
		case MMA8451_PL_LLB: LPrintln("Landscape Left Back");	break;
	}
}

///////////////////////////////////////////////////////////////////////////////
void MMA8451::measure()
{
  LMark;
	// Update sensor
	MMA.read();

	// Get a new sensor event
	sensors_event_t event;
	MMA.getEvent(&event);

	accel[0] = event.acceleration.x;
	accel[1] = event.acceleration.y;
	accel[2] = event.acceleration.z;

	// Get the orientation of the sensor
	orientation = MMA.getOrientation();
}

///////////////////////////////////////////////////////////////////////////////
void MMA8451::package(JsonObject json)
{
  LMark;
	JsonObject data = get_module_data_object(json, module_name);

	data["ax"] = accel[0];
	data["ay"] = accel[1];
	data["az"] = accel[2];

	if (package_verbosity == Verbosity::V_HIGH) {
		char buf[22];
		switch (orientation) {
			case MMA8451_PL_PUF: strcpy(buf, "Portrait Up Front");		break;
			case MMA8451_PL_PUB: strcpy(buf, "Portrait Up Back");		break;
			case MMA8451_PL_PDF: strcpy(buf, "Portrait Down Front");	break;
			case MMA8451_PL_PDB: strcpy(buf, "Portrait Down Back");		break;
			case MMA8451_PL_LRF: strcpy(buf, "Landscape Right Front");	break;
			case MMA8451_PL_LRB: strcpy(buf, "Landscape Right Back");	break;
			case MMA8451_PL_LLF: strcpy(buf, "Landscape Left Front");	break;
			case MMA8451_PL_LLB: strcpy(buf, "Landscape Left Back");	break;
		}

		data["orient"] = buf;
	}
}

// ///////////////////////////////////////////////////////////////////////////////
// void MMA8451::enable_interrupts(bool enable)
// {

// }

// ///////////////////////////////////////////////////////////////////////////////
// void MMA8451::set_transient_int_threshold(uint8_t range)
// {
// 	MMA.setTransientIntThreshold(range);
// }

// ///////////////////////////////////////////////////////////////////////////////
// void MMA8451::configure_interrupts()
// {
// 	uint8_t dataToWrite = 0;
// 	// MMA8451_REG_CTRL_REG2
// 	// sysatem control register 2

// 	//dataToWrite |= 0x80;		// Auto sleep/wake interrupt
// 	//dataToWrite |= 0x40;		// FIFO interrupt
// 	//dataToWrite |= 0x20;		// Transient interrupt - enabled
// 	//dataToWrite |= 0x10;		// orientation
// 	//dataToWrite |= 0x08;		// Pulse interrupt
// 	//dataToWrite |= 0x04;		// Freefall interrupt
// 	//dataToWrite |= 0x01;		// data ready interrupt, MUST BE ENABLED FOR USE WITH ARDUINO

// 	// MMA8451_REG_CTRL_REG3
// 	// Interrupt control register

// 	dataToWrite = 0;
// 	dataToWrite |= 0x80;		// FIFO gate option for wake/sleep transition, default 0, Asserting this allows the accelerometer to collect data the moment an impluse happens and percerve that data because the FIFO buffer is blocked. Thus at the end of a wake cycle the data from the initial transient wake up is still in the buffer
// 	dataToWrite |= 0x40;		// Wake from transient interrupt enable
// 	//dataToWrite |= 0x20;		// Wake from orientation interrupt enable
// 	//dataToWrite |= 0x10;		// Wake from Pulse function enable
// 	//dataToWrite |= 0x08;		// Wake from freefall/motion decect interrupt
// 	//dataToWrite |= 0x02;		// Interrupt polarity, 1 = active high
// 	dataToWrite |= 0x00; 		// (0) Push/pull or (1) open drain interrupt, determines whether bus is driven by device, or left to hang

// 	MMA.writeRegister8(MMA8451_REG_CTRL_REG3, dataToWrite);

// 	dataToWrite = 0;

// 	// MMA8451_REG_CTRL_REG4
// 	// Interrupt enable register, enables interrupts that are not commented

// 	//dataToWrite |= 0x80;		// Auto sleep/wake interrupt
// 	//dataToWrite |= 0x40;		// FIFO interrupt
// 	dataToWrite |= 0x20;		// Transient interrupt - enabled
// 	//dataToWrite |= 0x10;		// orientation
// 	//dataToWrite |= 0x08;		// Pulse interrupt
// 	//dataToWrite |= 0x04;		// Freefall interrupt
// 	dataToWrite |= 0x01;		// data ready interrupt, MUST BE ENABLED FOR USE WITH ARDUINO
// 	MMA.writeRegister8(MMA8451_REG_CTRL_REG4, dataToWrite | 0x01);

// 	dataToWrite = 0;

// 	// MMA8451_REG_CTRL_REG5
// 	// Interrupt pin 1/2 configuration register, bit == 1 => interrupt to pin 1
// 	// see datasheet for interrupt's description, threshold int routed to pin 1
// 	// comment = int2, uncoment = int1

// 	//dataToWrite |= 0x80;		// Auto sleep/wake
// 	//dataToWrite |= 0x40;		// FIFO
// 	dataToWrite |= 0x20;		// Transient, asserting this routes transients interrupts to INT1 pin
// 	//dataToWrite |= 0x10;		// orientation
// 	//dataToWrite |= 0x08;		// Pulse
// 	//dataToWrite |= 0x04;		// Freefall
// 	//dataToWrite |= 0x01;		// data ready

// 	MMA.writeRegister8(MMA8451_REG_CTRL_REG5, dataToWrite);

// 	dataToWrite = 0;

// 	// MMA8451_REG_TRANSIENT_CFG
// 	//dataToWrite |= 0x10;		// Latch enable to capture accel values when interrupt occurs
// 	dataToWrite |= 0x08;		// Z transient interrupt enable
// 	dataToWrite |= 0x04;		// Y transient interrupt enable
// 	dataToWrite |= 0x02;		// X transient interrupt enable
// 	//dataToWrite |= 0x01;		// High-pass filter bypass
// 	MMA.writeRegister8(MMA8451_REG_TRANSIENT_CFG, dataToWrite);

// 	LPrintln("MMA8451_REG_TRANSIENT_CFG: ");
// 	LPrintln(MMA.readRegister8(MMA8451_REG_TRANSIENT_CFG), HEX);

// 	dataToWrite = 0;

// 	// MMA8451_REG_TRANSIENT_THS
// 	// Transient interrupt threshold in units of .06g
// 	//Acceptable range is 1-127
// 	dataToWrite = 0x01;
// 	MMA.writeRegister8(MMA8451_REG_TRANSIENT_THS, dataToWrite);

// 	dataToWrite = 0;

// 	// MMA8451_REG_TRANSIENT_CT  0x20
// 	dataToWrite = 0;			// value is 0-255 for numer of counts to debounce for, depends on ODR
// 	MMA.writeRegister8(MMA8451_REG_TRANSIENT_CT, dataToWrite);

// 	dataToWrite = 0;
// }

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_SENSORS
