// ================================================================ 
// ===                ACCESSSING SENSOR STRUCTS                 === 
// ================================================================

// state_ads1231.grams

// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================

#define ADS1231_DIVISOR  6.6

#define ADS1231_TARE 0

#define ADS1231_TIMEOUT_HIGH         101   // Timeout waiting for HIGH
#define ADS1231_TIMEOUT_LOW          100   // Timeout waiting for LOW
#define ADS1231_WOULD_BLOCK          102   // weight not measured, measuring takes too long
#define ADS1231_STABLE_TIMEOUT       103   // weight not stable within timeout


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




unsigned long ads1231_last_millis = 0;
int ads1231_offset = ADS1231_OFFSET;

typedef unsigned char errv_t;


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================

struct state_ads1231_t {
	int grams;
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct state_ads1231_t state_ads1231;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_ads1231();
void package_ads1231(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_ads1231(OSCBundle *bndl, char packet_header_string[]);
void measure_ads1231();



void   ads1231_init(void);

void ads1231_powerup();
void ads1231_powerdown();

errv_t ads1231_get_value(long& val);
errv_t ads1231_get_grams(int& grams);
errv_t ads1231_get_stable_grams(int& grams);

errv_t ads1231_get_average_grams(int& grams);

errv_t ads1231_get_noblock(int& grams);
errv_t ads1231_tare(int& grams);


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any SHT31D setup
//
// @return  Whether or not sensor initialization was successful
//
bool setup_ads1231() {

	LOOM_DEBUG_Println("Setting up ADS1231");

	bool is_setup;

	ads1231_init();
	ads1231_powerup();

	LOOM_DEBUG_Println("ADS1231 setup complete");

	return is_setup;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE SHT31D --- (Multiplexer Version)
// 
// Adds last read SHT31D sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_ads1231(OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/ads1231/data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("grams").add( (int32_t)state_ads1231.grams);
	
	bndl->add(msg);
}

// Package function when not using multiplexer
void package_ads1231(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	sprintf(address_string, "%s%s", packet_header_string, "/ads1231_grams");
	bndl->add(address_string).add( (int32_t)state_ads1231.grams);
}


// --- MEASURE SHT31D ---
//
// Gets the current sensor readings of the SHT31D and stores into its state struct
// 
void measure_ads1231() 
{
	// ads1231_powerup();
	ads1231_get_average_grams( state_ads1231.grams );
	// ads1231_powerdown();
}






/*
 * Initialize the interface pins
 */
void ads1231_init(void)
{

	pinMode(ADS1231_PWR_PIN, OUTPUT);
	pinMode(ADS1231_CLK_PIN, OUTPUT);		// We send the clock
	// pinMode(ADS1231_DATA_PIN, INPUT);		// (32u4) ADS1231 sends the data
	pinMode(ADS1231_DATA_PIN, INPUT_PULLUP);		// (M0) ADS1231 sends the data

	digitalWrite(ADS1231_PWR_PIN, HIGH);	// Turn on 
	digitalWrite(ADS1231_DATA_PIN, HIGH); 	// Enable pullup to get a consistent state in case of disconnect
	digitalWrite(ADS1231_CLK_PIN, 0);		// Set CLK low to get the ADS1231 out of suspend
}



void ads1231_powerup()
{
	Serial.println("Power Up ADS1231 ========================\n");
	digitalWrite(ADS1231_PWR_PIN, HIGH);	// Turn on 
	delay(500);
}

void ads1231_powerdown()
{
	Serial.println("Power Down ADS1231 ========================\n");
	digitalWrite(ADS1231_PWR_PIN, LOW);	// Turn on 
}


/*
 * Get the raw ADC value. Can block up to 100ms in normal operation.
 * Returns 0 on success, an error code otherwise (see ads1231.h)
 */
errv_t ads1231_get_value(long& val)
{
	int i=0;
	unsigned long start;

	/* A high to low transition on the data pin means that the ADS1231
	 * has finished a measurement (see datasheet page 13).
	 * This can take up to 100ms (the ADS1231 runs at 10 samples per
	 * second!).
	 * Note that just testing for the state of the pin is unsafe.
	 */
	start = millis();
	while (digitalRead(ADS1231_DATA_PIN) != HIGH)
	{
		if (millis() > start+150)
			return ADS1231_TIMEOUT_HIGH; // Timeout waiting for HIGH
	}
	start = millis();
	while (digitalRead(ADS1231_DATA_PIN) != LOW)
	{
		if (millis() > start+150)
			return ADS1231_TIMEOUT_LOW; // Timeout waiting for LOW
	}
	ads1231_last_millis = millis();

	// Read 24 bits
	for (i=23 ; i >= 0; i--) {
		digitalWrite(ADS1231_CLK_PIN, HIGH);
		val = (val << 1) + digitalRead(ADS1231_DATA_PIN);
		digitalWrite(ADS1231_CLK_PIN, LOW);
	}

	/* Bit 23 is acutally the sign bit. Shift by 8 to get it to the
	 * right position (31), divide by 256 to restore the correct value.
	 */
	val = (val << 8) / 256;

	/* The data pin now is high or low depending on the last bit that
	 * was read.
	 * To get it to the default state (high) we toggle the clock one
	 * more time (see datasheet page 14 figure 19).
	 */
	digitalWrite(ADS1231_CLK_PIN, HIGH);
	digitalWrite(ADS1231_CLK_PIN, LOW);

	return 0; // Success
}


/*
 * Get the weight in grams. Can block up to 100ms in normal
 * operation because the ADS1231 makes only 10 measurements per second.
 * Returns 0 on sucess, an error code otherwise (see errors.h)
 */
errv_t ads1231_get_grams(int& grams)
{
	int ret;
	long raw;
	grams = 0; // On error, grams should always be zero

	ret = ads1231_get_value(raw);

	if (ret != 0) return ret; // Scale error

	grams = raw/ADS1231_DIVISOR + ads1231_offset;

	return 0; // Success
}


/*
 * Get the weight in grams but measure often until the same weight is measured
 * for 3 three times.
 * Can block for longer if the weight on scale is not stable. Something similar
 * is implemented in Bottle::turn_to() (see parameter 'stable_weight') in order
 * to get a stable weight without blocking the movement of the bottle.
 *
 * Returns 0 on sucess, an error code otherwise (see errors.h)
 */
errv_t ads1231_get_stable_grams(int& grams) {
	grams = 0; // needs to be 0 on error
	int i = 0;
	unsigned long start = millis();
	int weight_last, weight;

	RETURN_IFN_0(ads1231_get_grams(weight));
	while (i < 2) {
		delay(100); // TODO make this a constant in config.h
		weight_last = weight;
		RETURN_IFN_0(ads1231_get_grams(weight));
		// TODO maybe this would be more correct...? do we have abs?
		if ( abs(weight_last - weight) < WEIGHT_EPSILON ) {
		// if (weight_last == weight) {
			// weight stable
			i++;
		} else {
			// weight not stable
			i = 0;
		}
		DEBUG_START();
		DEBUG_MSG("Not stable: ");
		DEBUG_VAL(weight);
		DEBUG_VAL(weight_last);
		DEBUG_END();

		if (millis() - start > ADS1231_STABLE_MILLIS) {
			return ADS1231_STABLE_TIMEOUT;
		}
	}
	grams = weight;
	return 0;
}


// Takes 8 samples and averages them
errv_t ads1231_get_average_grams(int& grams) {
	long raw_tmp, raw_acc = 0;

	for (int i = 0; i < 8; i++) {
		ads1231_get_value(raw_tmp);
		raw_acc += raw_tmp;
	}

	if (raw_acc != 0) {
		grams = (raw_acc >> 3)/ADS1231_DIVISOR + ads1231_offset;
	} else {
		grams = 0;
	}

	return 0;
}



/**
 * Tare scale. Call this if there is nothing on scale to store offset and zero
 * current measured value.
 */
errv_t ads1231_tare(int& grams) {
	// get grams or return error immediately on error
	RETURN_IFN_0( ads1231_get_stable_grams(grams) );

	// success
	ads1231_offset += -grams;

	return 0;
}


/**
 * Get grams from scale if measurement fast enough, otherwise returns
 * with error ADS1231_WOULD_BLOCK. Should not block longer than 10ms.
 */
errv_t ads1231_get_noblock(int& grams) {
	 // ADS1231 supports 10 samples per second. That means after the last
	 // sample we need to wait 100ms. If 90ms passed already, it should be OK.
	unsigned long t = (millis() - ads1231_last_millis) % 100;
	if (t < 90) {
		return ADS1231_WOULD_BLOCK;
	}
	return ads1231_get_grams(grams);
}






