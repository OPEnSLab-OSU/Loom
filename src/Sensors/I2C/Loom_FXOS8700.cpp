
#include "Loom_FXOS8700.h"

#include <Adafruit_Sensor.h>


///////////////////////////////////////////////////////////////////////////////
Loom_FXOS8700::Loom_FXOS8700(
		byte			i2c_address, 
		uint8_t			mux_port
	)
	: LoomI2CSensor( "FXOS8700", Type::FXOS8700, i2c_address, mux_port )
	, inst_FXOS8700(Adafruit_FXOS8700(0x8700A, 0x8700B))
{
	bool setup = inst_FXOS8700.begin(ACCEL_RANGE_4G);
    
    for(int i = 0; i < 6; i++) {
        Values.push_back(var());
    }

	if (!setup) active = false;

	print_module_label();
	LPrintln("Initialize ", (setup) ? "sucessful" : "failed");
}

///////////////////////////////////////////////////////////////////////////////
Loom_FXOS8700::Loom_FXOS8700(JsonArrayConst p)
	: Loom_FXOS8700( EXPAND_ARRAY(p, 2) ) {}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXOS8700::print_measurements()
{
	print_module_label();
	LPrintln("Measurements:");
	LPrintln("\tAX: ", Values[0  ].retrieve<float>().value_or(0), " m/s^2");
	LPrintln("\tAY: ", Values[1  ].retrieve<float>().value_or(0), " m/s^2");
	LPrintln("\tAZ: ", Values[2  ].retrieve<float>().value_or(0), " m/s^2");
	LPrintln("\tMX: ", Values[0+3].retrieve<float>().value_or(0), " uT");
	LPrintln("\tMY: ", Values[1+3].retrieve<float>().value_or(0), " uT");
	LPrintln("\tMZ: ", Values[2+3].retrieve<float>().value_or(0), " uT");
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXOS8700::measure()
{
	sensors_event_t aevent, mevent;
	inst_FXOS8700.getEvent(&aevent, &mevent);

	Values[0] = aevent.acceleration.x;
	Values[1] = aevent.acceleration.y;
	Values[2] = aevent.acceleration.z;

	Values[0+3] = mevent.magnetic.x;
	Values[1+3] = mevent.magnetic.y;
	Values[2+3] = mevent.magnetic.z;
}

///////////////////////////////////////////////////////////////////////////////
void Loom_FXOS8700::package(JsonObject json)
{
	JsonObject data = get_module_data_object(json, module_name);
	
	data["ax"] = Values[0].retrieve<float>().value_or(0);
	data["ay"] = Values[1].retrieve<float>().value_or(0);
	data["az"] = Values[2].retrieve<float>().value_or(0);

	data["mx"] = Values[0+3].retrieve<float>().value_or(0);
	data["my"] = Values[1+3].retrieve<float>().value_or(0);
	data["mz"] = Values[2+3].retrieve<float>().value_or(0);
}

///////////////////////////////////////////////////////////////////////////////


