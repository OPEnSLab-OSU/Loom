

#include "_Loom_Config.h"
#include "_Loom_Preamble.h"




// Do these being pointers rather than global instances slow down startup?


Loom_Analog* 	AS;
// Loom_Neopixel*  NP;

// Loom_SHT31D*   SH;
// Loom_LoRa*      LR;

// Loom_OLED*      OL;
// Loom_SD*        SC;

// Loom_DS3231*     RT;
// Loom_PCF8523*     RT;
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
Loom_Interrupt_Manager* ITM;

 
LoomManager DeviceManager;



volatile byte state = LOW;
void blink() {
	state = !state;
}

void turn_on() {
	state = HIGH;
}

void turn_off() {
	state = LOW;
}



void setup() 
{
	pinMode(LED_BUILTIN, OUTPUT);   // Set the LED pin mode


	Serial.begin(115200);

	while(!Serial);        // Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
	delay(1000);

	Println("Initialized Serial!\n");

	// AnalogManager Asensor;

	Println();


	AS = new Loom_Analog();

	// SLM = new Loom_Sleep_Manager();
	ITM = new Loom_Interrupt_Manager();


	// WI = new Loom_WiFi_I();
	// RL = new Loom_Relay();
	// AS->print_config();

	// AS->measure();
	// AS->print_measurements();


	// Asensor.print_config();

	// NP = new Loom_Neopixel();
	// NP->print_config();


	Println("Adding Components");

	DeviceManager.add_module(AS);
	// DeviceManager.add_module(NP);
	// DeviceManager.add_module(SC);
	// DeviceManager.add_module(MP);
	// DeviceManager.add_module(OL);
	// DeviceManager.add_module(WI);

	// DeviceManager.add_module(MP);
	// DeviceManager.add_module(EI);

	DeviceManager.print_config();


	// DeviceManager.list_modules();


	Println("\n ** Setup Complete ** ");


	// AS->load_config();
	// AS->save_config();

	// Println2("Connected : ", EI->is_connected());
	// Println2("Time      : ", EI->get_time());
	// Println2("Time      : ", EI->get_time());

	

	// Println()

	// while(1);

	// delay(2000);
	digitalWrite(LED_BUILTIN, HIGH);  



	ITM->register_interrupt_ISR( {9, INT_CHANGE, blink } );
	ITM->register_interrupt_ISR( {10, INT_LOW, turn_on } );
	ITM->register_interrupt_ISR( {11, INT_LOW, turn_off } );

}

int i = 0;


void loop() 
{
	// OSCBundle bndl;
	// OSCBundle bndl2;

	// SH->measure();
	// SH->package(&bndl);

	// DeviceManager.measure();
	// AS->print_measurements();
	// DeviceManager.package(bndl);

	// print_bundle(bndl);
	// OL->log_bundle(bndl);

	digitalWrite(LED_BUILTIN, state);

	// SLM->sleep_for_time( TimeSpan(0,0,0,12) );
	// digitalWrite(LED_BUILTIN, HIGH);
	// delay(5000);
	// Println2("I: ", i++);





	// Println("\nDone");
	// while(1);



}


