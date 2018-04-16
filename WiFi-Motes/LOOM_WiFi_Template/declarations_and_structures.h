#include <OSCBundle.h>

// Packet header creation macro
#define STR_(x) #x                // Helper function
#define STR(x) STR_(x)            // To concatenate a predefined number to a string literal, use STR(x)
#define PacketHeaderString STR(/) FAMILY STR(/) DEVICE // Results in a single string, i.e. /LOOM/Device. the full prefix sent to this device should be /LOOM/Device#, but the number is parsed in the OSC bundle routing function




#define VBATPIN A7                // Pin to check for battery voltage



//------------------------------------------------------------------------------------------------------
// MEMORY TYPE: M0 uses flash (MEM_TYPE = 0), 32u4 uses EEPROM (MEM_TYPE = 1)
//------------------------------------------------------------------------------------------------------
#define MEM_FLASH 0
#define MEM_EEPROM 1  

#define is_m0
#define MEM_TYPE MEM_FLASH


#ifdef __SAMD21G18A__
  #define is_m0
  #define MEM_TYPE MEM_FLASH
#endif
#ifdef __AVR_ATmega32U4__
  #define is_32u4
  #define MEM_TYPE MEM_EEPROM
#endif




#if is_relay == 0
  #define button 10               // Using on-board button, specify attached pin, transmitting
#endif


#if is_analog == 1
  #define num_measurements 4      // Must be 1, 2, 4, or 8 number of analog measurements to sample and average per channel
  int16_t a0, a1, a2, a3, a4, a5; // Memory to store analog sensor values
#endif


#ifdef is_sleep_period
  #include <Adafruit_SleepyDog.h> // This must be included if you are transmitting at timed intervals
#endif


#ifdef is_sleep_interrupt
  #include <LowPower.h>           // Include this if transmitting on pin interrupt
#endif


#if is_wifi == 1
	enum WiFiMode {
		AP_MODE,
		WPA_CLIENT_MODE,
		WEP_CLIENT_MODE
	};
	
	#include <WiFi101.h>
	#include <WiFiUdp.h>

  #define AP_NAME   STR(FAMILY) STR(DEVICE) STR(INIT_INST)
  
  // WiFi global vars/structs
  WiFiUDP      Udp;
  WiFiServer   server(80);
  int status = WL_IDLE_STATUS;

  // Global variables to handle changes to WiFi ssid and password 
  char new_ssid[32];
  char new_pass[32];
  bool ssid_set;
  bool pass_set;
#endif // of is_wifi



#if is_lora == 1
	#include <RH_RF95.h>
	#include <RHReliableDatagram.h>

  #include <string.h>

  union data_value {
    int32_t i;
    float f;
    uint32_t u;
  };
  
	#define LORA_MESSAGE_SIZE RH_RF95_MAX_MESSAGE_LEN       // Defaults to 251 as 4 bytes are used for the header.
	#define RFM95_CS 8
	#define RFM95_RST 4
	#ifdef is_m0
		#define RFM95_INT 3     // Use this for the M0
	#endif
	#ifdef is_32u4
		#define RFM95_INT 7     // Use this for the 32u4
	#endif
	
	#if lora_device_type == 0
//		#include <Ethernet2.h>            --- currently in config.h
    EthernetClient client;            
    
		//Ethernet / Hub Info
    //byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    //IPAddress ip(10, 248, 55, 154);

//     IPAddress ip(128,193,56,138);    --- currently in config.h
	#endif
	
	RH_RF95 rf95(RFM95_CS, RFM95_INT);
	RHReliableDatagram manager(rf95, SERVER_ADDRESS);
#endif //of is_lora



#ifdef is_neopixel
  #include <Adafruit_NeoPixel.h>
  
  // Neopixel Data Structures (one per Ishield port)
  Adafruit_NeoPixel * pixels[3];
  
  // Based on config.h, enable Neopixel for specified ports
  bool pixel_enabled[3] = {NEO_0, NEO_1, NEO_2};
  
  // Store RGB vals for up to 1 pixel per port on Ishield
  int colorVals[3][3] = { {0, 0, 0},
                          {0, 0, 0},
                          {0, 0, 0}};
#endif



#ifdef is_relay
  #define RELAY_PIN0 5
  #define RELAY_PIN1 6
  
  // Array to hold relay states (on/off)
  bool relay_on[2];
#endif // of is_relay




#if num_servos > 0
  #include <Adafruit_PWMServoDriver.h>
  
  Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
  
  #define SERVOMIN  150     // This is the 'minimum' pulse length count (out of 4096)
  #define SERVOMAX  600     // This is the 'maximum' pulse length count (out of 4096)
  
  #if (num_servos==1)
    int predeg[1] = {0};
    double pre_pulselength[1] = {0.0};
  #elif (num_servos==2)
    int predeg[2] = {0, 0};
    double pre_pulselength[2] = {0.0, 0.0};
  #elif (num_servos==3)
    int predeg[3] = {0, 0, 0};
    double pre_pulselength[3] = {0.0, 0.0, 0.0};
  #elif (num_servos==4)
    int predeg[4] = {0, 0, 0, 0};
    double pre_pulselength[4] = {0.0, 0.0, 0.0, 0.0};
  #elif (num_servos==5)
    int predeg[5] = {0, 0, 0, 0, 0};
    double pre_pulselength[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
  #elif (num_servos==6)
    int predeg[6] = {0, 0, 0, 0, 0, 0};
    double pre_pulselength[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  #elif (num_servos==7)
    int predeg[7] = {0, 0, 0, 0, 0, 0, 0};
    double pre_pulselength[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  #elif (num_servos==8)
    int predeg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    double pre_pulselength[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  #endif // of if (num_servos==1) 
#endif // of num_servos


// Common global variables
int           led =  LED_BUILTIN;             // LED pin number
volatile bool ledState = LOW;                 // State of LED
float         vbat = 3.3;                     // Place to save measured battery voltage (3.3V max)
char          packetBuffer[255];              // Buffer to hold incoming packet
char          ReplyBuffer[] = "acknowledged"; // A string to send back
const byte    memValidationValue = 99;        // Used to check if configuration has been written to (number will only be 99 if mem has been written to)
OSCBundle     bndl;                           // Hold OSC bundle to received
OSCErrorCode  error;                          // Hold errors from OSC
uint32_t      button_timer;                   // For time button has been held
char addressString[255];



// Define struct to hold DEVICE CONFIG SETTINGS
// The structure that gets stored in non-volatile memory to keep settings
struct config_t {
  byte        checksum;                 // Value is changed when flash memory is written to.
  uint8_t     instance_number;          // Default 0, should be set on startup from a patch
  char        packet_header_string[80]; // String of expected packet header (dynamically formed based on config.h)
 
  #if is_wifi == 1
    IPAddress   ip;                     // Device's IP Address
    char*       my_ssid;                // Default AP name
    char        ssid[32];               // Host network name
    char        pass[32];               // Host network password
    int         keyIndex;               // Key Index Number (needed only for WEP)
    char*       ip_broadcast;           // IP to Broadcast data
    unsigned int localPort;             // Local port to listen on
    byte        mac[6];                 // Device's MAC Address
    WiFiMode    wifi_mode;              // Devices current wifi mode
  #endif
  
  //add any other stuff that needs to be stored based on the shields with a wrapped preprocessor statement HERE

  #ifdef is_mpu6050
    int   ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset; // MPU6050 config if needed
  #endif
  
}; //end of config_t


// Instance of config_t to holding device current configuration
struct config_t configuration;






#if MEM_TYPE == MEM_FLASH
  #include <FlashStorage.h>
  FlashStorage(flash_config,config_t);    // Setup the flash storage for the structure
#endif

