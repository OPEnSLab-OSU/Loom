#include "declarations_and_structures.h"

// Using Config defines, check which headers to include
#if DEBUG == 1
	#include <SPI.h>
#endif
#ifdef is_analog
  #include "subroutines_analogIn.h"
#endif
#ifdef is_i2c
  #include "subroutines_mpu6050.h"
#endif
#ifdef is_neopixel
  #include "subroutines_neopixel.h"
#endif
#ifdef is_servo  
  #include "subroutines_servo.h"
#endif
#ifdef is_relay
  #include "subroutines_relay.h"
#endif
#ifdef is_wifi
	#include "subroutines_wifi.h"
#endif
#ifdef is_lora
	#include "subroutines_lora.h"
#endif

// These may refer to functions in above headers
#include "subroutines_common.h"
