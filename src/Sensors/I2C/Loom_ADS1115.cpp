#include "Loom_ADS1115.h"
#include "ADS1115.cpp"

///////////////////////////////////////////////////////////////////////////////
LoomI2CSensorTemplate::LoomI2CSensorTemplate(
		byte			i2c_address, 
		const char*		module_name 
	) 
	: LoomI2CSensor( "ASD1115", TYPE::ADS1115 ,i2c_address, mux_port)//unsure about type::
{
	this->module_type = LoomModule::Type::I2CSensorTemplateType;//everything after duplicated from SHT31D
	
	bool setup = ads1115.begin(i2c_address);

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");//everything before duplicated from SHT31D
}

///////////////////////////////////////////////////////////////////////////////
Loom_ADS1115::Loom_ADS1115(JsonArrayConst p)
	: Loom_ADS1115( EXPAND_ARRAY(p, 8) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::print_config()
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
void Loom_ADS1115::print_state()
{
	Loom_ADS1115::print_state();
	// print information about Loom_ADS1115 state 
}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::print_measurements()
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
    adc0.setMultiplexer(ADS1115_MUX_P0_NG);
    adc0.triggerConversion();
    pollAlertReadyPin();}
    //Serial.print("A0: "); 
	//Serial.print(adc0.getMilliVolts(false)); 
	//Serial.print("mV\t");
	}
    if(analog_1_enabled==true){ 
    adc0.setMultiplexer(ADS1115_MUX_P1_NG);
    adc0.triggerConversion();
    pollAlertReadyPin();
    //Serial.print("A1: "); 
	//Serial.print(adc0.getMilliVolts(false)); 
	//Serial.print("mV\t");
	}
    
	if(analog_2_enabled==true){ 
    adc0.setMultiplexer(ADS1115_MUX_P2_NG);
    adc0.triggerConversion();
    pollAlertReadyPin();
    //Serial.print("A2: "); 
	//Serial.print(adc0.getMilliVolts(false)); 
	//Serial.print("mV\t");
	}

	if(analog_3_enabled==true){ 
    adc0.setMultiplexer(ADS1115_MUX_P3_NG);
    // Do conversion polling via I2C on this last reading: 
    //Serial.print("A3: "); 
	//Serial.print(adc0.getMilliVolts(true)); 
	//Serial.print("mV\t");
	}

    //Serial.println(digitalRead(alertReadyPin));
    //delay(500);
	// measure data from sensor
	
}

///////////////////////////////////////////////////////////////////////////////
void Loom_ADS1115::package(JsonObject json)
{
	package_json(json, module_name, 
		"A0"		"analog0";
		"A1"		"analog1";
		"A2"		"analog2";
		"A3"		"analog3";
		"Diff0"		"diff0";
		"Diff1"		"diff1";
	);
	// keys must be c-strings
	// values can be int, float, bool, c-string
}

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

}

///////////////////////////////////////////////////////////////////////////////
	