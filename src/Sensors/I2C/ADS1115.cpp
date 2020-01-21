#include "ADS1115.h"

//ADS1115 ADS1115(i2c_address);
///////////////////////////////////////////////////////////////////////////////
Loom_ADS1115::Loom_ADS1115(
LoomManager* manager,
const byte i2c_address,// =0x2A, 
			const uint8_t 		mux_port,
			const char*			module_name,// = "ADS1115",
			const int				analog0,
			const int				analog1,
			const int				analog2,
			const int				analog3,
			const int				diff0,
			const int				diff1,
			const bool 			analog_0_enabled,
			const bool 			analog_1_enabled,
			const bool 			analog_2_enabled,
			const bool 			analog_3_enabled,
			const bool 			diff_0_enabled,
			const bool 			diff_1_enabled
			
		 
	) 
	: LoomI2CSensor(manager, "ADS1115", LoomModule::Type::ADS1115 , i2c_address, mux_port)
{

	Adafruit_ADS1115 ads1115;
	ads1115.begin();//used ads1115 before
	//bool setup = adc0.begin();//used ads1115 before

	//if (!setup) active = false;

	//print_module_label();
	//LPrintln("Initialize ", (setup) ? "sucessful" : "failed");//everything before duplicated from SHT31D
}

///////////////////////////////////////////////////////////////////////////////
Loom_ADS1115::Loom_ADS1115(LoomManager* manager, JsonArrayConst p)
	: Loom_ADS1115(manager, EXPAND_ARRAY(p, 15) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::print_config() const
{			LPrintln('\t', "i2c_address: ", i2c_address);
			//byte			i2c_address		= 0x2A,
			LPrintln('\t', "module_name: ", "ADS1115" );
			//const char*		module_name		= "I2CSensorTemplate"
			LPrintln('\t', "Using Analog 0: ",analog_0_enabled);
			//int				analog0			= 0;
			LPrintln('\t', "Using Analog 1 : ",analog_1_enabled);
			//int				analog1			= 0;
			LPrintln('\t', "Using Analog 2 : ",analog_2_enabled);
			//int				analog2			= 0;
			LPrintln('\t', "Using Analog 3 : ",analog_3_enabled);
			//int				analog3			= 0;
			LPrintln('\t', "Using differential Measurements A0 and A1 : ", diff_0_enabled );
			//int				diff0			= 0;
			LPrintln('\t', "Using differential Measurements A2 and A3 : ", diff_1_enabled );
			//int				diff1			= 0;
}

///////////////////////////////////////////////////////////////////////////////
//void Loom_ADS1115::print_state()
//{
//	//ADS1115::print_state();
//	// print information about Loom_ADS1115 state 
//	return;
//}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::print_measurements() const
{
	print_module_label();
	LPrintln("************************ \n");
	LPrintln("Measurements for simple: \n");
	LPrintln("\t", "A0  : ", analog0);
	LPrintln("\t", "A1  : ", analog1);
	LPrintln("\t", "A2  : ", analog2);
	LPrintln("\t", "A3  : ", analog3);
	LPrintln("Measurements for derivative: \n");
	LPrintln("\t", "DA0  : ", diff0);
	LPrintln("\t", "DA1  : ", diff1);
}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::measure()
{ 
	
  


	if(analog_0_enabled==true){ 
	analog0 = ads1115.readADC_SingleEnded(0);
   
	}
    if(analog_1_enabled==true){ 
	analog1 = ads1115.readADC_SingleEnded(1);

	}
    
	if(analog_2_enabled==true){ 
	  analog2 = ads1115.readADC_SingleEnded(2);

	}

	if(analog_3_enabled==true){ 
	  analog3 = ads1115.readADC_SingleEnded(3);

	}
	if(diff_0_enabled==true){ 
	  diff0 = ads1115.readADC_Differential_0_1();

	}
	if(diff_1_enabled==true){ 
	  diff1 = ads1115.readADC_Differential_2_3();

	}
	

	
}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::package(JsonObject json)
{
		JsonObject data = get_module_data_object(json, module_name);
		data["i2c_address"] =i2c_address;
		data["mux_port"]=mux_port;
		data["module_name"]=module_name;
		data["analog0"]=analog0;
		data["analog1"]=analog1;
		data["analog2"]=analog2;
		data["analog3"]=analog3;
		data["diff0"]=diff0;
		data["diff1"]=diff1;
		data["analog_0_enabled"]=analog_0_enabled;
		data["analog_1_enabled"]=analog_1_enabled;
		data["analog_2_enabled"]=analog_2_enabled;
		data["analog_3_enabled"]=analog_3_enabled;
		data["diff_0_enabled"]=diff_0_enabled;
		data["diff_1_enabled"]=diff_1_enabled;
	
	// keys must be c-strings
	// values can be int, float, bool, c-string
}
/*
///////////////////////////////////////////////////////////////////////////////
bool Loom_ADS1115::dispatch(JsonObject json)
{
	if ( strcmp(json["module"], module_name) == 0 ) {
		JsonArray params = json["params"];
		return functionRoute(
			json["func"],
			"method_to_run", [this, params]() { if (params.size() >= 8) { method_to_run( EXPAND_ARRAY(params, 8) ); } else { LPrintln("Not enough parameters"); } } 
		);
		// The `5`s in the above command correspond to the number of parameters to `method_to_run`
		// Change to match the parameters of your method
	} else {
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::calibrate()
{
return;
}

///////////////////////////////////////////////////////////////////////////////
	*/