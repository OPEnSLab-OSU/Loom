// Define struct to hold DEVICE CONFIG SETTINGS
// The structure that gets stored in non-volatile memory to keep settings
struct config_t {
  byte        checksum;                 // Value is changed when flash memory is written to.
  uint8_t     instance_number;          // Default 0, should be set on startup from a patch
  char        packet_header_string[80]; // String of expected packet header (dynamically formed based on config.h)
 
  #if is_wifi == 1
    struct config_t_wifi config_wifi;
  #endif
  
  //add any other stuff that needs to be stored based on the shields with a wrapped preprocessor statement HERE

  #if is_mpu6050 == 1
    struct config_t_mpu6050 config_mpu6050; // MPU6050 config if needed
  #endif
  
}; //end of config_t

// Instance of config_t to holding device current configuration
struct config_t configuration;

#if MEM_TYPE == MEM_FLASH
  #include <FlashStorage.h>
  FlashStorage(flash_config,config_t);    // Setup the flash storage for the structure
#endif
