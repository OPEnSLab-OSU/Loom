# Loom Workshop

This folder contains templates and files used for Loom workshops.

## Kit Contents

4 Feather M0 WiFi boards and 4 different shields, as follows:

- Ishield
  - 2 analog inputs
  - 1 tricolor LED (Neopixel)
  - Button
  - 3 axis gyroscope and accelerometer 
- Relay
  - Toggles 12V wall power (adapter provided) on an off. Most kits demonstrate this with a fan
- Servo
  - Drives up to 8 servos
  - Included 5V wall power adapter
- Multiplexer
  - Allows up to 8 various I2C sensors to be used simultaneously



The provided sensors may vary but could be:

- Sonar distance
- Light
- Temperature
- Humidity
- Gesture Sensor

## Setup

This setup section is for a user, that is, the devices in the kit have already been flashed with the necessary code.

1. Install Max/MSP, download from [Cycling '74](https://cycling74.com/downloads)
2. Drop the provided [DataProcessorPlugins](https://github.com/OPEnSLab-OSU/InternetOfAg/tree/master/Max/_DataProcessorPlugins) folder in the Max library (e.g.  [user] / Documents / Max 7 / Library)
3. Open a provided template or the the DataProcessor.maxpat file
4. Make sure kit contents all appear to be provided and in order
5. Connect to the OPEnS WiFi network
6. In the 'Channel Manager' of the file from step 3, locate 'Family #' and set it to match your kit number
7. Plug in batteries to the devices

If everything is in order and the steps were followed correctly, the devices should start appearing in the 'Channel Manager', some may be slower than others.

## Templates

The workshop provides a variety of templates (i.e. starter DataProcessor files) with tailored focuses, such as:

- Sensors
- Actuators
- Mapping device input to another device output

The templates may have multiple levels of completion, so that they might be used as fully functional systems, or as guides to setting up a complete system.

