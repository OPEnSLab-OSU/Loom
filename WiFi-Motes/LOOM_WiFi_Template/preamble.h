#include "declarations_and_structures.h"

// Using Config defines, check which headers to include
#if DEBUG == 1
	#include <SPI.h>
#endif
#ifdef is_analog
  #include "analogIn_subroutines.h"
#endif
#ifdef is_i2c
  #include "mpu6050_subroutines.h"
#endif
#ifdef is_neopixel
  #include "neopixel_subroutines.h"
#endif
#ifdef is_servo  
  #include "servo_subroutines.h"
#endif
#ifdef is_relay
  #include "relay_subroutines.h"
#endif
#ifdef is_wifi
	#include "wifi_subroutines.h"
#endif
#ifdef is_lora
	#include "lora_subroutines.h"
#endif

// These may refer to functions in above headers
#include "common_subroutines.h"