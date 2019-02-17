

#include "_Loom_Preamble.h"




// Do these being pointers rather than global instances slow down startup?


Loom_Analog* 	AS;
Loom_Neopixel*  NP;

Loom_SHT31D*   SH;
// Loom_Lora*      LR;

// Loom_OLED*      OL;
Loom_SD*        SC;

// Loom_DS3231*     RT;
// Loom_PCF8523*     RT;

Loom_Multiplexer* 	MP;
// LoomModule* 	MP;

// LoomCommPlat* LR;

// Loom_Ethernet_I* EI;
 
LoomManager DeviceManager;

void setup() 
{
	Serial.begin(115200);

	while(!Serial);        // Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
	delay(1000);

	LOOM_DEBUG_Println("Initialized Serial!\n");

	// AnalogManager Asensor;

	LOOM_DEBUG_Println();


	AS = new Loom_Analog();
	// AS->print_config();

	// AS->measure();
	// AS->print_measurements();


	// Asensor.print_config();

	// NP = new Loom_Neopixel();
	// NP->print_config();

	// NP->set_color(2, 0, 100, 0, 150);

	// SH = new Loom_SHT31D();
	// SH->print_config();

	// LR = new LoomLora();
	// LR->print_config();

	// OL = new LoomOLED();
	// OL->print_config();

	SC = new Loom_SD();
	SC->print_config();

	// RT = new Loom_DS3231();
	// RT = new Loom_PCF8523();
	// RT->print_config();

	MP = new Loom_Multiplexer();
	MP->print_config();

	// MP->print_state();

	// EI = new Loom_Ethernet_I();

	// DeviceManager = new LoomManager();

	LOOM_DEBUG_Println("Adding Components");

	DeviceManager.add_module(AS);
	// DeviceManager.add_module(NP);
	DeviceManager.add_module(SC);
	// DeviceManager.add_module(OL);
	// DeviceManager.add_module(LR);
	// DeviceManager.add_module(SH);

// 
	DeviceManager.add_module(MP);
	// DeviceManager.add_module(EI);


	DeviceManager.print_config();


	// DeviceManager.list_modules();


	LOOM_DEBUG_Println("\n ** Setup Complete ** ");


	// AS->load_config();
	// AS->save_config();

	// LOOM_DEBUG_Println2("Connected : ", EI->is_connected());
	// LOOM_DEBUG_Println2("Time      : ", EI->get_time());
	// LOOM_DEBUG_Println2("Time      : ", EI->get_time());
	

	// LOOM_DEBUG_Println()

	// while(1);

	// delay(2000);

}

int b = 0;

void loop() 
{
	OSCBundle bndl;
	OSCBundle bndl2;

	// SH->measure();
	// SH->package(&bndl);

	DeviceManager.measure();
	DeviceManager.package(bndl);


	// SC->log_bundle(bndl);

	// MP->refresh_sensors();
	// MP->print_state();
	// MP->measure();
	// MP->print_measurements();
	// MP->get_sensor_list(&bndl);

	// MP->package(&bndl);

	print_bundle(bndl);


	LOOM_DEBUG_Println("\nDone");
	while(1);

	// delay(1000);

	// RT->print_time();
	// RT->package(&bndl, "prefix");
	// append_to_bundle_key_value(&bndl, "other", "Weekday", RT->get_weekday());

	// LOOM_DEBUG_Println("SENSOR LIST");
	// bndl.empty();
	// MP->get_sensor_list(&bndl);
	// print_bundle(bndl);


	// MP->measure();
	// bndl.empty();
	// MP->package(&bndl);
	// LOOM_DEBUG_Println("MUX PACKAGE");
	// print_bundle(bndl);


	// bndl.empty();	
	// AS->package(&bndl);
	// LOOM_DEBUG_Println("ANALOG PACKAGE");
	// print_bundle(bndl);

	// while(1);


	// DeviceManager.measure();
	// DeviceManager.package(&bndl);

	// // LOOM_DEBUG_Println("Bundle 1:");
	// print_bundle(bndl);

	// SC->log_bundle(&bndl);
// 
	// OL->log_bundle(&bndl);



}


