

#include "ads1231.h"


void setup()
{	
	Serial.begin(115200);
	while(!Serial);        // Ensure Serial is ready to go before anything happens in LOOM_DEBUG mode.
	Serial.println("Initialized Serial!\n");

	ads1231_init();

	Serial.println("Finished ADS1231 Init");



}


long raw_val;
int  grams;
int  stable_grams;
int  grams_no_block;
int  average_grams;

void loop()
{
	ads1231_powerup();

	for (int i= 0; i < 3; i++) {
		// ads1231_get_value(raw_val);
		// Serial.print("Raw Val: ");
		// Serial.println(raw_val);

		ads1231_get_grams(grams);
		Serial.print("Grams: ");
		Serial.println(grams);

		ads1231_get_average_grams(average_grams);
		Serial.print("Average Grams: ");
		Serial.println(average_grams);
		
		ads1231_get_stable_grams(stable_grams);
		Serial.print("Stable Grams: ");
		Serial.println(stable_grams);

		Serial.println();
		Serial.println();	

		delay(500);
	}
	
	ads1231_powerdown();

	for (int i =0; i < 2; i++) {
		// ads1231_get_value(raw_val);
		// Serial.print("Raw Val: ");
		// Serial.println(raw_val);

		ads1231_get_grams(grams);
		Serial.print("Grams: ");
		Serial.println(grams);

		ads1231_get_average_grams(average_grams);
		Serial.print("Average Grams: ");
		Serial.println(average_grams);
		
		// ads1231_get_stable_grams(stable_grams);
		// Serial.print("Stable Grams: ");
		// Serial.println(stable_grams);

		Serial.println();
		Serial.println();	

		delay(500);
	}

	delay(1000);
}