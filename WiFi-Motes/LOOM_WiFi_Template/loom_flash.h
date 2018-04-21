// Define struct to hold DEVICE CONFIG SETTINGS
// The structure that gets stored in non-volatile memory to keep settings
struct config_flash_t {
  byte        checksum;                 // Value is changed when flash memory is written to.
  uint8_t     instance_number;          // Default 0, should be set on startup from a patch
  char        packet_header_string[80]; // String of expected packet header (dynamically formed based on config.h)
 
  #if is_wifi == 1
    struct config_wifi_t config_wifi;
  #endif
  
  //add any other stuff that needs to be stored based on the shields with a wrapped preprocessor statement HERE

  #if is_mpu6050 == 1
    struct config_mpu6050_t config_mpu6050; // MPU6050 config if needed
  #endif
  
}; //end of config

// Instance of config to holding device current configuration
struct config_flash_t configuration;
void write_non_volatile();
void read_non_volatile();
#if MEM_TYPE == MEM_FLASH
  #include <FlashStorage.h>
  FlashStorage(flash_config,config_flash_t);    // Setup the flash storage for the structure
  void read_non_volatile(){
    configuration = flash_config.read();
  }
  void write_non_volatile(){
    flash_config.write(configuration);
  }
#elif MEM_TYPE == MEM_EEPROM
  #include <EEPROM.h>
  #include "EEPROMAnything.h"
  void read_non_volatile(){
    EEPROM_readAnything(0,configuration);
  }
  void write_non_volatile(){
    EEPROM_writeAnything(0,configuration);
  }
#endif



