// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include "Adafruit_SHT31.h"

// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define i2c_addr_ms5803 0x77	//0x44, 0x45


//I2C commands, ripped from the datasheet's "figure 2".
const byte MS_RESET =              0x1E;
const byte MS_CONVERTD1_256 =      0x40;
const byte MS_CONVERTD1_512 =      0x42;
const byte MS_CONVERTD1_1024 =     0x44;
const byte MS_CONVERTD1_2048 =     0x46;
const byte MS_CONVERTD1_4096 =     0x48;
const byte MS_CONVERTD2_256 =      0x50;
const byte MS_CONVERTD2_512 =      0x52;
const byte MS_CONVERTD2_1024 =     0x54;
const byte MS_CONVERTD2_2048 =     0x56;
const byte MS_CONVERTD2_4096 =     0x58;
const byte MS_ADCREAD =            0x00;
const byte MS_PROMREAD0 =          0xA0;

// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
// struct config_ms5803_t {

// };

struct state_ms5803_t {
	float 			tempC;		//compensated, final temperature
	unsigned long 	tempNC;		//uncompensated, raw temp value
	float 			pressureC;	//compensated, final pressure
	unsigned long 	pressureNC;	//uncompensated, raw pressure value
};

// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
// struct config_ms5803_t config_ms5803;

struct state_ms5803_t state_ms5803;

uint16_t MS_PROM[8]; //storing calibration data


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_ms5803();
void package_ms5803(OSCBundle *bndl, char packet_header_string[], uint8_t port);
void package_ms5803(OSCBundle *bndl, char packet_header_string[]);
void measure_ms5803();

void PSreset();
bool readPressure();
bool readTemp();


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================
//
// Runs any MS5803 setup
//
// @return  Whether or not sensor initialization was successful
//
bool setup_ms5803() {
	Wire.begin();
	PSreset();

	LOOM_DEBUG_Println("Initialized MS_5803 pressure sensor");
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================


// --- PACKAGE MS5803 --- (Multiplexer Version)
// 
// Adds last read MS5803 sensor readings to provided OSC bundle
//
// @param bndl                  The OSC bundle to be added to
// @param packet_header_string  The device-identifying string to prepend to OSC messages
// @param port                  Which port of the multiplexer the device is plugged into
//
void package_ms5803(OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
	char address_string[255];
	sprintf(address_string, "%s%s%d%s", packet_header_string, "/port", port, "/ms5803/data");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add("temp").add(state_ms5803.tempC);
	msg.add("pressure").add(state_ms5803.pressureC);
	
	bndl->add(msg);
}

void package_ms5803(OSCBundle *bndl, char packet_header_string[])
{
	char address_string[255];

	sprintf(addressString, "%s%s", packet_header_string, "/ms5803_temp");
	bndl->add(addressString).add(state_ms5803.tempC);
	sprintf(addressString, "%s%s", packet_header_string, "/ms5803_pressure");
	bndl->add(addressString ).add(state_ms5803.pressureC);
}


// --- MEASURE MS5803 ---
//
// Gets the current sensor readings of the MS5803 and stores into its state struct
// 
void measure_ms5803() 
{
	if ( readTemp() && readPressure() ) { 
		// calc_Temp_Press(); //calculate the compensated pressure and temperature
		int32_t dT = 0;
		state_ms5803.pressureNC = 0;
		state_ms5803.tempNC = 0;
		
		readTemp();
		
		//calculate compensated temperature
		dT = state_ms5803.tempNC - MS_PROM[5] * uint16_t(1<<8);
		state_ms5803.tempC = (2000 + (dT * MS_PROM[6]) / uint32_t(1<<23))/100.00;

		readPressure();

		//calculate temperature-compensated pressure
		int64_t OFF = MS_PROM[2]* uint32_t(1<<16) + (MS_PROM[4]*dT)/ uint16_t(1<<7);
		
		int64_t SENS = MS_PROM[1]* uint16_t(1<<15) + (MS_PROM[3]*dT)/ uint16_t(1<<8); 
		state_ms5803.pressureC = (((state_ms5803.pressureNC * SENS / uint32_t(1<<21)) - OFF) / uint16_t(1<<15));
	}
}



bool readPressure(){
	Wire.beginTransmission(MS_ADDRESS);//start a transmission sequence
	Wire.write(MS_CONVERTD1_4096);
	Wire.endTransmission();

	delay(10); //4096 OSR conversion requires 9ms max.
	
	Wire.beginTransmission(MS_ADDRESS);
	Wire.write(MS_ADCREAD);
	Wire.endTransmission();

	Wire.requestFrom(MS_ADDRESS, 3);
	if(!Wire.available()) return false;
	else{
		
		int i = 0;
		while(Wire.available() && i<3){
			pressures[i] = Wire.read();
			i++;
		}
		
		//concatenate the three sets of seven bits
		//p1 represents the MSB, p3 represents the LSB
		unsigned long p0 = pressures[0] << (2*8);
		unsigned long p1 = pressures[1] << (8);
		pressureNC = p0 + p1 + pressures[2];
		
		return true; 
	}

}

bool readTemp(){

	// tell the sensor to compensate and convert the temperature values
	// and store them on the ADC register.
	Wire.beginTransmission(MS_ADDRESS);//start a transmission sequence
	Wire.write(MS_CONVERTD2_4096);
	Wire.endTransmission();

	delay(10); //4096 OSR conversion requires 9ms max.

	// tell the sensor we want to prepare to send the ADC values
	Wire.beginTransmission(MS_ADDRESS);
	Wire.write(MS_ADCREAD);
	Wire.endTransmission();
	
	// tell the sensor to send over the ADC values
	Wire.requestFrom(MS_ADDRESS, 3);
	if(!Wire.available()) return false;
	else{
		int i = 0;
		while(Wire.available() && i<3){
			temperatures[i] = Wire.read();
			i++;
		}
		
		unsigned long t0 = temperatures[0] << (2*8);
		unsigned long t1 = temperatures[1] << (8);
		tempNC = (t0 + t1 + temperatures[2]);
		
		return true;
	}
}



void PSreset(){
	Wire.beginTransmission(MS_ADDRESS);
	Wire.write(MS_RESET);
	Wire.endTransmission();
	delay(10);
	
	//get PROM values
	for(int i=0; i<8; i++){
		//we want to read PROM variable 'i'
		Wire.beginTransmission(MS_ADDRESS);
		Wire.write(MS_PROMREAD0 + 2*i);
		Wire.endTransmission();

		//each PROM variable is stored in two bytes
		Wire.requestFrom(MS_ADDRESS, 2);
		/*if(Wire.available()){
			MS_PROM[i] = Wire.read();
		}
		Serial.print("PROM: " + String(MS_PROM[i]) + ", ");
		if(Wire.available()){
			MS_PROM[i] = uint16_t(MS_PROM[i]<<8) + uint16_t(Wire.read());
			Serial.println(String(MS_PROM[i]));
		}
		*/
		if(Wire.available()){
			MS_PROM[i] = Wire.read()<<8;
		}
		if(Wire.available()){
			MS_PROM[i] += Wire.read();
		}
		if(Serial) Serial.println(MS_PROM[i]);
	}
}


