#pragma once

/// Enum to check against to when finding individual component
/// managed by a LoomManager
enum class ModuleType {
	Unknown,
	// Actuators
	Neopixel, Relay, Servo, Stepper,
	// Sensors
	Analog, Digital,
	// I2C
	AS7262, AS7263, AS7265X, FXAS21002, FXOS8700, LIS3DH, MB1232, MPU6050, MS5803, SHT31D, TSL2561, TSL2591, ZXGesture,
	// SDI12
	Decagon5TM, DecagonGS3,
	// SPI
	MAX31856,
	// CommPlats
	LoRa, nRF, SlipSerial, Bluetooth,
	// LogPlats
	OLED, SDCARD,
	// InternetPlats
	Internet,
	// PublishPlats
	Publish,
	// RTC
	DS3231, PCF8523,
	// Other
	Sleep_Manager, Interrupt_Manager, Multiplexer
};

/// Different levels of verbosity (for printing or packaging)
enum class Verbosity {
	V_OFF,		///< Disable
	V_LOW, 		///< Minimal/Stardard
	V_HIGH 		///< Full details
};