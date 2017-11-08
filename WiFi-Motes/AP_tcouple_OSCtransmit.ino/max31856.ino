
#ifdef tcouple

//Hardware SPI CS pin definition
#define CS_PIN 10

//Thermocouple type definition
//#define TCTYPE MAX31856_TCTYPE_K
#define TCTYPE MAX31856_VMODE_G32
//#define TCTYPE MAX31856_VMODE_G8

#if TCTYPE == MAX31856_VMODE_G32
#define GAIN 32
#elif TCTYPE == MAX31856_VMODE_G8
#define GAIN 8
#endif

//Thermocouple unit definition
#define FAHRENHEIT
#define CELCIUS

#include <Adafruit_MAX31856.h>

//Provide CS pin to initialize hardward SPI
Adafruit_MAX31856 max = Adafruit_MAX31856(CS_PIN);

//setup
void tcouple_setup() {
  max.begin();
  max.setThermocoupleType(TCTYPE);
}

void measure_tcouple() {
#if TCTYPE == MAX31856_TCTYPE_K
  //Type K processing
  //Cold Junction Temp
  CJTemp = max.readCJTemperature();
  TCTemp = max.readThermocoupleTemperature();

  //Serial.print("Cold Junction Temp: "); Serial.println(CJTemp);
  //Serial.print("Thermocouple Temp: "); Serial.println(TCTemp);
  
  // Check and print any faults
  uint8_t fault = max.readFault();
  if (fault) {
    if (fault & MAX31856_FAULT_CJRANGE) Serial.println("Cold Junction Range Fault");
    if (fault & MAX31856_FAULT_TCRANGE) Serial.println("Thermocouple Range Fault");
    if (fault & MAX31856_FAULT_CJHIGH)  Serial.println("Cold Junction High Fault");
    if (fault & MAX31856_FAULT_CJLOW)   Serial.println("Cold Junction Low Fault");
    if (fault & MAX31856_FAULT_TCHIGH)  Serial.println("Thermocouple High Fault");
    if (fault & MAX31856_FAULT_TCLOW)   Serial.println("Thermocouple Low Fault");
    if (fault & MAX31856_FAULT_OVUV)    Serial.println("Over/Under Voltage Fault");
    if (fault & MAX31856_FAULT_OPEN)    Serial.println("Thermocouple Open Fault");
  }
      
#elif (TCTYPE == MAX31856_VMODE_G32 || TCTYPE == MAX31856_VMODE_G8)
  //Type VMODE_G32/VMODE_G8 processing
  
  int32_t temp24 = readRegister24(MAX31856_LTCBH_REG); //Grabs 24 bits from registers
  if (temp24 & 0x800000) {
    temp24 |= 0xFF000000; //This keeps the sign the same from the 24-bit signed int
  }

  temp24 >>= 5; //Only want the top 19 bits from the original data.

  vin = temp24/((float)(GAIN * 209715.2)); //temp24 = GAIN * 1.6 * 2^17 * vin
  
#endif
}

void udp_tcouple() {
  OSCBundle bndl;

#if TCTYPE == MAX31856_TCTYPE_K

#ifdef CELCIUS
  bndl.add(IDString "/CJTemp_C").add((float)CJTemp);
  bndl.add(IDString "/TCTemp_C").add((float)TCTemp);
#endif

#ifdef FAHRENHEIT
  bndl.add(IDString "/CJTemp_F").add((float)(CJTemp * 1.8 + 32));
  bndl.add(IDString "/TCTemp_F").add((float)(TCTemp * 1.8 + 32));
#endif

#elif (TCTYPE == MAX31856_VMODE_G32 || TCTYPE == MAX31856_VMODE_G8)
  bndl.add(IDString "/voltage").add((float)(vin));
#endif


  Udp.beginPacket(ip_broadcast, 9436);
    bndl.send(Udp);
  Udp.endPacket();
}

#endif
