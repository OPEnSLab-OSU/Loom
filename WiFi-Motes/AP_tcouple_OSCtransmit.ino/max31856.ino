
#ifdef tcouple

//Hardware SPI CS pin definition
#define CS_PIN 10

//Thermocouple type definition
#define TCTYPE MAX31856_TCTYPE_K
//#define TCTYPE MAX31856_VMODE_G32

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
      
#elif TCTYPE == MAX31856_VMODE_G32
    //Type VMODE_G32 processing
#endif
}

void udp_tcouple() {
  OSCBundle bndl;

#ifdef CELCIUS
  bndl.add(IDString "/CJTemp_C").add((float)CJTemp);
  bndl.add(IDString "/TCTemp_C").add((float)TCTemp);
#endif

#ifdef FAHRENHEIT
  bndl.add(IDString "/CJTemp_F").add((float)(CJTemp * 1.8 + 32));
  bndl.add(IDString "/TCTemp_F").add((float)(TCTemp * 1.8 + 32));
#endif

  Udp.beginPacket(ip_broadcast, 9436);
    bndl.send(Udp);
  Udp.endPacket();
}

#endif
