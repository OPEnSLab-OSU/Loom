//from http://playground.arduino.cc/Code/EEPROMWriteAnything

#include <EEPROM.h>
#include <Arduino.h>  // for type definitions


// --- EEPROM WRITE ANYTHING ---
//
// Write to EEPROM any data structure or variable
//
// @param ee     
// @param value   to to write
//
// @return 
//
template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
	const byte* p = (const byte*)(const void*)&value;
	unsigned int i;
	for (i = 0; i < sizeof(value); i++)
		EEPROM.write(ee++, *p++);
	return i;
}


// --- EEPROM READ ANYTHING ---
//
// Read from EEPROM a stored data structure or variable
//
// @param ee     Starting   
// @param value  Struct to read into
//
// @return 
//
template <class T> int EEPROM_readAnything(int ee, T& value)
{
	byte* p = (byte*)(void*)&value;
	unsigned int i;
	for (i = 0; i < sizeof(value); i++)
		*p++ = EEPROM.read(ee++);
	return i;
}







