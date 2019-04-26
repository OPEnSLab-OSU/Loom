
#include <Loom.h>



LoomManager DeviceManager("Manager", "Loom", 1, 1, DeviceType::NODE, Verbosity::V_HIGH, Verbosity::V_LOW);

// Loom_Sleep_Manager* SLM;
Loom_Interrupt_Manager* ITM;

 


//volatile int i = 0;
//
//
//void wake() {}
//void turn_on()  { digitalWrite(LED_BUILTIN, HIGH); Println("ON"); }
//void turn_off() { digitalWrite(LED_BUILTIN, LOW); Println("OFF"); }
//void toggle() { digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); Println("SWITCH"); }
//void print_ISR() { Println("ISR"); }
//void inc_I() { i++; }
//void timer_print0() { Println2("Timer 0: ", millis()); }
//void timer_print1() { Println2("Timer 1: ", millis()); }
//void button() { Println("Button"); }
//
//
//int timer_2_count = 3;
//bool clear_flag = true;
//void timer_print2() {
//	Println2("Timer 2: ", millis());
//	timer_2_count--;
//	Println3("\t", "timer_2_count:", timer_2_count);
//}

void setup() 
{
	pinMode(LED_BUILTIN, OUTPUT);   // Set the LED pin mode

	Serial.begin(115200);
	while(!Serial);       			// Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
	delay(1000);

	Println("Initialized Serial!\n");

//	DeviceManager.parse_config();
//	DeviceManager.print_config();

	// pinMode(5, INPUT_PULLUP);
	// pinMode(6, INPUT_PULLUP);

	// SLM = new Loom_Sleep_Manager();
	// ITM = new Loom_Interrupt_Manager();
	// AS  = new Loom_Analog();
	// DI  = new Loom_Digital();
	// PCF = new Loom_PCF8523();
	// TC  = new Loom_MAX31856();



	// DI->print_config();


	// Println("Adding Components");
	// DeviceManager.add_module(ITM);
	// DeviceManager.add_module(AS);
	// // DeviceManager.add_module(DI);
	// // DeviceManager.add_module(PCF);
	// // DeviceManager.add_module(TC);

	// DeviceManager.print_config();



	// SLM->set_RTC_module(RT);
	// ITM->set_RTC_module(RT);
	// SLM->set_Interrupt_Manager(ITM);
	// SLM->print_config();



	// DeviceManager.list_modules();

	// ITM->register_ISR( 5, toggle, CHANGE, true ); // switch
	// ITM->register_ISR( 6, button, FALLING, true ); // button

	// ITM->register_timer(0, 5000, timer_print0, true);
	// ITM->register_timer(1, 3000, timer_print1, false);
	// ITM->register_timer(2, 7000, timer_print2, true);

	// ITM->print_config();


// 	ITM->RTC_alarm_relative(0, 0, 0, 9);
// 	// ITM->RTC_alarm_relative(0, 0, 1, 10);
// 	// SLM->sleep_for_time(0, 0, 0, 10);

// 	// Println("Alarm Set");

// 	// for (auto i = 0; i < 20; i++) {
// 	// 	Println(i);
// 	// 	delay(1000);
// 	// }

// 	SLM->sleep();

	// digitalWrite(LED_BUILTIN, LOW);

	Println("\n ** Setup Complete ** ");

	// delay(60000);
	// while(1);
}



void loop() 
{
	OSCBundle bndl;

	// ITM->run_pending_ISRs();

//	DeviceManager.measure();
//	DeviceManager.package(bndl);
//	print_bundle(bndl);
//
//	delay(3000);



	// if ((timer_2_count <= 0) && (clear_flag)) {
	// 	ITM->clear_timer(2);
	// 	clear_flag = false;
	// }


	// SLM->sleep_for_time( TimeSpan(0,0,0,12) );
	// digitalWrite(LED_BUILTIN, HIGH);
	// delay(5000);
	// Println2("I: ", i++);
}
