#include "ADS1115.h"

//ADS1115 ADS1115(i2c_address);
///////////////////////////////////////////////////////////////////////////////
Loom_ADS1115::Loom_ADS1115(
		LoomManager* manager,
		const 	byte i2c_address,
		const	uint8_t 		mux_port,
		const 	bool 			analog_0_enabled,
		const 	bool 			analog_1_enabled,
		const 	bool 			analog_2_enabled,
		const 	bool 			analog_3_enabled,
		const 	bool 			diff_0_enabled,
		const 	bool 			diff_1_enabled,
		const	Gain			gain
	)
	: LoomI2CSensor(manager, "ADS1115", LoomModule::Type::ADS1115 , i2c_address, mux_port)
	, ads1115(i2c_address)
	, analog_enabled{ analog_0_enabled, analog_1_enabled, analog_2_enabled, analog_3_enabled }
	, diff_enabled{ diff_0_enabled, diff_1_enabled }
	, analog_reads{}
	, diff_reads()
{
  LMark;
	// Gain is an internal value in this driver, so this function
	// does not actually write to the I2C bus
	ads1115.setGain(static_cast<adsGain_t>(static_cast<uint32_t>(gain)));
  LMark;
	ads1115.begin();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
Loom_ADS1115::Loom_ADS1115(LoomManager* manager, JsonArrayConst p)
	: Loom_ADS1115(manager, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], (Gain)(uint32_t)p[8] ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::print_config() const
{
  LMark;
	LPrintln('\t', "i2c_address: ", i2c_address);
  LMark;
	LPrintln('\t', "module_name: ", "ADS1115" );
  LMark;
	LPrintln('\t', "Using Analog 0: ", analog_enabled[0]);
  LMark;
	LPrintln('\t', "Using Analog 1 : ", analog_enabled[1]);
  LMark;
	LPrintln('\t', "Using Analog 2 : ", analog_enabled[2]);
  LMark;
	LPrintln('\t', "Using Analog 3 : ", analog_enabled[3]);
  LMark;
	LPrintln('\t', "Using differential Measurements A0 and A1 : ", diff_enabled[0] );
  LMark;
	LPrintln('\t', "Using differential Measurements A2 and A3 : ", diff_enabled[1] );
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::print_measurements() const
{
  LMark;
	print_module_label();
  LMark;
	LPrintln("************************ \n");
  LMark;
	LPrintln("Measurements for simple: \n");
  LMark;
	if (analog_enabled[0]) LPrintln("\t", "A0  : ", analog_reads[0]);
  LMark;
	if (analog_enabled[1]) LPrintln("\t", "A1  : ", analog_reads[1]);
  LMark;
	if (analog_enabled[2]) LPrintln("\t", "A2  : ", analog_reads[2]);
  LMark;
	if (analog_enabled[3]) LPrintln("\t", "A3  : ", analog_reads[3]);
  LMark;
	if (diff_enabled[0] || diff_enabled[1]) {
   	LMark;
		LPrintln("Measurements for derivative: \n");
   	LMark;
		if (diff_enabled[0]) LPrintln("\t", "DA0  : ", diff_reads[0]);
   	LMark;
		if (diff_enabled[1]) LPrintln("\t", "DA1  : ", diff_reads[1]);
  	LMark;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::measure()
{
  LMark;
	for (uint8_t i = 0; i < 4; i++) {
   	LMark;
		if (analog_enabled[i])
			analog_reads[i] = ads1115.readADC_SingleEnded(i);
  	LMark;
	}
	if (diff_enabled[0])
		diff_reads[0] = ads1115.readADC_Differential_0_1();
  LMark;
	if (diff_enabled[1])
		diff_reads[1] = ads1115.readADC_Differential_2_3();
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::package(JsonObject json)
{
   	LMark;
		JsonObject data = get_module_data_object(json, module_name);
   	LMark;
		if (analog_enabled[0]) data["analog0"] = analog_reads[0];
   	LMark;
		if (analog_enabled[1]) data["analog1"] = analog_reads[1];
   	LMark;
		if (analog_enabled[2]) data["analog2"] = analog_reads[2];
   	LMark;
		if (analog_enabled[3]) data["analog3"] = analog_reads[3];
   	LMark;
		if (diff_enabled[0]) data["diff0"] = diff_reads[0];
   	LMark;
		if (diff_enabled[1]) data["diff1"] = diff_reads[1];
 	LMark;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::diagnose(bool& result){
  LMark;
	// implement here
}
