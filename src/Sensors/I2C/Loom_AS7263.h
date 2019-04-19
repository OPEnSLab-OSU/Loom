
#ifndef LOOM_AS7263_h
#define LOOM_AS7263_h

#include "Loom_I2C_Sensor.h"

#include "AS726X.h"

// # (LoomI2CSensor) | dependencies: [] | conflicts: []
/// AS7263 Near Infrared spectrum spectral sensor module
// # 
class Loom_AS7263 : public LoomI2CSensor
{

protected:

	/// Underlying AS7263 sensor manager instance
	AS726X		inst_AS7263;

	/// Measured near-infra-red band values (r,s,t,u,v,w)
	int			nir_vals[6];

	/// Whether or not to use bulb
	bool		use_bulb;

	/// Gain setting
	byte		gain;
	/// Sensor read mode
	byte		mode;
	/// Integration time setting
	byte		integration_time;

public:


	/// AS7263 module constructor
	///
	/// \param[in]	i2c_address				Set(Int) | <0x49> | {0x49} | I2C address
	/// \param[in]	module_name				String | <"AS7263"> | AS7263 module name					
	/// \param[in]	use_bulb				Bool | <false> | Whether or not to use bulb
	/// \param[in]	gain					Set(Int) | <1> | { 0("1x"), 1("3.7x"), 2("16x"), 3("64x") } | Gain level
	/// \param[in]	mode					Set(Int) | <3> | { 0("Continuous reading of STUV"), 1("Continuous reading of RTUX"), 2("Continuous reading all channels"), 3("One-shot reading of all channels") } | Read mode
	/// \param[in]	integration_time		Int | <50> | [0-255] | Integration time (time will be 2.8ms * [integration value]) 
	Loom_AS7263(	
			byte			i2c_address			= 0x49,
			const char*		module_name			= "AS7263",
			bool			use_bulb			= false,
			byte			gain				= 1,
			byte			mode				= 3,
			byte			integration_time	= 50
		);

	// --- DESTRUCTOR ---
	virtual ~Loom_AS7263();

	// Inherited (Overriding) Methods
	void		print_config() override;
	void		print_measurements() override;
	void		measure() override;
	void		package(OSCBundle& bndl, char* suffix="") override;


	void		enable_bulb(bool enable);
	// 0: 1x (power-on default), 1: 3.7x, 2: 16x, 3: 64x
	void		set_gain(byte gain);
	// 0: Continuous reading of STUV 
	// 1: Continuous reading of RTUX 
	// 2: Continuous reading of all channels (power-on default)
	// 3: One-shot reading of all channels
	void		set_mode(byte mode);
	// Time will be 2.8ms * [integration value]  (0-255), 50 is default
	void		set_integration_time(byte time);

private:

};


#endif
