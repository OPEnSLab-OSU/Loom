

#include "_Loom_Preamble.h"




// Do these being pointers rather than global instances slow down startup?


Loom_Analog* 	AS;
Loom_Neopixel*  NP;

Loom_SHT31D*   SH;
// LoomLora*      LR;

// LoomOLED*      OL;
// LoomSD*        SC;

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
	AS->print_config();

	AS->measure();
	AS->print_measurements();


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

	// SC = new LoomSD();
	// SC->print_config();

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
	// DeviceManager.add_module(SC);
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


	// MP->refresh_sensors();
	// MP->print_state();
	// MP->measure();
	// MP->print_measurements();
	// MP->get_sensor_list(&bndl);

	// MP->package(&bndl);

	LOOM_DEBUG_Println("Original:");
	print_bundle(bndl);

	// convert_bundle_structure(&bndl, &bndl2, SINGLEMSG);

	// LOOM_DEBUG_Println("Single");
	// print_bundle(bndl2);

	// convert_bundle_structure(&bndl, &bndl2, MULTIMSG);

	// LOOM_DEBUG_Println("Multi");
	// print_bundle(bndl2);


	// flatten_bundle(bndl);

	// LOOM_DEBUG_Println("Flattened");
	// print_bundle(bndl);

	// LOOM_DEBUG_Println("Array:");
	String key_values[6] = { "a", "12", "b", "3.45", "c", "def" };
	String keys[3] = { "abc", "def", "ghi" };
	char* vals[3] = { "12.3", "45.6", "789" };



	// convert_bundle_to_array_key_value(bndl, key_values, 25);
	// convert_bundle_to_arrays_assoc(bndl, keys, vals, 25);
	LOOM_DEBUG_Println("Keys:");
	print_array(keys, 3);
	LOOM_DEBUG_Println("Vals:");
	print_array(vals, 3);


	LOOM_DEBUG_Println("Convert");
	

	convert_key_value_array_to_bundle(key_values, bndl, "test", 6 );

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


