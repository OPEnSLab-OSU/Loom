# Project Loom: Limitations and Bugs

## 32u4 Limitations

The 32u4 has a number of limitations in comparision to the M0 processors, which are documented below.

**Program Storage**

The 32u4 has limited program storage, generally not enough to fit a moderately complex Loom project.

**Issues with Floats**

When compiling for the 32u4, the Arduino IDE seems to use a smaller subset of standard C functions. This is most apparent when using floats. For example, `strtof` is not provided (but is for M0), and `sprintf` does not work with floats (often requiring cumbersome work-arounds / using the String library).

**RAM**

It seems that exceeding the provided RAM causes freezing. In the LOOM library, it is not unusual for for a 32u4 to run normally until receiving a bundle, likely due to declaration of more variables exceeding RAM.

## Bugs

**MPU6050 library bug** 

The default MPU6050 library has a bug preventing the alternate I2C address (0x69 instead of 0x68) from being used. Namely, line 375 of 'MPU6050_6Axis_MotionApps20.h' needed to be changed from:

    setSlaveAddress(0, 0x68);
to:

```
setSlaveAddress(0, devAddr);
```

## Other Notes