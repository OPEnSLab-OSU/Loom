# Project Loom: Hardware

## Table of Contents
1. [Processors](#processors)
    1. [Adafruit Feather M0](#adafruit-feather-m0)
    2. [Adafruit Feather 32u4](#adafruit-feather-32u4)

2. [Wireless Capabilities](#wireless-capabilities)
    1. [WiFi](#wifi)
        1. [WiFi Dependencies](#wifi-dependencies)
    2. [LoRa](#lora)
        1. [LoRa Dependencies](#lora-dependencies)
        2. [EnableInterrupt.h and RH\_RF95.h compatibility](#enableinterrupth-and-rh_rf95h-compatibility)
    3. [nRF](#nRF)
        1. [nRF Dependencies](#nrf-dependencies)
        2. [Configuring Maximum Message Length](#configuring-maximum-message-length)

3. [Sensors](#sensors)
    1. [I2C Sensors](#i2c-sensors)
        1. [TSL2591 Lux Sensor](#tsl2591-lux-sensor)
        2. [FXOS8700 3-Axis Accelerometer/Magentometer](#fxos8700-3-axis-accelerometermagentometer)
        3. [FXAS21002 3-Axis Gyroscope](#fxas21002-3-axis-gyroscope)
        4. [Sparkfun ZX](#sparkfun-zx)
        5. [SHT31-D](#sht31-d)
        6. [MB1232](#mb1232)
        7. [TCA9548A Multiplexer](#tca9548a-multiplexer)   
    2. [SDI-12 Sensors](#sdi-12-sensors)
        1. [SDI-12 Dependencies](#sdi-12-dependencies)
        2. [Supported SDI-12 Pins](#supported-sdi-12-pins)
    3. [SPI Sensors](#spi-sensors)
        1. [Adafruit Universal Thermocouple Amplifier](#adafruit-universal-thermocouple-amplifier)

4. [Actuators](#actuators)
    1. [SG92R Servo](#sg92r-servo)
    2. [SM_42BYG011_25 Stepper Motor](#sm_42byg011_25-stepper-motor)



## Processors

### Adafruit Feather M0

Resources:
* [Adafruit M0 Documentation](https://learn.adafruit.com/adafruit-feather-m0-basic-proto/overview)
* [ATSAMD21 Datasheet](https://cdn-shop.adafruit.com/product-files/2772/atmel-42181-sam-d21_datasheet.pdf)

The compiler macro `__SAMD21G18A__` can be used to define code blocks specifically for the Feather M0.
For readability, we typically use this macro to define our own, more readable preprocessor
definition `is_M0`.  This can be done by including the following lines in your source code:

``` cpp
#ifdef __SAMD21G18A__
#define is_M0
#endif
```

### Adafruit Feather 32u4

Resources:
* [Adafruit 32u4 Documentation](https://learn.adafruit.com/adafruit-feather-32u4-basic-proto?view=all)
* [ATMega32U4 Datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf)

The compiler macro `__AVR_ATmega32U4__` can be used to define code blocks specifically for the Feather 32u4.
For readability, we typically use this macro to define our own, more readable preprocessor
definition `is_32U4`.  This can be done by including the following lines in your source code:

``` cpp
#ifdef __AVR_ATmega32U4__
#define is_32U4
#endif
```

## Wireless Capabilities

### WiFi

WiFi is supported by both the Adafruit Feather M0 and the Adafruit Feather 32u4.
Features:
* Send and receive arbitrary length Udp packets
* Works in Access Point mode as well as WPA-protected client mode

#### WiFi Dependencies

WiFi is dependent on the Arduino [WiFi101 library](https://github.com/arduino-libraries/WiFi101). This library provides the WiFiUdp object.

### LoRa

LoRa wireless communication is supported by both the Adafruit Feather M0 and the
Adafruit Feather 32u4.

Features:
* Send messages up to 2 km line-of-sight.
* LoRa devices can be addressed with any value between 0 and 255.
* Can send messages of length up to 251 bytes.

#### LoRa Dependencies

All LoRa modules used by Project Loom are provided by the 
[RadioHead library](https://github.com/adafruit/RadioHead). This library
provides both the radio drivers (i.e. `RH_RF95.h`) and a network manager
(i.e. `RHReliableDatagram.h`).

#### EnableInterrupt.h and RH\_RF95.h compatibility

EnableInterrupt.h and RH\_RF95.h both try to define the same interrupt vectors.
To use both of these libraries in the same file, include the following definition
in your source code:

``` cpp
#define EI_NOTEXTERNAL
```

### nRF

The nRF flavor of IoA is comprised with the Nordic nRF24L01+ radio tranceiver.
nRF is supported by both the Adafruit Feather M0 and the Adafruit Feather 32u4.

Features:
* nRF devices can be addressed with any value between 0 and 7.
* Fragmentation allows for messages of any length to be sent with some configuration.
* Multi-hopping is supported.

#### nRF Dependencies

* [RF24](https://github.com/nRF24/RF24)
* [RF24Network](https://github.com/nRF24/RF24Network)

#### Configuring Maximum Message Length

The maximum message length can be adjusted by editing the value of `MAIN_BUFFER_SIZE`,
a variable found in the `RF24Network_config.h` file.

## Sensors

### I2C Sensors

The following I2C Sensors are currently supported by Project Loom:
* [TSL2591](https://learn.adafruit.com/adafruit-tsl2591/overview)
* [FXOS8700](https://learn.adafruit.com/nxp-precision-9dof-breakout/overview)
* [FXAS21002](https://learn.adafruit.com/nxp-precision-9dof-breakout/overview)
* [SPARKFUN ZX](https://learn.adafruit.com/adafruit-sht31-d-temperature-and-humidity-sensor-breakout)
* [SHT31-D](https://www.sparkfun.com/products/13162)
* [MB1232](https://www.maxbotix.com/Ultrasonic_Sensors/MB1232.htm)

The system also supports the following multiplexer in order to allow the use of multiple sensors with the same address:
* [TCA9548A](https://learn.adafruit.com/adafruit-tca9548a-1-to-8-i2c-multiplexer-breakout/overview)

#### TSL2591 Lux Sensor
**Technical Details**
* Approximates Human eye Response
* Extremely wide dynamic range 1 to 600,000,000 Counts
* Lux Range: 188 uLux sensitivity, up to 88,000 Lux input measurements.
* Temperature range: -30 to 80 C
* Voltage range: 3.3-5V into onboard regulator
* I2C 7-bit address 0x29 

**Dependencies Required**
* [Adafruit_TSL2591](https://github.com/adafruit/Adafruit_TSL2591_Library)
* [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

**Available Functions**
* displaySensorDetails: Displays basic info about the sensor
* configureSensor: Sets the gain and integration time
* simpleRead: Grabs a simple read of the luminosity
* advancedRead: Reads both IR and Full Specrtum and converts to lux
* unifiedSensorAPIRead: Reads the data using the unified sensor API

**Additional Notes**
* Settable gain depending on light conditions
* Settable integration time for collecting light

**Datasheet:** [TSL2591 Datasheet](http://www.adafruit.com/datasheets/TSL25911_Datasheet_EN_v1.pdf)

#### FXOS8700 3-Axis Accelerometer/Magentometer
**Technical Details**
* 2-3.6V Supply
* ±2 g/±4 g/±8 g adjustable acceleration range
* ±1200 µT magnetic sensor range
* Output data rates (ODR) from 1.563 Hz to 800 Hz
* 14-bit ADC resolution for acceleration measurements
* 16-bit ADC resolution for magnetic measurements
* I2C 7-bit address 0x1C, 0x1D, 0x1E, 0x1F 

**Dependencies Required**
* [Adafruit_FXOS8700](https://github.com/adafruit/Adafruit_FXOS8700)
* [Adafruit_AHRS](https://github.com/adafruit/Adafruit_AHRS)
* [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

**Available Functions**
* displaySensorDetails_FXOS: Displays the max and min values of the accelrometer and magnetometer
* fxos_sensor_read: Displays accelrometer data in m/s^2 and magnetometer data in uTesla in X,Y,Z direction

**Additional Notes**
* Contained in the same sensor with FXAS21002

**Datasheet:** [FXOS8700 Datasheet]https://cdn-learn.adafruit.com/assets/assets/000/043/458/original/FXOS8700CQ.pdf?1499125614)

#### FXAS21002 3-Axis Gyroscope
**Technical Details**
* 2-3.6V Supply
* ±250/500/1000/2000°/s configurable range
* Output Data Rates (ODR) from 12.5 to 800 Hz
* 16-bit digital output resolution
* 192 bytes FIFO buffer (32 X/Y/Z samples)
* I2C 7-bit address 0x20, 0x21 

**Dependencies Required**
* [Adafruit_FXAS21002C](https://github.com/adafruit/Adafruit_FXAS21002C)
* [Adafruit_AHRS](https://github.com/adafruit/Adafruit_AHRS)
* [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

**Available Functions**
* displaySensorDetails_FXAS: Displays the max and min values
* fxas_sensor_read: Displays the gyrscope speed in reference to X,Y,and Z coordinates in rad/s

**Additional Notes**
* Contained in the same sensor with FXOS8700

**Datasheet:** [FXAS21002 Datasheet](https://cdn-learn.adafruit.com/assets/assets/000/040/671/original/FXAS21002.pdf?1491475056)

#### Sparkfun ZX
**Technical Details**
* 2-3.6V Supply
* I2C 7-bit address 0x10, 0x11 

**Dependencies Required**
* [SparkFun_ZX_Distance_and_Gesture_Sensor_Arduino_Library](https://github.com/sparkfun/SparkFun_ZX_Distance_and_Gesture_Sensor_Arduino_Library/archive/master.zip)

**Available Functions**
* displaySensorDetails_ZX: Displays the model version of the sensor
* zx_sensor_read_pos: Displays the position both in X and Z coordinates of the object in front of the sensor
* zx_sensor_read_gesture: Displays the direction (up, down, left, right) of the object movement

**Additional Notes**
* Can only use either the gesture function or the read function but can't use both at the same time
* Has UART capabilities

**Datasheet:** [Sparkfun ZX Datasheet](https://cdn.sparkfun.com/assets/learn_tutorials/3/4/5/XYZ_Interactive_Technologies_-_ZX_SparkFun_Sensor_Datasheet.pdf)

#### SHT31-D
**Technical Details**
* 2.4-5.5V Supply
* Temperature: -10-125 °C ±0.3°C 
* Relative Humidity:0-100% ±2%
* I2C 7-bit address 0x44, 0x45 

**Dependencies Required**
* [Adafruit_SHT31](https://github.com/adafruit/Adafruit_SHT31)

**Available Functions**
* readTemperature(): Reads in the temperature at the time 
* reatHumidity(): Reads in the humidity at the time
* sht31_sensor_read: Grabs and displays the temperature in Celsius and the humidity percentage
* heater(): Turns on the heater to remove condensation

**Additional Notes**
* Has a heater function to remove condensation

**Datasheet:** [SHT3x-DIS Datasheet](https://cdn-shop.adafruit.com/product-files/2857/Sensirion_Humidity_SHT3x_Datasheet_digital-767294.pdf)

#### MB1232
**Technical Details**
* Operates from 3.0-5.5V
* Range: 0-625cm
* Resolution of 1 cm
* Up to 40Hz reading rate
* 42kHz Ultrasonic sensor measures distance to objects
* I2C 7-bit address 0x70 

**Dependencies Required**
* None

**Available Functions**
* takeRangeReading(): Sends the range command to the sensor to take a reading
* requestRange(): Returns the last range that the sensor read in centimeters

**Additional Notes**
* Same address as multiplexer (0x70)
* **Multiplexer must use another address if being used with this sensor**

**Datasheet:** [MB1232 Datahsheet](https://www.maxbotix.com/documents/I2CXL-MaxSonar-EZ_Datasheet.pdf)

#### TCA9548A Multiplexer
**Technical Details**
* 1.8V - 5V Supply
* Use up to 8 multiplexer simultaneously
* Use up to 64 I2C sensors with the same address (8 per multiplexer)
* I2C 7-bit address 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77 

**Dependencies Required**
* None

**Additional Notes**
* Address can be changed by using solder bridges on the back of the board
* **Initial Multiplexer address (0x70) conflicts with MB1232**

**Datasheet:** [TCA9548A Datasheet](http://www.adafruit.com/datasheets/tca9548a.pdf)

### SDI-12 Sensors

The following SDI-12 sensors are currently supported by Project Loom:
* [Decagon GS3](http://manuals.decagon.com/Integration%20Guides/GS3%20Integrators%20Guide.pdf)
* [Decagon 5TM](http://www.ictinternational.com/content/uploads/2014/03/5TM-Integrators-Guide.pdf)

#### SDI-12 Dependencies
* [Enable Interrupt](https://github.com/GreyGnome/EnableInterrupt)
* [SDI-12 Functionality](https://github.com/EnviroDIY/Arduino-SDI-12)

#### Supported SDI-12 Pins

The following pins are available for use with SDI-12 sensors:

* **Feather M0:** 10, 11, A0, A1, A3, A4, A5
* **Feather 32u4:** 10, 11

Additional pins may be operational, but they have not been verified.

**NOTE:** To use the SDI-12 library with the 32u4, the user must define their own
pin interrupts using the Enable Interrupt library.  The following line from
the file SDI12.h must be uncommented so that external interrupts can be defined:

``` cpp
#define SDI12_EXTERNAL_PCINT // uncomment to use your own PCINT ISRs
```

The following is an example of how the Enable Interrupt library can be used
to define an interrupt on pin 10:

``` cpp
pinMode(10, INPUT_PULLUP);
enableInterrupt(10, handler_function, CHANGE);
```

### SPI Sensors

#### Adafruit Universal Thermocouple Amplifier

Project Loom currently supports operation of the Adafruit MAX31856 Universal
Thermocouple Amplifier in both K-type thermocouple operation and generic voltage
operation.  K-type thermocouple operation allows the user to directly measure
temperatures, when the amplifier is used in conjunction with a K-type thermocouple,
and the generic voltage operation allows the user to measure the voltage across any
thermocouple attached to the amplifier.

Dependencies and Documentation:
* [Adafruit MAX31856 Github](https://github.com/adafruit/Adafruit_MAX31856)
* [Adafruit MAX31856 Overview](https://learn.adafruit.com/adafruit-max31856-thermocouple-amplifier/overview)

**IMPORTANT NOTE:** The Adafruit thermocouple library has been modified to include functionality
to read the thermocouple voltage directly.  The modified library can be found
[here](https://github.com/OPEnSLab-OSU/InternetOfAg/blob/master/Dependencies/Adafruit_MAX31856.zip).

Alternatively, the following function can be added as a public member to the Adafruit\_MAX31856
class in the Adafruit\_MAX31856 library:

``` cpp
float Adafruit_MAX31856::readVoltage(int gain) {
  oneShotTemperature();

  int32_t temp24 = readRegister24(MAX31856_LTCBH_REG);
  if (temp24 & 0x800000) {
    temp24 |= 0xFF000000;
  }

  temp24 >>= 5;

  float tempfloat = temp24/((float)(gain * 209715.2)); //temp24 = gain * 1.6 * 2^17 * vin

  return tempfloat;
}

```

## Actuators

The following Actuators are currently supported by Project Loom:
* [SG92R](https://www.adafruit.com/product/169)
* [SM-42BYG011-25](https://www.adafruit.com/product/324)

### SG92R Servo
**Technical Details**
* Use [8-channel Servo FeatherWing](https://learn.adafruit.com/adafruit-8-channel-pwm-or-servo-featherwing?view=all)
* Up to 8 connections for SG92R
* Rotation range: around 180 degrees(90 in each direction). Position "0" (1.5ms pulse) is middle, "90" (~2ms pulse) is all the way to the right, "-90" (~1ms pulse) is all the way to the left.

SG92R servo uses a 50Hz signal giving a period of 20 ms to control the position of the servo. This position is determined by PWM. The duty cycle, or pulse width is 1ms(0 degrees) to 2ms(180 degrees). Therefore, the minimum pulse width is 1ms and the maximum pulse width is 2ms. The pulse width for specific angle is calculated from below equation. By manipulating pulse width in Code level, Servo can rotates up to 180 degrees.

* Voltage range: 3.3 - 5V into onboard regulator is required for logic voltage and additional 4.7 - 5.4V for each SG92R servo. 
* Current range: SG92R’s idle current with 5V supply is 6 (+/-10mA) and  running current with 5V supply is 220 +/-50mA
* Use I2C (SDA and SCL pins) 

**Dependencies Required**
* [Adafruit PWMServoDriver](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)

**Available Functions**
*  setPWMFreq: Determines how many full ‘pulses’ per second are generated by the IC.

**Additional Notes**
* Settable rotation degree depending on user selection

**Datasheet:** [PCA9685 PWM controller Datasheet](https://cdn-shop.adafruit.com/datasheets/PCA9685.pdf)

### SM_42BYG011_25 Stepper Motor
**Technical Details**
* Use [Adafruit DC Motor+Stepper FeatherWing](https://learn.adafruit.com/adafruit-stepper-dc-motor-featherwing)
* Up to 2 connections for SM-42BYG011-25
* Voltage range: 3.3 - 5V into onboard regulator is required for logic voltage and additional 12V for Stepper motor(SM-42BYG011-25) according to datasheet. Lower voltage (eg. 5V) also can be supplied. It lets the motor generate less heat but perform less torque.
* Use I2C (SDA and SCL pins) 

**Dependencies Required**
* [Adafruit MotorShield](https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library)
* [Adafruit PWMServoDriver](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)

**Available Functions**
* getsStepper: Steps indicates how many steps per revolution the motor has. A 7.5 degrees per step motor has 360/7.5 = 48 steps. SM-42BYG011-25 requires 200 steps. Stepper# is which port it is connected to. Port 1 is for the first stepper motor and port 2 is for the other stepper motor.
* setSpeed: Set the speed of the motor in terms of revolution per minute(rpm).
* Step: #steps is how many steps you’d like the stepper motor tatkes. Direction is either FORWARD(Clockwise) or BACKWARD(Counter clockwise) and the steptype is SINGLE, DOUBLE, INTERLEAVE, or MICROSTEP.

**Additional Notes**
* SINGLE steptype means one coil is activated.
* DOUBLE steptype means two coils are activated at a time for higher torque.
* INTERLEAVE steptype means alternate between single and double to create a half-step in between. It causes stepper motors to run smoother, but in halved speed.
* MICROSTEP steptype used to achieve higher resolution but with a loss in torque at lower speeds.

**Datasheet:** [SM-42BYG011-25 datasheet](https://cdn-shop.adafruit.com/product-files/324/C140-A+datasheet.jpg)