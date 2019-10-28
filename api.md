# Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`Enable`](#namespace_enable) | Blocks modules the can be toggled on/off or selected between a variety of selection.
`namespace `[`factory`](#namespacefactory) | 
`class `[`FactoryBase`](#class_factory_base) | Base class that factory is derived from.
`class `[`Loom_Analog`](#class_loom___analog) | Analog pin manager module.
`class `[`Loom_AS7262`](#class_loom___a_s7262) | AS7262 Visible spectrum spectral sensor module.
`class `[`Loom_AS7263`](#class_loom___a_s7263) | AS7263 Near Infrared spectrum spectral sensor module.
`class `[`Loom_AS7265X`](#class_loom___a_s7265_x) | AS72625X 3 Spectrum (Visible, near IR, UV) spectral sensor module.
`class `[`Loom_Bluetooth`](#class_loom___bluetooth) | Bluetooth communication platform module.
`class `[`Loom_Decagon5TM`](#class_loom___decagon5_t_m) | Decagon 5TM soil moisture sensor module.
`class `[`Loom_DecagonGS3`](#class_loom___decagon_g_s3) | Decagon GS3 soil moisture sensor module.
`class `[`Loom_Digital`](#class_loom___digital) | Digital pin manager module.
`class `[`Loom_DS3231`](#class_loom___d_s3231) | DS3231 RTC module.
`class `[`Loom_Ethernet`](#class_loom___ethernet) | InternetPlat built off of SSLClient running over an Ethernet Featherwing.
`class `[`Loom_FXAS21002`](#class_loom___f_x_a_s21002) | FXAS21002 3-Axis Gyroscope snesor module.
`class `[`Loom_FXOS8700`](#class_loom___f_x_o_s8700) | FXOS8700 3-Axis Acceleromter/Magnetometer sensor module.
`class `[`Loom_GoogleSheets`](#class_loom___google_sheets) | Module taking in and translating JSON into data understood by the Google Sheets script API.
`class `[`Loom_Interrupt_Manager`](#class_loom___interrupt___manager) | Submanager for managing interrupts, timers, and stopwatches.
`class `[`Loom_LIS3DH`](#class_loom___l_i_s3_d_h) | LIS3DH Accelerometer sensor module.
`class `[`Loom_LoRa`](#class_loom___lo_ra) | LoRa communication platform module.
`class `[`Loom_MAX31855`](#class_loom___m_a_x31855) | MAX31855 thermocouple temperature module.
`class `[`Loom_MAX31856`](#class_loom___m_a_x31856) | MAX31856 thermocouple module.
`class `[`Loom_MaxPub`](#class_loom___max_pub) | Module taking in and translating JSON into data understood by the Google Sheets script API.
`class `[`Loom_MaxSub`](#class_loom___max_sub) | Module for receiving data from Max.
`class `[`Loom_MB1232`](#class_loom___m_b1232) | MB1232 Sonar sensor module.
`class `[`Loom_MMA8451`](#class_loom___m_m_a8451) | MMA8451 accelerometer sensor module.
`class `[`Loom_Module_Stub`](#class_loom___module___stub) | Stub class to return pointer to instead of nullptr.
`class `[`Loom_MPU6050`](#class_loom___m_p_u6050) | MPU6050 6 axis gyroscope and accelerometer sensor module.
`class `[`Loom_MS5803`](#class_loom___m_s5803) | MS5803 Atmospheric Pressure / Temperature sensor module.
`class `[`Loom_Multiplexer`](#class_loom___multiplexer) | Multiplexer manager for interfacing with multiple I2C sensors.
`class `[`Loom_Neopixel`](#class_loom___neopixel) | Neopixel tri-color LED actuator module.
`class `[`Loom_nRF`](#class_loom__n_r_f) | Nordic Radio communication platform module.
`class `[`Loom_OLED`](#class_loom___o_l_e_d) | OLED logging platform module.
`class `[`Loom_PCF8523`](#class_loom___p_c_f8523) | PCF8523 RTC module.
`class `[`Loom_Relay`](#class_loom___relay) | Relay actuator module.
`class `[`Loom_SD`](#class_loom___s_d) | SD logging platform module.
`class `[`Loom_Servo`](#class_loom___servo) | Servo actuator module.
`class `[`Loom_SHT31D`](#class_loom___s_h_t31_d) | SHT31D Temperature / Humidty sensor module.
`class `[`Loom_Sleep_Manager`](#class_loom___sleep___manager) | Submanager to manage sleep functionality.
`class `[`Loom_SpoolPublish`](#class_loom___spool_publish) | Loom Spool publishing platform.
`class `[`Loom_Stepper`](#class_loom___stepper) | Stepper actuator module.
`class `[`Loom_TMP007`](#class_loom___t_m_p007) | TMP007 thermopile temperature sensor module.
`class `[`Loom_TSL2561`](#class_loom___t_s_l2561) | TSL2561 Luminosity sensor module.
`class `[`Loom_TSL2591`](#class_loom___t_s_l2591) | TSL2591 Lux sensor module.
`class `[`Loom_WiFi`](#class_loom___wi_fi) | WiFi InternetPlat.
`class `[`Loom_ZXGesture`](#class_loom___z_x_gesture) | ZXGesture position / gesture sensor module.
`class `[`LoomActuator`](#class_loom_actuator) | Abstract base class of actuator modules.
`class `[`LoomCommPlat`](#class_loom_comm_plat) | Abstract base of communication platform modules.
`class `[`LoomFactory`](#class_loom_factory) | Factory is used by [LoomManager](#class_loom_manager) when parsing Json to match module names to their associated constructors, and calling with parameters from the Json.
`class `[`LoomI2CSensor`](#class_loom_i2_c_sensor) | Abstract root for I2C sensor modules.
`class `[`LoomInternetPlat`](#class_loom_internet_plat) | Abstract internet communication module.
`class `[`LoomLogPlat`](#class_loom_log_plat) | Abstract base of logging platforms.
`class `[`LoomManager`](#class_loom_manager) | Manager to contain Loom modules and provide users with a simpler API.
`class `[`LoomModule`](#class_loom_module) | Abstract root of Loom component modules inheritance hierarchy.
`class `[`LoomNTPSync`](#class_loom_n_t_p_sync) | Glue code to synchronize an RTC using an InternetPlat.
`class `[`LoomPublishPlat`](#class_loom_publish_plat) | Abstract internet publishing module, implementing google sheets and mongodb functionality.
`class `[`LoomRTC`](#class_loom_r_t_c) | Abstract base class of RTC modules.
`class `[`LoomSDI12Sensor`](#class_loom_s_d_i12_sensor) | Abstract base class for SDI12 sensor modules.
`class `[`LoomSensor`](#class_loom_sensor) | Abstract root of Loom sensor modules hierarchy.
`class `[`LoomSPISensor`](#class_loom_s_p_i_sensor) | Abstract base class for SPI sensor modules.
`class `[`LoomSubscribePlat`](#class_loom_subscribe_plat) | Abstract subscribe platform module.
`class `[`LoomTempSync`](#class_loom_temp_sync) | Used to synchronize temperatures between sensors that read sensors and modules that need it.
`struct `[`LoomInternetPlat::ClientCleanup`](#struct_loom_internet_plat_1_1_client_cleanup) | Simply close the socket when the client dissapears, we don't want to delete the object because the client needs to cache sessions.
`struct `[`Loom_Interrupt_Manager::IntDetails`](#struct_loom___interrupt___manager_1_1_int_details) | Contains information defining an interrupt's configuration.
`struct `[`Loom_Interrupt_Manager::InternalTimerDetails`](#struct_loom___interrupt___manager_1_1_internal_timer_details) | Contains information defining a timer's configuration.
`struct `[`module_sort_comp`](#structmodule__sort__comp) | Used by [LoomManager](#class_loom_manager) to sort modules in its vector.
`struct `[`Loom_Interrupt_Manager::StopWatchDetails`](#struct_loom___interrupt___manager_1_1_stop_watch_details) | Contains information defining a stopwatch's configuration.
`struct `[`Loom_Interrupt_Manager::TimerDetails`](#struct_loom___interrupt___manager_1_1_timer_details) | Contains information defining a timer's configuration.
`struct `[`LoomInternetPlat::UDPDeletor`](#struct_loom_internet_plat_1_1_u_d_p_deletor) | Close the socket and delete the UDP object when the unique ptr dissapears.

# namespace `Enable` {#namespace_enable}

Blocks modules the can be toggled on/off or selected between a variety of selection.

Below, in [LoomFactory](#class_loom_factory), the integer index (zero indexed) of the enums is used to select between different includes, so make sure the includes correctly correspond to the index

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`enum `[`Internet`](#namespace_enable_1af24aafc1e3ae44b565ff729a2ef16a39)            | 
`enum `[`Sensors`](#namespace_enable_1a2c5357c28618c596547602949f27e4ca)            | 
`enum `[`Actuators`](#namespace_enable_1aad5ff0e67e478728f21443865359584b)            | 
`enum `[`Radios`](#namespace_enable_1adac37ec06673d08a6c6402a0feb1b232)            | 
`enum `[`Max`](#namespace_enable_1a91b4292c74e70133aaefa8bcceec2461)            | 

## Members

#### `enum `[`Internet`](#namespace_enable_1af24aafc1e3ae44b565ff729a2ef16a39) {#namespace_enable_1af24aafc1e3ae44b565ff729a2ef16a39}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
All            | All internet modules enabled.
Ethernet            | Only Ethernet relevant modules enabled.
WiFi            | Only WiFi relevant modules enabled.
Disabled            | Internet modules disabled.

#### `enum `[`Sensors`](#namespace_enable_1a2c5357c28618c596547602949f27e4ca) {#namespace_enable_1a2c5357c28618c596547602949f27e4ca}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
Enabled            | 
Disabled            | 

#### `enum `[`Actuators`](#namespace_enable_1aad5ff0e67e478728f21443865359584b) {#namespace_enable_1aad5ff0e67e478728f21443865359584b}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
Enabled            | 
Disabled            | 

#### `enum `[`Radios`](#namespace_enable_1adac37ec06673d08a6c6402a0feb1b232) {#namespace_enable_1adac37ec06673d08a6c6402a0feb1b232}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
Enabled            | 
Disabled            | 

#### `enum `[`Max`](#namespace_enable_1a91b4292c74e70133aaefa8bcceec2461) {#namespace_enable_1a91b4292c74e70133aaefa8bcceec2461}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
Enabled            | 
Disabled            | 

# namespace `factory` {#namespacefactory}

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`struct `[`factory::NameModulePair`](#structfactory_1_1_name_module_pair) | Struct to contain the elements of factory lookup table.

# struct `factory::NameModulePair` {#structfactory_1_1_name_module_pair}

Struct to contain the elements of factory lookup table.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public const char * `[`name`](#structfactory_1_1_name_module_pair_1abd0feebc88465d1acbd71eb3ed632fe2) | Module type to compare against.
`public `[`FactoryPtr`](#namespacefactory_1a946094cf09d106fc0ea76797ee4d10c7)` `[`Construct`](#structfactory_1_1_name_module_pair_1ada6abb90038e59d80f8b0bbfa2b0f39b) | Pointer to 'template<class T> LoomModule* Create(JsonArrayConst p)' with the type T set.
`public `[`FactoryPtrDefault`](#namespacefactory_1a27eb6b283b5a0314c314cf583bfa0540)` `[`ConstructDefault`](#structfactory_1_1_name_module_pair_1ad0df41ece733325bc0b1ba75f7e38e07) | Pointer to 'template<class T> LoomModule* CreateDefault()' with the type T set.

## Members

#### `public const char * `[`name`](#structfactory_1_1_name_module_pair_1abd0feebc88465d1acbd71eb3ed632fe2) {#structfactory_1_1_name_module_pair_1abd0feebc88465d1acbd71eb3ed632fe2}

Module type to compare against.

#### `public `[`FactoryPtr`](#namespacefactory_1a946094cf09d106fc0ea76797ee4d10c7)` `[`Construct`](#structfactory_1_1_name_module_pair_1ada6abb90038e59d80f8b0bbfa2b0f39b) {#structfactory_1_1_name_module_pair_1ada6abb90038e59d80f8b0bbfa2b0f39b}

Pointer to 'template<class T> LoomModule* Create(JsonArrayConst p)' with the type T set.

#### `public `[`FactoryPtrDefault`](#namespacefactory_1a27eb6b283b5a0314c314cf583bfa0540)` `[`ConstructDefault`](#structfactory_1_1_name_module_pair_1ad0df41ece733325bc0b1ba75f7e38e07) {#structfactory_1_1_name_module_pair_1ad0df41ece733325bc0b1ba75f7e38e07}

Pointer to 'template<class T> LoomModule* CreateDefault()' with the type T set.

# class `FactoryBase` {#class_factory_base}

Base class that factory is derived from.

Purpose is to allow LoomManger to have a pointer to a [LoomFactory](#class_loom_factory), which it cannot do directly because different parameter selections result in different factory classes

Resources

* [FactoryBase Documentation](https://openslab-osu.github.io/Loom/html/class_factory_base.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public void `[`print_table`](#class_factory_base_1a96c50a015c8e2b0e6cc4a8ee48ae2a12)`() const` | 
`public `[`LoomModule`](#class_loom_module)` * `[`Create`](#class_factory_base_1a6d42c63b0dffba3903db83418303e1c9)`(JsonVariant module) const` | 

## Members

#### `public void `[`print_table`](#class_factory_base_1a96c50a015c8e2b0e6cc4a8ee48ae2a12)`() const` {#class_factory_base_1a96c50a015c8e2b0e6cc4a8ee48ae2a12}

#### `public `[`LoomModule`](#class_loom_module)` * `[`Create`](#class_factory_base_1a6d42c63b0dffba3903db83418303e1c9)`(JsonVariant module) const` {#class_factory_base_1a6d42c63b0dffba3903db83418303e1c9}

# class `Loom_Analog` {#class_loom___analog}

```
class Loom_Analog
  : public LoomSensor
```  

Analog pin manager module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___analog.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected uint8_t `[`read_resolution`](#class_loom___analog_1a8593018496b601b75eec9a1df44574fb) | Which resolution to read at (generally use 12 or 10)
`protected bool `[`pin_enabled`](#class_loom___analog_1a1111168ede4b93a1507e0cd67d849fd7) | Whether pins A0-A5 are enabled for analog reading.
`protected uint16_t `[`analog_vals`](#class_loom___analog_1a9badecf02377889b100678c8c50ba387) | Last known analog readings for pin A0-A5.
`protected float `[`battery`](#class_loom___analog_1a1a1f859c11385e20ff2328f7df868436) | Battery voltage.
`protected bool `[`enable_conversions`](#class_loom___analog_1ab1be771ab1dfad8ddb955c42ba3a3de6) | [Enable](#namespace_enable) or disable all conversions.
`protected `[`Conversion`](#class_loom___analog_1a83079adfd115a272351323429cefad46)` `[`conversions`](#class_loom___analog_1a3bf07770f1244670cbfaa8d6e563f2ef) | Conversion (if any) to apply to analog value when printing / packaging.
`protected float `[`temperature`](#class_loom___analog_1ad01ead292b5fecf5178cd1baf9354ac3) | Temperature to use in conversions.
`enum `[`Conversion`](#class_loom___analog_1a83079adfd115a272351323429cefad46) | Different types of conversions from analog to specified unit.

## Members

#### `protected uint8_t `[`read_resolution`](#class_loom___analog_1a8593018496b601b75eec9a1df44574fb) {#class_loom___analog_1a8593018496b601b75eec9a1df44574fb}

Which resolution to read at (generally use 12 or 10)

#### `protected bool `[`pin_enabled`](#class_loom___analog_1a1111168ede4b93a1507e0cd67d849fd7) {#class_loom___analog_1a1111168ede4b93a1507e0cd67d849fd7}

Whether pins A0-A5 are enabled for analog reading.

#### `protected uint16_t `[`analog_vals`](#class_loom___analog_1a9badecf02377889b100678c8c50ba387) {#class_loom___analog_1a9badecf02377889b100678c8c50ba387}

Last known analog readings for pin A0-A5.

#### `protected float `[`battery`](#class_loom___analog_1a1a1f859c11385e20ff2328f7df868436) {#class_loom___analog_1a1a1f859c11385e20ff2328f7df868436}

Battery voltage.

#### `protected bool `[`enable_conversions`](#class_loom___analog_1ab1be771ab1dfad8ddb955c42ba3a3de6) {#class_loom___analog_1ab1be771ab1dfad8ddb955c42ba3a3de6}

[Enable](#namespace_enable) or disable all conversions.

#### `protected `[`Conversion`](#class_loom___analog_1a83079adfd115a272351323429cefad46)` `[`conversions`](#class_loom___analog_1a3bf07770f1244670cbfaa8d6e563f2ef) {#class_loom___analog_1a3bf07770f1244670cbfaa8d6e563f2ef}

Conversion (if any) to apply to analog value when printing / packaging.

#### `protected float `[`temperature`](#class_loom___analog_1ad01ead292b5fecf5178cd1baf9354ac3) {#class_loom___analog_1ad01ead292b5fecf5178cd1baf9354ac3}

Temperature to use in conversions.

#### `enum `[`Conversion`](#class_loom___analog_1a83079adfd115a272351323429cefad46) {#class_loom___analog_1a83079adfd115a272351323429cefad46}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
NONE            | No conversion applied.
VOLTAGE            | Analog to voltage.
THERMISTOR            | Thermistor.
PH            | pH value
TURBIDITY            | Turbidity.
EC            | Electrical Conductivity.
TDS            | Total Dissolved Solids.
SALINITY            | Salinity.

Different types of conversions from analog to specified unit.

# class `Loom_AS7262` {#class_loom___a_s7262}

```
class Loom_AS7262
  : public LoomI2CSensor
```  

AS7262 Visible spectrum spectral sensor module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___a_s7262.html)

* [Product Page: SparkFun Spectral Sensor Breakout - AS7262 Visible](https://www.sparkfun.com/products/14347)

* [Dependency: Sparkfun AS726X Arduino Library](https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS726X/tree/master/Libraries/Arduino)

* [Hookup Guide: AS726X NIR/VIS Spectral Sensor](https://learn.sparkfun.com/tutorials/as726x-nirvi)

* [Hardware GitHub](https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS726X)

* [Datasheet: AS7262](https://cdn.sparkfun.com/assets/f/b/c/c/f/AS7262.pdf)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected AS726X `[`inst_AS7262`](#class_loom___a_s7262_1a601df5126c5cf248fa51dace2596aa2f) | Underlying AS7262 sensor manager instance.
`protected uint16_t `[`color_vals`](#class_loom___a_s7262_1a951df5e3e0154cc32f79b3d4156109b7) | Measured color band values (violet, blue, green, yellow, orange, red).
`protected bool `[`use_bulb`](#class_loom___a_s7262_1a2fa9b5aeb99ac06738a52c3716053a89) | Whether or not to use bulb.
`protected uint8_t `[`gain`](#class_loom___a_s7262_1a3a9fc889a169e21181fa703ea152147d) | Gain setting.
`protected uint8_t `[`mode`](#class_loom___a_s7262_1afae9fa2dc1e8bac95947f957449212f2) | Sensor read mode.
`protected uint8_t `[`integration_time`](#class_loom___a_s7262_1aa8b8b093c0eabd30a7534270accae5c0) | Integration time setting.

## Members

#### `protected AS726X `[`inst_AS7262`](#class_loom___a_s7262_1a601df5126c5cf248fa51dace2596aa2f) {#class_loom___a_s7262_1a601df5126c5cf248fa51dace2596aa2f}

Underlying AS7262 sensor manager instance.

#### `protected uint16_t `[`color_vals`](#class_loom___a_s7262_1a951df5e3e0154cc32f79b3d4156109b7) {#class_loom___a_s7262_1a951df5e3e0154cc32f79b3d4156109b7}

Measured color band values (violet, blue, green, yellow, orange, red).

Units: counts / (μW/cm^2).

#### `protected bool `[`use_bulb`](#class_loom___a_s7262_1a2fa9b5aeb99ac06738a52c3716053a89) {#class_loom___a_s7262_1a2fa9b5aeb99ac06738a52c3716053a89}

Whether or not to use bulb.

#### `protected uint8_t `[`gain`](#class_loom___a_s7262_1a3a9fc889a169e21181fa703ea152147d) {#class_loom___a_s7262_1a3a9fc889a169e21181fa703ea152147d}

Gain setting.

#### `protected uint8_t `[`mode`](#class_loom___a_s7262_1afae9fa2dc1e8bac95947f957449212f2) {#class_loom___a_s7262_1afae9fa2dc1e8bac95947f957449212f2}

Sensor read mode.

#### `protected uint8_t `[`integration_time`](#class_loom___a_s7262_1aa8b8b093c0eabd30a7534270accae5c0) {#class_loom___a_s7262_1aa8b8b093c0eabd30a7534270accae5c0}

Integration time setting.

# class `Loom_AS7263` {#class_loom___a_s7263}

```
class Loom_AS7263
  : public LoomI2CSensor
```  

AS7263 Near Infrared spectrum spectral sensor module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___a_s7263.html)

* [Product Page: SparkFun Spectral Sensor Breakout - AS7263 NIR](https://www.sparkfun.com/products/14351)

* [Dependency: Sparkfun AS726X Arduino Library](https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS726X/tree/master/Libraries/Arduino)

* [Hookup Guide: AS726X NIR/VIS Spectral Sensor](https://learn.sparkfun.com/tutorials/as726x-nirvi)

* [Hardware GitHub](https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS726X)

* [Datasheet: AS7263](https://cdn.sparkfun.com/assets/1/b/7/3/b/AS7263.pdf)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected AS726X `[`inst_AS7263`](#class_loom___a_s7263_1a653b51c97f1c478ae71ba4efc04f6f31) | Underlying AS7263 sensor manager instance.
`protected uint16_t `[`nir_vals`](#class_loom___a_s7263_1a3d2e672d1bb20a793e3b0cde44fac99e) | Measured near-infra-red band values (r,s,t,u,v,w).
`protected bool `[`use_bulb`](#class_loom___a_s7263_1a58d52744648dd8354bc875f48ef4639a) | Whether or not to use bulb.
`protected uint8_t `[`gain`](#class_loom___a_s7263_1ab9b7223697e11e9a0ee4aa3a207f9be5) | Gain setting.
`protected uint8_t `[`mode`](#class_loom___a_s7263_1a3474bf1e38b6513f517f3e364d96df36) | Sensor read mode.
`protected uint8_t `[`integration_time`](#class_loom___a_s7263_1a763628cf0f7a1d22a9c98776caa44112) | Integration time setting.

## Members

#### `protected AS726X `[`inst_AS7263`](#class_loom___a_s7263_1a653b51c97f1c478ae71ba4efc04f6f31) {#class_loom___a_s7263_1a653b51c97f1c478ae71ba4efc04f6f31}

Underlying AS7263 sensor manager instance.

#### `protected uint16_t `[`nir_vals`](#class_loom___a_s7263_1a3d2e672d1bb20a793e3b0cde44fac99e) {#class_loom___a_s7263_1a3d2e672d1bb20a793e3b0cde44fac99e}

Measured near-infra-red band values (r,s,t,u,v,w).

Units: counts / (μW/cm^2).

#### `protected bool `[`use_bulb`](#class_loom___a_s7263_1a58d52744648dd8354bc875f48ef4639a) {#class_loom___a_s7263_1a58d52744648dd8354bc875f48ef4639a}

Whether or not to use bulb.

#### `protected uint8_t `[`gain`](#class_loom___a_s7263_1ab9b7223697e11e9a0ee4aa3a207f9be5) {#class_loom___a_s7263_1ab9b7223697e11e9a0ee4aa3a207f9be5}

Gain setting.

#### `protected uint8_t `[`mode`](#class_loom___a_s7263_1a3474bf1e38b6513f517f3e364d96df36) {#class_loom___a_s7263_1a3474bf1e38b6513f517f3e364d96df36}

Sensor read mode.

#### `protected uint8_t `[`integration_time`](#class_loom___a_s7263_1a763628cf0f7a1d22a9c98776caa44112) {#class_loom___a_s7263_1a763628cf0f7a1d22a9c98776caa44112}

Integration time setting.

# class `Loom_AS7265X` {#class_loom___a_s7265_x}

```
class Loom_AS7265X
  : public LoomI2CSensor
```  

AS72625X 3 Spectrum (Visible, near IR, UV) spectral sensor module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___a_s7265_x.html)

* [Product Page: SparkFun Triad Spectroscopy Sensor - AS7265x](https://www.sparkfun.com/products/15050)

* [Dependency: SparkFun_AS7265x_Arduino_Library](https://github.com/sparkfun/SparkFun_AS7265x_Arduino_Library)

* [Datasheet: AS72625X](https://cdn.sparkfun.com/assets/c/2/9/0/a/AS7265x_Datasheet.pdf)

* [Hookup Guide: Spectral Triad (AS7265x)](https://learn.sparkfun.com/tutorials/spectral-triad-as7265x-hookup-guide)

* [Hardware GitHub](https://github.com/sparkfun/Qwiic_Spectral_Sensor_AS7265x)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected AS7265X `[`inst_AS7265X`](#class_loom___a_s7265_x_1a929b53e4170b78bfe3ca8787525dc4e7) | Underlying AS7265X sensor manager instance.
`protected uint16_t `[`uv`](#class_loom___a_s7265_x_1a0724af751e2f38c7f71fc5828910f6fe) | Measured UV bands values. Units: counts / (μW/cm^2)
`protected uint16_t `[`color`](#class_loom___a_s7265_x_1aaf555d167aca2607fa0f13c71341f3b1) | Measured color bands values. Units: counts / (μW/cm^2)
`protected uint16_t `[`nir`](#class_loom___a_s7265_x_1a6f2c89c5aed39f75a275f37ce3ea21bd) | Measured near-infra-red bands values. Units: counts / (μW/cm^2)
`protected bool `[`use_bulb`](#class_loom___a_s7265_x_1a33831b2827d06b2114225b57862445f7) | Whether or not to use the bulb.
`protected uint8_t `[`gain`](#class_loom___a_s7265_x_1a204831caa774ead3b9ba57495e17bfd9) | Gain setting.
`protected uint8_t `[`mode`](#class_loom___a_s7265_x_1a02bc2eaf8dfea91f9f47a19f49dcc1e9) | Sensor mode.
`protected uint8_t `[`integration_time`](#class_loom___a_s7265_x_1a656e32aedff07ca33e3b9b0dc2768ba8) | Integration time setting.

## Members

#### `protected AS7265X `[`inst_AS7265X`](#class_loom___a_s7265_x_1a929b53e4170b78bfe3ca8787525dc4e7) {#class_loom___a_s7265_x_1a929b53e4170b78bfe3ca8787525dc4e7}

Underlying AS7265X sensor manager instance.

#### `protected uint16_t `[`uv`](#class_loom___a_s7265_x_1a0724af751e2f38c7f71fc5828910f6fe) {#class_loom___a_s7265_x_1a0724af751e2f38c7f71fc5828910f6fe}

Measured UV bands values. Units: counts / (μW/cm^2)

#### `protected uint16_t `[`color`](#class_loom___a_s7265_x_1aaf555d167aca2607fa0f13c71341f3b1) {#class_loom___a_s7265_x_1aaf555d167aca2607fa0f13c71341f3b1}

Measured color bands values. Units: counts / (μW/cm^2)

#### `protected uint16_t `[`nir`](#class_loom___a_s7265_x_1a6f2c89c5aed39f75a275f37ce3ea21bd) {#class_loom___a_s7265_x_1a6f2c89c5aed39f75a275f37ce3ea21bd}

Measured near-infra-red bands values. Units: counts / (μW/cm^2)

#### `protected bool `[`use_bulb`](#class_loom___a_s7265_x_1a33831b2827d06b2114225b57862445f7) {#class_loom___a_s7265_x_1a33831b2827d06b2114225b57862445f7}

Whether or not to use the bulb.

#### `protected uint8_t `[`gain`](#class_loom___a_s7265_x_1a204831caa774ead3b9ba57495e17bfd9) {#class_loom___a_s7265_x_1a204831caa774ead3b9ba57495e17bfd9}

Gain setting.

#### `protected uint8_t `[`mode`](#class_loom___a_s7265_x_1a02bc2eaf8dfea91f9f47a19f49dcc1e9) {#class_loom___a_s7265_x_1a02bc2eaf8dfea91f9f47a19f49dcc1e9}

Sensor mode.

#### `protected uint8_t `[`integration_time`](#class_loom___a_s7265_x_1a656e32aedff07ca33e3b9b0dc2768ba8) {#class_loom___a_s7265_x_1a656e32aedff07ca33e3b9b0dc2768ba8}

Integration time setting.

# class `Loom_Bluetooth` {#class_loom___bluetooth}

```
class Loom_Bluetooth
  : public LoomCommPlat
```  

Bluetooth communication platform module.

Needs more work for complete functionality.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___bluetooth.html)

* [Product Page: Adafruit Feather M0 Bluefruit LE](https://www.adafruit.com/product/2995)

* [Dependency: Adafruit_BluefruitLE_nRF51](https://github.com/adafruit/Adafruit_BluefruitLE_nRF51)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_BluefruitLE_SPI `[`BLE`](#class_loom___bluetooth_1a0f16366fb0573986b0205cfc73f02b7c) | Bluetooth manager instance.
`protected uint8_t `[`spi_CS`](#class_loom___bluetooth_1a111aeae48c0ea364b47b3d8f3536bd7c) | SPI chip select pin.
`protected uint8_t `[`spi_IRQ`](#class_loom___bluetooth_1a8ef44f1f6220e7272aeca00a0991a3c7) | SPI IRQ pin.
`protected uint8_t `[`spi_RST`](#class_loom___bluetooth_1a9b7f8f41e29fce31bcd8366e93edd7a6) | SPI reset pin.

## Members

#### `protected Adafruit_BluefruitLE_SPI `[`BLE`](#class_loom___bluetooth_1a0f16366fb0573986b0205cfc73f02b7c) {#class_loom___bluetooth_1a0f16366fb0573986b0205cfc73f02b7c}

Bluetooth manager instance.

#### `protected uint8_t `[`spi_CS`](#class_loom___bluetooth_1a111aeae48c0ea364b47b3d8f3536bd7c) {#class_loom___bluetooth_1a111aeae48c0ea364b47b3d8f3536bd7c}

SPI chip select pin.

#### `protected uint8_t `[`spi_IRQ`](#class_loom___bluetooth_1a8ef44f1f6220e7272aeca00a0991a3c7) {#class_loom___bluetooth_1a8ef44f1f6220e7272aeca00a0991a3c7}

SPI IRQ pin.

#### `protected uint8_t `[`spi_RST`](#class_loom___bluetooth_1a9b7f8f41e29fce31bcd8366e93edd7a6) {#class_loom___bluetooth_1a9b7f8f41e29fce31bcd8366e93edd7a6}

SPI reset pin.

# class `Loom_Decagon5TM` {#class_loom___decagon5_t_m}

```
class Loom_Decagon5TM
  : public LoomSDI12Sensor
```  

Decagon 5TM soil moisture sensor module.

Incomplete.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___decagon5_t_m.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `Loom_DecagonGS3` {#class_loom___decagon_g_s3}

```
class Loom_DecagonGS3
  : public LoomSDI12Sensor
```  

Decagon GS3 soil moisture sensor module.

Incomplete.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___decagon_g_s3.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected float `[`dielec_perm`](#class_loom___decagon_g_s3_1a4d980a6c88cf7eeed2594c1651a439dd) | Measured dielectric permativity.
`protected float `[`temp`](#class_loom___decagon_g_s3_1a24b5b1b72e6754add3768d4596832026) | Measured temperature.
`protected float `[`elec_cond`](#class_loom___decagon_g_s3_1a707a1d557e8b5eaa55202b45d369a350) | Measure electrical conductivity.

## Members

#### `protected float `[`dielec_perm`](#class_loom___decagon_g_s3_1a4d980a6c88cf7eeed2594c1651a439dd) {#class_loom___decagon_g_s3_1a4d980a6c88cf7eeed2594c1651a439dd}

Measured dielectric permativity.

#### `protected float `[`temp`](#class_loom___decagon_g_s3_1a24b5b1b72e6754add3768d4596832026) {#class_loom___decagon_g_s3_1a24b5b1b72e6754add3768d4596832026}

Measured temperature.

#### `protected float `[`elec_cond`](#class_loom___decagon_g_s3_1a707a1d557e8b5eaa55202b45d369a350) {#class_loom___decagon_g_s3_1a707a1d557e8b5eaa55202b45d369a350}

Measure electrical conductivity.

# class `Loom_Digital` {#class_loom___digital}

```
class Loom_Digital
  : public LoomSensor
```  

Digital pin manager module.

Available digital pins 5, 6, 9, 10, 11, 12, A0(14), A1(15), A2(16), A3(17), A4(18), A5(19)

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___digital.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected bool `[`pin_enabled`](#class_loom___digital_1a55f8c126c2bba909d412aebfefc024de) | Array of which pins are enabled.
`protected bool `[`digital_vals`](#class_loom___digital_1a34b30d1136b394a78e0ba09868cfbf35) | Array of last read digital values.

## Members

#### `protected bool `[`pin_enabled`](#class_loom___digital_1a55f8c126c2bba909d412aebfefc024de) {#class_loom___digital_1a55f8c126c2bba909d412aebfefc024de}

Array of which pins are enabled.

#### `protected bool `[`digital_vals`](#class_loom___digital_1a34b30d1136b394a78e0ba09868cfbf35) {#class_loom___digital_1a34b30d1136b394a78e0ba09868cfbf35}

Array of last read digital values.

# class `Loom_DS3231` {#class_loom___d_s3231}

```
class Loom_DS3231
  : public LoomRTC
```  

DS3231 RTC module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___d_s3231.html)

* [Product Page: Adafruit DS3231 Precision RTC FeatherWing](https://www.adafruit.com/product/3028)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected RTC_DS3231 `[`rtc_inst`](#class_loom___d_s3231_1a3407d04f9e8cd13b2cfee640818f5e4b) | Underlying DS3231 manager instance.

## Members

#### `protected RTC_DS3231 `[`rtc_inst`](#class_loom___d_s3231_1a3407d04f9e8cd13b2cfee640818f5e4b) {#class_loom___d_s3231_1a3407d04f9e8cd13b2cfee640818f5e4b}

Underlying DS3231 manager instance.

# class `Loom_Ethernet` {#class_loom___ethernet}

```
class Loom_Ethernet
  : public LoomInternetPlat
```  

InternetPlat built off of SSLClient running over an Ethernet Featherwing.

> Requires 7KB of free SRAM at runtime to use.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___ethernet.html)

* [Product Page: Adafruit Ethernet Featherwing](https://www.adafruit.com/product/3201)

* [Dependency: EthernetLarge](https://github.com/OPEnSLab-OSU/EthernetLarge) OPEbS Lab fork of Arduino Ethernet library

* [Dependency: SSLClient](https://github.com/OPEnSLab-OSU/SSLClient)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected std::vector< unsigned char > `[`m_cli_cert`](#class_loom___ethernet_1a170b3fca1097c195a94d75812913500d) | The client certificate, if one is provided (DER format)
`protected std::vector< unsigned char > `[`m_cli_key`](#class_loom___ethernet_1a9057f551bc492b08f53d818ae1fa6f1e) | The client private key, if one if provided (DER format)
`protected const br_x509_certificate `[`m_cert`](#class_loom___ethernet_1a71c9cd5d07865d67d32c458e042b7d3c) | 
`protected const SSLClientParameters `[`m_params`](#class_loom___ethernet_1a690805e1684d120e8d39a9d062709670) | 
`protected SSLClient< EthernetClient > `[`m_client`](#class_loom___ethernet_1a9603818fee4b19fcd43772683c6d4844) | Underlying Ethernet SSLclient instance.
`protected byte `[`m_mac`](#class_loom___ethernet_1ad3ac39aaf45c0c53965d029e9c1db7a4) | The Ethernet MAC address.
`protected IPAddress `[`m_ip`](#class_loom___ethernet_1ab7b3e7573fa75b43d99d99cec21b6a86) | The devices IP address.
`protected bool `[`m_is_connected`](#class_loom___ethernet_1a383b518b130bb1342bd8e4eae8c01e29) | Whether or not ethernet initialized successfully.

## Members

#### `protected std::vector< unsigned char > `[`m_cli_cert`](#class_loom___ethernet_1a170b3fca1097c195a94d75812913500d) {#class_loom___ethernet_1a170b3fca1097c195a94d75812913500d}

The client certificate, if one is provided (DER format)

#### `protected std::vector< unsigned char > `[`m_cli_key`](#class_loom___ethernet_1a9057f551bc492b08f53d818ae1fa6f1e) {#class_loom___ethernet_1a9057f551bc492b08f53d818ae1fa6f1e}

The client private key, if one if provided (DER format)

#### `protected const br_x509_certificate `[`m_cert`](#class_loom___ethernet_1a71c9cd5d07865d67d32c458e042b7d3c) {#class_loom___ethernet_1a71c9cd5d07865d67d32c458e042b7d3c}

#### `protected const SSLClientParameters `[`m_params`](#class_loom___ethernet_1a690805e1684d120e8d39a9d062709670) {#class_loom___ethernet_1a690805e1684d120e8d39a9d062709670}

#### `protected SSLClient< EthernetClient > `[`m_client`](#class_loom___ethernet_1a9603818fee4b19fcd43772683c6d4844) {#class_loom___ethernet_1a9603818fee4b19fcd43772683c6d4844}

Underlying Ethernet SSLclient instance.

#### `protected byte `[`m_mac`](#class_loom___ethernet_1ad3ac39aaf45c0c53965d029e9c1db7a4) {#class_loom___ethernet_1ad3ac39aaf45c0c53965d029e9c1db7a4}

The Ethernet MAC address.

#### `protected IPAddress `[`m_ip`](#class_loom___ethernet_1ab7b3e7573fa75b43d99d99cec21b6a86) {#class_loom___ethernet_1ab7b3e7573fa75b43d99d99cec21b6a86}

The devices IP address.

#### `protected bool `[`m_is_connected`](#class_loom___ethernet_1a383b518b130bb1342bd8e4eae8c01e29) {#class_loom___ethernet_1a383b518b130bb1342bd8e4eae8c01e29}

Whether or not ethernet initialized successfully.

# class `Loom_FXAS21002` {#class_loom___f_x_a_s21002}

```
class Loom_FXAS21002
  : public LoomI2CSensor
```  

FXAS21002 3-Axis Gyroscope snesor module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___f_x_a_s21002.html)

* [Product Page: Adafruit Precision NXP 9-DOF Breakout Board - FXOS8700 + FXAS21002](https://www.adafruit.com/product/3463)

* [Dependency: Adafruit_FXAS21002C](https://github.com/adafruit/Adafruit_FXAS21002C)

* [Dependency: Adafruit_AHRS](https://github.com/adafruit/Adafruit_AHRS)

* [Dependency: Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

* [Datasheet: 3-Axis Digital Angular Rate Gyroscope](https://cdn-learn.adafruit.com/assets/assets/000/040/671/original/FXAS21002.pdf?1491475056)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_FXAS21002C `[`inst_FXAS21002`](#class_loom___f_x_a_s21002_1a2f7e11c7ae7894a1990beedf7894e9aa) | Underlying FXAS21002 sensor manager instance.
`protected float `[`gyro`](#class_loom___f_x_a_s21002_1a092bb83d142cf80a10ca907cacb75d4e) | Measured gyro values (x,y,z). Units: °/s.

## Members

#### `protected Adafruit_FXAS21002C `[`inst_FXAS21002`](#class_loom___f_x_a_s21002_1a2f7e11c7ae7894a1990beedf7894e9aa) {#class_loom___f_x_a_s21002_1a2f7e11c7ae7894a1990beedf7894e9aa}

Underlying FXAS21002 sensor manager instance.

#### `protected float `[`gyro`](#class_loom___f_x_a_s21002_1a092bb83d142cf80a10ca907cacb75d4e) {#class_loom___f_x_a_s21002_1a092bb83d142cf80a10ca907cacb75d4e}

Measured gyro values (x,y,z). Units: °/s.

# class `Loom_FXOS8700` {#class_loom___f_x_o_s8700}

```
class Loom_FXOS8700
  : public LoomI2CSensor
```  

FXOS8700 3-Axis Acceleromter/Magnetometer sensor module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___f_x_o_s8700.html)

* [Product Page: Adafruit Precision NXP 9-DOF Breakout Board - FXOS8700 + FXAS21002](https://www.adafruit.com/product/3463)

* [Dependency: Adafruit_FXOS8700](https://github.com/adafruit/Adafruit_FXOS8700)

* [Dependency: Adafruit_AHRS](https://github.com/adafruit/Adafruit_AHRS)

* [Dependency: Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

* [Datasheet: FXOS8700CQ](https://cdn-learn.adafruit.com/assets/assets/000/043/458/original/FXOS8700CQ.pdf?1499125614)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_FXOS8700 `[`inst_FXOS8700`](#class_loom___f_x_o_s8700_1ad3ff453ce86ac6fb61d8109b89877f7b) | Underlying FXOS8700 sensor manager instance.
`protected float `[`accel`](#class_loom___f_x_o_s8700_1aadfab4b0fa7df60dceffd2242494a457) | Measured acceleration values (x,y,z). Units: g.
`protected float `[`mag`](#class_loom___f_x_o_s8700_1af4ac468c842d26be3d1ef3ed3a67644f) | Measured magnetic field values (x,y,z). Units: μT.

## Members

#### `protected Adafruit_FXOS8700 `[`inst_FXOS8700`](#class_loom___f_x_o_s8700_1ad3ff453ce86ac6fb61d8109b89877f7b) {#class_loom___f_x_o_s8700_1ad3ff453ce86ac6fb61d8109b89877f7b}

Underlying FXOS8700 sensor manager instance.

#### `protected float `[`accel`](#class_loom___f_x_o_s8700_1aadfab4b0fa7df60dceffd2242494a457) {#class_loom___f_x_o_s8700_1aadfab4b0fa7df60dceffd2242494a457}

Measured acceleration values (x,y,z). Units: g.

#### `protected float `[`mag`](#class_loom___f_x_o_s8700_1af4ac468c842d26be3d1ef3ed3a67644f) {#class_loom___f_x_o_s8700_1af4ac468c842d26be3d1ef3ed3a67644f}

Measured magnetic field values (x,y,z). Units: μT.

# class `Loom_GoogleSheets` {#class_loom___google_sheets}

```
class Loom_GoogleSheets
  : public LoomPublishPlat
```  

Module taking in and translating JSON into data understood by the Google Sheets script API.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___google_sheets.html)

* [wiki: Using Loom with Google Sheets](https://github.com/OPEnSLab-OSU/Loom/wiki/Using-Loom-with-Google-Sheets)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `Loom_Interrupt_Manager` {#class_loom___interrupt___manager}

```
class Loom_Interrupt_Manager
  : public LoomModule
```  

Submanager for managing interrupts, timers, and stopwatches.

Resources

* [Documentation](https://openslab-osu.github.io/Loom/html/class_loom___interrupt___manager.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected `[`LoomRTC`](#class_loom_r_t_c)` * `[`RTC_Inst`](#class_loom___interrupt___manager_1af209a82a1929466fa1d00a14733120c6) | Pointer to an RTC object for managing timers / timed interrupts.
`protected `[`Loom_Sleep_Manager`](#class_loom___sleep___manager)` * `[`Sleep_Manager`](#class_loom___interrupt___manager_1a1afa84cd52b350fcc4a48ec2e45431a9) | Pointer to a Sleep Manager object.
`protected IntDetails `[`int_settings`](#class_loom___interrupt___manager_1a8b625720d3238492bc51a301e4e9b09a) | List of interrupts configurations.
`protected bool `[`interrupts_enabled`](#class_loom___interrupt___manager_1a042032ad0c2ed2748e1d2849dcae1a2f) | [Enable](#namespace_enable) or disable all interrupts  currently only disables bottom halves.
`protected InternalTimerDetails `[`internal_timer`](#class_loom___interrupt___manager_1a9a00f26aff65a7294699d3518a3b7e74) | 
`protected DateTime `[`last_alarm_time`](#class_loom___interrupt___manager_1a099eebe8ec30cee46432701945b0e758) | Last time an alarm went off.
`protected AsyncDelay `[`timers`](#class_loom___interrupt___manager_1aee0fc09dd7023b4d740932bb44a705c4) | 
`protected TimerDetails `[`timer_settings`](#class_loom___interrupt___manager_1a77b5bb21e151ca476b9e9cf8e45eaad6) | 
`protected StopWatchDetails `[`stopwatch_settings`](#class_loom___interrupt___manager_1a70c3fa86880738c1090071003b76cdc5) | 

## Members

#### `protected `[`LoomRTC`](#class_loom_r_t_c)` * `[`RTC_Inst`](#class_loom___interrupt___manager_1af209a82a1929466fa1d00a14733120c6) {#class_loom___interrupt___manager_1af209a82a1929466fa1d00a14733120c6}

Pointer to an RTC object for managing timers / timed interrupts.

#### `protected `[`Loom_Sleep_Manager`](#class_loom___sleep___manager)` * `[`Sleep_Manager`](#class_loom___interrupt___manager_1a1afa84cd52b350fcc4a48ec2e45431a9) {#class_loom___interrupt___manager_1a1afa84cd52b350fcc4a48ec2e45431a9}

Pointer to a Sleep Manager object.

#### `protected IntDetails `[`int_settings`](#class_loom___interrupt___manager_1a8b625720d3238492bc51a301e4e9b09a) {#class_loom___interrupt___manager_1a8b625720d3238492bc51a301e4e9b09a}

List of interrupts configurations.

#### `protected bool `[`interrupts_enabled`](#class_loom___interrupt___manager_1a042032ad0c2ed2748e1d2849dcae1a2f) {#class_loom___interrupt___manager_1a042032ad0c2ed2748e1d2849dcae1a2f}

[Enable](#namespace_enable) or disable all interrupts  currently only disables bottom halves.

#### `protected InternalTimerDetails `[`internal_timer`](#class_loom___interrupt___manager_1a9a00f26aff65a7294699d3518a3b7e74) {#class_loom___interrupt___manager_1a9a00f26aff65a7294699d3518a3b7e74}

#### `protected DateTime `[`last_alarm_time`](#class_loom___interrupt___manager_1a099eebe8ec30cee46432701945b0e758) {#class_loom___interrupt___manager_1a099eebe8ec30cee46432701945b0e758}

Last time an alarm went off.

#### `protected AsyncDelay `[`timers`](#class_loom___interrupt___manager_1aee0fc09dd7023b4d740932bb44a705c4) {#class_loom___interrupt___manager_1aee0fc09dd7023b4d740932bb44a705c4}

#### `protected TimerDetails `[`timer_settings`](#class_loom___interrupt___manager_1a77b5bb21e151ca476b9e9cf8e45eaad6) {#class_loom___interrupt___manager_1a77b5bb21e151ca476b9e9cf8e45eaad6}

#### `protected StopWatchDetails `[`stopwatch_settings`](#class_loom___interrupt___manager_1a70c3fa86880738c1090071003b76cdc5) {#class_loom___interrupt___manager_1a70c3fa86880738c1090071003b76cdc5}

# class `Loom_LIS3DH` {#class_loom___l_i_s3_d_h}

```
class Loom_LIS3DH
  : public LoomI2CSensor
```  

LIS3DH Accelerometer sensor module.

Needs more work on properly implementing support for the sensors interrupts

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___l_i_s3_d_h.html)

* [Product Page: SparkFun Triple Axis Accelerometer Breakout - LIS3DH](https://www.sparkfun.com/products/13963)

* [Dependency: Sparkfun LIS3DH Arduino Library](https://github.com/sparkfun/SparkFun_LIS3DH_Arduino_Library)

* [Hookup Guide: LIS3DH](https://learn.sparkfun.com/tutorials/lis3dh-hookup-guide)

* [Hardware GitHub](https://github.com/sparkfun/LIS3DH_Breakout)

* [Datasheet: LIS3DH](https://cdn.sparkfun.com/assets/f/b/c/c/f/AS7262.pdf)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected LIS3DH `[`inst_LIS3DH`](#class_loom___l_i_s3_d_h_1a1f063e7cac82509ec31e9d5e0715db1f) | Underlying LIS3DH sensor manager instance.
`protected float `[`accel`](#class_loom___l_i_s3_d_h_1a592a7fce4016957246d2ad5b39bd5d0e) | Measured acceleration values (x,y,z). Units: g.

## Members

#### `protected LIS3DH `[`inst_LIS3DH`](#class_loom___l_i_s3_d_h_1a1f063e7cac82509ec31e9d5e0715db1f) {#class_loom___l_i_s3_d_h_1a1f063e7cac82509ec31e9d5e0715db1f}

Underlying LIS3DH sensor manager instance.

#### `protected float `[`accel`](#class_loom___l_i_s3_d_h_1a592a7fce4016957246d2ad5b39bd5d0e) {#class_loom___l_i_s3_d_h_1a592a7fce4016957246d2ad5b39bd5d0e}

Measured acceleration values (x,y,z). Units: g.

# class `Loom_LoRa` {#class_loom___lo_ra}

```
class Loom_LoRa
  : public LoomCommPlat
```  

LoRa communication platform module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___lo_ra.html)

* [Product Page: Adafruit Feather M0 LoRa](https://www.adafruit.com/product/3178)

* [Documentation: RadioHead Library](https://www.airspayce.com/mikem/arduino/RadioHead/index.html)

* [Dependency: RadioHead](https://github.com/adafruit/RadioHead)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected RH_RF95 `[`driver`](#class_loom___lo_ra_1ab600601fa4ec84c05f34d448dd2787e6) | Underlying rf95 object.
`protected RHReliableDatagram * `[`manager`](#class_loom___lo_ra_1a6abe945ce65128b9261b02f3479b8d4c) | Manager for driver.
`protected uint8_t `[`address`](#class_loom___lo_ra_1a22a97e267e96a09325ab02ea0ae49f5b) | Device Address (should this be part of [LoomCommPlat](#class_loom_comm_plat)? – maybe not as each platform handles addresses differently)
`protected uint8_t `[`power_level`](#class_loom___lo_ra_1ab512503f36bb6be6b3134cb66e4cc84c) | Power level to send at.
`protected uint8_t `[`retry_count`](#class_loom___lo_ra_1a477a700fccb38f5ac120bdfeb4ed4e4d) | Number of transmission retries allowed.
`protected uint16_t `[`retry_timeout`](#class_loom___lo_ra_1ad3f2a482ed0ec0c7ecc3ca2e64b47ef9) | Delay between transmission retries (in milliseconds)

## Members

#### `protected RH_RF95 `[`driver`](#class_loom___lo_ra_1ab600601fa4ec84c05f34d448dd2787e6) {#class_loom___lo_ra_1ab600601fa4ec84c05f34d448dd2787e6}

Underlying rf95 object.

#### `protected RHReliableDatagram * `[`manager`](#class_loom___lo_ra_1a6abe945ce65128b9261b02f3479b8d4c) {#class_loom___lo_ra_1a6abe945ce65128b9261b02f3479b8d4c}

Manager for driver.

#### `protected uint8_t `[`address`](#class_loom___lo_ra_1a22a97e267e96a09325ab02ea0ae49f5b) {#class_loom___lo_ra_1a22a97e267e96a09325ab02ea0ae49f5b}

Device Address (should this be part of [LoomCommPlat](#class_loom_comm_plat)? – maybe not as each platform handles addresses differently)

#### `protected uint8_t `[`power_level`](#class_loom___lo_ra_1ab512503f36bb6be6b3134cb66e4cc84c) {#class_loom___lo_ra_1ab512503f36bb6be6b3134cb66e4cc84c}

Power level to send at.

#### `protected uint8_t `[`retry_count`](#class_loom___lo_ra_1a477a700fccb38f5ac120bdfeb4ed4e4d) {#class_loom___lo_ra_1a477a700fccb38f5ac120bdfeb4ed4e4d}

Number of transmission retries allowed.

#### `protected uint16_t `[`retry_timeout`](#class_loom___lo_ra_1ad3f2a482ed0ec0c7ecc3ca2e64b47ef9) {#class_loom___lo_ra_1ad3f2a482ed0ec0c7ecc3ca2e64b47ef9}

Delay between transmission retries (in milliseconds)

# class `Loom_MAX31855` {#class_loom___m_a_x31855}

```
class Loom_MAX31855
  : public LoomSPISensor
```  

MAX31855 thermocouple temperature module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___m_a_x31856.html)

* [Product Page: Thermocouple Amplifier MAX31855 breakout board](https://www.adafruit.com/product/269)

* [Dependency: Adafruit-MAX31855-library](https://github.com/adafruit/Adafruit-MAX31855-library)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_MAX31855 `[`inst_max`](#class_loom___m_a_x31855_1abc8e110d6676a57bb91f64f2c5b1c25e) | Underlying MAX31855 sensor manager instance.
`protected float `[`temperature`](#class_loom___m_a_x31855_1a1e8c46105fee70cedcd505f25a58d62c) | Measured temperature.
`protected float `[`internal_temp`](#class_loom___m_a_x31855_1a02476a27b49d4193d0534b732246fec4) | Measured internal temperature.

## Members

#### `protected Adafruit_MAX31855 `[`inst_max`](#class_loom___m_a_x31855_1abc8e110d6676a57bb91f64f2c5b1c25e) {#class_loom___m_a_x31855_1abc8e110d6676a57bb91f64f2c5b1c25e}

Underlying MAX31855 sensor manager instance.

#### `protected float `[`temperature`](#class_loom___m_a_x31855_1a1e8c46105fee70cedcd505f25a58d62c) {#class_loom___m_a_x31855_1a1e8c46105fee70cedcd505f25a58d62c}

Measured temperature.

#### `protected float `[`internal_temp`](#class_loom___m_a_x31855_1a02476a27b49d4193d0534b732246fec4) {#class_loom___m_a_x31855_1a02476a27b49d4193d0534b732246fec4}

Measured internal temperature.

# class `Loom_MAX31856` {#class_loom___m_a_x31856}

```
class Loom_MAX31856
  : public LoomSPISensor
```  

MAX31856 thermocouple module.

Measures temperature.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___m_a_x31856.html)

* [Product Page: Adafruit Universal Thermocouple Amplifier MAX31856 Breakout](https://www.adafruit.com/product/3263)

* [Dependency: Adafruit_MAX31856](https://github.com/adafruit/Adafruit_MAX31856)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_MAX31856 `[`inst_max`](#class_loom___m_a_x31856_1ac4f07d257613a0c750438ebfc33d03a8) | Underlying MAX31856 sensor manager instance.
`protected float `[`temperature`](#class_loom___m_a_x31856_1a108e2bfbf0836dce8b3ec9d4790bf94c) | Measured temperature.
`protected float `[`cj_temp`](#class_loom___m_a_x31856_1af5eb2323d4f26391f72a0a9e81307f51) | Measured cold junction temperature.

## Members

#### `protected Adafruit_MAX31856 `[`inst_max`](#class_loom___m_a_x31856_1ac4f07d257613a0c750438ebfc33d03a8) {#class_loom___m_a_x31856_1ac4f07d257613a0c750438ebfc33d03a8}

Underlying MAX31856 sensor manager instance.

#### `protected float `[`temperature`](#class_loom___m_a_x31856_1a108e2bfbf0836dce8b3ec9d4790bf94c) {#class_loom___m_a_x31856_1a108e2bfbf0836dce8b3ec9d4790bf94c}

Measured temperature.

#### `protected float `[`cj_temp`](#class_loom___m_a_x31856_1af5eb2323d4f26391f72a0a9e81307f51) {#class_loom___m_a_x31856_1af5eb2323d4f26391f72a0a9e81307f51}

Measured cold junction temperature.

# class `Loom_MaxPub` {#class_loom___max_pub}

```
class Loom_MaxPub
  : public LoomPublishPlat
```  

Module taking in and translating JSON into data understood by the Google Sheets script API.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___max_pub.html)

* [Max 8](https://cycling74.com)

* [Loom Max Patches](https://github.com/OPEnSLab-OSU/Max-Loom2)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected uint16_t `[`UDP_port`](#class_loom___max_pub_1aabdb739b93a98972610d836d2f4ace19) | Which UDP port to transmit on.
`protected IPAddress `[`remoteIP`](#class_loom___max_pub_1a61f53dddbf95967c3e34f19a159e9e15) | Which IP address to send to.
`protected `[`LoomInternetPlat::UDPPtr`](#class_loom_internet_plat_1a0b2cc742a6ac5f4f7054b75ea99b345c)` `[`UDP_Inst`](#class_loom___max_pub_1a39f9cb9d6c5bc08bc4f2c164ab834620) | Pointer to UDP object.

## Members

#### `protected uint16_t `[`UDP_port`](#class_loom___max_pub_1aabdb739b93a98972610d836d2f4ace19) {#class_loom___max_pub_1aabdb739b93a98972610d836d2f4ace19}

Which UDP port to transmit on.

#### `protected IPAddress `[`remoteIP`](#class_loom___max_pub_1a61f53dddbf95967c3e34f19a159e9e15) {#class_loom___max_pub_1a61f53dddbf95967c3e34f19a159e9e15}

Which IP address to send to.

#### `protected `[`LoomInternetPlat::UDPPtr`](#class_loom_internet_plat_1a0b2cc742a6ac5f4f7054b75ea99b345c)` `[`UDP_Inst`](#class_loom___max_pub_1a39f9cb9d6c5bc08bc4f2c164ab834620) {#class_loom___max_pub_1a39f9cb9d6c5bc08bc4f2c164ab834620}

Pointer to UDP object.

# class `Loom_MaxSub` {#class_loom___max_sub}

```
class Loom_MaxSub
  : public LoomSubscribePlat
```  

Module for receiving data from Max.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___max_sub.html)

* [Max 8](https://cycling74.com)

* [Loom Max Patches](https://github.com/OPEnSLab-OSU/Max-Loom2)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected uint16_t `[`UDP_port`](#class_loom___max_sub_1a0b60c2de2c1a5e2259cc5723edd9d51c) | Which UDP port to receive on.
`protected bool `[`auto_dispatch`](#class_loom___max_sub_1adb1bf0c7394ff4ebaa1e893fb55356ff) | True to immediately call [LoomManager::dispatch()](#class_loom_manager_1a80fb13974b3bde9b0cedcd43732d7226) when packet received.
`protected `[`LoomInternetPlat::UDPPtr`](#class_loom_internet_plat_1a0b2cc742a6ac5f4f7054b75ea99b345c)` `[`UDP_Inst`](#class_loom___max_sub_1aa41703fadde1c5aca2c265eba0e2d13e) | Pointer to UDP object.

## Members

#### `protected uint16_t `[`UDP_port`](#class_loom___max_sub_1a0b60c2de2c1a5e2259cc5723edd9d51c) {#class_loom___max_sub_1a0b60c2de2c1a5e2259cc5723edd9d51c}

Which UDP port to receive on.

#### `protected bool `[`auto_dispatch`](#class_loom___max_sub_1adb1bf0c7394ff4ebaa1e893fb55356ff) {#class_loom___max_sub_1adb1bf0c7394ff4ebaa1e893fb55356ff}

True to immediately call [LoomManager::dispatch()](#class_loom_manager_1a80fb13974b3bde9b0cedcd43732d7226) when packet received.

#### `protected `[`LoomInternetPlat::UDPPtr`](#class_loom_internet_plat_1a0b2cc742a6ac5f4f7054b75ea99b345c)` `[`UDP_Inst`](#class_loom___max_sub_1aa41703fadde1c5aca2c265eba0e2d13e) {#class_loom___max_sub_1aa41703fadde1c5aca2c265eba0e2d13e}

Pointer to UDP object.

# class `Loom_MB1232` {#class_loom___m_b1232}

```
class Loom_MB1232
  : public LoomI2CSensor
```  

MB1232 Sonar sensor module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___m_b1232.html)

* [Product Page: MB1232 I2CXL-MaxSonar-EZ3](https://www.maxbotix.com/Ultrasonic_Sensors/MB1232.htm)

* [Datasheet: I2CXL-MaxSonar- EZTM Series](https://www.maxbotix.com/documents/I2CXL-MaxSonar-EZ_Datasheet.pdf)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected uint16_t `[`range`](#class_loom___m_b1232_1acfb38d20f72e16e517ed5cc670a4d1a8) | Measure distance. Units: cm.

## Members

#### `protected uint16_t `[`range`](#class_loom___m_b1232_1acfb38d20f72e16e517ed5cc670a4d1a8) {#class_loom___m_b1232_1acfb38d20f72e16e517ed5cc670a4d1a8}

Measure distance. Units: cm.

# class `Loom_MMA8451` {#class_loom___m_m_a8451}

```
class Loom_MMA8451
  : public LoomI2CSensor
```  

MMA8451 accelerometer sensor module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___m_m_a8451.html)

* [Product Page: Adafruit Triple-Axis Accelerometer - ±2/4/8g @ 14-bit - MMA8451](https://www.adafruit.com/product/2019)

* [Dependency: Adafruit_MMA8451_Library](https://github.com/adafruit/Adafruit_MMA8451_Library)

* [Dependency: Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

* [Datasheet: Xtrinsic MMA8451Q 3-Axis, 14-bit/8-bit Digital Accelerometer](https://cdn-shop.adafruit.com/datasheets/MMA8451Q-1.pdf)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_MMA8451 `[`MMA`](#class_loom___m_m_a8451_1a748213c00e6b00cac365e936a97c54d7) | Underlying MMA8451 sensor manager instance.
`protected float `[`accel`](#class_loom___m_m_a8451_1a4fbceda7e98b971a800aeb7c09ce7670) | Acceleration values for each axis. Units: g.
`protected uint8_t `[`orientation`](#class_loom___m_m_a8451_1afaa9b009ce6745e7c74ef030630e751e) | Orientation.
`protected mma8451_range_t `[`range`](#class_loom___m_m_a8451_1a8d48bf01b43b936b5364b13e5c64d242) | Range setting (2/4/8 G)

## Members

#### `protected Adafruit_MMA8451 `[`MMA`](#class_loom___m_m_a8451_1a748213c00e6b00cac365e936a97c54d7) {#class_loom___m_m_a8451_1a748213c00e6b00cac365e936a97c54d7}

Underlying MMA8451 sensor manager instance.

#### `protected float `[`accel`](#class_loom___m_m_a8451_1a4fbceda7e98b971a800aeb7c09ce7670) {#class_loom___m_m_a8451_1a4fbceda7e98b971a800aeb7c09ce7670}

Acceleration values for each axis. Units: g.

#### `protected uint8_t `[`orientation`](#class_loom___m_m_a8451_1afaa9b009ce6745e7c74ef030630e751e) {#class_loom___m_m_a8451_1afaa9b009ce6745e7c74ef030630e751e}

Orientation.

#### `protected mma8451_range_t `[`range`](#class_loom___m_m_a8451_1a8d48bf01b43b936b5364b13e5c64d242) {#class_loom___m_m_a8451_1a8d48bf01b43b936b5364b13e5c64d242}

Range setting (2/4/8 G)

# class `Loom_Module_Stub` {#class_loom___module___stub}

```
class Loom_Module_Stub
  : public LoomModule
```  

Stub class to return pointer to instead of nullptr.

Ideally we find a different implementation for similar behavior.

This module is broken, as it attempts to implement member functions of classes it does not inherit from. If you need to check if you received a stub module, check the modules type using (LoomModule*)(module)->[get_module_type()](#class_loom_module_1aa2358e60278183425a969ddb08ce8f89).

Resources

* [Loom_Module_Stub Documentation](https://openslab-osu.github.io/Loom/html/class_loom___module___stub.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline  `[`Loom_Module_Stub`](#class_loom___module___stub_1aac87811e575db23bf7f268c994324053)`()` | 
`public inline  `[`~Loom_Module_Stub`](#class_loom___module___stub_1abad854d5db09b50271ae951eff443fbe)`()` | 

## Members

#### `public inline  `[`Loom_Module_Stub`](#class_loom___module___stub_1aac87811e575db23bf7f268c994324053)`()` {#class_loom___module___stub_1aac87811e575db23bf7f268c994324053}

#### `public inline  `[`~Loom_Module_Stub`](#class_loom___module___stub_1abad854d5db09b50271ae951eff443fbe)`()` {#class_loom___module___stub_1abad854d5db09b50271ae951eff443fbe}

# class `Loom_MPU6050` {#class_loom___m_p_u6050}

```
class Loom_MPU6050
  : public LoomI2CSensor
```  

MPU6050 6 axis gyroscope and accelerometer sensor module.

> Need to modify line 7 of MPU6050_tockn.h to have I2C address of 0x69

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___m_p_u6050.html)

* [Product Page: Amazon –Gy-521 MPU-6050 MPU6050 Module 3 Axis Analog Gyro Sensors+ 3 Axis Accelerometer Module](https://www.amazon.com/MPU-6050-MPU6050-Accelerometer-Gyroscope-Converter/dp/B008BOPN40/)

* [Dependency: MPU6050_tockn](https://github.com/tockn/MPU6050_tockn)

* [Previous Dependency: MPU6050](https://github.com/jrowberg/i2cdevlib/tree/master/Arduino/MPU6050)

* [Datasheet: MPU6050](https://store.invensense.com/datasheets/invensense/MPU-6050_DataSheet_V3%204.pdf)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected float `[`temp`](#class_loom___m_p_u6050_1af9c5eb7e6a657f92500dd39df03383b3) | Temperature. Units: °C.
`protected float `[`accX`](#class_loom___m_p_u6050_1abe1855f17309051bcecddf084a7f2a95) | X-axis acceleration value. Units: g.
`protected float `[`accY`](#class_loom___m_p_u6050_1a12e89f1a6d8e9564600e6e85375abca9) | Y-axis acceleration value. Units: g.
`protected float `[`accZ`](#class_loom___m_p_u6050_1a7d9d3324a3b3bf58f6a11993e8d28b61) | Z-axis acceleration value. Units: g.
`protected float `[`gyroX`](#class_loom___m_p_u6050_1afd0b222a584ee71d922532ac12763f9a) | X-axis gyro value. Units: °/s.
`protected float `[`gyroY`](#class_loom___m_p_u6050_1a1edb0ae9dd59317f4c1aafad72f3d5ef) | Y-axis gyro value. Units: °/s.
`protected float `[`gyroZ`](#class_loom___m_p_u6050_1a0e8122d20fc6e686e2952e93e1ff1a89) | Z-axis gyro value. Units: °/s.
`protected float `[`accAngleX`](#class_loom___m_p_u6050_1ab8ccba95253cd13a7505927d07a81abc) | X-axis acceleration angle.
`protected float `[`accAngleY`](#class_loom___m_p_u6050_1a54de31e46d5c353b521483bcb88832c2) | Y-axis acceleration angle.
`protected float `[`gyroAngleX`](#class_loom___m_p_u6050_1a291abc624cd3687d6120fa406e362391) | X-axis acceleration angle.
`protected float `[`gyroAngleY`](#class_loom___m_p_u6050_1a1d1cde22eab898d09d653f4c001c0127) | Y-axis acceleration angle.
`protected float `[`gyroAngleZ`](#class_loom___m_p_u6050_1a78da02da066a030873cf72ee36026a64) | Z-axis acceleration angle.
`protected float `[`angleX`](#class_loom___m_p_u6050_1a3f223a2721a44ff4d49a6194189a55c4) | X-axis angle.
`protected float `[`angleY`](#class_loom___m_p_u6050_1acccd878ef904048c3e770011f2f36a1c) | Y-axis angle.
`protected float `[`angleZ`](#class_loom___m_p_u6050_1a8d8c30186abcda908795de978a5f6cd3) | Z-axis angle.

## Members

#### `protected float `[`temp`](#class_loom___m_p_u6050_1af9c5eb7e6a657f92500dd39df03383b3) {#class_loom___m_p_u6050_1af9c5eb7e6a657f92500dd39df03383b3}

Temperature. Units: °C.

#### `protected float `[`accX`](#class_loom___m_p_u6050_1abe1855f17309051bcecddf084a7f2a95) {#class_loom___m_p_u6050_1abe1855f17309051bcecddf084a7f2a95}

X-axis acceleration value. Units: g.

#### `protected float `[`accY`](#class_loom___m_p_u6050_1a12e89f1a6d8e9564600e6e85375abca9) {#class_loom___m_p_u6050_1a12e89f1a6d8e9564600e6e85375abca9}

Y-axis acceleration value. Units: g.

#### `protected float `[`accZ`](#class_loom___m_p_u6050_1a7d9d3324a3b3bf58f6a11993e8d28b61) {#class_loom___m_p_u6050_1a7d9d3324a3b3bf58f6a11993e8d28b61}

Z-axis acceleration value. Units: g.

#### `protected float `[`gyroX`](#class_loom___m_p_u6050_1afd0b222a584ee71d922532ac12763f9a) {#class_loom___m_p_u6050_1afd0b222a584ee71d922532ac12763f9a}

X-axis gyro value. Units: °/s.

#### `protected float `[`gyroY`](#class_loom___m_p_u6050_1a1edb0ae9dd59317f4c1aafad72f3d5ef) {#class_loom___m_p_u6050_1a1edb0ae9dd59317f4c1aafad72f3d5ef}

Y-axis gyro value. Units: °/s.

#### `protected float `[`gyroZ`](#class_loom___m_p_u6050_1a0e8122d20fc6e686e2952e93e1ff1a89) {#class_loom___m_p_u6050_1a0e8122d20fc6e686e2952e93e1ff1a89}

Z-axis gyro value. Units: °/s.

#### `protected float `[`accAngleX`](#class_loom___m_p_u6050_1ab8ccba95253cd13a7505927d07a81abc) {#class_loom___m_p_u6050_1ab8ccba95253cd13a7505927d07a81abc}

X-axis acceleration angle.

#### `protected float `[`accAngleY`](#class_loom___m_p_u6050_1a54de31e46d5c353b521483bcb88832c2) {#class_loom___m_p_u6050_1a54de31e46d5c353b521483bcb88832c2}

Y-axis acceleration angle.

#### `protected float `[`gyroAngleX`](#class_loom___m_p_u6050_1a291abc624cd3687d6120fa406e362391) {#class_loom___m_p_u6050_1a291abc624cd3687d6120fa406e362391}

X-axis acceleration angle.

#### `protected float `[`gyroAngleY`](#class_loom___m_p_u6050_1a1d1cde22eab898d09d653f4c001c0127) {#class_loom___m_p_u6050_1a1d1cde22eab898d09d653f4c001c0127}

Y-axis acceleration angle.

#### `protected float `[`gyroAngleZ`](#class_loom___m_p_u6050_1a78da02da066a030873cf72ee36026a64) {#class_loom___m_p_u6050_1a78da02da066a030873cf72ee36026a64}

Z-axis acceleration angle.

#### `protected float `[`angleX`](#class_loom___m_p_u6050_1a3f223a2721a44ff4d49a6194189a55c4) {#class_loom___m_p_u6050_1a3f223a2721a44ff4d49a6194189a55c4}

X-axis angle.

#### `protected float `[`angleY`](#class_loom___m_p_u6050_1acccd878ef904048c3e770011f2f36a1c) {#class_loom___m_p_u6050_1acccd878ef904048c3e770011f2f36a1c}

Y-axis angle.

#### `protected float `[`angleZ`](#class_loom___m_p_u6050_1a8d8c30186abcda908795de978a5f6cd3) {#class_loom___m_p_u6050_1a8d8c30186abcda908795de978a5f6cd3}

Z-axis angle.

# class `Loom_MS5803` {#class_loom___m_s5803}

```
class Loom_MS5803
  : public LoomI2CSensor
```  

MS5803 Atmospheric Pressure / Temperature sensor module.

> Library is modified to support different I2C addresses and will not compile with Loom without the change. The modifed verision can be found in the [Loom_Auxiliary](https://github.com/OPEnSLab-OSU/Loom_Auxiliary/blob/master/Dependencies/Dependencies_Individual/MS5803_02.zip) repository

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___m_s5803.html)

* [Product Page: MS580302BA01-00 Miniature Altimeter Module](https://www.digikey.com/product-detail/en/te-connectivity-measurement-specialties/MS580302BA01-00/223-1624-5-ND/5277629)

* [Dependency: MS5803_2](https://github.com/millerlp/MS5803_02)

* [Datasheet: MS5803-02BA](https://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=MS5803-02BA&DocType=Data+Sheet&DocLang=English)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected MS_5803 `[`inst_MS5803`](#class_loom___m_s5803_1a51efd19c53ce03744197e05c6c21ec2d) | Underlying MS5803 sensor manager instance.
`protected uint16_t `[`MS_PROM`](#class_loom___m_s5803_1aef9ff121171d7b27f369a60495666ab7) | Read calibration data stored on sensor.
`protected float `[`pressure`](#class_loom___m_s5803_1afe0623825dcab69c7f204e32b0fcab56) | Measure pressure. Units: mbar.
`protected float `[`temp`](#class_loom___m_s5803_1a12df20babc5ac276523664c7d6607810) | Measured temperature. Units: °C.

## Members

#### `protected MS_5803 `[`inst_MS5803`](#class_loom___m_s5803_1a51efd19c53ce03744197e05c6c21ec2d) {#class_loom___m_s5803_1a51efd19c53ce03744197e05c6c21ec2d}

Underlying MS5803 sensor manager instance.

#### `protected uint16_t `[`MS_PROM`](#class_loom___m_s5803_1aef9ff121171d7b27f369a60495666ab7) {#class_loom___m_s5803_1aef9ff121171d7b27f369a60495666ab7}

Read calibration data stored on sensor.

#### `protected float `[`pressure`](#class_loom___m_s5803_1afe0623825dcab69c7f204e32b0fcab56) {#class_loom___m_s5803_1afe0623825dcab69c7f204e32b0fcab56}

Measure pressure. Units: mbar.

#### `protected float `[`temp`](#class_loom___m_s5803_1a12df20babc5ac276523664c7d6607810) {#class_loom___m_s5803_1a12df20babc5ac276523664c7d6607810}

Measured temperature. Units: °C.

# class `Loom_Multiplexer` {#class_loom___multiplexer}

```
class Loom_Multiplexer
  : public LoomModule
```  

Multiplexer manager for interfacing with multiple I2C sensors.

Resources

* [Documentation](https://openslab-osu.github.io/Loom/html/class_loom___multiplexer.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected `[`LoomI2CSensor`](#class_loom_i2_c_sensor)` ** `[`sensors`](#class_loom___multiplexer_1a1ca4e73888b5d46fef8c09997148a937) | Array of I2C sensor objects.
`protected byte `[`i2c_address`](#class_loom___multiplexer_1af382145cee4e802bbf431e799983423b) | The multiplexer's I2C address.
`protected uint8_t `[`num_ports`](#class_loom___multiplexer_1a444e58e98dd2ef120e2bb5a2e6befc45) | The number of ports on the multiplexer.
`protected bool `[`dynamic_list`](#class_loom___multiplexer_1a90202eb1f078d7448cf0b0763ddbcb3d) | Whether or not sensor list is dynamic (refresh sensor list periodically)
`protected uint `[`update_period`](#class_loom___multiplexer_1a34d6b989ed4a1b5059fd28c277f9b0c9) | Interval to update sensor list at.
`protected unsigned long `[`last_update_time`](#class_loom___multiplexer_1a2e924abfa8444f66dc1b9cdf3a3aad47) | When the sensor list was last updated.

## Members

#### `protected `[`LoomI2CSensor`](#class_loom_i2_c_sensor)` ** `[`sensors`](#class_loom___multiplexer_1a1ca4e73888b5d46fef8c09997148a937) {#class_loom___multiplexer_1a1ca4e73888b5d46fef8c09997148a937}

Array of I2C sensor objects.

#### `protected byte `[`i2c_address`](#class_loom___multiplexer_1af382145cee4e802bbf431e799983423b) {#class_loom___multiplexer_1af382145cee4e802bbf431e799983423b}

The multiplexer's I2C address.

#### `protected uint8_t `[`num_ports`](#class_loom___multiplexer_1a444e58e98dd2ef120e2bb5a2e6befc45) {#class_loom___multiplexer_1a444e58e98dd2ef120e2bb5a2e6befc45}

The number of ports on the multiplexer.

#### `protected bool `[`dynamic_list`](#class_loom___multiplexer_1a90202eb1f078d7448cf0b0763ddbcb3d) {#class_loom___multiplexer_1a90202eb1f078d7448cf0b0763ddbcb3d}

Whether or not sensor list is dynamic (refresh sensor list periodically)

#### `protected uint `[`update_period`](#class_loom___multiplexer_1a34d6b989ed4a1b5059fd28c277f9b0c9) {#class_loom___multiplexer_1a34d6b989ed4a1b5059fd28c277f9b0c9}

Interval to update sensor list at.

#### `protected unsigned long `[`last_update_time`](#class_loom___multiplexer_1a2e924abfa8444f66dc1b9cdf3a3aad47) {#class_loom___multiplexer_1a2e924abfa8444f66dc1b9cdf3a3aad47}

When the sensor list was last updated.

# class `Loom_Neopixel` {#class_loom___neopixel}

```
class Loom_Neopixel
  : public LoomActuator
```  

Neopixel tri-color LED actuator module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___neopixel.html)

* [Product Page: Adafruit Neopixel](https://www.adafruit.com/category/168)

* [Dependency](https://github.com/adafruit/Adafruit_NeoPixel)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_NeoPixel `[`pixels`](#class_loom___neopixel_1a3f177f651d50d29a0ca12d2a9ba5eb79) | Array of Neopixel instances to manage up to 3 Neopixels.
`protected bool `[`pin_enabled`](#class_loom___neopixel_1aec3c3270cf9a4c9f942296a7ebd5147d) | Whether pins A0-A2 are enabled.
`protected uint8_t `[`color_vals`](#class_loom___neopixel_1ac084d5a5236d29d024c166a79e9423f1) | The associated color values (RGB) for each of the 3 Neopixels.

## Members

#### `protected Adafruit_NeoPixel `[`pixels`](#class_loom___neopixel_1a3f177f651d50d29a0ca12d2a9ba5eb79) {#class_loom___neopixel_1a3f177f651d50d29a0ca12d2a9ba5eb79}

Array of Neopixel instances to manage up to 3 Neopixels.

#### `protected bool `[`pin_enabled`](#class_loom___neopixel_1aec3c3270cf9a4c9f942296a7ebd5147d) {#class_loom___neopixel_1aec3c3270cf9a4c9f942296a7ebd5147d}

Whether pins A0-A2 are enabled.

#### `protected uint8_t `[`color_vals`](#class_loom___neopixel_1ac084d5a5236d29d024c166a79e9423f1) {#class_loom___neopixel_1ac084d5a5236d29d024c166a79e9423f1}

The associated color values (RGB) for each of the 3 Neopixels.

# class `Loom_nRF` {#class_loom__n_r_f}

```
class Loom_nRF
  : public LoomCommPlat
```  

Nordic Radio communication platform module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom__n_r_f.html)

* [Dependency: RF24](https://github.com/nRF24/RF24) optimized fork

* [Dependency: RF24Network](https://github.com/nRF24/RF24Network) optimized fork

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected RF24 `[`radio`](#class_loom__n_r_f_1a785fc4d36d622edea8a8a1e35de80e88) | Underlying nRF24L01(+) radio object.
`protected RF24Network * `[`network`](#class_loom__n_r_f_1ac9f3accb15e32baa80557f03da7a4990) | Network layer manager for radio.
`protected uint8_t `[`address`](#class_loom__n_r_f_1a9bd096cf262ff923c4ba8e8cbd85d63b) | Device Address (should this be part of [LoomCommPlat](#class_loom_comm_plat)? – maybe not as each platform handles addresses differently)
`protected uint8_t `[`data_rate`](#class_loom__n_r_f_1aa07c72a33cf70c119ff333b3177aadd0) | Data rate.
`protected uint8_t `[`power_level`](#class_loom__n_r_f_1a47eb5605398f1d83febef1c0ec3ef27b) | Power level to send at.
`protected uint8_t `[`retry_count`](#class_loom__n_r_f_1a53c54395b6ac8e89b8c3a0b3d582a9fe) | Number of transmission retries allowed.
`protected uint16_t `[`retry_timeout`](#class_loom__n_r_f_1a57f2765c35a200cf53ad055f5e1bef13) | Delay between transmission retries (in milliseconds)
`protected uint8_t `[`multicast_level`](#class_loom__n_r_f_1a91e29e17ba984dc899ca11507adfd9f9) | Which level(s) of the network to broadcast to.

## Members

#### `protected RF24 `[`radio`](#class_loom__n_r_f_1a785fc4d36d622edea8a8a1e35de80e88) {#class_loom__n_r_f_1a785fc4d36d622edea8a8a1e35de80e88}

Underlying nRF24L01(+) radio object.

#### `protected RF24Network * `[`network`](#class_loom__n_r_f_1ac9f3accb15e32baa80557f03da7a4990) {#class_loom__n_r_f_1ac9f3accb15e32baa80557f03da7a4990}

Network layer manager for radio.

#### `protected uint8_t `[`address`](#class_loom__n_r_f_1a9bd096cf262ff923c4ba8e8cbd85d63b) {#class_loom__n_r_f_1a9bd096cf262ff923c4ba8e8cbd85d63b}

Device Address (should this be part of [LoomCommPlat](#class_loom_comm_plat)? – maybe not as each platform handles addresses differently)

#### `protected uint8_t `[`data_rate`](#class_loom__n_r_f_1aa07c72a33cf70c119ff333b3177aadd0) {#class_loom__n_r_f_1aa07c72a33cf70c119ff333b3177aadd0}

Data rate.

#### `protected uint8_t `[`power_level`](#class_loom__n_r_f_1a47eb5605398f1d83febef1c0ec3ef27b) {#class_loom__n_r_f_1a47eb5605398f1d83febef1c0ec3ef27b}

Power level to send at.

#### `protected uint8_t `[`retry_count`](#class_loom__n_r_f_1a53c54395b6ac8e89b8c3a0b3d582a9fe) {#class_loom__n_r_f_1a53c54395b6ac8e89b8c3a0b3d582a9fe}

Number of transmission retries allowed.

#### `protected uint16_t `[`retry_timeout`](#class_loom__n_r_f_1a57f2765c35a200cf53ad055f5e1bef13) {#class_loom__n_r_f_1a57f2765c35a200cf53ad055f5e1bef13}

Delay between transmission retries (in milliseconds)

#### `protected uint8_t `[`multicast_level`](#class_loom__n_r_f_1a91e29e17ba984dc899ca11507adfd9f9) {#class_loom__n_r_f_1a91e29e17ba984dc899ca11507adfd9f9}

Which level(s) of the network to broadcast to.

# class `Loom_OLED` {#class_loom___o_l_e_d}

```
class Loom_OLED
  : public LoomLogPlat
```  

OLED logging platform module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___o_l_e_d.html)

* [Product Page: Adafruit Featherwing OLED – 128x32](https://www.adafruit.com/product/2900)

* [Product Page: Adafruit Monochrome 128x32 I2C OLED](https://www.adafruit.com/product/2927)

* [Dependency: Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306)

* [Dependency: Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_SSD1306 `[`display`](#class_loom___o_l_e_d_1a0cc78f1f1fd95a4c3d16fe32c9d33499) | Underlying OLED controller.
`protected `[`Version`](#class_loom___o_l_e_d_1aaf40c7e29901e7130f86046a26cb7400)` `[`version`](#class_loom___o_l_e_d_1af0bc56c14688195fed9cafb74601bec3) | What type the OLED is (FeatherWing or breakout)
`protected byte `[`reset_pin`](#class_loom___o_l_e_d_1a8962f61e0211d70d1d1de489d1d6bbfa) | The reset pin (only applies to breakout version)
`protected `[`Format`](#class_loom___o_l_e_d_1aade282dab7eb118271f73b3b3bfc0ffd)` `[`display_format`](#class_loom___o_l_e_d_1a1cfdc8965297ea1f697d8a4d3e44b5fd) | How to display the data on OLED.
`protected uint `[`scroll_duration`](#class_loom___o_l_e_d_1ad6893fc088b080440f2f1d5d6f84d461) | The duration to complete a full cycle through a bundle of data (milliseconds)(non-blocking)
`protected byte `[`freeze_pin`](#class_loom___o_l_e_d_1a693dc20156729cb7d06e877247e914d0) | Which pin to check if display should freeze.
`protected `[`FreezeType`](#class_loom___o_l_e_d_1adedc20c1c901fe472d976c722abf0bb9)` `[`freeze_behavior`](#class_loom___o_l_e_d_1aafea3f7f393170a4cc7c5e7ff8583344) | What 'freezing' behavior should be followed.
`enum `[`Version`](#class_loom___o_l_e_d_1aaf40c7e29901e7130f86046a26cb7400) | Different forms of the OLED display.
`enum `[`Format`](#class_loom___o_l_e_d_1aade282dab7eb118271f73b3b3bfc0ffd) | Different formats to display information in.
`enum `[`FreezeType`](#class_loom___o_l_e_d_1adedc20c1c901fe472d976c722abf0bb9) | Differnt freeze behaviors.

## Members

#### `protected Adafruit_SSD1306 `[`display`](#class_loom___o_l_e_d_1a0cc78f1f1fd95a4c3d16fe32c9d33499) {#class_loom___o_l_e_d_1a0cc78f1f1fd95a4c3d16fe32c9d33499}

Underlying OLED controller.

#### `protected `[`Version`](#class_loom___o_l_e_d_1aaf40c7e29901e7130f86046a26cb7400)` `[`version`](#class_loom___o_l_e_d_1af0bc56c14688195fed9cafb74601bec3) {#class_loom___o_l_e_d_1af0bc56c14688195fed9cafb74601bec3}

What type the OLED is (FeatherWing or breakout)

#### `protected byte `[`reset_pin`](#class_loom___o_l_e_d_1a8962f61e0211d70d1d1de489d1d6bbfa) {#class_loom___o_l_e_d_1a8962f61e0211d70d1d1de489d1d6bbfa}

The reset pin (only applies to breakout version)

#### `protected `[`Format`](#class_loom___o_l_e_d_1aade282dab7eb118271f73b3b3bfc0ffd)` `[`display_format`](#class_loom___o_l_e_d_1a1cfdc8965297ea1f697d8a4d3e44b5fd) {#class_loom___o_l_e_d_1a1cfdc8965297ea1f697d8a4d3e44b5fd}

How to display the data on OLED.

#### `protected uint `[`scroll_duration`](#class_loom___o_l_e_d_1ad6893fc088b080440f2f1d5d6f84d461) {#class_loom___o_l_e_d_1ad6893fc088b080440f2f1d5d6f84d461}

The duration to complete a full cycle through a bundle of data (milliseconds)(non-blocking)

#### `protected byte `[`freeze_pin`](#class_loom___o_l_e_d_1a693dc20156729cb7d06e877247e914d0) {#class_loom___o_l_e_d_1a693dc20156729cb7d06e877247e914d0}

Which pin to check if display should freeze.

#### `protected `[`FreezeType`](#class_loom___o_l_e_d_1adedc20c1c901fe472d976c722abf0bb9)` `[`freeze_behavior`](#class_loom___o_l_e_d_1aafea3f7f393170a4cc7c5e7ff8583344) {#class_loom___o_l_e_d_1aafea3f7f393170a4cc7c5e7ff8583344}

What 'freezing' behavior should be followed.

#### `enum `[`Version`](#class_loom___o_l_e_d_1aaf40c7e29901e7130f86046a26cb7400) {#class_loom___o_l_e_d_1aaf40c7e29901e7130f86046a26cb7400}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
FEATHERWING            | FeatherWing OLED.
BREAKOUT            | Breakout board.

Different forms of the OLED display.

#### `enum `[`Format`](#class_loom___o_l_e_d_1aade282dab7eb118271f73b3b3bfc0ffd) {#class_loom___o_l_e_d_1aade282dab7eb118271f73b3b3bfc0ffd}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
FOUR            | 4 Key values
EIGHT            | 8 Key values
SCROLL            | Scrolling.

Different formats to display information in.

#### `enum `[`FreezeType`](#class_loom___o_l_e_d_1adedc20c1c901fe472d976c722abf0bb9) {#class_loom___o_l_e_d_1adedc20c1c901fe472d976c722abf0bb9}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
DISABLE            | Freeze disabled.
DATA            | Screen freezes.
SCROLL            | Scroll freezes, data updates.

Differnt freeze behaviors.

# class `Loom_PCF8523` {#class_loom___p_c_f8523}

```
class Loom_PCF8523
  : public LoomRTC
```  

PCF8523 RTC module.

OPEnS Lab usually uses the PCF8523 on the Adafruit Adalogger Featherwing

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___p_c_f8523.html)

* [Product Page: Adafruit Adalogger Featherwing - RTC + SD](https://www.adafruit.com/product/2922)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected PCF8523 `[`rtc_inst`](#class_loom___p_c_f8523_1a1eb8eaffae7033c6c2e376775e44e383) | Underlying PCF8523 manager instance.

## Members

#### `protected PCF8523 `[`rtc_inst`](#class_loom___p_c_f8523_1a1eb8eaffae7033c6c2e376775e44e383) {#class_loom___p_c_f8523_1a1eb8eaffae7033c6c2e376775e44e383}

Underlying PCF8523 manager instance.

# class `Loom_Relay` {#class_loom___relay}

```
class Loom_Relay
  : public LoomActuator
```  

Relay actuator module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___relay.html)

* [Product Page: Adafruit Latching Relay Featherwing](https://www.adafruit.com/product/3191)

* [Relay Datasheet](https://cdn-shop.adafruit.com/product-files/3191/G5LE-14-DC3-Omron-datasheet-10841140.pdf)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected byte `[`pin`](#class_loom___relay_1ad3cc1e820888c8bbf632dc03036d6a84) | The digital pin the relay is connect to.
`protected bool `[`on`](#class_loom___relay_1aaad7e04f955ebe499b45e5af5a4ed7d6) | Whether the relay is on or off.

## Members

#### `protected byte `[`pin`](#class_loom___relay_1ad3cc1e820888c8bbf632dc03036d6a84) {#class_loom___relay_1ad3cc1e820888c8bbf632dc03036d6a84}

The digital pin the relay is connect to.

#### `protected bool `[`on`](#class_loom___relay_1aaad7e04f955ebe499b45e5af5a4ed7d6) {#class_loom___relay_1aaad7e04f955ebe499b45e5af5a4ed7d6}

Whether the relay is on or off.

# class `Loom_SD` {#class_loom___s_d}

```
class Loom_SD
  : public LoomLogPlat
```  

SD logging platform module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___s_d.html)

* [Product Page: Adafruit Adalogger Featherwing](https://www.adafruit.com/product/2922)

* [Product Page: Adafruit SD Breakout](https://www.adafruit.com/product/254)

* [Dependency: SD](https://github.com/arduino-libraries/SD)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected SdFat `[`sd`](#class_loom___s_d_1a0602d9ef1a9b7f8b31870702bcb692fd) | File system object.
`protected const byte `[`chip_select`](#class_loom___s_d_1a404b47d978c23b9426dcfe1fe3a185e1) | Chip select pin.
`protected char `[`filename`](#class_loom___s_d_1a970a01045c1d73b67f23982842249097) | String of file to write to if not filename explicitly provided (should not exceed 6 characters)
`protected `[`LoomRTC`](#class_loom_r_t_c)` * `[`RTC_Inst`](#class_loom___s_d_1a3dcce942e87eabc4e3912d1af283618c) | Pointer to an RTC object for timestamps.

## Members

#### `protected SdFat `[`sd`](#class_loom___s_d_1a0602d9ef1a9b7f8b31870702bcb692fd) {#class_loom___s_d_1a0602d9ef1a9b7f8b31870702bcb692fd}

File system object.

#### `protected const byte `[`chip_select`](#class_loom___s_d_1a404b47d978c23b9426dcfe1fe3a185e1) {#class_loom___s_d_1a404b47d978c23b9426dcfe1fe3a185e1}

Chip select pin.

#### `protected char `[`filename`](#class_loom___s_d_1a970a01045c1d73b67f23982842249097) {#class_loom___s_d_1a970a01045c1d73b67f23982842249097}

String of file to write to if not filename explicitly provided (should not exceed 6 characters)

#### `protected `[`LoomRTC`](#class_loom_r_t_c)` * `[`RTC_Inst`](#class_loom___s_d_1a3dcce942e87eabc4e3912d1af283618c) {#class_loom___s_d_1a3dcce942e87eabc4e3912d1af283618c}

Pointer to an RTC object for timestamps.

# class `Loom_Servo` {#class_loom___servo}

```
class Loom_Servo
  : public LoomActuator
```  

Servo actuator module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___servo.html)

* [Product Page: Adafruit Servo Featherwing](https://www.adafruit.com/product/2928)

* [Dependency: Adafruit-PWM-Servo-Driver](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)

* [Datasheet](https://cdn-shop.adafruit.com/datasheets/PCA9685.pdf)

* [Product Page: Adafruit Servo](https://www.adafruit.com/product/169)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_PWMServoDriver `[`servo_driver`](#class_loom___servo_1ad002c8ffd5282b4e5b6127fc2c058e9f) | Underlying servo driver instance.
`protected uint8_t `[`positions`](#class_loom___servo_1a6e278d382b0a2fd1762b2062921c5fa1) | Positions of the servos.

## Members

#### `protected Adafruit_PWMServoDriver `[`servo_driver`](#class_loom___servo_1ad002c8ffd5282b4e5b6127fc2c058e9f) {#class_loom___servo_1ad002c8ffd5282b4e5b6127fc2c058e9f}

Underlying servo driver instance.

#### `protected uint8_t `[`positions`](#class_loom___servo_1a6e278d382b0a2fd1762b2062921c5fa1) {#class_loom___servo_1a6e278d382b0a2fd1762b2062921c5fa1}

Positions of the servos.

# class `Loom_SHT31D` {#class_loom___s_h_t31_d}

```
class Loom_SHT31D
  : public LoomI2CSensor
```  

SHT31D Temperature / Humidty sensor module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___s_h_t31_d.html)

* [Product Page: Adafruit Sensirion SHT31-D - Temperature & Humidity Sensor](https://www.adafruit.com/product/2857)

* [Dependency: Adafruit_SHT31](https://github.com/adafruit/Adafruit_SHT31)

* [Datasheet: Datasheet SHT3x-DIS](https://cdn-shop.adafruit.com/product-files/2857/Sensirion_Humidity_SHT3x_Datasheet_digital-767294.pdf)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_SHT31 `[`inst_sht31d`](#class_loom___s_h_t31_d_1a83749e1a9e51c760894f9ba0ec097bbf) | Underlying SHT31D sensor manager instance.
`protected float `[`temp`](#class_loom___s_h_t31_d_1a3a3d827860de7979d783b9f472379028) | Measured temperature. Units: C.
`protected float `[`humid`](#class_loom___s_h_t31_d_1aa52f9a6c11c80630b8616e5e7c54e803) | Measured humidity Units: %.

## Members

#### `protected Adafruit_SHT31 `[`inst_sht31d`](#class_loom___s_h_t31_d_1a83749e1a9e51c760894f9ba0ec097bbf) {#class_loom___s_h_t31_d_1a83749e1a9e51c760894f9ba0ec097bbf}

Underlying SHT31D sensor manager instance.

#### `protected float `[`temp`](#class_loom___s_h_t31_d_1a3a3d827860de7979d783b9f472379028) {#class_loom___s_h_t31_d_1a3a3d827860de7979d783b9f472379028}

Measured temperature. Units: C.

#### `protected float `[`humid`](#class_loom___s_h_t31_d_1aa52f9a6c11c80630b8616e5e7c54e803) {#class_loom___s_h_t31_d_1aa52f9a6c11c80630b8616e5e7c54e803}

Measured humidity Units: %.

# class `Loom_Sleep_Manager` {#class_loom___sleep___manager}

```
class Loom_Sleep_Manager
  : public LoomModule
```  

Submanager to manage sleep functionality.

Resources

* [Documentation](https://openslab-osu.github.io/Loom/html/class_loom___sleep___manager.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected `[`Loom_Interrupt_Manager`](#class_loom___interrupt___manager)` * `[`interrupt_manager`](#class_loom___sleep___manager_1a41c366974539c348f16c6b6e7f6ddae1) | Pointer to interrupt_manager instance.
`protected bool `[`use_LED`](#class_loom___sleep___manager_1ae9ffd9f5310fdac6d1df1f23741598a6) | Whether or not to use LED to indicate wake status.
`protected bool `[`delay_on_wake`](#class_loom___sleep___manager_1a8404672e11d82bea2ba0934f1c197764) | Whether to provide delay on wake.
`protected `[`Mode`](#class_loom___sleep___manager_1ab1cb2baaf00c386048490c3787a2de13)` `[`sleep_mode`](#class_loom___sleep___manager_1a3fdbc0a285480a155459ddc675a2482c) | Which sleep mode to use.
`protected byte `[`power_off_pin`](#class_loom___sleep___manager_1a46e360dc9b81c2a76e5f7f8d485dbae2) | Which pin to use to power board off (requires power board)
`enum `[`Mode`](#class_loom___sleep___manager_1ab1cb2baaf00c386048490c3787a2de13) | Different options available to sleep in.

## Members

#### `protected `[`Loom_Interrupt_Manager`](#class_loom___interrupt___manager)` * `[`interrupt_manager`](#class_loom___sleep___manager_1a41c366974539c348f16c6b6e7f6ddae1) {#class_loom___sleep___manager_1a41c366974539c348f16c6b6e7f6ddae1}

Pointer to interrupt_manager instance.

#### `protected bool `[`use_LED`](#class_loom___sleep___manager_1ae9ffd9f5310fdac6d1df1f23741598a6) {#class_loom___sleep___manager_1ae9ffd9f5310fdac6d1df1f23741598a6}

Whether or not to use LED to indicate wake status.

#### `protected bool `[`delay_on_wake`](#class_loom___sleep___manager_1a8404672e11d82bea2ba0934f1c197764) {#class_loom___sleep___manager_1a8404672e11d82bea2ba0934f1c197764}

Whether to provide delay on wake.

Used to allow user to restart Serial Monitor

#### `protected `[`Mode`](#class_loom___sleep___manager_1ab1cb2baaf00c386048490c3787a2de13)` `[`sleep_mode`](#class_loom___sleep___manager_1a3fdbc0a285480a155459ddc675a2482c) {#class_loom___sleep___manager_1a3fdbc0a285480a155459ddc675a2482c}

Which sleep mode to use.

#### `protected byte `[`power_off_pin`](#class_loom___sleep___manager_1a46e360dc9b81c2a76e5f7f8d485dbae2) {#class_loom___sleep___manager_1a46e360dc9b81c2a76e5f7f8d485dbae2}

Which pin to use to power board off (requires power board)

#### `enum `[`Mode`](#class_loom___sleep___manager_1ab1cb2baaf00c386048490c3787a2de13) {#class_loom___sleep___manager_1ab1cb2baaf00c386048490c3787a2de13}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
IDLE            | Idle.
STANDBY            | Standby.
SLEEPYDOG            | SleepyDog (sleep 'hack')
OPENS_LOWPOWER            | OPEnS board to shut board off completely.

Different options available to sleep in.

# class `Loom_SpoolPublish` {#class_loom___spool_publish}

```
class Loom_SpoolPublish
  : public LoomPublishPlat
```  

Loom Spool publishing platform.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___spool_publish.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `Loom_Stepper` {#class_loom___stepper}

```
class Loom_Stepper
  : public LoomActuator
```  

Stepper actuator module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___stepper.html)

* [Product Page: Adafruit Stepper Feather](https://www.adafruit.com/product/2927)

* [Dependency: Adafruit-PWM-Servo-Driver](https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library)

* [Dependency: Adafruit_Motor_Shield_V2_Library](https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library)

* [Datasheet: Motor Driver](https://cdn-shop.adafruit.com/datasheets/TB6612FNG_datasheet_en_20121101.pdf)

* [Product Page: Adafruit Stepper Motor](https://www.adafruit.com/product/324)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_MotorShield * `[`AFMS`](#class_loom___stepper_1a6e1cef0a5db1d2d69c1fb09f1046770a) | Underlying motor shield controller.
`protected Adafruit_StepperMotor * `[`motors`](#class_loom___stepper_1a208acec02d1249fb449a17ca5e334cee) | Array of stepper controllers.

## Members

#### `protected Adafruit_MotorShield * `[`AFMS`](#class_loom___stepper_1a6e1cef0a5db1d2d69c1fb09f1046770a) {#class_loom___stepper_1a6e1cef0a5db1d2d69c1fb09f1046770a}

Underlying motor shield controller.

#### `protected Adafruit_StepperMotor * `[`motors`](#class_loom___stepper_1a208acec02d1249fb449a17ca5e334cee) {#class_loom___stepper_1a208acec02d1249fb449a17ca5e334cee}

Array of stepper controllers.

# class `Loom_TMP007` {#class_loom___t_m_p007}

```
class Loom_TMP007
  : public LoomI2CSensor
```  

TMP007 thermopile temperature sensor module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___t_m_p007.html)

* [Product Page: Contact-less Infrared Thermopile Sensor Breakout - TMP007](https://www.adafruit.com/product/2023)

* [Dependency: Adafruit_TMP007_Library](https://github.com/adafruit/Adafruit_TMP007_Library)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_TMP007 `[`inst_tmp007`](#class_loom___t_m_p007_1ab32371c9649b51b8592d2a783617cf97) | Underlying TMP007 sensor manager instance.
`protected float `[`object_temp`](#class_loom___t_m_p007_1ae75459c624c69fd36cf699200ead2c8a) | Object temperature. Units: °C.
`protected float `[`die_temp`](#class_loom___t_m_p007_1a93d28d31fe70ff78782942b7f2a7e840) | Die temperature. Units: °C.

## Members

#### `protected Adafruit_TMP007 `[`inst_tmp007`](#class_loom___t_m_p007_1ab32371c9649b51b8592d2a783617cf97) {#class_loom___t_m_p007_1ab32371c9649b51b8592d2a783617cf97}

Underlying TMP007 sensor manager instance.

#### `protected float `[`object_temp`](#class_loom___t_m_p007_1ae75459c624c69fd36cf699200ead2c8a) {#class_loom___t_m_p007_1ae75459c624c69fd36cf699200ead2c8a}

Object temperature. Units: °C.

#### `protected float `[`die_temp`](#class_loom___t_m_p007_1a93d28d31fe70ff78782942b7f2a7e840) {#class_loom___t_m_p007_1a93d28d31fe70ff78782942b7f2a7e840}

Die temperature. Units: °C.

# class `Loom_TSL2561` {#class_loom___t_s_l2561}

```
class Loom_TSL2561
  : public LoomI2CSensor
```  

TSL2561 Luminosity sensor module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___t_s_l2561.html)

* [Product Page: Adafruit TSL2561 Digital Luminosity/Lux/Light Sensor Breakout](https://www.adafruit.com/product/439)

* [Dependency: Adafruit_TSL2561](https://github.com/adafruit/Adafruit_TSL2561)

* [Dependency: Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

* [Datasheet: Light-To-Digital Converter](https://cdn-shop.adafruit.com/datasheets/TSL2561.pdf)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_TSL2561_Unified `[`inst_TSL2561`](#class_loom___t_s_l2561_1a9e9a549aee3695274a46be5f85e8e54e) | Underlying TSL2561 sensor manager instance.
`protected uint8_t `[`gain`](#class_loom___t_s_l2561_1a6a5848bae19a14f561d39ea447a999c4) | Gain level.
`protected uint8_t `[`resolution`](#class_loom___t_s_l2561_1a600b48b5fa17b6777fa343cdf145f382) | Sensor resolution setting.
`protected uint16_t `[`lightIR`](#class_loom___t_s_l2561_1a290105175178ce8cbc47d8d2b958d7f8) | Measured infra-red. Units: lux.
`protected uint16_t `[`lightFull`](#class_loom___t_s_l2561_1a0b91894008dd82a63b4401794275fb2f) | Measure full spectrum. Units: lux.

## Members

#### `protected Adafruit_TSL2561_Unified `[`inst_TSL2561`](#class_loom___t_s_l2561_1a9e9a549aee3695274a46be5f85e8e54e) {#class_loom___t_s_l2561_1a9e9a549aee3695274a46be5f85e8e54e}

Underlying TSL2561 sensor manager instance.

#### `protected uint8_t `[`gain`](#class_loom___t_s_l2561_1a6a5848bae19a14f561d39ea447a999c4) {#class_loom___t_s_l2561_1a6a5848bae19a14f561d39ea447a999c4}

Gain level.

#### `protected uint8_t `[`resolution`](#class_loom___t_s_l2561_1a600b48b5fa17b6777fa343cdf145f382) {#class_loom___t_s_l2561_1a600b48b5fa17b6777fa343cdf145f382}

Sensor resolution setting.

#### `protected uint16_t `[`lightIR`](#class_loom___t_s_l2561_1a290105175178ce8cbc47d8d2b958d7f8) {#class_loom___t_s_l2561_1a290105175178ce8cbc47d8d2b958d7f8}

Measured infra-red. Units: lux.

#### `protected uint16_t `[`lightFull`](#class_loom___t_s_l2561_1a0b91894008dd82a63b4401794275fb2f) {#class_loom___t_s_l2561_1a0b91894008dd82a63b4401794275fb2f}

Measure full spectrum. Units: lux.

# class `Loom_TSL2591` {#class_loom___t_s_l2591}

```
class Loom_TSL2591
  : public LoomI2CSensor
```  

TSL2591 Lux sensor module.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___t_s_l2591.html)

* [Product Page: Adafruit TSL2591 High Dynamic Range Digital Light Sensor](https://www.adafruit.com/product/1980)

* [Dependency: Adafruit_TSL2591_Library](https://github.com/adafruit/Adafruit_TSL2591_Library)

* [Dependency: Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)

* [Datasheet: TSL2591](https://cdn-shop.adafruit.com/datasheets/TSL25911_Datasheet_EN_v1.pdf)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected Adafruit_TSL2591 `[`inst_tsl2591`](#class_loom___t_s_l2591_1a158e5d5f081b8d690c034c74d9856199) | Underlying TSL2591 sensor manager instance.
`protected uint16_t `[`vis`](#class_loom___t_s_l2591_1a89d7af5c80c4fd1c51206e7fff0fa292) | Measured visable. Units: lux.
`protected uint16_t `[`ir`](#class_loom___t_s_l2591_1a5ff1546e2339ae92ff7166e223619b1f) | Measured infra-red. Units: lux.
`protected uint16_t `[`full`](#class_loom___t_s_l2591_1a68c7142a08c2aebc03056f701a272d9b) | Measured full spectrum. Units: lux.
`protected uint8_t `[`gain_level`](#class_loom___t_s_l2591_1ab780e3d0e00a84a01405235182821708) | Sensor gain level setting to use.
`protected uint8_t `[`timing_level`](#class_loom___t_s_l2591_1a5909e1a24e7a1f15995cc001c64c70dd) | Sensor integration time setting.

## Members

#### `protected Adafruit_TSL2591 `[`inst_tsl2591`](#class_loom___t_s_l2591_1a158e5d5f081b8d690c034c74d9856199) {#class_loom___t_s_l2591_1a158e5d5f081b8d690c034c74d9856199}

Underlying TSL2591 sensor manager instance.

#### `protected uint16_t `[`vis`](#class_loom___t_s_l2591_1a89d7af5c80c4fd1c51206e7fff0fa292) {#class_loom___t_s_l2591_1a89d7af5c80c4fd1c51206e7fff0fa292}

Measured visable. Units: lux.

#### `protected uint16_t `[`ir`](#class_loom___t_s_l2591_1a5ff1546e2339ae92ff7166e223619b1f) {#class_loom___t_s_l2591_1a5ff1546e2339ae92ff7166e223619b1f}

Measured infra-red. Units: lux.

#### `protected uint16_t `[`full`](#class_loom___t_s_l2591_1a68c7142a08c2aebc03056f701a272d9b) {#class_loom___t_s_l2591_1a68c7142a08c2aebc03056f701a272d9b}

Measured full spectrum. Units: lux.

#### `protected uint8_t `[`gain_level`](#class_loom___t_s_l2591_1ab780e3d0e00a84a01405235182821708) {#class_loom___t_s_l2591_1ab780e3d0e00a84a01405235182821708}

Sensor gain level setting to use.

#### `protected uint8_t `[`timing_level`](#class_loom___t_s_l2591_1a5909e1a24e7a1f15995cc001c64c70dd) {#class_loom___t_s_l2591_1a5909e1a24e7a1f15995cc001c64c70dd}

Sensor integration time setting.

# class `Loom_WiFi` {#class_loom___wi_fi}

```
class Loom_WiFi
  : public LoomInternetPlat
```  

WiFi InternetPlat.

> Requires 7KB of free SRAM at runtime to use.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___wifi.html)

* [Product Page: Adafruit Feather M0 WiFi](https://www.adafruit.com/product/3010)

* [Dependency: WiFi201](https://github.com/OPEnSLab-OSU/WiFi201) OPEbS Lab fork of Arduino WiFi101 library

* [Dependency: WiFi101](https://github.com/arduino-libraries/WiFi101)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected const String `[`SSID`](#class_loom___wi_fi_1a79239f0be6d0094eef2b2e47cac5e252) | Host WiFi network name.
`protected const String `[`pass`](#class_loom___wi_fi_1a36402713f3a011add130d1b0b8182822) | Host WiFi network password.
`protected WiFiSSLClient `[`client`](#class_loom___wi_fi_1a1a28f99f2084e4407157215543a5c099) | SSLClient object for WiFi.

## Members

#### `protected const String `[`SSID`](#class_loom___wi_fi_1a79239f0be6d0094eef2b2e47cac5e252) {#class_loom___wi_fi_1a79239f0be6d0094eef2b2e47cac5e252}

Host WiFi network name.

#### `protected const String `[`pass`](#class_loom___wi_fi_1a36402713f3a011add130d1b0b8182822) {#class_loom___wi_fi_1a36402713f3a011add130d1b0b8182822}

Host WiFi network password.

#### `protected WiFiSSLClient `[`client`](#class_loom___wi_fi_1a1a28f99f2084e4407157215543a5c099) {#class_loom___wi_fi_1a1a28f99f2084e4407157215543a5c099}

SSLClient object for WiFi.

# class `Loom_ZXGesture` {#class_loom___z_x_gesture}

```
class Loom_ZXGesture
  : public LoomI2CSensor
```  

ZXGesture position / gesture sensor module.

Only position or gestures can be measured at a given time, not both.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom___z_x_gesture.html)

* [Product Page: Sparkfun ZX Distance and Gesture Sensor](https://www.sparkfun.com/products/13162)

* [Dependency: SparkFun ZX Distance and Gesture Sensor Arduino Library](https://github.com/sparkfun/SparkFun_ZX_Distance_and_Gesture_Sensor_Arduino_Library)

* [Datasheet: ZX SparkFun Sensor Datasheet](https://cdn.sparkfun.com/assets/learn_tutorials/3/4/5/XYZ_Interactive_Technologies_-_ZX_SparkFun_Sensor_Datasheet.pdf)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected ZX_Sensor `[`inst_ZX`](#class_loom___z_x_gesture_1a7b338ec49ed044f2b8faf13384dd19a1) | Underlying ZX sensor manager instance.
`protected `[`Mode`](#class_loom___z_x_gesture_1a4c381d3f296be2cac6188fd8c5f8b7dd)` `[`mode`](#class_loom___z_x_gesture_1a06039a2e005fee4e1542534cc71b1316) | Sensor mode (read positions or gesture - mutually exclusive)
`protected uint8_t `[`pos`](#class_loom___z_x_gesture_1aceb243194a3f03ca569ddbb4b41ef56c) | Measured positions (x, y pos). Units: mm.
`protected GestureType `[`gesture`](#class_loom___z_x_gesture_1ad671101df307f96d5a8c1e281a3eaea9) | Last read gesture type.
`protected String `[`gesture_type`](#class_loom___z_x_gesture_1afc1047aaf8abcbb6e9b155e2c025ddb9) | String labelling last read gesture type.
`protected uint8_t `[`gesture_speed`](#class_loom___z_x_gesture_1a9236f5979e3a46c0db2164bd343be305) | The speed of the last measured gesture.
`enum `[`Mode`](#class_loom___z_x_gesture_1a4c381d3f296be2cac6188fd8c5f8b7dd) | Different modes of the ZX gesture sensor.

## Members

#### `protected ZX_Sensor `[`inst_ZX`](#class_loom___z_x_gesture_1a7b338ec49ed044f2b8faf13384dd19a1) {#class_loom___z_x_gesture_1a7b338ec49ed044f2b8faf13384dd19a1}

Underlying ZX sensor manager instance.

#### `protected `[`Mode`](#class_loom___z_x_gesture_1a4c381d3f296be2cac6188fd8c5f8b7dd)` `[`mode`](#class_loom___z_x_gesture_1a06039a2e005fee4e1542534cc71b1316) {#class_loom___z_x_gesture_1a06039a2e005fee4e1542534cc71b1316}

Sensor mode (read positions or gesture - mutually exclusive)

#### `protected uint8_t `[`pos`](#class_loom___z_x_gesture_1aceb243194a3f03ca569ddbb4b41ef56c) {#class_loom___z_x_gesture_1aceb243194a3f03ca569ddbb4b41ef56c}

Measured positions (x, y pos). Units: mm.

#### `protected GestureType `[`gesture`](#class_loom___z_x_gesture_1ad671101df307f96d5a8c1e281a3eaea9) {#class_loom___z_x_gesture_1ad671101df307f96d5a8c1e281a3eaea9}

Last read gesture type.

#### `protected String `[`gesture_type`](#class_loom___z_x_gesture_1afc1047aaf8abcbb6e9b155e2c025ddb9) {#class_loom___z_x_gesture_1afc1047aaf8abcbb6e9b155e2c025ddb9}

String labelling last read gesture type.

#### `protected uint8_t `[`gesture_speed`](#class_loom___z_x_gesture_1a9236f5979e3a46c0db2164bd343be305) {#class_loom___z_x_gesture_1a9236f5979e3a46c0db2164bd343be305}

The speed of the last measured gesture.

#### `enum `[`Mode`](#class_loom___z_x_gesture_1a4c381d3f296be2cac6188fd8c5f8b7dd) {#class_loom___z_x_gesture_1a4c381d3f296be2cac6188fd8c5f8b7dd}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
POS            | Measure z and x position.
GEST            | Measure gestures.

Different modes of the ZX gesture sensor.

# class `LoomActuator` {#class_loom_actuator}

```
class LoomActuator
  : public LoomModule
```  

Abstract base class of actuator modules.

All actuator modules inherit from this class.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_actuator.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `LoomCommPlat` {#class_loom_comm_plat}

```
class LoomCommPlat
  : public LoomModule
```  

Abstract base of communication platform modules.

All communication platform modules inherit from this class.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_comm_plat.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected uint16_t `[`max_message_len`](#class_loom_comm_plat_1a2b4f69c2ec15028f5f281c3d1d399dba) | The maximum message length.
`protected int16_t `[`signal_strength`](#class_loom_comm_plat_1a20b6f639d308b8b2f05cea41597bd6d3) | RSSI for Lora (need to determine what the other platforms use)
`protected StaticJsonDocument< 1500 > `[`messageJson`](#class_loom_comm_plat_1a065bda8c5f83a087359bda645b51c87e) | Document to read incoming data into.

## Members

#### `protected uint16_t `[`max_message_len`](#class_loom_comm_plat_1a2b4f69c2ec15028f5f281c3d1d399dba) {#class_loom_comm_plat_1a2b4f69c2ec15028f5f281c3d1d399dba}

The maximum message length.

#### `protected int16_t `[`signal_strength`](#class_loom_comm_plat_1a20b6f639d308b8b2f05cea41597bd6d3) {#class_loom_comm_plat_1a20b6f639d308b8b2f05cea41597bd6d3}

RSSI for Lora (need to determine what the other platforms use)

#### `protected StaticJsonDocument< 1500 > `[`messageJson`](#class_loom_comm_plat_1a065bda8c5f83a087359bda645b51c87e) {#class_loom_comm_plat_1a065bda8c5f83a087359bda645b51c87e}

Document to read incoming data into.

CommPlatforms need their own JsonDocument because an incoming message can only be deserialized into JsonDocuments, not JsonObjects. And it seemed bad design to pass around references to the [LoomManager](#class_loom_manager)'s internal JsonDocument. Especially as the [LoomManager](#class_loom_manager) is intended to be non-mandatory for usage of Loom

# class `LoomFactory` {#class_loom_factory}

```
class LoomFactory
  : public FactoryBase
```  

Factory is used by [LoomManager](#class_loom_manager) when parsing Json to match module names to their associated constructors, and calling with parameters from the Json.

The template parameters are used to select whether certain blocks of modules are included in the lookup table. This is determined at compile time to not include unnecessary module classes and supporting code, thus reducing code size

Resources

* [LoomFactory Documentation](https://openslab-osu.github.io/Loom/html/class_loom_factory.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  `[`LoomFactory`](#class_loom_factory_1a58a9d3e047ee49d9111506390f122abd)`() = default` | Constructor.
`public  `[`~LoomFactory`](#class_loom_factory_1a15b465cec2e3d8b88493ba773d81aadc)`() = default` | Destructor.
`public inline virtual void `[`print_table`](#class_loom_factory_1a46abdb293410e927afe5219cca6793f8)`() const` | Print the contents of the lookup table.
`public inline virtual `[`LoomModule`](#class_loom_module)` * `[`Create`](#class_loom_factory_1a3dcc9df5dcdcafa4f9b2fb69fa533e7c)`(JsonVariant module) const` | Create a module based on a subset of a Json configuration.

## Members

#### `public  `[`LoomFactory`](#class_loom_factory_1a58a9d3e047ee49d9111506390f122abd)`() = default` {#class_loom_factory_1a58a9d3e047ee49d9111506390f122abd}

Constructor.

#### `public  `[`~LoomFactory`](#class_loom_factory_1a15b465cec2e3d8b88493ba773d81aadc)`() = default` {#class_loom_factory_1a15b465cec2e3d8b88493ba773d81aadc}

Destructor.

#### `public inline virtual void `[`print_table`](#class_loom_factory_1a46abdb293410e927afe5219cca6793f8)`() const` {#class_loom_factory_1a46abdb293410e927afe5219cca6793f8}

Print the contents of the lookup table.

#### `public inline virtual `[`LoomModule`](#class_loom_module)` * `[`Create`](#class_loom_factory_1a3dcc9df5dcdcafa4f9b2fb69fa533e7c)`(JsonVariant module) const` {#class_loom_factory_1a3dcc9df5dcdcafa4f9b2fb69fa533e7c}

Create a module based on a subset of a Json configuration.

Needs name and parameters to the constructor as an array (or the word 'default') if that module has default values for all parameters 
#### Parameters
* `module` Subset of a Json configuration, used to identify module to create and with what parameters 

#### Returns
[LoomModule](#class_loom_module) if created, nullptr if it could not be created

# class `LoomI2CSensor` {#class_loom_i2_c_sensor}

```
class LoomI2CSensor
  : public LoomSensor
```  

Abstract root for I2C sensor modules.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_i2_c_sensor.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected const byte `[`i2c_address`](#class_loom_i2_c_sensor_1a6ff389c1f015152a9ebfccb037d3d90e) | The sensor's I2C address.
`protected const uint8_t `[`port_num`](#class_loom_i2_c_sensor_1ab5c61951a994a76a2aa2e5400089dad1) | Used with multiplexer, keep track of port it is on.

## Members

#### `protected const byte `[`i2c_address`](#class_loom_i2_c_sensor_1a6ff389c1f015152a9ebfccb037d3d90e) {#class_loom_i2_c_sensor_1a6ff389c1f015152a9ebfccb037d3d90e}

The sensor's I2C address.

If the sensor supports mutliple, make sure this matches the current configuration of the i2c address

#### `protected const uint8_t `[`port_num`](#class_loom_i2_c_sensor_1ab5c61951a994a76a2aa2e5400089dad1) {#class_loom_i2_c_sensor_1ab5c61951a994a76a2aa2e5400089dad1}

Used with multiplexer, keep track of port it is on.

# class `LoomInternetPlat` {#class_loom_internet_plat}

```
class LoomInternetPlat
  : public LoomModule
```  

Abstract internet communication module.

All internet modules inherit from this class.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_internet_plat.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `LoomLogPlat` {#class_loom_log_plat}

```
class LoomLogPlat
  : public LoomModule
```  

Abstract base of logging platforms.

All logging modules inherit from this class.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_log_plat.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected bool `[`enable_rate_filter`](#class_loom_log_plat_1a6d343b76b79a1e51572bcf2991966e61) | Whether or not to enable a minimum delay between logging.
`protected uint16_t `[`min_filter_delay`](#class_loom_log_plat_1a3d4ffe4204560b9677b8007be7f522be) | Minimum delay between logging (milliseconds)
`protected unsigned long `[`last_log_millis`](#class_loom_log_plat_1aa6ad61d7d6c126aa13dcc91debef67e3) | Value of millis() at last executed log time.

## Members

#### `protected bool `[`enable_rate_filter`](#class_loom_log_plat_1a6d343b76b79a1e51572bcf2991966e61) {#class_loom_log_plat_1a6d343b76b79a1e51572bcf2991966e61}

Whether or not to enable a minimum delay between logging.

#### `protected uint16_t `[`min_filter_delay`](#class_loom_log_plat_1a3d4ffe4204560b9677b8007be7f522be) {#class_loom_log_plat_1a3d4ffe4204560b9677b8007be7f522be}

Minimum delay between logging (milliseconds)

#### `protected unsigned long `[`last_log_millis`](#class_loom_log_plat_1aa6ad61d7d6c126aa13dcc91debef67e3) {#class_loom_log_plat_1aa6ad61d7d6c126aa13dcc91debef67e3}

Value of millis() at last executed log time.

# class `LoomManager` {#class_loom_manager}

Manager to contain Loom modules and provide users with a simpler API.

Resources

* [LoomManager Documentation](https://openslab-osu.github.io/Loom/html/class_loom_manager.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected char `[`device_name`](#class_loom_manager_1aac06e8c17477ab5d8991e782f9ffecca) | The name of the device.
`protected uint8_t `[`instance`](#class_loom_manager_1af2ce718d7f1d10bdd48344acabf47488) | The instance / channel ID within the subnet.
`protected uint16_t `[`interval`](#class_loom_manager_1af5742e2169b750d8b4221778e5c9f629) | Default value for [pause()](#class_loom_manager_1ad602ce9ea8a6a804ed07dd3a5f40891e)/nap().
`protected `[`DeviceType`](#class_loom_manager_1a23170d165993ad196a7604fd08b29400)` `[`device_type`](#class_loom_manager_1af280305895cc3b2c5022d4a5fabeb2c4) | Device type (Hub / Node)
`protected `[`Loom_Interrupt_Manager`](#class_loom___interrupt___manager)` * `[`interrupt_manager`](#class_loom_manager_1a9e706e9a8e7acdd2a92ef77121921a0c) | Pointer to an interrupt manager.
`protected `[`Loom_Sleep_Manager`](#class_loom___sleep___manager)` * `[`sleep_manager`](#class_loom_manager_1a2c63f985019cf7ee5766ab503cd7f562) | Pointer to a sleep manager.
`protected `[`LoomRTC`](#class_loom_r_t_c)` * `[`rtc_module`](#class_loom_manager_1a5ff1841a189242d33d2f0ff313e8a74a) | RTC object pointer.
`protected std::vector< `[`LoomModule`](#class_loom_module)` * > `[`modules`](#class_loom_manager_1a2880117fb788e2210560e87ed655cb8e) | Vectors of [LoomModule](#class_loom_module) pointers.
`protected `[`Verbosity`](#_loom___module_8h_1abf3be10d03894afb391f3a2935e3b313)` `[`print_verbosity`](#class_loom_manager_1a00b6fc4d662e25ea1b88a427909b8a94) | Print detail verbosity.
`protected `[`Verbosity`](#_loom___module_8h_1abf3be10d03894afb391f3a2935e3b313)` `[`package_verbosity`](#class_loom_manager_1a7e57c09ed534c554f96c457127294f20) | Package detail verbosity.
`protected StaticJsonDocument< 2000 > `[`doc`](#class_loom_manager_1ac1b761a5c37d853db374c13a1cb3226c) | Json data.
`protected uint `[`packet_number`](#class_loom_manager_1a7537185cd89f2b472d338268fa2418c9) | Packet number, incremented each time package is called.
`enum `[`DeviceType`](#class_loom_manager_1a23170d165993ad196a7604fd08b29400) | Different general types of devices.

## Members

#### `protected char `[`device_name`](#class_loom_manager_1aac06e8c17477ab5d8991e782f9ffecca) {#class_loom_manager_1aac06e8c17477ab5d8991e782f9ffecca}

The name of the device.

#### `protected uint8_t `[`instance`](#class_loom_manager_1af2ce718d7f1d10bdd48344acabf47488) {#class_loom_manager_1af2ce718d7f1d10bdd48344acabf47488}

The instance / channel ID within the subnet.

#### `protected uint16_t `[`interval`](#class_loom_manager_1af5742e2169b750d8b4221778e5c9f629) {#class_loom_manager_1af5742e2169b750d8b4221778e5c9f629}

Default value for [pause()](#class_loom_manager_1ad602ce9ea8a6a804ed07dd3a5f40891e)/nap().

Used so that manager can control interval, rather than code in .ino

#### `protected `[`DeviceType`](#class_loom_manager_1a23170d165993ad196a7604fd08b29400)` `[`device_type`](#class_loom_manager_1af280305895cc3b2c5022d4a5fabeb2c4) {#class_loom_manager_1af280305895cc3b2c5022d4a5fabeb2c4}

Device type (Hub / Node)

#### `protected `[`Loom_Interrupt_Manager`](#class_loom___interrupt___manager)` * `[`interrupt_manager`](#class_loom_manager_1a9e706e9a8e7acdd2a92ef77121921a0c) {#class_loom_manager_1a9e706e9a8e7acdd2a92ef77121921a0c}

Pointer to an interrupt manager.

Used for convenience, another pointer can exist in modules vector

#### `protected `[`Loom_Sleep_Manager`](#class_loom___sleep___manager)` * `[`sleep_manager`](#class_loom_manager_1a2c63f985019cf7ee5766ab503cd7f562) {#class_loom_manager_1a2c63f985019cf7ee5766ab503cd7f562}

Pointer to a sleep manager.

Used for convenience, another pointer can exist in modules vector

#### `protected `[`LoomRTC`](#class_loom_r_t_c)` * `[`rtc_module`](#class_loom_manager_1a5ff1841a189242d33d2f0ff313e8a74a) {#class_loom_manager_1a5ff1841a189242d33d2f0ff313e8a74a}

RTC object pointer.

Used for convenience, another pointer can exist in modules vector

#### `protected std::vector< `[`LoomModule`](#class_loom_module)` * > `[`modules`](#class_loom_manager_1a2880117fb788e2210560e87ed655cb8e) {#class_loom_manager_1a2880117fb788e2210560e87ed655cb8e}

Vectors of [LoomModule](#class_loom_module) pointers.

#### `protected `[`Verbosity`](#_loom___module_8h_1abf3be10d03894afb391f3a2935e3b313)` `[`print_verbosity`](#class_loom_manager_1a00b6fc4d662e25ea1b88a427909b8a94) {#class_loom_manager_1a00b6fc4d662e25ea1b88a427909b8a94}

Print detail verbosity.

#### `protected `[`Verbosity`](#_loom___module_8h_1abf3be10d03894afb391f3a2935e3b313)` `[`package_verbosity`](#class_loom_manager_1a7e57c09ed534c554f96c457127294f20) {#class_loom_manager_1a7e57c09ed534c554f96c457127294f20}

Package detail verbosity.

#### `protected StaticJsonDocument< 2000 > `[`doc`](#class_loom_manager_1ac1b761a5c37d853db374c13a1cb3226c) {#class_loom_manager_1ac1b761a5c37d853db374c13a1cb3226c}

Json data.

#### `protected uint `[`packet_number`](#class_loom_manager_1a7537185cd89f2b472d338268fa2418c9) {#class_loom_manager_1a7537185cd89f2b472d338268fa2418c9}

Packet number, incremented each time package is called.

#### `enum `[`DeviceType`](#class_loom_manager_1a23170d165993ad196a7604fd08b29400) {#class_loom_manager_1a23170d165993ad196a7604fd08b29400}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
HUB            | Central device.
NODE            | Data collecting / actuating node.
REPEATER            | Forwards messages between other devices.

Different general types of devices.

# class `LoomModule` {#class_loom_module}

Abstract root of Loom component modules inheritance hierarchy.

All modules in Loom inherit from [LoomModule](#class_loom_module)

Resources

* [LoomModule Documentation](https://openslab-osu.github.io/Loom/html/class_loom_module.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected const `[`Type`](#class_loom_module_1aee91d0a75140d51ee428fc2d4417d865)` `[`module_type`](#class_loom_module_1a152d394f37236a2b159dae19da67eeb0) | Module type (hub / node)
`protected `[`LoomManager`](#class_loom_manager)` * `[`device_manager`](#class_loom_module_1a28fe8b2fb5a9a0d14bebe13ac1c92bdf) | Pointer to manager.
`protected const String `[`module_name_base`](#class_loom_module_1a7cb2d5ef593dfc99e938c632f8843c84) | The name of the module (Should have a DEFAULT but can be overriden if provided to constructor)
`protected const char * `[`module_name`](#class_loom_module_1adf6e68ad7e9fa2acfca7a8a280680764) | 
`protected bool `[`active`](#class_loom_module_1af61a7b93ef2b9acfc22c7ad7968a45c7) | Whether or not the module should be treated as active.
`protected `[`Verbosity`](#_loom___module_8h_1abf3be10d03894afb391f3a2935e3b313)` `[`print_verbosity`](#class_loom_module_1a6e73663a6a5ebdcbe3f92153ca88ce8c) | Print verbosity.
`protected `[`Verbosity`](#_loom___module_8h_1abf3be10d03894afb391f3a2935e3b313)` `[`package_verbosity`](#class_loom_module_1aec8db10ee69e265779822792f177f982) | Package verbosity.
`enum `[`Type`](#class_loom_module_1aee91d0a75140d51ee428fc2d4417d865) | Enum to check against to when finding individual component managed by a [LoomManager](#class_loom_manager).
`enum `[`Category`](#class_loom_module_1a0c6ecc60315f95af551138dc83567068) | Enum to classify modules.

## Members

#### `protected const `[`Type`](#class_loom_module_1aee91d0a75140d51ee428fc2d4417d865)` `[`module_type`](#class_loom_module_1a152d394f37236a2b159dae19da67eeb0) {#class_loom_module_1a152d394f37236a2b159dae19da67eeb0}

Module type (hub / node)

#### `protected `[`LoomManager`](#class_loom_manager)` * `[`device_manager`](#class_loom_module_1a28fe8b2fb5a9a0d14bebe13ac1c92bdf) {#class_loom_module_1a28fe8b2fb5a9a0d14bebe13ac1c92bdf}

Pointer to manager.

[LoomManager](#class_loom_manager) provides to any modules passed to add_module

#### `protected const String `[`module_name_base`](#class_loom_module_1a7cb2d5ef593dfc99e938c632f8843c84) {#class_loom_module_1a7cb2d5ef593dfc99e938c632f8843c84}

The name of the module (Should have a DEFAULT but can be overriden if provided to constructor)

#### `protected const char * `[`module_name`](#class_loom_module_1adf6e68ad7e9fa2acfca7a8a280680764) {#class_loom_module_1adf6e68ad7e9fa2acfca7a8a280680764}

#### `protected bool `[`active`](#class_loom_module_1af61a7b93ef2b9acfc22c7ad7968a45c7) {#class_loom_module_1af61a7b93ef2b9acfc22c7ad7968a45c7}

Whether or not the module should be treated as active.

If inactive at setup (due to failed initialization, module will be deleted)

#### `protected `[`Verbosity`](#_loom___module_8h_1abf3be10d03894afb391f3a2935e3b313)` `[`print_verbosity`](#class_loom_module_1a6e73663a6a5ebdcbe3f92153ca88ce8c) {#class_loom_module_1a6e73663a6a5ebdcbe3f92153ca88ce8c}

Print verbosity.

#### `protected `[`Verbosity`](#_loom___module_8h_1abf3be10d03894afb391f3a2935e3b313)` `[`package_verbosity`](#class_loom_module_1aec8db10ee69e265779822792f177f982) {#class_loom_module_1aec8db10ee69e265779822792f177f982}

Package verbosity.

#### `enum `[`Type`](#class_loom_module_1aee91d0a75140d51ee428fc2d4417d865) {#class_loom_module_1aee91d0a75140d51ee428fc2d4417d865}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
Unknown            | 
Other            | 
Interrupt_Manager            | 
Sleep_Manager            | 
Multiplexer            | 
NTP            | 
TempSync            | 
Sensor            | 
Analog            | 
Digital            | 
I2C            | 
AS7262            | 
AS7263            | 
AS7265X            | 
FXAS21002            | 
FXOS8700            | 
LIS3DH            | 
MB1232            | 
MMA8451            | 
MPU6050            | 
MS5803            | 
SHT31D            | 
TMP007            | 
TSL2561            | 
TSL2591            | 
ZXGesture            | 
SDI12            | 
Decagon5TM            | 
DecagonGS3            | 
SPI            | 
MAX31855            | 
MAX31856            | 
L_RTC            | 
DS3231            | 
PCF8523            | 
Actuator            | 
Neopixel            | 
Relay            | 
Servo            | 
Stepper            | 
LogPlat            | 
OLED            | 
SDCARD            | 
CommPlat            | 
LoRa            | 
nRF            | 
Bluetooth            | 
InternetPlat            | 
Ethernet            | 
WiFi            | 
PublishPlat            | 
GoogleSheets            | 
MaxPub            | 
SpoolPub            | 
SubscribePlats            | 
MaxSub            | 

Enum to check against to when finding individual component managed by a [LoomManager](#class_loom_manager).

Used because we cannot use dynamic_cast to check type of modules (rtti disabled by Arduino IDE)

#### `enum `[`Category`](#class_loom_module_1a0c6ecc60315f95af551138dc83567068) {#class_loom_module_1a0c6ecc60315f95af551138dc83567068}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
Unknown            | Unknown.
Other            | Other.
Sensor            | Sensors.
L_RTC            | RTC.
Actuator            | Actuators.
LogPlat            | LogPlats.
CommPlat            | CommPlats.
InternetPlat            | InternetPlats.
PublishPlat            | PublishPlats.
SubscribePlat            | SubscribePlats.

Enum to classify modules.

Similar [LoomModule::Type](#class_loom_module_1aee91d0a75140d51ee428fc2d4417d865) but represents the astract classes rather than the leaf node modules in the inheritance tree. For a given module, its category often shares a name with the abstract class it is derived from

# class `LoomNTPSync` {#class_loom_n_t_p_sync}

```
class LoomNTPSync
  : public LoomModule
```  

Glue code to synchronize an RTC using an InternetPlat.

Always synchronizes the RTC from [Loom_Interrupt_Manager::get_RTC_module()](#class_loom___interrupt___manager_1afc68746155378f7734f75384d6615125).

Requires a [LoomRTC](#class_loom_r_t_c) and [LoomInternetPlat](#class_loom_internet_plat) module to work.

Resources

* [Documentation](https://openslab-osu.github.io/Loom/html/class_loom_n_t_p_sync.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `LoomPublishPlat` {#class_loom_publish_plat}

```
class LoomPublishPlat
  : public LoomModule
```  

Abstract internet publishing module, implementing google sheets and mongodb functionality.

All actuator modules inherit from this class.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_publish_plat.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected `[`LoomInternetPlat`](#class_loom_internet_plat)` * `[`m_internet`](#class_loom_publish_plat_1a9e07931b084ea022c48485a2df28cc83) | Pointer to internet platform to use to publish.
`protected `[`LoomModule::Type`](#class_loom_module_1aee91d0a75140d51ee428fc2d4417d865)` `[`internet_type`](#class_loom_publish_plat_1aab697868e61843787893f5509d22b9ed) | Type of internet platform used to publish.

## Members

#### `protected `[`LoomInternetPlat`](#class_loom_internet_plat)` * `[`m_internet`](#class_loom_publish_plat_1a9e07931b084ea022c48485a2df28cc83) {#class_loom_publish_plat_1a9e07931b084ea022c48485a2df28cc83}

Pointer to internet platform to use to publish.

#### `protected `[`LoomModule::Type`](#class_loom_module_1aee91d0a75140d51ee428fc2d4417d865)` `[`internet_type`](#class_loom_publish_plat_1aab697868e61843787893f5509d22b9ed) {#class_loom_publish_plat_1aab697868e61843787893f5509d22b9ed}

Type of internet platform used to publish.

Needed because finding the module for m_internet happens in [second_stage_ctor()](#class_loom_publish_plat_1a9be9c86b766a6308fb2851875b50ba56), rather than the regular constructor.

# class `LoomRTC` {#class_loom_r_t_c}

```
class LoomRTC
  : public LoomModule
```  

Abstract base class of RTC modules.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_r_t_c.html)

* [Dependency: OPEnS_RTC](https://github.com/OPEnSLab-OSU/OPEnS_RTC)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected `[`TimeZone`](#class_loom_r_t_c_1a103a9db94b031555185bbce688d2527a)` `[`timezone`](#class_loom_r_t_c_1a6c1336bf000fd9a385f315d43bbbc5b0) | The TimeZone to use.
`protected bool `[`use_utc_time`](#class_loom_r_t_c_1a47104be0e6a54508f75c635c82ed6b15) | Whether or not use UTC time, else local time.
`protected char `[`datestring`](#class_loom_r_t_c_1a86d3babf565f32bcbdffcf6e2a5efcdd) | Latest saved string of the Date (year/month/day)
`protected char `[`timestring`](#class_loom_r_t_c_1a30e77eaeb4b0e25c8fde9239e64fe13a) | Latest saved string of the time (hour:minute:second)
`enum `[`TimeZone`](#class_loom_r_t_c_1a103a9db94b031555185bbce688d2527a) | Different time zones.

## Members

#### `protected `[`TimeZone`](#class_loom_r_t_c_1a103a9db94b031555185bbce688d2527a)` `[`timezone`](#class_loom_r_t_c_1a6c1336bf000fd9a385f315d43bbbc5b0) {#class_loom_r_t_c_1a6c1336bf000fd9a385f315d43bbbc5b0}

The TimeZone to use.

#### `protected bool `[`use_utc_time`](#class_loom_r_t_c_1a47104be0e6a54508f75c635c82ed6b15) {#class_loom_r_t_c_1a47104be0e6a54508f75c635c82ed6b15}

Whether or not use UTC time, else local time.

#### `protected char `[`datestring`](#class_loom_r_t_c_1a86d3babf565f32bcbdffcf6e2a5efcdd) {#class_loom_r_t_c_1a86d3babf565f32bcbdffcf6e2a5efcdd}

Latest saved string of the Date (year/month/day)

#### `protected char `[`timestring`](#class_loom_r_t_c_1a30e77eaeb4b0e25c8fde9239e64fe13a) {#class_loom_r_t_c_1a30e77eaeb4b0e25c8fde9239e64fe13a}

Latest saved string of the time (hour:minute:second)

#### `enum `[`TimeZone`](#class_loom_r_t_c_1a103a9db94b031555185bbce688d2527a) {#class_loom_r_t_c_1a103a9db94b031555185bbce688d2527a}

 Values                         | Descriptions                                
--------------------------------|---------------------------------------------
WAT            | 
AT            | 
ADT            | 
AST            | 
EDT            | 
EST            | 
CDT            | 
CST            | 
MDT            | 
MST            | 
PDT            | 
PST            | 
ALDT            | 
ALST            | 
HST            | 
SST            | 
GMT            | 
BST            | 
CET            | 
CEST            | 
EET            | 
EEST            | 
BT            | 
ZP4            | 
ZP5            | 
ZP6            | 
ZP7            | 
AWST            | 
AWDT            | 
ACST            | 
ACDT            | 
AEST            | 
AEDT            | 

Different time zones.

# class `LoomSDI12Sensor` {#class_loom_s_d_i12_sensor}

```
class LoomSDI12Sensor
  : public LoomSensor
```  

Abstract base class for SDI12 sensor modules.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_s_d_i12_sensor.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `LoomSensor` {#class_loom_sensor}

```
class LoomSensor
  : public LoomModule
```  

Abstract root of Loom sensor modules hierarchy.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_sensor.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected uint8_t `[`num_samples`](#class_loom_sensor_1a0e74ebbaecde15ed1c71e1bb6bc6aebe) | How many samples to take and average each call to [measure()](#class_loom_sensor_1a390ef79a4d5d6b6386e099c7fe56ed1a)

## Members

#### `protected uint8_t `[`num_samples`](#class_loom_sensor_1a0e74ebbaecde15ed1c71e1bb6bc6aebe) {#class_loom_sensor_1a0e74ebbaecde15ed1c71e1bb6bc6aebe}

How many samples to take and average each call to [measure()](#class_loom_sensor_1a390ef79a4d5d6b6386e099c7fe56ed1a)

# class `LoomSPISensor` {#class_loom_s_p_i_sensor}

```
class LoomSPISensor
  : public LoomSensor
```  

Abstract base class for SPI sensor modules.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_s_p_i_sensor.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# class `LoomSubscribePlat` {#class_loom_subscribe_plat}

```
class LoomSubscribePlat
  : public LoomModule
```  

Abstract subscribe platform module.

All SubscribePlat modules inherit from this class.

Resources

* [Module Documentation](https://openslab-osu.github.io/Loom/html/class_loom_subscribe_plat.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`protected `[`LoomInternetPlat`](#class_loom_internet_plat)` * `[`m_internet`](#class_loom_subscribe_plat_1ab3992d3d1cc20c9e46794142f5e778e2) | Pointer to internet platform to use to publish.
`protected const `[`LoomModule::Type`](#class_loom_module_1aee91d0a75140d51ee428fc2d4417d865)` `[`internet_type`](#class_loom_subscribe_plat_1ac1f89a414dd68151b156c3d76a5cbffe) | Type of internet platform used to publish.
`protected StaticJsonDocument< 1000 > `[`messageJson`](#class_loom_subscribe_plat_1ae06b201b8f5928c0434beeb29f0c6ac2) | Subscribe Platforms need their own JsonDocument because an incoming message can only be deserialized into JsonDocuments, not JsonObjects.

## Members

#### `protected `[`LoomInternetPlat`](#class_loom_internet_plat)` * `[`m_internet`](#class_loom_subscribe_plat_1ab3992d3d1cc20c9e46794142f5e778e2) {#class_loom_subscribe_plat_1ab3992d3d1cc20c9e46794142f5e778e2}

Pointer to internet platform to use to publish.

#### `protected const `[`LoomModule::Type`](#class_loom_module_1aee91d0a75140d51ee428fc2d4417d865)` `[`internet_type`](#class_loom_subscribe_plat_1ac1f89a414dd68151b156c3d76a5cbffe) {#class_loom_subscribe_plat_1ac1f89a414dd68151b156c3d76a5cbffe}

Type of internet platform used to publish.

Needed because finding the module for m_internet happens in [second_stage_ctor()](#class_loom_subscribe_plat_1a5394d492cec0f9403d0442ea46bce2db), rather than the regular constructor.

#### `protected StaticJsonDocument< 1000 > `[`messageJson`](#class_loom_subscribe_plat_1ae06b201b8f5928c0434beeb29f0c6ac2) {#class_loom_subscribe_plat_1ae06b201b8f5928c0434beeb29f0c6ac2}

Subscribe Platforms need their own JsonDocument because an incoming message can only be deserialized into JsonDocuments, not JsonObjects.

And it seemed bad design to pass around references to the [LoomManager](#class_loom_manager)'s internal JsonDocument. Also as the [LoomManager](#class_loom_manager) is intended to be non-mandatory for usage of Loom

# class `LoomTempSync` {#class_loom_temp_sync}

```
class LoomTempSync
  : public LoomModule
```  

Used to synchronize temperatures between sensors that read sensors and modules that need it.

Sources include:

* [Loom_MS5803](#class_loom___m_s5803) (code: 2110)

* [Loom_SHT31D](#class_loom___s_h_t31_d) (code: 2111) 

Dependants include:

* [Loom_Analog](#class_loom___analog) (code 2001)

Resources

* [Documentation](https://openslab-osu.github.io/Loom/html/class_loom_temp_sync.html)

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------

## Members

# struct `LoomInternetPlat::ClientCleanup` {#struct_loom_internet_plat_1_1_client_cleanup}

Simply close the socket when the client dissapears, we don't want to delete the object because the client needs to cache sessions.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void `[`operator()`](#struct_loom_internet_plat_1_1_client_cleanup_1a4cdce631ff1486963529977904b57ebf)`(Client * c)` | 

## Members

#### `public inline void `[`operator()`](#struct_loom_internet_plat_1_1_client_cleanup_1a4cdce631ff1486963529977904b57ebf)`(Client * c)` {#struct_loom_internet_plat_1_1_client_cleanup_1a4cdce631ff1486963529977904b57ebf}

# struct `Loom_Interrupt_Manager::IntDetails` {#struct_loom___interrupt___manager_1_1_int_details}

Contains information defining an interrupt's configuration.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`ISRFuncPtr`](#_loom___interrupt___manager_8h_1acbfdac63f5accbdb41955751e273a5e6)` `[`ISR`](#struct_loom___interrupt___manager_1_1_int_details_1a0a43a2092b0b18ad9dd5d9a643ea59f3) | Function pointer to ISR. Set null if no interrupt linked.
`public byte `[`type`](#struct_loom___interrupt___manager_1_1_int_details_1a78248ee361844e8ee4af682d453e4f96) | Interrupt signal type to detect. LOW: 0, HIGH: 1, CHANGE: 2, FALLING: 3, INT_RISING: 4.
`public `[`ISR_Type`](#_loom___interrupt___manager_8h_1a1cb40265e710cbdf07f99beb6c4020d1)` `[`run_type`](#struct_loom___interrupt___manager_1_1_int_details_1a2fd24dd5227a5a2265680caa352eda1a) | True if ISR is called directly upon interrupt, false if called next check of flags.
`public bool `[`enabled`](#struct_loom___interrupt___manager_1_1_int_details_1a60fe1d1f7de76f29f563461eeaaa7ba0) | Whether or not this interrupt is enabled.

## Members

#### `public `[`ISRFuncPtr`](#_loom___interrupt___manager_8h_1acbfdac63f5accbdb41955751e273a5e6)` `[`ISR`](#struct_loom___interrupt___manager_1_1_int_details_1a0a43a2092b0b18ad9dd5d9a643ea59f3) {#struct_loom___interrupt___manager_1_1_int_details_1a0a43a2092b0b18ad9dd5d9a643ea59f3}

Function pointer to ISR. Set null if no interrupt linked.

#### `public byte `[`type`](#struct_loom___interrupt___manager_1_1_int_details_1a78248ee361844e8ee4af682d453e4f96) {#struct_loom___interrupt___manager_1_1_int_details_1a78248ee361844e8ee4af682d453e4f96}

Interrupt signal type to detect. LOW: 0, HIGH: 1, CHANGE: 2, FALLING: 3, INT_RISING: 4.

#### `public `[`ISR_Type`](#_loom___interrupt___manager_8h_1a1cb40265e710cbdf07f99beb6c4020d1)` `[`run_type`](#struct_loom___interrupt___manager_1_1_int_details_1a2fd24dd5227a5a2265680caa352eda1a) {#struct_loom___interrupt___manager_1_1_int_details_1a2fd24dd5227a5a2265680caa352eda1a}

True if ISR is called directly upon interrupt, false if called next check of flags.

#### `public bool `[`enabled`](#struct_loom___interrupt___manager_1_1_int_details_1a60fe1d1f7de76f29f563461eeaaa7ba0) {#struct_loom___interrupt___manager_1_1_int_details_1a60fe1d1f7de76f29f563461eeaaa7ba0}

Whether or not this interrupt is enabled.

# struct `Loom_Interrupt_Manager::InternalTimerDetails` {#struct_loom___interrupt___manager_1_1_internal_timer_details}

Contains information defining a timer's configuration.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`ISRFuncPtr`](#_loom___interrupt___manager_8h_1acbfdac63f5accbdb41955751e273a5e6)` `[`ISR`](#struct_loom___interrupt___manager_1_1_internal_timer_details_1a662ae43e5d31c8cc1a11b1d54e7ba471) | Function pointer to ISR. Set null if no interrupt linked.
`public `[`ISR_Type`](#_loom___interrupt___manager_8h_1a1cb40265e710cbdf07f99beb6c4020d1)` `[`run_type`](#struct_loom___interrupt___manager_1_1_internal_timer_details_1aec4faebdbd35178497a45463cde96170) | True if ISR is called directly upon interrupt, false if called next check of flags.
`public uint `[`duration`](#struct_loom___interrupt___manager_1_1_internal_timer_details_1ac9867b2ae42107241b4fcda8ba186f6e) | The timer duration.
`public bool `[`repeat`](#struct_loom___interrupt___manager_1_1_internal_timer_details_1a0ca216efba7dcd4ac69d5441f331c260) | Whether or not timer should repeat.
`public bool `[`enabled`](#struct_loom___interrupt___manager_1_1_internal_timer_details_1ad82f818b400d04854930c2b071b218d8) | Whether or not this timer is enabled.

## Members

#### `public `[`ISRFuncPtr`](#_loom___interrupt___manager_8h_1acbfdac63f5accbdb41955751e273a5e6)` `[`ISR`](#struct_loom___interrupt___manager_1_1_internal_timer_details_1a662ae43e5d31c8cc1a11b1d54e7ba471) {#struct_loom___interrupt___manager_1_1_internal_timer_details_1a662ae43e5d31c8cc1a11b1d54e7ba471}

Function pointer to ISR. Set null if no interrupt linked.

#### `public `[`ISR_Type`](#_loom___interrupt___manager_8h_1a1cb40265e710cbdf07f99beb6c4020d1)` `[`run_type`](#struct_loom___interrupt___manager_1_1_internal_timer_details_1aec4faebdbd35178497a45463cde96170) {#struct_loom___interrupt___manager_1_1_internal_timer_details_1aec4faebdbd35178497a45463cde96170}

True if ISR is called directly upon interrupt, false if called next check of flags.

#### `public uint `[`duration`](#struct_loom___interrupt___manager_1_1_internal_timer_details_1ac9867b2ae42107241b4fcda8ba186f6e) {#struct_loom___interrupt___manager_1_1_internal_timer_details_1ac9867b2ae42107241b4fcda8ba186f6e}

The timer duration.

#### `public bool `[`repeat`](#struct_loom___interrupt___manager_1_1_internal_timer_details_1a0ca216efba7dcd4ac69d5441f331c260) {#struct_loom___interrupt___manager_1_1_internal_timer_details_1a0ca216efba7dcd4ac69d5441f331c260}

Whether or not timer should repeat.

#### `public bool `[`enabled`](#struct_loom___interrupt___manager_1_1_internal_timer_details_1ad82f818b400d04854930c2b071b218d8) {#struct_loom___interrupt___manager_1_1_internal_timer_details_1ad82f818b400d04854930c2b071b218d8}

Whether or not this timer is enabled.

# struct `module_sort_comp` {#structmodule__sort__comp}

Used by [LoomManager](#class_loom_manager) to sort modules in its vector.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline bool `[`operator()`](#structmodule__sort__comp_1ae6683693769eb0580e7605c519de7175)`(`[`LoomModule`](#class_loom_module)` * left,`[`LoomModule`](#class_loom_module)` * right) const` | 

## Members

#### `public inline bool `[`operator()`](#structmodule__sort__comp_1ae6683693769eb0580e7605c519de7175)`(`[`LoomModule`](#class_loom_module)` * left,`[`LoomModule`](#class_loom_module)` * right) const` {#structmodule__sort__comp_1ae6683693769eb0580e7605c519de7175}

# struct `Loom_Interrupt_Manager::StopWatchDetails` {#struct_loom___interrupt___manager_1_1_stop_watch_details}

Contains information defining a stopwatch's configuration.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public unsigned long `[`start_time`](#struct_loom___interrupt___manager_1_1_stop_watch_details_1a378a0aa93e7905dd86f32f0101f4587b) | The millis time when stopwatch started.
`public bool `[`enabled`](#struct_loom___interrupt___manager_1_1_stop_watch_details_1a0b6c6cb2be37ef484cfc580644fc4e91) | Whether or not this stopwatch is enabled.

## Members

#### `public unsigned long `[`start_time`](#struct_loom___interrupt___manager_1_1_stop_watch_details_1a378a0aa93e7905dd86f32f0101f4587b) {#struct_loom___interrupt___manager_1_1_stop_watch_details_1a378a0aa93e7905dd86f32f0101f4587b}

The millis time when stopwatch started.

#### `public bool `[`enabled`](#struct_loom___interrupt___manager_1_1_stop_watch_details_1a0b6c6cb2be37ef484cfc580644fc4e91) {#struct_loom___interrupt___manager_1_1_stop_watch_details_1a0b6c6cb2be37ef484cfc580644fc4e91}

Whether or not this stopwatch is enabled.

# struct `Loom_Interrupt_Manager::TimerDetails` {#struct_loom___interrupt___manager_1_1_timer_details}

Contains information defining a timer's configuration.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`ISRFuncPtr`](#_loom___interrupt___manager_8h_1acbfdac63f5accbdb41955751e273a5e6)` `[`ISR`](#struct_loom___interrupt___manager_1_1_timer_details_1a07910352fab001df66ba7623cd350eb5) | Not a real ISR, just a function called if timer has expired.
`public uint `[`duration`](#struct_loom___interrupt___manager_1_1_timer_details_1acb8a9599ea17bdabf13f1978724b68e3) | The timer duration.
`public bool `[`repeat`](#struct_loom___interrupt___manager_1_1_timer_details_1a817a807aaad99d23cee2c62c50a618e6) | Whether or not timer should repeat.
`public bool `[`enabled`](#struct_loom___interrupt___manager_1_1_timer_details_1aa807507581014aa764730709ad88de24) | Whether or not this timer is enabled.

## Members

#### `public `[`ISRFuncPtr`](#_loom___interrupt___manager_8h_1acbfdac63f5accbdb41955751e273a5e6)` `[`ISR`](#struct_loom___interrupt___manager_1_1_timer_details_1a07910352fab001df66ba7623cd350eb5) {#struct_loom___interrupt___manager_1_1_timer_details_1a07910352fab001df66ba7623cd350eb5}

Not a real ISR, just a function called if timer has expired.

#### `public uint `[`duration`](#struct_loom___interrupt___manager_1_1_timer_details_1acb8a9599ea17bdabf13f1978724b68e3) {#struct_loom___interrupt___manager_1_1_timer_details_1acb8a9599ea17bdabf13f1978724b68e3}

The timer duration.

#### `public bool `[`repeat`](#struct_loom___interrupt___manager_1_1_timer_details_1a817a807aaad99d23cee2c62c50a618e6) {#struct_loom___interrupt___manager_1_1_timer_details_1a817a807aaad99d23cee2c62c50a618e6}

Whether or not timer should repeat.

#### `public bool `[`enabled`](#struct_loom___interrupt___manager_1_1_timer_details_1aa807507581014aa764730709ad88de24) {#struct_loom___interrupt___manager_1_1_timer_details_1aa807507581014aa764730709ad88de24}

Whether or not this timer is enabled.

# struct `LoomInternetPlat::UDPDeletor` {#struct_loom_internet_plat_1_1_u_d_p_deletor}

Close the socket and delete the UDP object when the unique ptr dissapears.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline void `[`operator()`](#struct_loom_internet_plat_1_1_u_d_p_deletor_1a66b8e3d6b1032d02ca30f26dbd238c9e)`(UDP * p)` | 

## Members

#### `public inline void `[`operator()`](#struct_loom_internet_plat_1_1_u_d_p_deletor_1a66b8e3d6b1032d02ca30f26dbd238c9e)`(UDP * p)` {#struct_loom_internet_plat_1_1_u_d_p_deletor_1a66b8e3d6b1032d02ca30f26dbd238c9e}

Generated by [Moxygen](https://sourcey.com/moxygen)