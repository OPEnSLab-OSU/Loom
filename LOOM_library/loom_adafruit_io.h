// ================================================================
// ===                        LIBRARIES                         ===
// ================================================================
#include <SPI.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFi101.h> 				// See if I can abstract this to WiFi OR Ethernet 
#include "Adafruit_NeoPixel.h"

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
// struct config_<module>_t config_<module>;
// struct state_<module>_t state_<module>;

Adafruit_MQTT_Client mqtt(&wifiClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish   photocell   = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/photocell");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/toggle");

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_adafruitio();
void adafruitio_subscribe();
void adafruitio_publish();
void MQTT_connect();
//void package_<module>(OSCBundle *, char[]);
//void measure_<module>();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any startup for <module> that should occur on device startup
// 
// @return  Whether or not sensor initialization was successful
//
bool setup_adafruitio() 
{
	mqtt.subscribe(&onoffbutton); 		// Try to make this a list defined in config, iterate through and subscribe to each

}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


void adafruitio_subscribe()
{
	// Ensure the connection to the MQTT server is alive (this will make the first
	// connection and automatically reconnect when disconnected).  See the MQTT_connect
	// function definition further below.
	MQTT_connect();

	// this is our 'wait for incoming subscription packets' busy subloop
	Adafruit_MQTT_Subscribe *subscription;
	while ((subscription = mqtt.readSubscription(5000))) {
		if (subscription == &onoffbutton) {
			Serial.print(F("Got: "));
			Serial.println((char *)onoffbutton.lastread);

			if (0 == strcmp((char *)onoffbutton.lastread, "OFF")) {
				digitalWrite(led, LOW);
//				pixels.setPixelColor(0, pixels.Color(0, 0, 0));
//				pixels.show();
			}
			if (0 == strcmp((char *)onoffbutton.lastread, "ON")) {
				digitalWrite(led, HIGH);
//				pixels.setPixelColor(0, pixels.Color(50, 50, 50));
//				pixels.show();
			}
		}
	}
}

void adafruitio_publish()
{
	MQTT_connect();
	
//	int photo_val = analogRead(SENSOR_PIN);
//
//	LOOM_DEBUG_Print(F("\nSending photocell val "));
//	LOOM_DEBUG_Print(photo_val);
//	LOOM_DEBUG_Print("...");
//	if (! photocell.publish(photo_val)) {
//		LOOM_DEBUG_Println(F("Failed"));
//	} else {
//		LOOM_DEBUG_Println(F("OK!"));
//	}

}




// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
	int8_t ret;

	// attempt to connect to Wifi network:
	// while (WiFi.status() != WL_CONNECTED) {
	// 	Serial.print("Attempting to connect to SSID: ");
	// 	Serial.println(ssid);
	// 	// Connect to WPA/WPA2 network. Change this line if using open or WEP network:
	// 	status = WiFi.begin(ssid, pass);

	// 	// wait 10 seconds for connection:
	// 	uint8_t timeout = 10;
	// 	while (timeout && (WiFi.status() != WL_CONNECTED)) {
	// 		timeout--;
	// 		delay(1000);
	// 	}
	// }
	
	// Stop if already connected.
	if (mqtt.connected()) {
		return;
	}

	LOOM_DEBUG_Print("Connecting to MQTT... ");

	// int i = 0;
	while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
		LOOM_DEBUG_Println(mqtt.connectErrorString(ret));
		LOOM_DEBUG_Println("Retrying MQTT connection in 2 seconds...");
		mqtt.disconnect();
		delay(2000);  // wait 1 second
	}
	LOOM_DEBUG_Println("MQTT Connected!");
}





// --- PACKAGE <MODULE> ---
//
// Adds OSC Message of most recent sensor readings to a provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
//   if I2C multiplexer sensor, then also
//
// void package_<module>(OSCBundle *bndl, char packet_header_string[]) 
// {

// }



// --- MEASURE <MODULE> ---
//
// Gets the current sensor readings of the <Module and stores into variables
// If debug prints are enabled, can also print any relevant information 
//
// void measure_<module>() 
// {

// }







