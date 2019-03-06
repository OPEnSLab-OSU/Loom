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
        1. [AS7262 Spectral Sensor (visible)](#as7262-spectral-sensor-visible)
        2. [AS7263 Spectral Sensor (near infrared)](#as7263-spectral-sensor-near-infrared)
        3. [AS72625X Spectral Sensor Triad](#as7265X-spectral-sensor-visible-near-infrared-ultraviolet)
        4. [FXAS21002 3-Axis Gyroscope](#fxas21002-3-axis-gyroscope)
        5. [FXOS8700 3-Axis Accelerometer/Magnetometer](#fxos8700-3-axis-accelerometermagnetometer)
        6. [HX711 Load Cell Amplifier](#hx711-load-cell-amplifier)
        7. [LIS3DH Accelerometer](#lis3dh-accelerometer)
        8. [MB1232 Sonar](#mb1232-sonar)
        9. [MPU6050 Accelerometer / Gyroscope](#accelerometer-gyroscope)
        10. [MS5803 Atmospheric Pressure / Temperature Sensor](#ms5803-atmospheric-pressure-/-temperature-sensor)
        11. [SHT31-D Temperature and Humidity](#sht31-d-temperature-and-humidity)
        12. [TSL2561 Luminosity Sensor](#tsl2561-luminosity-sensor)
        13. [TSL2591 Light Sensor](#tsl2591-light-sensor)
        14. [ZX Gesture Sensor](#zx-distance-and-gesture-sensor)
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
5. [Other Hardware](#other-hardware)
    1. [RTC](#rtc)
    2. [MicroSD](#microsd)
    3. [OLED Display](#oled-display)



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

## I2C Sensors

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

### TSL2591 Light Sensor

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



## SDI-12 Sensors

The following SDI-12 sensors are currently supported by Loom:
* Decagon GS3
* [Decagon 5TM Datasheet](http://www.ictinternational.com/content/uploads/2014/03/5TM-Integrators-Guide.pdf)

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

## SPI Sensors

### MAX31856 Universal Thermocouple Amplifier

Loom currently supports operation of the Adafruit MAX31856 Universal
Thermocouple Amplifier in both K-type thermocouple operation and generic voltage
operation.  K-type thermocouple operation allows the user to directly measure
temperatures, when the amplifier is used in conjunction with a K-type thermocouple,
and the generic voltage operation allows the user to measure the voltage across any
thermocouple attached to the amplifier.

**Resources:** [Adafruit Product Page](https://www.adafruit.com/product/3263)

**Dependencies:** [Adafruit MAX31856](https://github.com/adafruit/Adafruit_MAX31856)

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

### Servos (8-Channel PWM or Servo FeatherWing)

**Resources:** [Adafruit Product Page](https://www.adafruit.com/product/2928) | [PCA9685 Datasheet](https://cdn-shop.adafruit.com/datasheets/PCA9685.pdf)

**Dependencies:** [Adafruit_PWMServorDriver](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)

**Technical Details**

- There's an I2C-controlled PWM driver with a built in clock. That means that, unlike the TLC5940 family, you do not need to continuously send it signal tying up your microcontroller, its completely free running!
- It is 5V compliant, which means you can control it from a 3.3V Feather and still safely drive up to 6V outputs (this is good for when you want to control white or blue LEDs with 3.4+ forward voltages)
- 6 address select pins so you can stack up to 62 of these on a single i2c bus, a total of 992 outputs - that's a lot of servos or LEDs
- Adjustable frequency PWM up to about 1.6 KHz
- 12-bit resolution for each output - for servos, that means about 4us resolution at 60Hz update rate
- Configurable push-pull or open-drain output

**Possible Servos**

- [Adafruit micro servo](https://www.adafruit.com/product/169)

### Stepper Motors (DC Motor + Stepper FeatherWing)

**Resources:** [Adafruit Product Page](https://www.adafruit.com/product/2927) | [TB6612FNG Datasheet](https://cdn-shop.adafruit.com/datasheets/TB6612FNG_datasheet_en_20121101.pdf)

**Dependencies:** [Adafruit_PWMServorDriver](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library) | [Adafruit MotorShield](https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library)

**Technical Details**

- 4 full H-Bridges: the TB6612 chipset provides **1.2A per bridge** with thermal shutdown protection, internal kickback protection diodes. Can run motors on 4.5VDC to 13.5VDC.
- **Up to 4 bi-directional DC** motors with individual 12-bit speed selection (so, about 0.02% resolution)
- **Up to 2 stepper motors** (unipolar or bipolar) with single coil, double coil, interleaved or micro-stepping.
- Motors automatically disabled on power-up
- Big 3.5mm terminal block connectors to easily hook up wires (18-26AWG) and power
- Polarity protected 2-pin terminal block and jumper to connect external power, for separate logic/motor supplies
- Completely stackable design: 5 address-select jumper pads means up to 32 stackable wings: that's 64 steppers or 128 DC motors! What on earth could you do with that many steppers? I have no idea but if you come up with something send us a photo because that would be a pretty glorious project.
- Download the easy-to-use Arduino software library, check out the examples and you're ready to go!

**Possible Stepper Motors**

- [Adafruit stepper motor](https://www.adafruit.com/product/324)

### Relay

**Resources:** [Adafruit Product Page](https://www.adafruit.com/product/3191) | [G5LE Datasheet](https://cdn-shop.adafruit.com/product-files/3191/G5LE-14-DC3-Omron-datasheet-10841140.pdf)

**Dependencies:** None

**Technical Details**

- Latching
- Switch up to 10A of resistive-load current at 120VAC, 5A at 240VAC.

#### Neopixel

**Resources:** [Adafruit Page](https://www.adafruit.com/category/168)

**Dependencies:** [Adafruit Neopixel](https://github.com/adafruit/Adafruit_NeoPixel)

Tri-color LEDs

## Other Hardware

### RTC

**Precision RTC FeatherWing**

[Adafruit Product Page](https://www.adafruit.com/product/3028)

DS3231 RTC

Supports interrupts

**AdaLogger FeatherWing**

 [Adafruit Product Page](https://www.adafruit.com/product/2922)

PCF8523

Library does not support interrupts

### MicroSD

- [AdaLogger FeatherWing](https://www.adafruit.com/product/2922)
- [Breakout](https://www.adafruit.com/product/254)

### OLED Display

- [FeatherWing OLED](https://www.adafruit.com/product/2900)
- [Breakout](https://www.adafruit.com/product/931)