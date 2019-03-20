
#ifndef LOOM_ZXGESTURE_h
#define LOOM_ZXGESTURE_h

#include "Loom_I2C_Sensor.h"

#include <ZX_Sensor.h>

enum class ZXMode { ZX_POS, ZX_GEST };



class Loom_ZXGesture : public LoomI2CSensor
{

protected:

	/// Underlying ZX sensor manager instance
	ZX_Sensor*		inst_ZX;

	/// Sensor mode (read positions or gesture - mutually exclusive)
	ZXMode			mode;

	/// Measured positions (x, y pos)
	int pos[2];

	/// Last read gesture type
	GestureType		gesture;
	/// String labelling last read gesture type
	String			gesture_type;
	/// The speed of the last measured gesture
	uint8_t			gesture_speed;

public:

	// --- CONSTRUCTOR ---
	Loom_ZXGesture(	
			byte		i2c_address				= ZXGESTURE_I2C_Address, // 0x10, // 0x10, 0x11
			
			char*		module_name				= ZXGESTURE_Module_Name, // "ZXGesture",
			char*		sensor_description		= ZXGESTURE_Sensor_Description, // "Temperature/Humidty",

			ZXMode		mode					= ZXGESTURE_Mode // ZXMode::ZX_POS
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_ZXGesture();

	// Inherited (Overriding) Methods
	void		print_config();
	void		print_measurements();
	void		measure();
	void		package(OSCBundle& bndl, char* suffix="");

private:

};


#endif
