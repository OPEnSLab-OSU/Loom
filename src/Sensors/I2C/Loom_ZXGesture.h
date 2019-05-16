
#ifndef LOOM_ZXGESTURE_h
#define LOOM_ZXGESTURE_h

#include "Loom_I2C_Sensor.h"

#include <ZX_Sensor.h>


/// Different modes of the ZX gesture sensor
enum class ZXMode {
	ZX_POS,		///< Measure z and x position
	ZX_GEST 	///< Measure gestures
};


// ### (LoomI2CSensor) | dependencies: [] | conflicts: []
/// ZXGesture position / gesture sensor module
// ###
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

	/// ZXGesture module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x10> | {0x10, 0x11} | I2C address
	/// \param[in]	module_name				String | <"ZXGesture"> | null | ZXGesture module name
	/// \param[in]	mode							Set(ZXMode) | <0> | { 0("Position"), 1("Gesture") } | Gain level
	Loom_ZXGesture(
			byte			i2c_address		= 0x10,
			const char*		module_name		= "ZXGesture",
			ZXMode			mode			= ZXMode::ZX_POS
		);

	Loom_ZXGesture(JsonVariant p);


	// --- DESTRUCTOR ---
	virtual ~Loom_ZXGesture();

	// Inherited (Overriding) Methods
	void		print_config() override;
	void		print_measurements() override;
	void		measure() override;
	void 		package(JsonObject json);

private:

};


#endif
