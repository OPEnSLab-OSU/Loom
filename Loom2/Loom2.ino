
#include "loom_macros.h"

// // enum Verbosity { VERB_OFF, VERB_LOW, VERB_HIGH }; // Move back to "loom_abstract_module_class.h"
// enum CommScope { SCOPE_DEVICE, SCOPE_SUBNET, SCOPE_FAMILY, SCOPE_SUBNET_ONLY, SCOPE_FAMILY_ONLY, SCOPE_ANY }; // Move back to "loom_comm_plat_class.h"
// // enum DeviceType { HUB, NODE, REPEATER };
// enum LogPlatform { SDCARD, PUSHINGBOX, OLED, SERIAL_MON };
// enum OLED_Version { FEATHERWING, BREAKOUT, OTHER };
// enum OLED_Format { OLED_4, OLED_8, OLED_SCROLL };
// enum OLED_Freeze { FREEZE_DISABLE, FREEZE_DATA, FREEZE_SCROLL };
// enum BundleStructure { SINGLEMSG, MULTIMSG };


#include <OSCBundle.h>

#include "loom_device_class.h"
#include "loom_abstract_module_class.h"

#include "loom_sensor_class.h"
#include "loom_sensor_analog_class.h"

#include "loom_actuator_class.h"
#include "loom_neopixel_class.h"

#include "loom_sensor_i2c_class.h"
#include "loom_sht31d_class.h"

#include "loom_comm_plat_class.h"
#include "loom_lora_class.h"

#include "loom_logging_plat_class.h"
#include "loom_oled_class.h"
#include "loom_sd_class.h"




// AnalogManager Asensor;

// Do these being pointers rather than global instances slow down startup?

// AnalogManager Asensor;

AnalogManager* AS;
Neopixel*      NP;

// // Neopixel      NP;

// Loom_SHT31D*   SH;
// LoomLora*      LR;

// LoomOLED*      OL;
// LoomSD*        SC;

LoomCommPlat* LR;

LoomDevice DeviceManager;

void setup() 
{
	Serial.begin(115200);

	while(!Serial);        // Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
	delay(1000);

	LOOM_DEBUG_Println("Initialized Serial!\n");

	// AnalogManager Asensor;

	LOOM_DEBUG_Println();


	AS = new AnalogManager();
	AS->print_config();

	// LOOM_DEBUG_Println("Change Verbosity");
	// LOOM_DEBUG_Println( AS->get_print_verbosity() );
	// AS->set_print_verbosity(VERB_HIGH);
	// AS->print_config();


	// Asensor.print_config();

	// NP = new Neopixel();
	// NP->print_config();

	// NP->set_color(2, 0, 100, 0, 150);

	// SH = new Loom_SHT31D();
	// SH->print_config();

	LR = new LoomLora();
	LR->print_config();


	// OL = new LoomOLED();
	// OL->print_config();

	// SC = new LoomSD();
	// SC->print_config();

	// DeviceManager = new LoomDevice();

	LOOM_DEBUG_Println("Adding Components");

	DeviceManager.add_module(AS);
	// DeviceManager.add_module(NP);
	// DeviceManager.add_module(SC);
	// DeviceManager.add_module(OL);
	DeviceManager.add_module(LR);



	DeviceManager.print_config();



	// DeviceManager.list_modules();


	LOOM_DEBUG_Println("\n ** Setup Complete ** ");


	// AS->load_config();
	// AS->save_config();

	// while(1);

	delay(2000);

}

int b = 0;

void loop() 
{
	OSCBundle bndl;





	while(1);


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

	// print_bundle(&bndl);

	// NP->set_color(2, 0, 0, b++, 0);
// 

	// NP->set_color(2, 0, 0, 0, AS->get_analog_val(0)*255/(4096) );

	// while(1);

	delay(1000);

}





	// OSCBundle b[12];
	// int tmp;
	// char buf[50];

	// b[0].add("/D/Loom/1/LoomDevice/1/data");
	// b[1].add("/D/Loom/1/Ishield/1/data");
	// b[2].add("/D/Loom/1/LoomDevice/2/data");

	// b[3].add("/S/Loom/1/LoomDevice/1/data");
	// b[4].add("/S/Loom/2/Ishield/1/data");
	// b[5].add("/S/Other/1/data");

	// b[6].add("/F/Loom/1/LoomDevice/1/data");
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


