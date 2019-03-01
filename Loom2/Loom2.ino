

#include "_Loom_Config.h"
#include "_Loom_Preamble.h"


#include <Adafruit_SleepyDog.h>


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

int i = 0;



volatile byte state = LOW;
void blink() {
	// state = !state;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

}

void turn_on() {
	state = HIGH;
}

void turn_off() {
	state = LOW;
}


void print_ISR() {
	Println("ISR");
}

void inc_I() {
	i++;
}

void setup() 
{
	pinMode(LED_BUILTIN, OUTPUT);   // Set the LED pin mode

	digitalWrite(LED_BUILTIN, HIGH);  


	Serial.begin(115200);

	while(!Serial);        // Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
	delay(1000);

	Println("Initialized Serial!\n");

	// AnalogManager Asensor;

	Println();


	AS = new Loom_Analog();

	SLM = new Loom_Sleep_Manager();
	ITM = new Loom_Interrupt_Manager();

	SLM->print_config();


	// WI = new Loom_WiFi_I();
	// RL = new Loom_Relay();
	// AS->print_config();


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




	// AS->load_config();
	// AS->save_config();

	// Println2("Connected : ", EI->is_connected());
	// Println2("Time      : ", EI->get_time());
	// Println2("Time      : ", EI->get_time());

	digitalWrite(LED_BUILTIN, HIGH);  


	// ITM->register_ISR( 9, blink, CHANGE, true );
	// ITM->register_ISR( 10, turn_on, LOW, true );
	// ITM->register_ISR( 11, turn_off, LOW, true );
	// ITM->register_ISR( 12, print_ISR, LOW, false );
	// ITM->register_ISR( 6, inc_I, LOW, false );

	// SLM->sleep();


	int countdownMS = Watchdog.enable(4000);
	Print("Enabled the watchdog with max countdown of ");
	Print(countdownMS);
	Println(" milliseconds!");
	Println();


	Println("\n ** Setup Complete ** ");


	delay(6000);

	Println("Done");
	while(1);

}



void loop() 
{

	// ITM->run_ISR_bottom_halves();




	// digitalWrite(LED_BUILTIN, state);

	// delay(50);


	// OSCBundle bndl;
	// OSCBundle bndl2;

	// SH->measure();
	// SH->package(&bndl);

	// DeviceManager.measure();
	// DeviceManager.package(bndl);



	// SLM->sleep_for_time( TimeSpan(0,0,0,12) );
	// digitalWrite(LED_BUILTIN, HIGH);
	// delay(5000);
	// Println2("I: ", i++);



	// Println("\nDone");
	// while(1);

}


