#include "Loom_Module_Factory.h"



const  NameModulePair Factory::LookupTable[] =
{
	// InterruptManager
	{"Loom_Interrupt_Manager",	Construct<Loom_Interrupt_Manager>,	ModuleSortType::InterruptManager },

	// SleepManager
	{"Loom_Sleep_Manager",		Construct<Loom_Sleep_Manager>,	ModuleSortType::SleepManager },
	
	// Multiplexer
	{"Loom_Multiplexer",		Construct<Loom_Multiplexer>,	ModuleSortType::Multiplexer },
	
	// Actuator
	{"Loom_Neopixel",			Construct<Loom_Neopixel>,		ModuleSortType::Actuator },
	{"Loom_Relay",				Construct<Loom_Relay>,			ModuleSortType::Actuator },
	{"Loom_Servo",				Construct<Loom_Servo>,			ModuleSortType::Actuator },
	{"Loom_Stepper",			Construct<Loom_Stepper>,		ModuleSortType::Actuator },
		
	// // CommPlat
	{"Loom_LoRa",				Construct<Loom_LoRa>,			ModuleSortType::CommPlat },
	{"Loom_nRF",				Construct<Loom_nRF>,			ModuleSortType::CommPlat },
	{"Loom_Bluetooth",			Construct<Loom_Bluetooth>,		ModuleSortType::CommPlat },
	
	// InternetPlat
	{"Loom_Ethernet",			Construct<Loom_Ethernet_I>,		ModuleSortType::InternetPlat },

	// PublishPlat
	{"Loom_GoogleSheets",		Construct<Loom_GoogleSheets>,	ModuleSortType::PublishPlat },

	// LogPlat
	{"Loom_OLED",				Construct<Loom_OLED>,			ModuleSortType::LogPlat },
	{"Loom_SD",					Construct<Loom_SD>,				ModuleSortType::LogPlat },
	
	// Rtc
	{"Loom_DS3231",				Construct<Loom_DS3231>,			ModuleSortType::Rtc },
	{"Loom_PCF8523",			Construct<Loom_PCF8523>,		ModuleSortType::Rtc },
	
	// Sensor
	{"Loom_Analog",				Construct<Loom_Analog>,			ModuleSortType::Sensor },
	{"Loom_Digital",			Construct<Loom_Digital>,		ModuleSortType::Sensor },
    {"Loom_SDS011",             Construct<Loom_SDS011>,         ModuleSortType::Sensor },
    {"Loom_K30",                Construct<Loom_K30>,            ModuleSortType::Sensor },
	{"Loom_AS7262",				Construct<Loom_AS7262>,			ModuleSortType::Sensor },
	{"Loom_AS7263",				Construct<Loom_AS7263>,			ModuleSortType::Sensor },
	{"Loom_AS7265X",			Construct<Loom_AS7265X>,		ModuleSortType::Sensor },
	{"Loom_FXAS21002",			Construct<Loom_FXAS21002>,		ModuleSortType::Sensor },
	{"Loom_FXOS8700",			Construct<Loom_FXOS8700>,		ModuleSortType::Sensor },
	{"Loom_LIS3DH",				Construct<Loom_LIS3DH>,			ModuleSortType::Sensor },
	{"Loom_MB1232",				Construct<Loom_MB1232>,			ModuleSortType::Sensor },
	{"Loom_MS5803",				Construct<Loom_MS5803>,			ModuleSortType::Sensor },
	{"Loom_SHT31D",				Construct<Loom_SHT31D>,			ModuleSortType::Sensor },
	{"Loom_TSL2561",			Construct<Loom_TSL2561>,		ModuleSortType::Sensor },
	{"Loom_TSL2591",			Construct<Loom_TSL2591>,		ModuleSortType::Sensor },
	{"Loom_ZXGesture",			Construct<Loom_ZXGesture>,		ModuleSortType::Sensor },
	
	{"Loom_Decagon5TM",			Construct<Loom_Decagon5TM>,		ModuleSortType::Sensor },
	{"Loom_DecagonGS3",			Construct<Loom_DecagonGS3>,		ModuleSortType::Sensor },
	
	{"Loom_MAX31856",			Construct<Loom_MAX31856>,		ModuleSortType::Sensor },

	// Other
	{"Loom_NTP_Sync", 			Construct<LoomNTPSync>,			ModuleSortType::Other }
};


/////////////////////////////////////////////////////////////////////
Factory::Factory() {

}

/////////////////////////////////////////////////////////////////////
Factory::~Factory() {

}

/////////////////////////////////////////////////////////////////////
LoomModule* Factory::Create(JsonVariant module)
{
	const char* name = module["name"];
	JsonVariant p = module["parameters"];

	// LPrintln("Module Name:: ", name);
	// LPrintln("Parameter count:: ", p.size());

	LoomModule* newObject;

	for (auto elem : LookupTable) {
		if ( strcmp(name, elem.name) == 0 ) {
			newObject = elem.Construct(p);
			newObject->print_config();
			return newObject;
		}
	}

	LPrintln("'", name, "' could not be created");
	return nullptr;
}

/////////////////////////////////////////////////////////////////////
void Factory::CreateAndSort(
	JsonVariant module, 
	Loom_Interrupt_Manager*& interrupt_manager, 
	Loom_Sleep_Manager*& sleep_manager,
	LoomRTC*& rtc,
	LoomModule*& other_module, 
	LoomSensor*& sensor,
	LoomActuator*& actuator,
	LoomCommPlat*& comm_plat,
	LoomInternetPlat*& internet_plat,
	LoomPublishPlat*& publish_plat,
	LoomLogPlat*& log_plat
) {		
	LoomModule* tmp = Create(module);
	ModuleSortType sort_to = get_sort_type(module);

	// if (tmp == nullptr) {
	// 	LPrintln("Module could not be created"); 
	// } 

	switch (sort_to) {
		case ModuleSortType::InterruptManager:	interrupt_manager = (Loom_Interrupt_Manager*)tmp; return;
		case ModuleSortType::SleepManager:		sleep_manager = (Loom_Sleep_Manager*)tmp; return;
		case ModuleSortType::Multiplexer:		other_module = tmp; return;
		case ModuleSortType::Actuator:			actuator = (LoomActuator*)tmp; return;
		case ModuleSortType::CommPlat:			comm_plat = (LoomCommPlat*)tmp; return;
		case ModuleSortType::InternetPlat:		internet_plat = (LoomInternetPlat*)tmp; return;
		case ModuleSortType::PublishPlat:		publish_plat = (LoomPublishPlat*)tmp; return;
		case ModuleSortType::LogPlat:			log_plat = (LoomLogPlat*)tmp; return;
		case ModuleSortType::Rtc:				rtc = (LoomRTC*)tmp; return;
		case ModuleSortType::Sensor:			sensor = (LoomSensor*)tmp; return;
		case ModuleSortType::Other:				other_module = tmp; return;
		case ModuleSortType::Unknown:			other_module = tmp; return;
		default: 								other_module = tmp; return;
	}
}

/////////////////////////////////////////////////////////////////////
ModuleSortType Factory::get_sort_type(JsonVariant module) {
	const char* name = module["name"];

	for (auto elem : LookupTable) {
		if ( strcmp(name, elem.name) == 0 ) {
			return elem.sort_type;
		}
	}

	return ModuleSortType::Unknown;
}
