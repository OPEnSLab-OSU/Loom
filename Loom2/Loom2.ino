
// Preamble will only need to include leafs in the heirarchy
// abstract (noon-leaves) already included by leaves

#include <OSCBundle.h>

#include "Loom_Macros.h"


#include "Loom_Manager.h"
#include "Loom_Module.h"


#include "Loom_Analog.h"
// #include "Loom_Sensor.h"

// #include "Loom_Actuator.h"
#include "Loom_Neopixel.h"

// #include "Loom_I2C_Sensor.h"
#include "Loom_SHT31D.h"
#include "Loom_TSL2591.h"

// #include "Loom_CommPlat.h"
#include "Loom_LoRa.h"

// #include "Loom_LogPlat.h"
#include "Loom_OLED.h"
#include "Loom_SD.h"

// #include "Loom_RTC.h"
#include "Loom_DS3231.h"
#include "Loom_PCF8523.h"

// #include "Loom_InternetPlat.h"

#include "Loom_Multiplexer.h"


// #include "Loom_InternetPlat.h"
#include "Loom_InternetEthernet.h"








// Do these being pointers rather than global instances slow down startup?


Loom_Analog* 	AS;
Loom_Neopixel*      NP;

// Loom_SHT31D*   SH;
// LoomLora*      LR;

// LoomOLED*      OL;
// LoomSD*        SC;

// Loom_DS3231*     RT;
// Loom_PCF8523*     RT;

Loom_Multiplexer* 	MP;
// LoomModule* 	MP;

// LoomCommPlat* LR;

Loom_Ethernet_I* EI;
 
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

	// MP = new Loom_Multiplexer();
	// MP->print_config();

	// MP->print_state();

	// EI = new Loom_Ethernet_I();

	// DeviceManager = new LoomManager();

	LOOM_DEBUG_Println("Adding Components");

	DeviceManager.add_module(AS);
	// DeviceManager.add_module(NP);
	// DeviceManager.add_module(SC);
	// DeviceManager.add_module(OL);
	// DeviceManager.add_module(LR);
// 
	// DeviceManager.add_module(MP);
	// DeviceManager.add_module(EI);


	DeviceManager.print_config();


	// DeviceManager.list_modules();


	LOOM_DEBUG_Println("\n ** Setup Complete ** ");


	// AS->load_config();
	// AS->save_config();

	LOOM_DEBUG_Println2("Connected : ", EI->is_connected());
	LOOM_DEBUG_Println2("Time      : ", EI->get_time());
	LOOM_DEBUG_Println2("Time      : ", EI->get_time());
	

	while(1);

	delay(2000);

}

int b = 0;

void loop() 
{
	OSCBundle bndl;




	// LOOM_DEBUG_Println(b++);

	// MP->refresh_sensors();
	// MP->print_state();
	// MP->measure();
	// MP->print_measurements();

	// MP->package(&bndl);
	// print_bundle(&bndl);

	// delay(1000);

	// RT->print_time();
	// RT->package(&bndl, "prefix");
	// append_to_bundle_key_value(&bndl, "other", "Weekday", RT->get_weekday());

	// LOOM_DEBUG_Println("SENSOR LIST");
	// bndl.empty();
	// MP->get_sensor_list(&bndl);
	// print_bundle(&bndl);


	// MP->measure();
	// bndl.empty();
	// MP->package(&bndl);
	// LOOM_DEBUG_Println("MUX PACKAGE");
	// print_bundle(&bndl);


	// bndl.empty();	
	// AS->package(&bndl);
	// LOOM_DEBUG_Println("ANALOG PACKAGE");
	// print_bundle(&bndl);

	// while(1);


	// DeviceManager.measure();
	// DeviceManager.package(&bndl);

	// // LOOM_DEBUG_Println("Bundle 1:");
	// print_bundle(&bndl);

	// SC->log_bundle(&bndl);
// 
	// OL->log_bundle(&bndl);



	// while(1);

	// AS->measure();
	// DeviceManager.measure();

	// // LOOM_DEBUG_Println("\nBuild Bundle");
	// DeviceManager.package(&bndl);
// 
	// print_bundle(&bndl);

	// NP->set_color(2, 0, 0, b++, 0);
// 

	// NP->set_color(2, 0, 0, 0, AS->get_analog_val(0)*255/(4096) );

	// while(1);


}





	// OSCBundle b[12];
	// int tmp;
	// char buf[50];

	// b[0].add("/D/Loom/1/LoomManager/1/data");
	// b[1].add("/D/Loom/1/Ishield/1/data");
	// b[2].add("/D/Loom/1/LoomManager/2/data");

	// b[3].add("/S/Loom/1/LoomManager/1/data");
	// b[4].add("/S/Loom/2/Ishield/1/data");
	// b[5].add("/S/Other/1/data");

	// b[6].add("/F/Loom/1/LoomManager/1/data");
	// b[7].add("/F/Loom/data");
	// b[8].add("/F/Other/1/data");

	// // LOOM_DEBUG_Println("\n\n[SCOPE_DEVICE]\n");
	// // LR->set_subnet_scope(SCOPE_DEVICE);
	// for (int i = 0; i < 9; i++) {
	// 	LOOM_DEBUG_Println3("[", i, "] ");
	// 	LR->scope_filter(&b[i], &tmp);
	// 	LOOM_DEBUG_Println2("TMP: ", tmp);

	// 	if (tmp) {
	// 		b[i].getOSCMessage(0)->getAddress(buf, tmp);
	// 		LOOM_DEBUG_Println2("CMD: ", buf);
	// 	}
	// }


