
// ================================================================ 
// ===                 LOOM CONFIGURATION FILE                  === 
// ================================================================



// ================================================================ 
// ===  DEVICE IDENTIFICATION  === 
// ================================================================

#define Device_Name					"Device"
#define Instance_Num				1
#define Family_Name					"Loom"
#define Family_Num					1


// ================================================================ 
// ===  GENERAL OPTIONS  === 
// ================================================================

#define LOOM_DEBUG 					1
#define SERIAL_BAUD 				115200 

#define Device_Type					NODE	// this doesnt reference enum currently

#define Enable_Flash				0

#define Print_Verbosity				2		// this doesnt reference enum currently
#define Package_Verbosity			2		// this doesnt reference enum currently


// ================================================================ 
// ===  SUB MANAGERS  === 
// ================================================================

#define Use_Interrupt_Manager		1
#define Use_Sleep_Manager			1


// ================================================================ 
// ===  INTERNET PLATFORMS  === 
// ================================================================

#define Use_WiFi					0
#define Use_Ethernet				0
#define Use_GMS						0


// ================================================================ 
// ===  COMMUNICATION PLATFORMS  === 
// ================================================================

#define Use_WiFi					0
#define Use_LoRa					0
#define Use_nRF						0


// ================================================================ 
// ===  LOGGING PLATFORMS  === 
// ================================================================

#define Use_SD						0
#define Use_OLED					0


// ================================================================ 
// ===  RTC  === 
// ================================================================

#define Use_DS3231					0
#define Use_PCF8523					0


// ================================================================ 
// ===  SENSORS  === 
// ================================================================


// General
#define Use_Analog					0		// probably just leave enabled, but specific individual pins?
#define Use_DS3231					0


// I2C
#define Use_AS7265X 				0	// Spectral Sensor Triad [cannot be used with TSL2561 or AS726X]
#define Use_AS726X 					0	// Spectral Sensor (visible 7262 /near IR 7263) [cannot be used with TSL2561 or AS7265X]
#define Use_FXAS21002 				0	// Gyroscope
#define Use_FXOS8700 				0	// Accelerometer / Magnetometer
#define Use_LIS3DH 					0	// Accelerometer
#define Use_MB1232 					0	// Sonar
#define Use_MPU6050 				0	// Accelerometer / Gyroscope
#define Use_MS5803 					0	// Atmospheric Pressure / Temperature Sensor
#define Use_SHT31D 					0	// Temperature / Humidity
#define Use_TSL2561 				0	// Lux Sensor [cannot be used with AS726X or AS7265X]
#define Use_TSL2591 				0	// Lux Sensor
#define Use_ZXGESTURESENSOR 		0	// ZX Distance Sensor


// SPI


// SDI-12


// ================================================================ 
// ===  ACTUATORS  === 
// ================================================================


#define Use_Neopixel				0
#define Use_Relay					0
#define Use_Servo					0
#define Use_Stepper					0


