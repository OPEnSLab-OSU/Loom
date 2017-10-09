/*
 Copyright (C) 2011 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include <avr/pgmspace.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "sleep.h"

// Define your sensor type here by commenting and un-commenting
//#define is_analog 3      // also define number of channels
#define is_i2c = 0x68    // also define i2c address of device
/*
// Type conversions for 8.8 fixed-type conversions (high byte integer, low byte fraction, 1.5 = 0x0180)
#define int2fix(a)   (((int)(a))<<7)            //Convert char to fix. a is a char
#define fix2int(a)   ((signed char)((a)>>8))    //Convert fix to char. a is an int
#define float2fix(a) ((int)((a)*256.0))         //Convert float to fix. a is a float
#define fix2float(a) ((float)(a)/256.0)         //Convert fix to float. a is an int 
*/
#ifdef is_analog
  #include "A_message.h"
#endif

#ifdef is_i2c
// Include libraries for serial and i2c devices
  #include "S_message.h"
  #include "I2Cdev.h"
  #include "MPU6050_6Axis_MotionApps20.h"
  MPU6050 mpu; // create instance of MPU6050 called mpu
  MPU6050 accelgyro; // another called accelgyro
// Uncoment one or more of these to determine which readings and format to send
  #define OUTPUT_READABLE_YAWPITCHROLL
  //#define OUTPUT_BINARY_YAWPITCHROLL
  //#define OUTPUT_READABLE_REALACCEL
  //#define OUTPUT_READABLE_WORLDACCEL  // this is pretty cool
  //#define OUTPUT_BINARY_ACCELGYRO
#define INTERRUPT_PIN 3  // use pin 2 on Arduino Uno, pin 3 on Adafruit ProTrinket!
// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

// Place to save accel gyro mpu6050 readings
int16_t ax, ay, az;
int16_t gx, gy, gz;


// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}
#endif
// ** End if i2c check

// Declaire Radio
RF24 radio(9,10);
RF24Network network(radio);

// Node Address Defs
const uint16_t this_node = 02;  // Address of our node, Serial/i2c "S" family 02, 12, 22, etc
const uint16_t base_node = 00;  // Address of destination node in Octal format

// The message that we send is just an unsigned int, containing a sensor reading. 
unsigned int message;

// The pin our sensor is on
const short sensor_pin = A0;
// Pins for sensors
const int temp_pin = A2;
const int voltage_pin = A3;

// How many measurements to take. Good for averaging, Stick to powers of 2
const short num_measurements = 4;

// What voltage is a reading of 1023?
const unsigned voltage_reference = 3 * 256; // 3.0V

// Number of packets we've failed to send since we last sent one
// successfully
uint16_t lost_packets = 0;

// Sleep constants.  In this example, the watchdog timer wakes up
// every 16ms, and every 16th wakeup we power up the radio and send
// a reading.  In real use, these numbers which be much higher.
// Try wdt_8s and 7 cycles for one reading per minute.> 1
//const wdt_prescalar_e wdt_prescalar = wdt_16ms;
//const short sleep_cycles_per_transmission = 16;

const wdt_prescalar_e wdt_prescalar = wdt_16ms;
uint8_t sleep_cycles_per_transmission = 4;

void setup(void)
{
  Serial.begin(57600);
  #ifdef is_i2c
  // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    pinMode(INTERRUPT_PIN, INPUT);
  Serial.println("RF24Network/examples/sensornetTX_S/");
// *** Init MPU 6050 and serial stuff
  accelgyro.initialize();
  mpu.initialize();
  devStatus = mpu.dmpInitialize();
  // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
// make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

// ** end serial stuff
 #endif 
  // Pull node address out of eeprom 

// Analog Stuff if using:
#ifdef is_analog
    Serial.println("RF24Network/examples/sensornetTX_A/");
#endif // End Analog stuff
  // Which node are we?
//  this_node = nodeconfig_read();

  //
  // Prepare sleep parameters
  //

  // Only the leaves sleep.  
  if ( this_node > 0 ) 
    Sleep.begin(wdt_prescalar,sleep_cycles_per_transmission);
    
  //
  // Bring up the RF network
  //

  SPI.begin();
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH); // set powerlevel to max
  radio.setDataRate(RF24_250KBPS); // set to 250kbps for better reception
  network.begin(/*channel*/ 95, /*node address*/ this_node);
}

void loop(void)
{
  // Pump the network regularly
  network.update();

  // send sensor readings to the base if transmitter type node
  if ( this_node > 0 )
  {
// If node type is analog
#ifdef is_analog
    A_message message;
    message.num_readings = is_analog; // number of analog channels to send to hub
                                      // "hi hub, i'm sending you this many sensor readings"
// set message type, defined in A_message.h file
    char message_type = 'A';
#endif

#if (is_analog > 0) //// Take a reading
    message.A0_reading = measure_sonar(0);
#endif 
#if (is_analog > 1) //// Take a reading
    message.A1_reading = measure_analog(1);
#endif 
#if (is_analog > 2) //// Take a reading
    message.A2_reading = measure_fsr_small(2);
#endif 
#if (is_analog > 3) //// Take a reading
    message.A3_reading = measure_analog(3);
#endif 
#if (is_analog > 4) //// Take a reading
    message.A4_reading = measure_analog(4);
#endif 
#if (is_analog > 5) //// Take a reading
    message.A5_reading = measure_analog(5);
#endif 
#if (is_analog > 6) //// Take a reading
    message.A6_reading = measure_analog(6);
#endif 
#if (is_analog > 7) //// Take a reading
    message.A7_reading = measure_analog(7);
#endif 

// If node type is i2c
#ifdef is_i2c
    S_message message;
    message.num_readings = 9; // number of analog channels to send to hub
                                      // "hi hub, i'm sending you this many sensor readings"
// set message type, defined in A_message.h file
    char message_type = 'S';
// Now measure MPU6050, update values in global registers
    measure_mpu6050();

// update packet values from MPU6050
    message.Yaw_reading = ypr[0] * 180/M_PI;
    message.Pitch_reading = ypr[1] * 180/M_PI;
    message.Roll_reading = ypr[2] * 180/M_PI;
    message.AX_reading = ax;
    message.AY_reading = ay;
    message.AZ_reading = az;
    message.GX_reading = gx;
    message.GY_reading = gy;
    message.GZ_reading = gz;
#endif

// do this no matter the node type
//    message.lost_packets = min(lost_packets,0xff); 
    
    // Set up packet transmit
    // By default send to the base
    uint16_t to_node = base_node;
   
   Serial.print("Sending...");
   //delay(10);
   
   //Power up radio
   radio.powerUp();
   // Send data to the base
    RF24NetworkHeader header(to_node,message_type);
    bool ok = network.write(header,&message,sizeof(message));
    if (ok)
    {
      Serial.println("ok.");
      lost_packets = 0;
      // blink green LED here eventually
    }
    else
    {
      Serial.println("transmit fail");
      ++lost_packets;
      // blink red LED here eventually
    }
     
    // Power down the radio.  Note that the radio will get powered back up
    // on the next write() call.
    radio.powerDown();

    // Be sure to flush the serial first before sleeping, so everything
    // gets printed properly
      Serial.flush();
    #ifdef is_i2c
    // flush MPU6050 FIFO to avoid overflows if using i2c
      mpu.resetFIFO();
    #endif
    // Sleep the MCU.  The watchdog timer will awaken in a short while, and
    // continue execution here.
    Sleep.go();
  }

}
// vim:ai:cin:sts=2 sw=2 ft=cpp
