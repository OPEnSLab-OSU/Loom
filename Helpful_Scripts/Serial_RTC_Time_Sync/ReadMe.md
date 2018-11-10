# RTC Time Sychronization 

The RTC clocks commonly used by Loom are the DS3231 [breakout](https://www.adafruit.com/product/3013) or [FeatherWing](https://www.adafruit.com/product/3028)  and PCF8523 [Adalogger FeatherWing](https://www.adafruit.com/product/2922). The clock times can easily be set to the compile time, but that time tends to be a few seconds behind, accounting for the time to upload the code and start the program. The Arduino [Time library](https://github.com/PaulStoffregen/Time) provides code for the Arduino and the computer, via [Processing](https://processing.org/download/), from which get the time, but that tended to be off as well. The code provided here is a slightly modified version of the Arduino sketch and commands/scripts that replace the Processing program with higher accuracy.



## Arduino Code



#### Options

Lines 17 / 18 define which of RTC clock is being used, make sure this matches the type of clock you are using.

The 'second_adjustment' on line 20 is used to account for time zone or if there is any consistent difference in what the RTC gets set to.

## Code for Computer

### Mac / Linux

On Mac and Linux, all you need is the Terminal / command line.

- Upload the provided sketch to the Arduino (leaving the Serial Monitor closed)
-  Check the COM port of the Arduino via the Tools > Port menu of the IDE 
  - (Or Run `ls /dev/cu.usbmodem*` in the Terminal and note the output)
- Open Terminal / Linux equivalent
- Run `date +T%s > /dev/cu.usbmodem#` where # matches the number at the end of the output of the previous command
- Open the Arduino IDE Serial Monitor – it should print out the time of the RTC clock

### Windows

The process is the same as above, but instead of running `date +T%s > /dev/cu.usbmodem#`, run the GetTimeT.bat script with like `getTimeT > COM10` or whatever com port the Arduino is on