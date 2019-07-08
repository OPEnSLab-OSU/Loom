
#include <Loom.h>

#include "LowPower.h"



// Include configuration
const char* json_config = 
#include "config.h"
;

#define ALARM_PIN 6
#define REED_PIN 11


volatile bool alarmFlag = false;
volatile bool reedFlag = false;


void alarmISR() { 
	detachInterrupt(digitalPinToInterrupt(ALARM_PIN)); 
	// detachInterrupt(digitalPinToInterrupt(REED_PIN)); 
	alarmFlag = true;
	// LPrintln("\n\n", "Alarm went off", "\n");
}
void reedISR() { 
	// detachInterrupt(digitalPinToInterrupt(ALARM_PIN)); 
	detachInterrupt(digitalPinToInterrupt(REED_PIN));
	reedFlag = true;
	// LPrintln("\n\n", "Reed switch triggered", "\n"); 
}



LoomManager Manager("Manager", "Loom", 1, 1, DeviceType::NODE, Verbosity::V_HIGH, Verbosity::V_LOW);



void setup() 
{
	pinMode(LED_BUILTIN, OUTPUT);   // Set the LED pin mode

	pinMode(10, OUTPUT);   

	digitalWrite(LED_BUILTIN, LOW);

	for (int i = 0; i < 10; i++) {
		digitalWrite(LED_BUILTIN, LOW);
		delay(200);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(200);
	}


	Serial.begin(115200);
	// while(!Serial);       			// Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
	delay(1000);

	LPrintln("Initialized Serial!\n");

	Manager.parse_config(json_config);
	Manager.print_config();

	pinMode(ALARM_PIN, OUTPUT);
	pinMode(REED_PIN, OUTPUT);

	Manager.InterruptManager().register_ISR(ALARM_PIN, alarmISR, LOW, ISR_Type::IMMEDIATE);
	Manager.InterruptManager().register_ISR(REED_PIN, reedISR, LOW, ISR_Type::IMMEDIATE);

	LPrintln("\n ** Setup Complete ** ");

}






void loop() 
{


	LPrintln("AlarmFlag : ", alarmFlag);
	LPrintln("ReedFlag  : ", reedFlag);

	digitalWrite(LED_BUILTIN, HIGH);

	digitalWrite(10, HIGH);
	delay(200);

	Manager.measure();

	digitalWrite(10, LOW);


	JsonObject tmp = Manager.package();


	JsonObject wakeType = tmp["contents"].createNestedObject();
	wakeType["module"] = "wakeType";
	wakeType.createNestedObject("data")["type"] = alarmFlag ? "alarm" : "reed";

	serializeJsonPretty(tmp, Serial);

	Manager.SDCARD().log(tmp);

	
	delay(500);
	Manager.InterruptManager().register_ISR(ALARM_PIN, alarmISR, LOW, ISR_Type::IMMEDIATE);
	Manager.InterruptManager().register_ISR(REED_PIN, reedISR, LOW, ISR_Type::IMMEDIATE);
	Manager.InterruptManager().RTC_alarm_duration(TimeSpan(600));

	// delay(4000);

	alarmFlag = false;
	reedFlag = false;

	// Go to sleep
	Manager.SleepManager().sleep();
	// 
	
}
