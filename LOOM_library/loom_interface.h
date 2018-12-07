// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================

// Main loop interface functions
void receive_bundle(OSCBundle *bndl, CommPlatform platform);
void process_bundle(OSCBundle *bndl);
void measure_sensors();
void package_data(OSCBundle *bndl);
void send_bundle( OSCBundle *send_bndl, CommPlatform platform, int port);
void send_bundle( OSCBundle *send_bndl, CommPlatform platform);
void log_bundle(  OSCBundle *send_bndl, LogPlatform  platform, char* file); //filename for SD files
void log_bundle(  OSCBundle *send_bndl, LogPlatform  platform);
bool bundle_empty(OSCBundle *bndl);
void additional_loop_checks();

void sleep_for(int amount, TimeUnits units, SleepMode mode);
void sleep_until_time(SleepMode mode, int hour, int min, int sec);


// ================================================================ 
// ===               GENERAL INTERFACE FUNCTIONS                === 
// ================================================================



// --- SEND BUNDLE ---
//
// Fills an OSC bundle with packets received the specified platform
// if data exists and platform is enabled
// 
// @param bndl       The bundle to fill
// @param platform   The wireless platform to receive on, the values are
//                    encoded to a Platform enum to reduce chance for errors
// 
void receive_bundle(OSCBundle *bndl, CommPlatform platform)
{
	bndl->empty();

	switch(platform) {
		#if is_wifi == 1
		case WIFI :
			// Handle wifi bundle if it exists
			// Checks device unique UDP port and common UDP port
			wifi_receive_bundle(bndl, &UdpDevice, configuration.config_wifi.devicePort, "Device"); 

			// AP mode only uses the device port
			if (configuration.config_wifi.wifi_mode != AP_MODE)	{
				wifi_receive_bundle(bndl, &UdpSubnet, configuration.config_wifi.subnetPort, "Subnet"); 
				wifi_receive_bundle(bndl, &UdpGlobal, GLOBAL_PORT, "Global");
			}
			break;
			
		#endif

		#if is_lora == 1
		case LORA : lora_receive_bundle(bndl); break;
		#endif

		#if is_nrf == 1
		case NRF : nrf_receive_bundle(bndl); break;
		#endif

		#if LOOM_DEBUG == 1
		default : LOOM_DEBUG_Println3("That platform (", platform, ") is not enabled to receiving");
		#endif 
	} // of switch
}





// --- PROCESS BUNDLE --- 
// 
// Examine the provided OSC bundle (presumably filled via receive_bundle()
// If bundle is not empty,  has no errors, and is addressed to this device, then
// attempt to perform action bundle specifies
// 
// @param bndl  The OSC bundle to be processed 
//
void process_bundle(OSCBundle *bndl)
{
	if (bndl->size()){

		// If no bundle error
		if (!bndl->hasError()) {
			char address_string[255];
			bndl->getOSCMessage(0)->getAddress(address_string, 0);
	
			LOOM_DEBUG_Println2("Number of items in bundle: ", bndl->size());
			LOOM_DEBUG_Println2("First message address string: ", address_string);


			// --- Message Routing ---
	
			// These are the most important part of this function 
			// Send the bndle to the routing function,
			// which will route/dispatch messages to the currect handling functions
			// Most commands will be finished once control returns here 
			
			routing_match = false;
			
			// Device Specific Message
			bndl->route(configuration.packet_header_string, msg_router);

			// Family Subnet Message
			if (!routing_match) { bndl->route(STR(/) FAMILY STR(FAMILY_NUM), msg_router); }

			// Family Global Message
			if (!routing_match) { bndl->route(STR(/) FAMILY , msg_router); }


		} else { // of !bndl.hasError()
			error = bndl->getError();
			LOOM_DEBUG_Println2("Bundle Error: ", error);
		} // of else
	} // of if (bndl->size())

	// bndl->empty();
}





// --- MEASURE SENSORS ---
//
// Update stored readings from sensors by calling measure 
// on each enabled sensor
//
void measure_sensors()
{
	// Get battery voltage
	#if (is_analog == 1) || (is_m0 == 1)
		vbat = read_analog(VBATPIN);
		vbat = (vbat * 2 * 3.3) / 4096; // We divided by 2, so multiply back, multiply by 3.3V, our reference voltage, div by 1024 to convert to voltage
	#else
		vbat = analogRead(VBATPIN);
		vbat = (vbat * 2 * 3.3) / 1024; // We divided by 2, so multiply back, multiply by 3.3V, our reference voltage, div by 1024 to convert to voltage
	#endif

	//	Get button state
	#if is_button == 1
		button_state = digitalRead(button_pin);
	#endif
	
	//	Measure multiplexer sensors
	// #if is_multiplexer == 1
	// 	measure_tca9548a();
	// 	if (millis()-state_tca9548a.last_update_time > state_tca9548a.mux_update_period){
	// 		update_sensors();
	// 		LOOM_DEBUG_Println("Update MuxShield Sensorlist");
	// 		state_tca9548a.last_update_time = millis();
	// 	}
	// #endif
	
	// Update MPU6050 Data
	#if is_mpu6050 == 1
		measure_mpu6050();      // Now measure MPU6050, update values in global registers 
	#endif 

	// Update Thermocouple
	#if is_max31856 == 1
		measure_max31856();
	#endif

	// Get analog readings
	#if (is_analog == 1) && (is_sapflow != 1)
		measure_analog();
	#endif

	// Get decagon readings
	#if is_decagon == 1
		measure_decagon();
	#endif

	// Update time
	#if is_rtc == 1
		measure_rtc();
	#endif


	// If using I2C sensor without multiplexer
	#if is_multiplexer != 1
		#if is_as726X == 1
			measure_as726X();
		#endif
		#if is_as7265X == 1 
			measure_as7265X();
		#endif
		#if is_fxas21002 == 1 
			measure_fxas21002();
		#endif
		#if is_fxos8700 == 1
			measure_fxos8700();
		#endif
		#if is_hx711 == 1
			measure_hx711();
		#endif
		#if is_lis3dh == 1
			measure_lis3dh();
		#endif
		#if is_mb1232 == 1
			measure_mb1232();
		#endif
		#if is_mpu6050 == 1
			measure_mpu6050(); 
		#endif
		#if is_ms5803 == 1
			measure_ms5803();
		#endif
		#if is_sht31d == 1
			measure_sht31d();
		#endif
		#if is_tsl2591 == 1
			measure_tsl2591();
		#endif
		#if is_tsl2561 == 1
			measure_tsl2561();
		#endif
		#if is_zxgesturesensor == 1
			measure_zxgesturesensor();
		#endif
	#endif


	 // Get analog readings
	#if is_sapflow == 1 && hub_node_type == 1
		measure_sapflow();
		measure_sht31d();
		heat();
	#endif
}





// --- PACKAGE DATA ---
// 
// Fill the provided OSC bundle with latest stored sensor readings
//
// @param bndl  The OSC bundle to be filled
//
void package_data(OSCBundle *bndl)
{
	// Clear any previous contents
	bndl->empty();

	// LOOM_DEBUG_Println("HERE1");
	// print_bundle(bndl);

	// Add battery data
	char address_string[80]; 
	sprintf(address_string, "%s%s", configuration.packet_header_string, "/vbat");
	bndl->add(address_string).add(vbat); 
	
	// Add button state
	#if is_button == 1
		sprintf(address_string, "%s%s", configuration.packet_header_string, "/button");
		bndl->add(address_string).add((int32_t)button_state);
	#endif

	// //	Add multiplexer sensor data
	// #if is_multiplexer == 1
	// 	package_tca9548a(bndl, configuration.packet_header_string);
	// #endif 

	// LOOM_DEBUG_Println("HERE2");
	// print_bundle(bndl);

	// Update MPU6050 Data
	#if is_mpu6050 == 1
		// LOOM_DEBUG_Println("PACKAGE MPU");
		package_mpu6050(bndl, configuration.packet_header_string, 0);
	#endif 

	#if is_max31856 == 1
		package_max31856(bndl, configuration.packet_header_string);
	#endif
	
	// Get analog readings
	#if (is_analog == 1) && (is_sapflow != 1)
		package_analog(bndl, configuration.packet_header_string);
	#endif

	#if is_decagon == 1
		package_decagon(&bndl, configuration.packet_header_string);
	#endif

	#if is_sapflow == 1 && hub_node_type == 1
		package_sapflow(bndl, configuration.packet_header_string);
		package_sht31d(bndl, configuration.packet_header_string);
	#endif

	// LOOM_DEBUG_Println("HERE3");
	// print_bundle(bndl);


	// If using I2C sensor without multiplexer
	#if is_multiplexer != 1
		#if is_as726X == 1 
			package_as726X(bndl, configuration.packet_header_string);
		#endif
		#if is_as7265X == 1 
			package_as7265X(bndl, configuration.packet_header_string);
		#endif
		#if is_fxas21002 == 1 
			package_fxas21002(bndl, configuration.packet_header_string);
		#endif
		#if is_fxos8700 == 1
			package_fxos8700(bndl, configuration.packet_header_string);
		#endif
		#if is_hx711 == 1
			package_hx711(bndl, configuration.packet_header_string);
		#endif
		#if is_lis3dh == 1
			package_lis3dh(bndl, configuration.packet_header_string);
		#endif
		#if is_mb1232 == 1
			package_mb1232(bndl, configuration.packet_header_string);
		#endif
		// #if is_mpu6050 == 1
		// 	package_mpu6050(bndl, configuration.packet_header_string); 
		// #endif
		#if is_ms5803 == 1
			package_ms5803(bndl, configuration.packet_header_string);
		#endif
		#if is_sht31d == 1
			package_sht31d(bndl, configuration.packet_header_string);
		#endif
		#if is_tsl2591 == 1
			package_tsl2591(bndl, configuration.packet_header_string);
		#endif
		#if is_tsl2561 == 1
			package_tsl2561(bndl, configuration.packet_header_string);
		#endif
		#if is_zxgesturesensor == 1
			package_zxgesturesensor(bndl, configuration.packet_header_string);
		#endif
	#endif // of #if is_multiplexer != 1


	//	Add multiplexer sensor data
	#if is_multiplexer == 1
		package_tca9548a(bndl, configuration.packet_header_string);
	#endif 

	#if (is_lora == 1) && (package_lora_rssi == 1)
		sprintf(address_string, "%s%s", configuration.packet_header_string, "/rssi");
		bndl->add(address_string).add((int32_t)lora_last_rssi);
	#endif


}





// --- SEND BUNDLE ---
//
// Sends a packaged bundle on the specified platform
// 
// @param send_bndl  The bundle to be sent
// @param platform   The wireless platform to send on, the values are
//                    encoded to Platform enum to reduce chance for errors
// @param file       The file name when saving to SD card
// 
// void send_bundle(OSCBundle *send_bndl, Platform platform, char* file)

void send_bundle(OSCBundle *send_bndl, CommPlatform platform, int port)
// void send_bundle(OSCBundle *send_bndl, CommPlatform platform)
{
	if (!send_bndl->size()) return;

	switch(platform) {
		#if is_wifi == 1
			case WIFI : wifi_send_bundle(send_bndl); break;
			// case WIFI : wifi_send_bundle(send_bndl, port); break;
		#endif

		#if is_lora == 1
			case LORA : lora_send_bundle(send_bndl); break;
				// (!lora_bundle_fragment) ? lora_send_bundle(send_bndl) : lora_send_bundle_fragment(send_bndl); break;
		#endif

		#if is_nrf == 1
			case NRF : nrf_send_bundle(send_bndl); break;
				// (!nrf_bundle_fragment) ? nrf_send_bundle(send_bndl) : nrf_send_bundle_fragment(send_bndl); break;
		#endif
		
		#if LOOM_DEBUG == 1
			default : Serial.println("That platform is not enabled for sending");
		#endif 
	} // of switch
}

// Overloaded version that doesn't take the optional UDP port 
// If WiFi, calling this function will default to Device Unique port
void send_bundle(OSCBundle *send_bndl, CommPlatform platform)
{
	send_bundle(send_bndl, platform, configuration.instance_number);
}



// --- LOG BUNDLE ---
//
// Used to log a bundle to SD card, Google sheets,
// Adafruit IO, or the serial monitor
//
// @param log_bndl  The bndl to be logged
// @param platform  On which platform to log the bundle
// @param file      If logging to SD card, name of file to save to
//
void log_bundle(OSCBundle *log_bndl, LogPlatform platform, char* file)
{
	if (!log_bndl->size()) return;

	switch(platform) {
		#if is_sd == 1
		case SDCARD : 
			if (!SD_working) return;
			if (!file)  { 
				LOOM_DEBUG_Println("Logging to SD card requires filename");
				return;
			}
			// LOOM_DEBUG_Println("Saving bundle");
			sd_save_bundle(file, log_bndl, 0, sd_save_time_format); 
			return;
		#endif

		#if is_pushingbox == 1
		case PUSHINGBOX : 
			// LOOM_DEBUG_Println("Sending bundle data to PushingBox");
			sendToPushingBox(log_bndl);
			return;	
		#endif

		// #if is_adafruitio == 1
		// 	case ADAFRUITIO : 
		// 		LOOM_DEBUG_Println("Error: Adafruit IO doesn't take a filename"); 
		// 		return;	
		// #endif

		#if LOOM_DEBUG == 1
		case SERIAL_MON :
			print_bundle(log_bndl);
			return;
		#endif

		#if is_oled == 1
		case OLED :
			oled_display_bundle(log_bndl);
			return;
		#endif

		#if LOOM_DEBUG == 1
		default :
			Serial.println("That platform is not enabled for logging");
		#endif 
	} // of switch
}


void log_bundle(OSCBundle *log_bndl, LogPlatform platform)
{
	log_bundle(log_bndl, platform, NULL);
}



// --- ADDITIONAL LOOP CHECKS ---
//
// Performs any miscellaneous Loom tasks that happen each loop iteration
// but are not handled by any of the other 5 interface functions
//
void additional_loop_checks()
{
	// Reset to AP mode if button held for ~5 seconds
	// #if (is_button == 1) && (is_wifi == 1)
	// 	check_button_held();      
	// #endif

	#if is_wifi == 1
		// Compare the previous status to the current status
		if (config_wifi->wifi_mode == AP_MODE) {
			wifi_check_status();
		}
	#endif

	// Delay between loop iterations
	#ifdef is_sleep_period
		loop_sleep();
	#elif defined(is_sleep_interrupt)
		delay(50);
	#endif

	#if is_adafruitio == 1
		adafruitio_subscribe();
		adafruitio_publish();
	#endif

	#if (hub_node_type == 0) && (advanced_interdev_comm == 1)
		check_device_refresh_interval();
	#endif

	bndl.empty();
}





// ================================================================ 
// ===                 RTC INTERFACE FUNCTIONS                  === 
// ================================================================



#if is_rtc3231 == 1

void prep_before_sleep()
{
	LOOM_DEBUG_Println("Entering STANDBY");
	#if LOOM_DEBUG == 1
		Serial.end();
		USBDevice.detach(); 
	#endif
	rtc_interrupt_reset(); //clear interrupt registers, attach interrupts
	delay(50);
	// #if LOOM_DEBUG == 1
		digitalWrite(LED_BUILTIN, LOW);
	// #endif
}

void prep_after_sleep()
{
	#if LOOM_DEBUG == 1
		digitalWrite(LED_BUILTIN, HIGH);
		#if wake_delay == 1
			delay(5000); // give user 5s to close and reopen serial monitor!
		#endif
	#endif
	rtc_interrupt_reset(); //clear interrupt registers, attach interrupts
	clearRTCAlarms(); //prevent double trigger of alarm interrupt

	// Not sure why these have to be repeated but it seems to make a difference
	rtc_interrupt_reset();
	clearRTCAlarms();

	#if LOOM_DEBUG == 1
		USBDevice.attach();
		Serial.begin(SERIAL_BAUD);
		LOOM_DEBUG_Println("WAKE");
		print_DateTime(rtc_inst.now());
		delay(50);  // delay so serial stuff has time to print out all the way
	#endif
}

#endif // of '#if is_rtc3231 == 1'





void sleep_for(int amount, TimeUnits units, SleepMode mode) 
{

	switch(mode) {
		// case IDLE: {
		// 	return;
		// }
	#if is_rtc3231 == 1
		case STANDBY: {
			LOOM_DEBUG_Println("Sleep for time in 'Standby' mode");

			// Set alarm specified time into the future
			switch (units) {
				case SECONDS: {
					if (amount < 255) {
						setRTCAlarm_Relative(0, 0, amount);       
					} else {
						setRTCAlarm_Relative(0, amount/60, amount%60);  
					}
					break;
				}
				case MINUTES: setRTCAlarm_Relative(0, amount, 0);      break; 
			}

			prep_before_sleep();		// Prepare for sleep
			digitalWrite(LED_BUILTIN, LOW);

			LowPower.standby(); 		// Go to sleep, will here wait until RTC interrupt

			#if LOOM_DEBUG == 1
				digitalWrite(LED_BUILTIN, HIGH);
			#endif
			prep_after_sleep();			// Any necessary management when returning from sleep
			LOOM_DEBUG_Println("Done with standby");
			return;
		}
	#endif // of 'is_rtc3231 == 1'


	#if is_lora != 1
		case SLEEPYDOG: {
			int duration = second_duration(amount, units); // time in seconds

			// LOOM_DEBUG_Println("Sleep in 'Sleepydog' mode");

			// LOOM_DEBUG_Println2("Amount: ", amount);
			// switch (units) {
			// 	case SECONDS:

			// 		break; 
			// 	case MINUTES:
			// 		int count = amount << 2;
			// 		int arr[count];

			// 		LOOM_DEBUG_Println3("Will sleep for a total of: ", amount, " minutes");
			// 		digitalWrite(LED_BUILTIN, LOW);
			// 		for (int i = 0; i < count; i++) {
			// 			int sleepMS = Watchdog.sleep(20000);
			// 			arr[i] = sleepMS;
			// 			#if LOOM_DEBUG == 1
			// 				LOOM_DEBUG_Println(i);
			// 				digitalWrite(LED_BUILTIN, HIGH);
			// 				delay(100);
			// 				digitalWrite(LED_BUILTIN, LOW);  
			// 			#endif
			// 		}
			// 		LOOM_DEBUG_Println3("Done sleeping ", amount, " minutes");
			// 		digitalWrite(LED_BUILTIN, HIGH);

			// 		delay(5000);
			// 		for (int i = 0; i < count; i++) {
			// 			LOOM_DEBUG_Println(arr[i]);
			// 		}
			// 		break; 
			// }


			// int duration = 0;
			// if (duration > 60) {
				
			// 	int temp = duration >> 4;
			// 	temp = temp << 2;

			// 	LOOM_DEBUG_Println4("Sleep for: ", duration, " | ", temp);


			// } else {
			// 	LOOM_DEBUG_Println2("Sleep for: ", duration);
			// }

			// return;

			// calculate number of iterations of 15s sleep

			int iterations = duration / 16;
			int remainder  = duration % 16;
			int total = 0;

			LOOM_DEBUG_Println3("Will sleep for a total of: ", duration, " seconds");
			LOOM_DEBUG_Print3("Using ", iterations, " blocks of 15 seconds");
			LOOM_DEBUG_Println3(" and ", remainder, " milliseconds");

			#if LOOM_DEBUG == 1
				USBDevice.detach();
			#endif

			digitalWrite(LED_BUILTIN, LOW);

			LOOM_DEBUG_Println("Going to sleep in 15 second blocks");
			for (int i = 0; i < iterations; i++) {
				int sleepMS = Watchdog.sleep(16000);
				LOOM_DEBUG_Println3("Just slept for: ", sleepMS, " milliseconds");
				total += sleepMS;
				LOOM_DEBUG_Println3("Slept a total of: ", total, " milliseconds");

				digitalWrite(LED_BUILTIN, HIGH);  
				delay(100);                       
				digitalWrite(LED_BUILTIN, LOW);   
			}

			if (remainder > 0) {
				LOOM_DEBUG_Println3("Sleeping the remaining ", remainder, " milliseconds");
				int sleepMS = Watchdog.sleep(remainder);
			}

			LOOM_DEBUG_Println3("Done sleeping a total of: ", total, " milliseconds");

			flash_led(5, 40, 30);
 
 			#if LOOM_DEBUG == 1
				USBDevice.attach();
			#endif

			return;
		}
	#endif // of '#if is_lora != 1'
		default: {
			LOOM_DEBUG_Println("Cannot sleep with a mode that is not enabled");
		}
	}
}


// Could take DateTime object (probably with overloading)
// there should be a global datetime (lastWake) that gets updated upon device wake
	// maybe hava a timespan for consistent increments
// Or could take Hour, Min, Sec 
	// would assume to trigger first occurance of that regardless of day
void sleep_until_time(SleepMode mode, int hour, int min, int sec) 
{
	switch(mode) {
		#if is_rtc3231 == 1
		case STANDBY: {
			// DateTime now = rtc_inst.now();
			// Create futue DateTime class with constructor
			// Takes Days, Hours, Minutes, Seconds
		    // DateTime future(now + TimeSpan(0,4,3,30));
			
			LOOM_DEBUG_Println("Sleep until time in 'Standby' mode");

			// Set alarm specified time into the future
			setRTCAlarm_Absolute(hour, min, sec);

			prep_before_sleep();		// Prepare for sleep
			digitalWrite(LED_BUILTIN, LOW);

			LowPower.standby(); 		// Go to sleep, will here wait until RTC interrupt

			#if LOOM_DEBUG == 1
				digitalWrite(LED_BUILTIN, HIGH);
			#endif
			prep_after_sleep();			// Any necessary management when returning from sleep
			LOOM_DEBUG_Println("Done with standby");
			return;	
		}
		#endif // of '#if is_rtc3231 == 1'

		case SLEEPYDOG: {
			// might work if using SD wing RTC 
			return;
		}

		default: {
			LOOM_DEBUG_Println("Cannot sleep with a mode that is not enabled");
		}

	}
}




// Should take an array of pins that will be used ?
// void sleep_until_interrupt(int interrupts, SleepMode mode)
// {
// 	switch(mode) {
// 		case STANDBY: {
// 		}

// 		case SLEEPYDOG: {
// 		}

// 	}
// }

 

// ?? 
// void configure_interrupts() 
// {

// }



