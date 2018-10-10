// ================================================================
// ===               I2C Sensor Address Selection               === 
// ================================================================

// 1 to enable a given I2C address, 0 to disable
// Multiplexers shouldn't usually have to modify the enabled addresses section unless
//   Multiple of the same sensors are being used and have different I2C addresses

// You can also name your sensors if not using a multiplexer, which
//	can be useful for distinquishing sensors 

// Make sure you have no conflicts of addresses 
//	(e.g. TSL2561 and TSL2591 both on 0x29)

// You can have multiple of the same sensor

// If there is only one I2C address available, the enable/disable feature is still here
//  for completeness sake - if you don't use a given sensor disable it with the is_<sensor> define instead though

#if is_tsl2591 == 1
	#define i2c_addr_tsl2591_0x29 	1

#endif
#if is_tsl2561 == 1
	// 2 are used on evaporometer b/c no multiplexer
	#define i2c_addr_tsl2561_0x29 	0 // generally disabled because 2591 uses this address, disable that to use 2561 on the address
	#define i2c_addr_tsl2561_0x39 	1 
	#define i2c_addr_tsl2561_0x49 	1

#endif
#if is_fxos8700 == 1
	#define i2c_addr_fxos8700_0x1C 	1 // The default
	#define i2c_addr_fxos8700_0x1D 	0
	#define i2c_addr_fxos8700_0x1E 	0
	#define i2c_addr_fxos8700_0x1F 	0

#endif
#if is_fxas21002 == 1
	#define i2c_addr_fxas21002_0x20 1 // Default
	#define i2c_addr_fxas21002_0x21	0

#endif
#if is_sht31d == 1
	#define i2c_addr_sht31d_0x44 	1 // Default
	#define i2c_addr_sht31d_0x45	0
#endif
#if is_mb1232 == 1
	#define i2c_addr_fxas21002_0x70	1
#endif
#if is_ms5803 == 1
	#define i2c_addr_ms5803_0x76 	1 // Default
	#define i2c_addr_ms5803_0x77	0
#endif
#if is_mpu6050 == 1
	#define i2c_addr_mpu6050_0x68 	1
	#define i2c_addr_mpu6050_0x69	0
#endif
#if is_lis3dh == 1
	#define i2c_addr_lis3dh_0x20 	1
#endif
#if is_zxgesturesensor == 1
	#define i2c_addr_zxgesturesensor_0x10 	1 // Default
	#define i2c_addr_zxgesturesensor_0x11	0
#endif



