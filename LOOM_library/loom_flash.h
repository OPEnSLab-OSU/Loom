// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#if MEM_TYPE == MEM_FLASH
	#include <FlashStorage.h>
#elif MEM_TYPE == MEM_EEPROM
	#include <EEPROM.h>
	#include "EEPROMAnything.h"
#endif  

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define memValidationValue 99


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================ 

// Define struct to hold DEVICE CONFIG SETTINGS
// The structure that gets stored in non-volatile memory to keep settings
struct config_flash_t {
	byte        checksum;                 	// Value is changed when flash memory is written to.
	uint8_t     instance_number;          	// Default 0, should be set on startup from a patch
	char        packet_header_string[80]; 	// String of expected packet header (dynamically formed based on config.h)
 
	uint16_t sample_rate;

	#if is_wifi == 1
		struct config_wifi_t config_wifi;
	#endif

	#if enable_hub_scripts == 1
		struct config_dynamic_scripts_t config_dynamic_scripts;
	#endif

	#if is_pushingbox == 1
		struct config_pushingbox_t config_pushingbox;
	#endif


	//add any other stuff that needs to be stored based on the shields with a wrapped preprocessor statement HERE

	#if is_mpu6050 == 1
		struct config_mpu6050_t config_mpu6050; // MPU6050 config if needed
	#endif
	
}; //end of config


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
// Instance of config to holding device current configuration
struct config_flash_t configuration;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void write_non_volatile();
void read_non_volatile();
void setup_flash_config();


// ================================================================
// ===                          SETUP                           ===
// ================================================================

// Select how to read from and write to non volatile memory
// based on which device is bieng used

#if MEM_TYPE == MEM_FLASH
	FlashStorage(flash_config,config_flash_t);    // Setup the flash storage for the structure
	void read_non_volatile(){
		#if enable_flash != 1
			return;
		#endif
		configuration = flash_config.read();
	}
	void write_non_volatile(){
		#if enable_flash != 1
			return;
		#endif
		flash_config.write(configuration);
	}
#elif MEM_TYPE == MEM_EEPROM
	void read_non_volatile(){
		#if enable_flash != 1
			return;
		#endif
		EEPROM_readAnything(0,configuration);
	}
	void write_non_volatile(){
		#if enable_flash != 1
			return;
		#endif
		EEPROM_writeAnything(0,configuration);
	}
#endif



// --- SETUP FLASH CONFIG ---
//
// Handles configuration of reading device configuration settings from flash (if config is saved)
// else uses config.h specified settings, and then writes these to flash
// 
void setup_flash_config()
{
	
	// Link module configuration structures
	#if is_wifi == 1
		packet_header_string = configuration.packet_header_string;
		link_config_wifi(&configuration.config_wifi);
	#endif
	#if is_mpu6050 == 1
		link_config_mpu6050(&configuration.config_mpu6050);
	#endif

	#if enable_hub_scripts == 1
		link_config_dynamic_scripts(&configuration.config_dynamic_scripts);
	#endif

	#if is_pushingbox == 1
		link_config_pushingbox(&configuration.config_pushingbox);
	#endif


	#if MEM_TYPE == MEM_FLASH || MEM_TYPE == MEM_EEPROM

		
		#if enable_flash == 1
			read_non_volatile(); //reads configuration from non_volatile memory
			LOOM_DEBUG_Println("Reading from non-volatile memory...");
			LOOM_DEBUG_Println2("Checksum: ", configuration.checksum);
		#endif
		
		if ((configuration.checksum != memValidationValue) || (!enable_flash)) {     // Write default values to flash
																// The or '!enable_flash' here is to make sure the configuration struct is still populated (there wont be a valid checksum)
			configuration.instance_number = INIT_INST;
			sprintf(configuration.packet_header_string,"%s%d\0",PacketHeaderString,configuration.instance_number);
			
			LOOM_DEBUG_Println2("Expecting OSC header: ", configuration.packet_header_string);
			
			#if is_lora == 0
				configuration.sample_rate = is_sleep_period;
			#endif

			#if is_wifi == 1
				configuration.config_wifi.my_ssid = AP_NAME;                   // Default AP name
				strcpy(configuration.config_wifi.ssid,DEFAULT_NETWORK);        // Default network name
				strcpy(configuration.config_wifi.pass,DEFAULT_PASSWORD);       // AP password (needed only for WEP, must be exactly 10 or 26 characters in length)
				configuration.config_wifi.keyIndex     = 0;                    // Your network key Index number (needed only for WEP)
				configuration.config_wifi.ip_broadcast = "255.255.255.255";    // IP to Broadcast data 
				configuration.config_wifi.devicePort   = INIT_PORT;            // Local port to listen on
				configuration.config_wifi.subnetPort   = SUBNET_PORT;          // Port that all wifi devices also check
				configuration.config_wifi.wifi_mode    = DEFAULT_MODE;         // WiFi mode to start in (AP_MODE, WPA_CLIENT_MODE, WEP_CLIENT_MODE)
				configuration.config_wifi.request_settings = REQUEST_SETTINGS; // Bool of whether or not to request new channel settings on startup
			#endif


			// Add any other behavior/calibration wrapped in an '#ifdef is_something' preprocessor directive HERE
			#if (is_mpu6050 == 1) && (skip_mpu_cal != 1)
			// #if (is_mpu6050 == 1) && (is_multiplexer != 1)
				calMPU6050();                                 // Calibration writes memValidationValue for us
			#endif
			
			configuration.checksum = memValidationValue;      // Configuration has been written successfully, so we write the checksum



			#if enable_flash == 1
				LOOM_DEBUG_Println("Writing to flash for the first time.");
				write_non_volatile();
				LOOM_DEBUG_Println("Done writing to flash.");
			#endif

			// Flash memory has limited writes and we don't want to waste it on unnecessary tests
		} // of if (configuration.checksum != memValidationValue)
	#endif //of MEM_TYPE

//	packet_header_string = configuration.packet_header_string;
	sprintf(global_packet_header_string,"%s",configuration.packet_header_string);

}



