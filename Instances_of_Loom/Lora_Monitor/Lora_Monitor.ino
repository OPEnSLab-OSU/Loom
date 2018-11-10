// ================================================================
// ===              INCLUDE CONFIGURATION FILE                  ===
// ===    INCLUDE DECLARATIONS, STRUCTS, AND FUNCTIONS FROM     ===
// ===            OTHER FILES AS SET IN CONFIG.H                ===
// ================================================================

// Config has to be first has it hold all user specified options
#include "config.h"


//#define hub_node_type 1
// Preamble includes any relevant subroutine files based 
// on options specified in the above config
#include "loom_preamble.h"



#define HUB_ID "/EVAP2/EvapHub0/Ping"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET A2
#define pingDelay  2

Adafruit_SSD1306 display(OLED_RESET);
unsigned long pingMillis, lastReceived; 

// ================================================================ 
// ===                           SETUP                          ===
// ================================================================ 
void setup() 
{
	// LOOM_begin calls any relevant (based on config) LOOM device setup functions
	Loom_begin();	

	// Any custom setup code
	pingMillis = millis()+5000;
	lastReceived = millis()-5000;


	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
	display.display();
	delay(1000);
	display.clearDisplay();
}

// ================================================================ 
// ===                        MAIN LOOP                         ===
// ================================================================ 
void loop() 
{
	OSCBundle bndl, send_bndl;

	// Send pings on 
	if (millis() - pingMillis >= (1000*pingDelay)) {
		send_bndl.add(HUB_ID); // name of hub device, not this device
		print_bundle(&send_bndl);
		send_bundle(&send_bndl, LORA);
		send_bndl.empty();
		pingMillis = millis();
	} 
	
	receive_bundle(&bndl, LORA);
	if (bndl.size()) {
		print_bundle(&bndl);

		if ( strstr(get_address_string(bndl.getOSCMessage(0)).c_str(), "PingResponse") ) {
			// Update display
			display.clearDisplay();
			display.setTextColor(WHITE);
			display.setTextSize(2);
			display.setCursor(0,0);
			
			display.print("RSSI: ");
			display.println(rf95.lastRssi());
			
			display.setCursor(0, 16);
			display.print("HUB : ");
			display.println(  (get_data_value( bndl.getOSCMessage(0), 0)).c_str()  );


			display.setTextSize(1);

			if (rf95.lastRssi() > -120) {
				display.setCursor(120, 0);
				display.print("*");
			}

			if (rf95.lastRssi() > -90) {
				display.setCursor(120, 8);
				display.print("*");
			}

			if (rf95.lastRssi() > -60) {
				display.setCursor(120, 16);
				display.print("*");
			}

			if (rf95.lastRssi() > -40) {
				display.setCursor(120, 24);
				display.print("*");
			}
			
			
			display.display();	
		}

		lastReceived = millis();

	}

	if (millis() - lastReceived >= (5000)) {
		display.clearDisplay();
		display.setTextColor(WHITE);
		display.setTextSize(2);
		display.setCursor(0,0);
		display.print("Out of\nRange :(");
//		lastReceived = millis();
		display.display();	

	} 

	delay(50);
}
