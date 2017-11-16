//------------------------------------------------------------------------------------------------------
// DEBUG MODE: Set to 1 if you want to see serial printouts, else, set to 0 for field use to save memory
//------------------------------------------------------------------------------------------------------
#ifndef DEBUG
#define DEBUG 0
#endif

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
#if DEBUG == 1
        Serial.println(F("FIFO overflow!"));
#endif
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
#if DEBUG == 1
/*            Serial.print("ypr\t");
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr[2] * 180/M_PI); */
#endif
        #endif

        #ifdef OUTPUT_BINARY_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            yaw = ypr[0] * 180/M_PI;
            pitch = ypr[1] * 180/M_PI;
            roll = ypr[2] * 180/M_PI;
#if DEBUG == 1
            Serial.write((uint8_t)(yaw >> 8)); Serial.write((uint8_t)(yaw & 0xFF));
            Serial.write((uint8_t)(pitch >> 8)); Serial.write((uint8_t)(pitch & 0xFF));
            Serial.write((uint8_t)(roll >> 8)); Serial.write((uint8_t)(roll & 0xFF));
#endif
        #endif

        #ifdef OUTPUT_READABLE_REALACCEL
            // display real acceleration, adjusted to remove gravity
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
#if DEBUG == 1
            Serial.print("areal\t");
            Serial.print(aaReal.x);
            Serial.print("\t");
            Serial.print(aaReal.y);
            Serial.print("\t");
            Serial.println(aaReal.z);
#endif
        #endif

        #ifdef OUTPUT_READABLE_WORLDACCEL
            // display initial world-frame acceleration, adjusted to remove gravity
            // and rotated based on known orientation from quaternion
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetAccel(&aa, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
            mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
#if DEBUG == 1
          /*
            Serial.print("aworld\t");
            Serial.print(aaWorld.x);
            Serial.print("\t");
            Serial.print(aaWorld.y);
            Serial.print("\t");
            Serial.println(aaWorld.z);
          */
#endif
        #endif

        #ifdef OUTPUT_READABLE_ACCELGYRO
        // display tab-separated accel/gyro x/y/z values
#if DEBUG == 1
        Serial.print("a/g:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.print("\t");
        Serial.print(gx); Serial.print("\t");
        Serial.print(gy); Serial.print("\t");
        Serial.println(gz);
#endif
    #endif

    #ifdef OUTPUT_BINARY_ACCELGYRO
#if DEBUG == 1
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
#endif
    #endif
    }
    
 //   return (void);
}


void udp_mpu6050(void)
{
  // Format MPU6050 data into OSC
  // declare the bundle
    OSCBundle bndl;
  // messages want an OSC address as first argument
  // compile bundle
  /*
    bndl.add("/MPU6050/mac0").add((int)mac[0]);
    bndl.add("/MPU6050/mac1").add((int)mac[1]);
    bndl.add("/MPU6050/mac2").add((int)mac[2]);
    bndl.add("/MPU6050/mac3").add((int)mac[3]);
    bndl.add("/MPU6050/mac4").add((int)mac[4]);
    bndl.add("/MPU6050/mac5").add((int)mac[5]);
    bndl.add("/MPU6050/ip").add((String)ip);
 */
// Assemble UDP Packet
// IP1 IP2 Yaw Pitch Roll aX aY aZ gX gY gZ vBatt
#ifdef OUTPUT_READABLE_YAWPITCHROLL
    bndl.add("/MPU6050/ip1").add((int32_t)ip[2]);  // Send last 2 values of IP address for Acknowledge
    bndl.add("/MPU6050/ip2").add((int32_t)ip[3]);
    bndl.add("/MPU6050/yaw").add((float)(ypr[0] * 180/M_PI));
    bndl.add("/MPU6050/pitch").add((float)(ypr[1] * 180/M_PI));
    bndl.add("/MPU6050/roll").add((float)(ypr[2] * 180/M_PI));
#endif
    bndl.add("/MPU6050/ax").add((int32_t)ax);
    bndl.add("/MPU6050/ay").add((int32_t)ay);
    bndl.add("/MPU6050/az").add((int32_t)az);
    bndl.add("/MPU6050/gx").add((int32_t)gx);
    bndl.add("/MPU6050/gy").add((int32_t)gy);
    bndl.add("/MPU6050/gz").add((int32_t)gz);
#ifdef OUTPUT_READABLE_WORLDACCEL
    bndl.add("/MPU6050/rwx").add(aaWorld.x);
    bndl.add("/MPU6050/rwy").add(aaWorld.y);
    bndl.add("/MPU6050/rwz").add(aaWorld.z);
#endif
    bndl.add("/MPU6050/vbat").add(vbat);     // Tack battery voltage onto here. Will want to change this for other sensors

   // UDP Packet
    Udp.beginPacket(ip_broadcast, 9436);
      bndl.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
   
   // empty the bundle to free room for a new one
    bndl.empty(); 
    
}



#endif // Send serial i2c subroutines
