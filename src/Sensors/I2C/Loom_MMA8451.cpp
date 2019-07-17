
#include "Loom_MMA8451.h"


///////////////////////////////////////////////////////////////////////////////
Loom_MMA8451::Loom_MMA8451(
		byte			i2c_address, 
		const char*		module_name,
		mma8451_range_t	range
	) 
	: LoomI2CSensor( module_name, i2c_address )
	, range{range}
{
	this->module_type = LoomModule::Type::MMA8451;

	bool setup = MMA.begin(i2c_address);

	// Set range
	MMA.setRange(range);

	if (!setup) active = false;
	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");

}

///////////////////////////////////////////////////////////////////////////////
Loom_MMA8451::Loom_MMA8451(JsonArrayConst p)
	: Loom_MMA8451(p[0], p[1], (mma8451_range_t)(int)p[2]) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_MMA8451::print_config()
{
	LoomI2CSensor::print_config();
	LPrintln('\t', "Range               : ", 2 << MMA.getRange(), "G" );
}

///////////////////////////////////////////////////////////////////////////////
void Loom_MMA8451::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\t", "Accel X     : ", accel[0], " m/s^2");
	LPrintln("\t", "Accel Y     : ", accel[1], " m/s^2");
	LPrintln("\t", "Accel Z     : ", accel[2], " m/s^2");
	
	LPrint("\t", "Orientation : ");
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
void Loom_MMA8451::measure()
{
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
void Loom_MMA8451::package(JsonObject json)
{
	package_json(json, module_name, 
		"accel_x",	accel[0],
		"accel_y",	accel[1],
		"accel_z",	accel[2]
	);

	if (package_verbosity == Verbosity::V_HIGH) {
		char buf[22];
		switch (orientation) {
			case MMA8451_PL_PUF: strcpy(buf, "Portrait Up Front"); 		break;
			case MMA8451_PL_PUB: strcpy(buf, "Portrait Up Back"); 		break;
			case MMA8451_PL_PDF: strcpy(buf, "Portrait Down Front"); 	break;
			case MMA8451_PL_PDB: strcpy(buf, "Portrait Down Back"); 	break;
			case MMA8451_PL_LRF: strcpy(buf, "Landscape Right Front");	break;
			case MMA8451_PL_LRB: strcpy(buf, "Landscape Right Back");	break;
			case MMA8451_PL_LLF: strcpy(buf, "Landscape Left Front");	break;
			case MMA8451_PL_LLB: strcpy(buf, "Landscape Left Back");	break;
		}

		package_json(json, module_name, 
			"orient", buf);
	}
}

///////////////////////////////////////////////////////////////////////////////
	



 // void configInterrupts()
 //    {
 //        uint8_t dataToWrite = 0;
 //        // MMA8451_REG_CTRL_REG2
 //        // sysatem control register 2

 //        //dataToWrite |= 0x80;    // Auto sleep/wake interrupt
 //        //dataToWrite |= 0x40;    // FIFO interrupt
 //        //dataToWrite |= 0x20;    // Transient interrupt - enabled
 //        //dataToWrite |= 0x10;    // orientation
 //        //dataToWrite |= 0x08;    // Pulse interrupt
 //        //dataToWrite |= 0x04;    // Freefall interrupt
 //        //dataToWrite |= 0x01;    // data ready interrupt, MUST BE ENABLED FOR USE WITH ARDUINO

 //        // MMA8451_REG_CTRL_REG3
 //        // Interrupt control register

 //        dataToWrite = 0;
 //        dataToWrite |= 0x80; // FIFO gate option for wake/sleep transition, default 0, Asserting this allows the accelerometer to collect data the moment an impluse happens and percerve that data because the FIFO buffer is blocked. Thus at the end of a wake cycle the data from the initial transient wake up is still in the buffer
 //        dataToWrite |= 0x40; // Wake from transient interrupt enable
 //        //dataToWrite |= 0x20;    // Wake from orientation interrupt enable
 //        //dataToWrite |= 0x10;    // Wake from Pulse function enable
 //        //dataToWrite |= 0x08;    // Wake from freefall/motion decect interrupt
 //        //dataToWrite |= 0x02;    // Interrupt polarity, 1 = active high
 //        dataToWrite |= 0x00; // (0) Push/pull or (1) open drain interrupt, determines whether bus is driven by device, or left to hang

 //        state_mma8451.inst.writeRegister8_public(MMA8451_REG_CTRL_REG3, dataToWrite);

 //        dataToWrite = 0;

 //        // MMA8451_REG_CTRL_REG4
 //        // Interrupt enable register, enables interrupts that are not commented

 //        //dataToWrite |= 0x80;    // Auto sleep/wake interrupt
 //        //dataToWrite |= 0x40;    // FIFO interrupt
 //        dataToWrite |= 0x20; // Transient interrupt - enabled
 //        //dataToWrite |= 0x10;    // orientation
 //        //dataToWrite |= 0x08;    // Pulse interrupt
 //        //dataToWrite |= 0x04;    // Freefall interrupt
 //        dataToWrite |= 0x01; // data ready interrupt, MUST BE ENABLED FOR USE WITH ARDUINO
 //        state_mma8451.inst.writeRegister8_public(MMA8451_REG_CTRL_REG4, dataToWrite | 0x01);

 //        dataToWrite = 0;

 //        // MMA8451_REG_CTRL_REG5
 //        // Interrupt pin 1/2 configuration register, bit == 1 => interrupt to pin 1
 //        // see datasheet for interrupt's description, threshold int routed to pin 1
 //        // comment = int2, uncoment = int1

 //        //dataToWrite |= 0x80;    // Auto sleep/wake
 //        //dataToWrite |= 0x40;    // FIFO
 //        dataToWrite |= 0x20; // Transient, asserting this routes transients interrupts to INT1 pin
 //        //dataToWrite |= 0x10;    // orientation
 //        //dataToWrite |= 0x08;    // Pulse
 //        //dataToWrite |= 0x04;    // Freefall
 //        //dataToWrite |= 0x01;    // data ready

 //        state_mma8451.inst.writeRegister8_public(MMA8451_REG_CTRL_REG5, dataToWrite);

 //        dataToWrite = 0;

 //        // MMA8451_REG_TRANSIENT_CFG
 //        //dataToWrite |= 0x10;  // Latch enable to capture accel values when interrupt occurs
 //        dataToWrite |= 0x08; // Z transient interrupt enable
 //        dataToWrite |= 0x04; // Y transient interrupt enable
 //        dataToWrite |= 0x02; // X transient interrupt enable
 //        //dataToWrite |= 0x01;    // High-pass filter bypass
 //        state_mma8451.inst.writeRegister8_public(MMA8451_REG_TRANSIENT_CFG, dataToWrite);

 //        Serial.print("MMA8451_REG_TRANSIENT_CFG: ");
 //        Serial.println(state_mma8451.inst.readRegister8(MMA8451_REG_TRANSIENT_CFG), HEX);

 //        dataToWrite = 0;

 //        // MMA8451_REG_TRANSIENT_THS
 //        // Transient interrupt threshold in units of .06g
 //        //Acceptable range is 1-127
 //        dataToWrite = 0x01;
 //        state_mma8451.inst.writeRegister8_public(MMA8451_REG_TRANSIENT_THS, dataToWrite);

 //        dataToWrite = 0;

 //        // MMA8451_REG_TRANSIENT_CT  0x20
 //        dataToWrite = 0; // value is 0-255 for numer of counts to debounce for, depends on ODR
 //        state_mma8451.inst.writeRegister8_public(MMA8451_REG_TRANSIENT_CT, dataToWrite);

 //        dataToWrite = 0;
 //    }
