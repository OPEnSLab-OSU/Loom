//https://www.instructables.com/id/Track-CO2-Carbon-Dioxide-Levels-With-Ardunio-and-C/

// AN-126 Demo of K-30 using Software Serial
#include <SoftwareSerial.h>
/*
 Basic Arduino example for K-Series sensor
 Created by Jason Berger
 Co2meter.com
*/
#include "SoftwareSerial.h"
SoftwareSerial K_30_Serial(12,13); //Sets up a virtual serial port
//SoftwareSerial K_30_Serial(12,13); //Sets up a virtual serial port
 //Using pin 12 for Rx and pin 13 for Tx
byte readCO2[] = {0xFE, 0X44, 0X00, 0X08, 0X02, 0X9F, 0X25}; //Command packet to read Co2 (see app note)
byte response[] = {0,0,0,0,0,0,0}; //create an array to store the response
//multiplier for value. default is 1. set to 3 for K-30 3% and 10 for K-33 ICB
int valMultiplier = 1;
void setup()
{
 // put your setup code here, to run once:
 Serial.begin(9600); //Opens the main serial port to communicate with the computer
 K_30_Serial.begin(9600); //Opens the virtual serial port with a baud of 9600
 Serial.println(" Demo of AN-126 Software Serial and K-40 Sensor");
}
void loop()
{
 sendRequest(readCO2);
 unsigned long valCO2 = getValue(response);
 Serial.print("Co2 ppm = ");
 Serial.println(valCO2);
 delay(2000);
}
void sendRequest(byte packet[])
{
 while(!K_30_Serial.available()) //keep sending request until we start to get a response
 {
  Serial.println("waiting for Software.serial port availability");
 K_30_Serial.write(readCO2,7);
 delay(50); 
 }
 int timeout=0; //set a timeout counter
 while(K_30_Serial.available() < 7 ) //Wait to get a 7 byte response
 {
 timeout++;
 if(timeout > 10) //if it takes too long there was probably an error
 {
 while(K_30_Serial.available()) //flush whatever we have
 K_30_Serial.read();
 break; //exit and try again
 }
 delay(50);
 }
 for (int i=0; i < 7; i++)
 {
 response[i] = K_30_Serial.read();
 }
}
unsigned long getValue(byte packet[])
{
 int high = packet[3]; //high byte for value is 4th byte in packet in the packet
 int low = packet[4]; //low byte for value is 5th byte in the packet
 unsigned long val = high*256 + low; //Combine high byte and low byte with this formula to get value
 return val* valMultiplier;
}
