/*
 * Library for the TI ADS1231 24-Bit Analog-to-Digital Converter
 */

#ifndef ADS1231_H
#define ADS1231_H

// #include "errors.h"


#include <Arduino.h>


// pull signal of power down pin low 
// high to turn on 
// pin A3


typedef unsigned char errv_t;


#define ADS1231_DATA_PIN A0
#define ADS1231_CLK_PIN  A1

#define ADS1231_PWR_PIN  A2

#define ADS1231_TARE 0


#define ADS1231_TIMEOUT_HIGH         101   // Timeout waiting for HIGH
#define ADS1231_TIMEOUT_LOW          100   // Timeout waiting for LOW
#define ADS1231_WOULD_BLOCK          102   // weight not measured, measuring takes too long
#define ADS1231_STABLE_TIMEOUT       103   // weight not stable within timeout

// ADC counts per milligram
#define ADS1231_DIVISOR  6.6
#define ADS1231_OFFSET -27.2727272727



// Zero offset, grams
// How to calibrate using a weight (in grams) and the measured raw value
// as returned by ads1231_get_value():
// raw1    = -159119.02777777778
// weight1 = 0.
// raw2    = 761393.42307692312
// weight2 = 679.2
// ADS1231_DIVISOR = (raw1 - raw2) / (weight1 - weight2)
// ADS1231_OFFSET  = weight1 - (raw1 * (weight1 - weight2)) / (raw1 - raw2)

// raw1    = 180
// weight1 = 0
// raw2    = 1500
// weight2 = 200

// divisor = (180 - 1500) / (0 - 200) = 6.6
// offset = 0 - (180 * (0 - 200) ) / (180 - 1500) = -27.2727272727

#define ADS1231_STABLE_MILLIS 5000
#define ADS1231_GET_STABLE_INTERVAL 100
#define WEIGHT_EPSILON 3



#define RETURN_IFN_0(code) do { \
								errv_t ret_RETURN_IFN_0 = code; \
								if (ret_RETURN_IFN_0 != 0)   \
									return ret_RETURN_IFN_0; \
							  } while(0)


#define DEBUG_START() Serial.print("DEBUG     ")
#define DEBUG_END() Serial.println()

#define DEBUG_MSG(msg) Serial.print(msg)
#define DEBUG_VAL(val) do { Serial.print(#val); \
							Serial.print(": "); \
							Serial.print(val);  \
							Serial.print(", "); \
						  } while (0)

#define DEBUG_MSG_LN(msg) do { DEBUG_START(); \
							   Serial.println(msg); \
							 } while (0)
#define DEBUG_VAL_LN(val) do { DEBUG_START(); \
							   DEBUG_VAL(val); \
							   DEBUG_END(); \
							 } while (0)

#define MSG(msg) Serial.println(msg)

#define ERROR(msg) do { Serial.print("ERROR "); \
                        Serial.print(msg); \
                        Serial.println(" "); \
                      } while (0)



extern unsigned long ads1231_last_millis;
extern int ads1231_offset;


void   ads1231_init(void);

void ads1231_powerup();
void ads1231_powerdown();

errv_t ads1231_get_value(long& val);
errv_t ads1231_get_grams(int& grams);
errv_t ads1231_get_stable_grams(int& grams);

errv_t ads1231_get_average_grams(int& grams);

errv_t ads1231_get_noblock(int& grams);
errv_t ads1231_tare(int& grams);





#endif // of #ifndef ADS1231_H
