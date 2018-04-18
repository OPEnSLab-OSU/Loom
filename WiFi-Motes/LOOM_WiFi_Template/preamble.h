#include "declarations_and_structures.h"


// Using Config defines, check which headers to include
#if DEBUG == 1
	#include <SPI.h>
#endif

#if is_analog > 0
  #include "subroutines_analogIn.h"
#endif
#ifdef is_i2c 
  #include "subroutines_mpu6050.h"
#endif
#if is_neopixel == 1
  #include "subroutines_neopixel.h"
#endif
#if num_servos > 0
  #include "subroutines_servo.h"
#endif
#if is_relay == 1
  #include "subroutines_relay.h"
#endif
#if is_wifi == 1
	#include "subroutines_wifi.h"
#endif
#if is_lora == 1
	#include "subroutines_lora.h"
#endif


// These may refer to functions in above headers
#include "subroutines_common.h"
