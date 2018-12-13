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
        2. [TSL2561 Lux Sensor](#tsl2561-lux-sensor)
        3. [FXOS8700 3-Axis Accelerometer/Magnetometer](#fxos8700-3-axis-accelerometermagnetometer)
        4. [FXAS21002 3-Axis Gyroscope](#fxas21002-3-axis-gyroscope)
        5. [ZX Gesture Sensor](#zx-distance-and-gesture-sensor)
        6. [SHT31-D Temperature and Humidity](#sht31-d-temperature-and-humidity)
        7. [MB1232 Sonar](#mb1232-sonar)
        8. [MPU6050 Accelerometer / Gyroscope](#accelerometer-gyroscope)
        9. [LIS3DH Accelerometer](#lis3dh-accelerometer)
        10. [MS5803 Atmospheric Pressure / Temperature Sensor](#ms5803-atmospheric-pressure-/-temperature-sensor)
        11. [HX711 Load Cell Amplifier](#hx711-load-cell-amplifier)
        12. [AS7262 Spectral Sensor (visible)](#as7262-spectral-sensor-visible)
        13. [AS7263 Spectral Sensor (near infrared)](#as7263-spectral-sensor-near-infrared)
        14. [AS72625X Spectral Sensor Triad](#as7265X-spectral-sensor-visible-near-infrared-ultraviolet)
        15. [TCA9548A Multiplexer](#tca9548a-multiplexer)   
    2. [SDI-12 Sensors](#sdi-12-sensors)
        1. [SDI-12 Dependencies](#sdi-12-dependencies)
        2. [Supported SDI-12 Pins](#supported-sdi-12-pins)
    3. [SPI Sensors](#spi-sensors)
        1. [Adafruit Universal Thermocouple Amplifier](#adafruit-universal-thermocouple-amplifier)

4. [Actuators](#actuators)
    1. [SG92R Servo](#sg92r-servo)
    2. [SM_42BYG011_25 Stepper Motor](#sm_42byg011_25-stepper-motor)
    3. [Relay](#relay)
    4. [Neopixel](#neopixel)



## Processors

### Adafruit Feather M0

**Resources**: [Adafruit M0 Documentation](https://learn.adafruit.com/adafruit-feather-m0-basic-proto/overview) | [ATSAMD21 Datasheet](https://cdn-shop.adafruit.com/product-files/2772/atmel-42181-sam-d21_datasheet.pdf)

The compiler macro `__SAMD21G18A__` can be used to define code blocks specifically for the Feather M0.
For readability, we typically use this macro to define our own, more readable preprocessor
definition `is_M0`.  This can be done by including the following lines in your source code:

``` cpp
#ifdef __SAMD21G18A__
    #define is_M0
#endif
```

### Adafruit Feather 32u4

**Resources**: [Adafruit 32u4 Documentation](https://learn.adafruit.com/adafruit-feather-32u4-basic-proto?view=all) | [ATMega32U4 Datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7766-8-bit-AVR-ATmega16U4-32U4_Datasheet.pdf)

The compiler macro `__AVR_ATmega32U4__` can be used to define code blocks specifically for the Feather 32u4. For readability, we typically use this macro to define our own, more readable preprocessor
definition `is_32U4`.  This can be done by including the following lines in your source code:

``` cpp
#ifdef __AVR_ATmega32U4__
    #define is_32U4
#endif
```

*Note – The Feather 32u4 has less support than the Feather M0 due to its limitations*

## Wireless Capabilities

### WiFi

**Dependencies:** [WiFi101](https://github.com/arduino-libraries/WiFi101)

WiFi is supported by both the Adafruit Feather M0 and the Adafruit Feather 32u4.

**Features:**

* Send and receive arbitrary length UDP packets
* Works in Access Point mode as well as WPA-protected client mode

### LoRa

**Dependencies:** [RadioHead](https://github.com/adafruit/RadioHead)

LoRa wireless communication is supported by both the Adafruit Feather M0 and the
Adafruit Feather 32u4.

**Features:**

* Send messages up to 2 km line-of-sight.
* LoRa devices can be addressed with any value between 0 and 255.
* Can send messages of length up to 251 bytes.

**EnableInterrupt.h and RH\_RF95.h compatibility**

EnableInterrupt.h and RH\_RF95.h both try to define the same interrupt vectors.
To use both of these libraries in the same file, include the following definition
in your source code:

``` cpp
#define EI_NOTEXTERNAL
```

### nRF

**Dependencies:** [RF24](https://github.com/nRF24/RF24) | [RF24Network](https://github.com/nRF24/RF24Network)

The libraries for nRF support - [RF24](https://github.com/nRF24/RF24) and [RF24Network](https://github.com/nRF24/RF24Network) - need to be the optimized forks, not the defaults provided the Arduino library manager. The correct versions are provided in the [Loom dependencies](https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Dependencies) folder.

The nRF flavor of IoA is comprised with the Nordic nRF24L01+ radio tranceiver.
nRF is supported by both the Adafruit Feather M0 and the Adafruit Feather 32u4.

**Features:**

* nRF devices can be addressed with any value between 0 and 5.
* Fragmentation allows for messages of any length to be sent with some configuration.
* Multi-hopping is supported.

**Configuring Maximum Message Length**

The maximum message length can be adjusted by editing the value of `MAIN_BUFFER_SIZE`,
a variable found in the `RF24Network_config.h` file.

## Sensors

### I2C Sensors

The following I2C Sensors are currently supported by Project Loom:
* AS7262 Spectral Sensor (visible)
* AS7263 Spectral Sensor (near infrared)
* AS7265X Spectral Sensor Triad (visible, near infrared, ultraviolet)
* FXAS21002 Gyroscope
* FXOS8700 Accelerometer / Magnetometer
* HX711 Load Cell
* LIS3DH Accelerometer
* MB1232 Sonar
* MPU6050 Accelerometer / Gyroscope
* MS5803 Atmospheric Pressure / Temperature Sensor 
* SHT31-D Temperature / Humidity
* TSL2561 Lux Sensor
* TSL2591Lux Sensor
* ZX Gesture Sensor ZX Distance Sensor

The system also supports the following multiplexer in order to allow the use of multiple sensors with the same address:
* TCA9548A I2C Multiplexer



### AS7262 Spectral Sensor (visible)

**Resources:** [SparkFun Product Page](https://www.sparkfun.com/products/14347) | [AS7262 Datasheet](https://cdn.sparkfun.com/assets/f/b/c/c/f/AS7262.pdf) |[GitHub](https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS726X)

**Dependencies:** [SparkFun AS726X](https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS726X/tree/master/Libraries/Arduino)

**Technical Details**

- 6 visible channels: 450nm, 500nm, 550nm, 570nm, 600nm and 650nm, each with 40nm FWHM
- Visible filter set realized by silicon interference filters
- 16-bit ADC with digital access
- Programmable LED drivers
- 2.7V to 3.6V with I2C interface
- 2x Qwiic connectors

### AS7263 Spectral Sensor (near infrared)
**Resources:** [SparkFun Product Page](https://www.sparkfun.com/products/14351) | [AS7263 Datasheet](https://cdn.sparkfun.com/assets/1/b/7/3/b/AS7263.pdf) | [GitHub](https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS726X)

**Dependencies:** [SparkFun_AS726X](https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS726X/tree/master/Libraries/Arduino)

**Technical Details**

- 6 near-IR channels: 610nm, 680nm, 730nm, 760nm, 810nm and 860nm, each with 20nm FWHM
- NIR filter set realized by silicon interference filters
- 16-bit ADC with digital access
- Programmable LED drivers
- 2.7V to 3.6V with I2C interface
- 2x Qwiic connectors

### AS7265X Spectral Sensor Triad (visible, near infrared, ultraviolet)

**Resources:** [SparkFun Product Page](https://www.sparkfun.com/products/15050) | [AS7265x Datasheet](https://cdn.sparkfun.com/assets/c/2/9/0/a/AS7265x_Datasheet.pdf)

**Dependencies:** [SparkFun_AS7265x](https://github.com/sparkfun/SparkFun_AS7265x_Arduino_Library)

**Technical Details**

- Selectable interface: I2C or Serial (115200bps)
- 18 frequencies of light sensing from 410nm to 940nm
- 28.6 nW/cm2 per count
- Accuracy of +/-12%
- Integrated 405nm UV, 5700k White, and 875nm IR LEDs
- Software control over each illumination LED as well as current control
- Optional external bulb or illumination control
- Programmed with latest firmware from AMS

### FXAS21002 3-Axis Gyroscope

**Resources:** [Adafruit Product Page](https://www.adafruit.com/product/3463) | [FXAS21002 Datasheet](https://cdn-learn.adafruit.com/assets/assets/000/040/671/original/FXAS21002.pdf?1491475056)

**Dependencies:** [Adafruit_FXAS21002C](https://github.com/adafruit/Adafruit_FXAS21002C) | [Adafruit_AHRS](https://github.com/adafruit/Adafruit_AHRS) |  [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

**Technical Details**

- 2-3.6V Supply
- ±250/500/1000/2000°/s configurable range
- Output Data Rates (ODR) from 12.5 to 800 Hz
- 16-bit digital output resolution
- 192 bytes FIFO buffer (32 X/Y/Z samples)
- I2C 7-bit address 0x20, 0x21 

**Additional Notes**

- Contained in the same sensor with FXOS8700

### FXOS8700 3-Axis Accelerometer/Magentometer

**Resources:** [Adafruit Product Page](https://www.adafruit.com/product/3463) | [FXOS8700 Datasheet](https://cdn-learn.adafruit.com/assets/assets/000/043/458/original/FXOS8700CQ.pdf?1499125614)

**Dependencies:** [Adafruit_FXOS8700](https://github.com/adafruit/Adafruit_FXOS8700) | [Adafruit_AHRS](https://github.com/adafruit/Adafruit_AHRS) | [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

**Technical Details**

- 2-3.6V Supply
- ±2 g/±4 g/±8 g adjustable acceleration range
- ±1200 µT magnetic sensor range
- Output data rates (ODR) from 1.563 Hz to 800 Hz
- 14-bit ADC resolution for acceleration measurements
- 16-bit ADC resolution for magnetic measurements
- I2C 7-bit address 0x1C, 0x1D, 0x1E, 0x1F 

**Additional Notes**

- Contained in the same sensor with FXAS21002

### HX711 Load Cell Amplifier

**Resources:** [SparkFun Product Page](https://www.sparkfun.com/products/13879) | [HX711 Datasheet](https://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf)

**Dependencies:** [HX711](https://github.com/bogde/HX711)

**Technical Details**

- Operation Voltage: 2.7V–5V
- Operation Current: < 1.5mA
- Selectable 10SPS or 80SPS output data rate
- Simultaneous 50 and 60Hz supply rejection

### LIS3DH Accelerometer

**Resources:** [SparkFun Product Page](https://www.sparkfun.com/products/13963) | [LIS3DH Datasheet](https://cdn.sparkfun.com/assets/b/c/1/3/a/CD00274221.pdf) | [GitHub](https://github.com/sparkfun/LIS3DH_Breakout)

**Dependencies:** [SparkFunLIS3DH](https://github.com/sparkfun/SparkFun_LIS3DH_Arduino_Library)

**Technical Details**

- 1.7V–3.6V
- Three Modes:
  - Power-Down
  - Normal
  - Low-Power
- ±2g/±4g/±8g/±16g Dynamically Selectable Fullscale
- 10bit, 32-Level FIFO
- 6D/4D Orientation
- Free-fall Detection
- Motion Detection
- Embedded Temperature Sensor

### MB1232 Sonar

**Resources:** [MaxBoti Product Pagex](https://www.maxbotix.com/Ultrasonic_Sensors/MB1232.htm) | [MB1232 Datahsheet](https://www.maxbotix.com/documents/I2CXL-MaxSonar-EZ_Datasheet.pdf)

**Dependencies:** None

**Technical Details**

- Operates from 3.0-5.5V
- Range: 0-625cm
- Resolution of 1 cm
- Up to 40Hz reading rate
- 42kHz Ultrasonic sensor measures distance to objects
- I2C 7-bit address 0x70

**Additional Notes**

- Same address as multiplexer (0x70)
- **Multiplexer must use another address if being used with this sensor**

### MPU6050 Accelerometer / Gyroscope

**Resources:** [Amazon Product Page](https://www.amazon.com/MPU-6050-MPU6050-Accelerometer-Gyroscope-Converter/dp/B008BOPN40/ref=asc_df_B008BOPN40/?tag=hyprod-20&linkCode=df0&hvadid=309773039951&hvpos=1o2&hvnetw=g&hvrand=3728702383655605371&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=1024429&hvtargid=pla-366166873147&psc=1) | [MPU6050 Datasheet](https://store.invensense.com/datasheets/invensense/MPU-6050_DataSheet_V3%204.pdf)

**Dependencies:** [MPU6050](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050)

**Technical Details**

- Use the chip: MPU-6050
- Power supply :3-5v (internal low dropout regulator)
- Communication modes: standard IIC communications protocol
- Chip built-in 16bit AD converter, 16-bit data output
- Gyroscope range: ± 250 500 1000 2000 ° / s

### MS5803 Atmospheric Pressure / Temperature Sensor 

**Resources:** [Digi-Key Product Page](https://www.digikey.com/product-detail/en/te-connectivity-measurement-specialties/MS580302BA01-00/223-1624-5-ND/5277629) | [MS5803-02BA Datasheet](https://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=MS5803-02BA&DocType=Data+Sheet&DocLang=English)

**Dependencies:** [MS5803_02](https://github.com/millerlp/MS5803_02)

**Technical Details**

- High resolution module, 20cm 
- Fast conversion down to 1 ms 
- Low power, 1 μA (standby < 0.15 μA) 
- Integrated digital pressure sensor (24 bit ΔΣ ADC) 
- Supply voltage 1.8 to 3.6 V 
- Operating range: 300 to 1100 mbar, -40 to +85 °C 
- Extended Pressure Range: 10 to 2000mbar 
- I2C and SPI interface (Mode 0, 3) 
- No external components (Internal oscillator) 
- Excellent long term stability 
- Hermetically sealable for outdoor devices 

**Additional Notes**

- Does not come with PCB / breakout

### SHT31-D Temperature / Humidity

**Resources:** [Adafruit Product Page](https://www.adafruit.com/product/2857) | [SHT3x-DIS Datasheet](https://cdn-shop.adafruit.com/product-files/2857/Sensirion_Humidity_SHT3x_Datasheet_digital-767294.pdf)

**Dependencies:** [Adafruit_SHT31](https://github.com/adafruit/Adafruit_SHT31)

**Technical Details**

- 2.4-5.5V Supply
- Temperature: -10-125 °C ±0.3°C 
- Relative Humidity:0-100% ±2%
- I2C 7-bit address 0x44, 0x45 

**Additional Notes**

- Has a heater function to remove condensation

### TSL2561 Luminosity Sensor

**Resources:** [Adafruit Product Page](https://www.adafruit.com/product/439) | [TSL2561 Datasheet](https://cdn-shop.adafruit.com/datasheets/TSL2561.pdf)

**Dependencies:** [Adafruit_TSL2561_U](https://github.com/adafruit/Adafruit_TSL2561) | [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

**Technical Details**

- Approximates Human eye Response 
- Precisely Measures Illuminance in Diverse Lighting Conditions
- Temperature range: -30 to 80 *C
- Dynamic range (Lux): 0.1 to 40,000 Lux
- Voltage range: 2.7-3.6V
- Interface: I2C
- This board/chip uses I2C 7-bit addresses 0x39, 0x29, 0x49, selectable with jumpers

### TSL2591 Lux Sensor

**Resources:** [Adafruit Product Page](https://www.adafruit.com/product/1980) | [TSL2591 Datasheet](http://www.adafruit.com/datasheets/TSL25911_Datasheet_EN_v1.pdf)

**Dependencies:** [Adafruit_TSL2591](https://github.com/adafruit/Adafruit_TSL2591_Library) | [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

**Technical Details**

* Approximates Human eye Response
* Extremely wide dynamic range 1 to 600,000,000 Counts
* Lux Range: 188 uLux sensitivity, up to 88,000 Lux input measurements.
* Temperature range: -30 to 80 C
* Voltage range: 3.3-5V into onboard regulator
* I2C 7-bit address 0x29 

**Additional Notes**
* Settable gain depending on light conditions
* Settable integration time for collecting light 

### ZX Distance and Gesture Sensor

**Resources:** [SparkFun Product Page](https://www.sparkfun.com/products/13162) | [Sparkfun ZX Datasheet](https://cdn.sparkfun.com/assets/learn_tutorials/3/4/5/XYZ_Interactive_Technologies_-_ZX_SparkFun_Sensor_Datasheet.pdf)

**Dependencies:** [ZX_Sensor](https://github.com/sparkfun/SparkFun_ZX_Distance_and_Gesture_Sensor_Arduino_Library/archive/master.zip)

**Technical Details**

* 2-3.6V Supply
* I2C 7-bit address 0x10, 0x11

**Additional Notes**
* Can only use either the gesture function or the read function but can't use both at the same time
* Has UART capabilities

### TCA9548A Multiplexer

**Product Page:** [Adafruit](https://www.adafruit.com/product/2717) | [TCA9548A Datasheet](http://www.adafruit.com/datasheets/tca9548a.pdf)

**Dependencies:** None

**Technical Details**
* 1.8V - 5V Supply
* Use up to 8 multiplexer simultaneously
* Use up to 64 I2C sensors with the same address (8 per multiplexer)
* I2C 7-bit address 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77 

**Additional Notes**

* Address can be changed by using solder bridges on the back of the board
* **Initial Multiplexer address (0x70) conflicts with MB1232**



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

**Datasheet:** [PCA9685 PWM controller Datasheet](https://cdn-shop.adafruit.com/datasheets/PCA9685.pdf)

**Dependencies:** [Adafruit PWMServoDriver](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)

**Technical Details**

* Use [8-channel Servo FeatherWing](https://learn.adafruit.com/adafruit-8-channel-pwm-or-servo-featherwing?view=all)
* Up to 8 connections for SG92R
* Rotation range: around 180 degrees(90 in each direction). Position "0" (1.5ms pulse) is middle, "90" (~2ms pulse) is all the way to the right, "-90" (~1ms pulse) is all the way to the left.

SG92R servo uses a 50Hz signal giving a period of 20 ms to control the position of the servo. This position is determined by PWM. The duty cycle, or pulse width is 1ms(0 degrees) to 2ms(180 degrees). Therefore, the minimum pulse width is 1ms and the maximum pulse width is 2ms. The pulse width for specific angle is calculated from below equation. By manipulating pulse width in Code level, Servo can rotates up to 180 degrees.

* Voltage range: 3.3 - 5V into onboard regulator is required for logic voltage and additional 4.7 - 5.4V for each SG92R servo. 
* Current range: SG92R’s idle current with 5V supply is 6 (+/-10mA) and  running current with 5V supply is 220 +/-50mA
* Use I2C (SDA and SCL pins) 

**Available Functions**

*  setPWMFreq: Determines how many full ‘pulses’ per second are generated by the IC.

**Additional Notes**
* Settable rotation degree depending on user selection

### SM_42BYG011_25 Stepper Motor

**Datasheet:** [SM-42BYG011-25 datasheet](https://cdn-shop.adafruit.com/product-files/324/C140-A+datasheet.jpg)

**Dependencies:** [Adafruit MotorShield](https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library) | [Adafruit PWMServoDriver](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)

**Technical Details**
* Use [Adafruit DC Motor+Stepper FeatherWing](https://learn.adafruit.com/adafruit-stepper-dc-motor-featherwing)
* Up to 2 connections for SM-42BYG011-25
* Voltage range: 3.3 - 5V into onboard regulator is required for logic voltage and additional 12V for Stepper motor(SM-42BYG011-25) according to datasheet. Lower voltage (eg. 5V) also can be supplied. It lets the motor generate less heat but perform less torque.
* Use I2C (SDA and SCL pins) 

**Available Functions**
* getsStepper: Steps indicates how many steps per revolution the motor has. A 7.5 degrees per step motor has 360/7.5 = 48 steps. SM-42BYG011-25 requires 200 steps. Stepper# is which port it is connected to. Port 1 is for the first stepper motor and port 2 is for the other stepper motor.
* setSpeed: Set the speed of the motor in terms of revolution per minute(rpm).
* Step: #steps is how many steps you’d like the stepper motor tatkes. Direction is either FORWARD(Clockwise) or BACKWARD(Counter clockwise) and the steptype is SINGLE, DOUBLE, INTERLEAVE, or MICROSTEP.

**Additional Notes**
* SINGLE steptype means one coil is activated.
* DOUBLE steptype means two coils are activated at a time for higher torque.
* INTERLEAVE steptype means alternate between single and double to create a half-step in between. It causes stepper motors to run smoother, but in halved speed.
* MICROSTEP steptype used to achieve higher resolution but with a loss in torque at lower speeds.

### Relay

**Datasheet:**

**Dependencies**

**Technical Details**

- 

**Additional Notes**

- 

#### Neopixel

**Datasheet:**

**Dependencies**

**Technical Details**

- 

**Additional Notes**

- 

#### 