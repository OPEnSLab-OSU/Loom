#ifdef is_analog
// Generic subroutine for reading raw sensor data
uint32_t measure_analog(uint8_t chnl)
{
    int i = num_measurements;
    uint32_t reading = 0;
    while(i--)
      reading += analogRead(chnl);
    
    // Then divide by 4 to get average sample value
    return (reading >> 2);
}

// Subroutine for reading FSR small sensor data
// need 50k ohm resistor for voltage divider
// add math to convert to some known force unit
uint32_t measure_fsr_small(uint8_t chnl)
{
    int i = num_measurements;
    uint32_t reading = 0;
    while(i--)
      reading += analogRead(chnl);
    
    // Then divide by 4 to get average sample value
    return (reading >> 2);
}

/*************************************************
Temperature Compensation when using the Analog Voltage Range output on the
regular XL
Tc is the temperature in degrees C
Dcm is the distance in cm
MaxSonar sensor outputs a voltage of one bit per cm.
Dcm = (SonarVal) * (20.05*SQRT(Tc+273.15)/2)
****************************************************/
// Example subroutine for processing sonar XL sensor to Meters
uint32_t measure_sonar(uint8_t chnl)
{
    int i = num_measurements;
    uint32_t reading = 0;
    while(i--)
      reading += analogRead(chnl);
    // Then divide by 4 to get average sample value
    reading = reading >> 2;
    // ***** If using Temp Compensation, be sure to take temp value before reading sonar! *******
 // reading = reading * (20.05*SQRT(Tc+273.15)/2);
    // Send sample value
    return (reading); 
}

// Example subroutine for processing Tempature sensor MCP9700
uint32_t measure_temp()
{
    int i = num_measurements;
    uint32_t reading = 0;
    while(i--)
      reading += analogRead(temp_pin);
    
    // Convert the reading to celcius*256
    // This is the formula for MCP9700.
    // C = reading * 1.1
    // C = ( V - 1/2 ) * 100
    //
    // Then adjust for the calibation value on this sensor
    return ( ( ( ( reading * 0x120 ) - 0x800000 ) * 0x64 ) >> 16 );
}

// Example subroutine for measuring battery voltage with internal 1.1V reference
uint32_t measure_voltage()
{
    // Take the voltage reading 
    int i = num_measurements;
    uint32_t reading = 0;
    while(i--)
      reading += analogRead(voltage_pin);

    // Convert the voltage reading to volts*256
    return ( reading * voltage_reference ) >> 16; 
}
#endif // Finish Analog subroutines if used

//If using serial i2c:
#ifdef is_i2c
// *** i2c subroutines ***
// Generic subroutine for reading MPU6050 i2c Data
uint32_t measure_mpu6050(void)
{
    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }
   
   // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gy, &gx, &gz);

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));
        // NOTE: If you get this message, MPU6050 library, file "MPU6050_6Axis_MotionApps20.h" modify last byte of line 305 0x07 to 0x09

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;
  
   // now format and compile readings based on definitions from top of sketch     
        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
/*            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI); */
        #endif

        #ifdef OUTPUT_BINARY_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            yaw = ypr[0] * 180/M_PI;
            pitch = ypr[1] * 180/M_PI;
            roll = ypr[2] * 180/M_PI;
            Serial.write((uint8_t)(yaw >> 8)); Serial.write((uint8_t)(yaw & 0xFF));
            Serial.write((uint8_t)(pitch >> 8)); Serial.write((uint8_t)(pitch & 0xFF));
            Serial.write((uint8_t)(roll >> 8)); Serial.write((uint8_t)(roll & 0xFF));
        #endif

        #ifdef OUTPUT_READABLE_REALACCEL
            // display real acceleration, adjusted to remove gravity
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            Serial.print("areal\t");
            Serial.print(aaReal.x);
            Serial.print("\t");
            Serial.print(aaReal.y);
            Serial.print("\t");
            Serial.println(aaReal.z);
        #endif

        #ifdef OUTPUT_READABLE_WORLDACCEL
            // display initial world-frame acceleration, adjusted to remove gravity
            // and rotated based on known orientation from quaternion
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
            Serial.print("aworld\t");
            Serial.print(aaWorld.x);
            Serial.print("\t");
            Serial.print(aaWorld.y);
            Serial.print("\t");
            Serial.println(aaWorld.z);
        #endif

        #ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
        Serial.print("a/g:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.println(gz);
    #endif

    #ifdef OUTPUT_BINARY_ACCELGYRO
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
    #endif
    }
    
 //   return (void);
}
#endif // Send serial i2c subroutines
