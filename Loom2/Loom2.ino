
#include "_Loom_Preamble.h"


// Do these being pointers rather than global instances slow down startup?


Loom_Analog* 	AS;
// Loom_Neopixel*  NP;

Loom_SHT31D*   SH;
// Loom_Lora*      LR;

// Loom_OLED*      OL;
// Loom_SD*        SC;

// Loom_DS3231*     RT;
// Loom_PCF8523*     RT;

Loom_Multiplexer* 	MP;
// LoomModule* 	MP;

// LoomCommPlat* LR;

// Loom_Ethernet_I* EI;
// Loom_Relay* RL;

Loom_WiFi_I* WI;

Loom_Sleep_Manager* SLM;

 
LoomManager DeviceManager;

void setup() 
{
	Serial.begin(115200);

	while(!Serial);        // Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
	delay(1000);

	Println("Initialized Serial!\n");

	// AnalogManager Asensor;

	Println();


	AS = new Loom_Analog();

	SLM = new Loom_Sleep_Manager();

	// WI = new Loom_WiFi_I();
	// RL = new Loom_Relay();
	// AS->print_config();

	// AS->measure();
	// AS->print_measurements();


	// Asensor.print_config();

	// NP = new Loom_Neopixel();
	// NP->print_config();


	// SH = new Loom_SHT31D();
	// SH->print_config();

	// LR = new LoomLora();
	// LR->print_config();

	// OL = new Loom_OLED();
	// OL->print_config();

	// SC = new Loom_SD();
	// SC->print_config();

	// RT = new Loom_DS3231();
	// RT = new Loom_PCF8523();
	// RT->print_config();

	// MP = new Loom_Multiplexer();
	// MP->print_config();

	// MP->print_state();

	// EI = new Loom_Ethernet_I();

	// DeviceManager = new LoomManager();

	Println("Adding Components");

	DeviceManager.add_module(AS);
	// DeviceManager.add_module(NP);
	// DeviceManager.add_module(SC);
	// DeviceManager.add_module(MP);
	// DeviceManager.add_module(OL);
	// DeviceManager.add_module(WI);


	DeviceManager.print_config();


	// DeviceManager.list_modules();


	Println("\n ** Setup Complete ** ");


	// AS->load_config();
	// AS->save_config();

	// Println2("Connected : ", EI->is_connected());
	// Println2("Time      : ", EI->get_time());
	// Println2("Time      : ", EI->get_time());

}

int b = 0;

void loop() 
{
	OSCBundle bndl;
	OSCBundle bndl2;

	// SH->measure();
	// SH->package(&bndl);

	DeviceManager.measure();
	// AS->print_measurements();
	DeviceManager.package(bndl);

	print_bundle(bndl);
	// OL->log_bundle(bndl);

	// WI->print_state();

	// MP->refresh_sensors();
	// MP->print_state();
	// MP->measure();
	// MP->print_measurements();
	// MP->get_sensor_list(&bndl);

	// MP->package(&bndl);

	// print_bundle(bndl);
	delay(1000);

	// Println("\nDone");




}


