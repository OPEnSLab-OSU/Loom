

#ifdef is_analog
  #define num_measurements 4      // Must be 1, 2, 4, or 8 number of analog measurements to sample and average per channel
  int16_t a0, a1, a2, a3, a4, a5; // Memory to store analog sensor values
#endif


#ifdef is_sleep_period
  #include <Adafruit_SleepyDog.h> // This must be included if you are transmitting at timed intervals
#endif


#ifdef is_sleep_interrupt
  #include <LowPower.h>           //Include this if transmitting on pin interrupt
#endif


// Packet header creation macro
#define STR_(x) #x                // Helper function
#define STR(x) STR_(x)            // To concatenate a predefined number to a string literal, use STR(x)
#define PacketHeaderString STR(/) FAMILY STR(/) DEVICE // Results in a single string, i.e. /LOOM/Device. the full prefix sent to this device should be /LOOM/Device#, but the number is parsed in the OSC bundle routing function



// Common global variables
int           led =  LED_BUILTIN;             // LED pin number
volatile bool ledState = LOW;                 // State of LED
float         vbat = 3.3;                     // Place to save measured battery voltage (3.3V max)
char          packetBuffer[255];              // Buffer to hold incoming packet
char          ReplyBuffer[] = "acknowledged"; // A string to send back
const byte    memValidationValue = 99;
OSCErrorCode  error;                          // Hold errors from OSC
uint32_t      button_timer;                   // For time button has been held


// Global variables to handle changes to WiFi ssid and password 
char new_ssid[32];
char new_pass[32];
bool ssid_set;
bool pass_set;


// Define struct to hold DEVICE CONFIG SETTINGS
struct config_t {
  byte        checksum;                 // Value is changed when flash memory is written to.
  uint8_t     instance_number;          // Default 0, should be set on startup from a patch
  char        packet_header_string[80]; // String of expected packet header (dynamically formed based on config.h)
  IPAddress   ip;                       // Device's IP Address
  char*       my_ssid;                  // Default AP name
  char        ssid[32];                 // Host network name
  char        pass[32];                 // Host network password
  int         keyIndex;                 // Key Index Number (needed only for WEP)
  char*       ip_broadcast;             // IP to Broadcast data
  unsigned int localPort;               // Local port to listen on
  byte        mac[6];                   // Device's MAC Address
  WiFiMode    wifi_mode;                // Devices current wifi mode
  
  //add any other stuff that needs to be stored based on the shields with a wrapped preprocessor statement HERE

  #ifdef is_mpu6050
    int   ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset; // MPU6050 config if needed
  #endif
  
}; //end of config_t


// Instance of config_t
struct config_t configuration;


#if MEM_TYPE == MEM_FLASH
  #include <FlashStorage.h>
  FlashStorage(flash_config,config_t);    // Setup the flash storage for the structure
#endif



// WiFi global vars/structs
WiFiUDP      Udp;
WiFiServer   server(80);
int status = WL_IDLE_STATUS;

