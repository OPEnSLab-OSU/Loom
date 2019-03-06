
// #ifndef LOOM_MPU6050_h
// #define LOOM_MPU6050_h


// #include "Loom_I2C_Sensor.h"

// #include "Adafruit_SHT31.h"



// class Loom_MPU6050 : public LoomI2CSensor
// {

// protected:
// 	// --- PROTECTED MEMBERS ---
// 	Adafruit_SHT31 inst_sht31d;

// 	float temp;
// 	float humid;

// public:

// 	// --- PUBLIC MEMBERS ---


// 	// --- CONSTRUCTOR ---
// 	Loom_MPU6050(	byte i2c_address 			= 0x44,
					
// 					char* module_name 			= "MPU6050",
// 					char* sensor_description 	= "Temperature/Humidty" 
// 				);

// 	// --- DESTRUCTOR ---
// 	virtual ~Loom_MPU6050();

// 	void print_measurements();

// 	void measure();
// 	void package(OSCBundle& bndl, char* suffix="");
//	// void package_mux(OSCBundle* bndl, char* id_prefix, uint8_t port);

// private:

// };


// #endif
