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
  char addressString[255]; //declare address string buffer
  // Format MPU6050 data into OSC
  // declare the bundle
    OSCBundle bndl;
  // messages want an OSC address as first argument
  // compile bundle

  float axf = (float)ax/16000;
  float ayf = (float)ay/16000;
  float azf = (float)az/16000;

  bool freefall = false;
  // Evaluate if accelerometers are all around zero-G, if so, set freefall bool to true
  if((axf > -0.2 && axf < 0.2) && (ayf > -0.2 && ayf < 0.2) && (azf > -0.2 && azf < 0.2))
    freefall = true;
  /*
    bndl.add(PacketHeaderString "/mac0").add((int)configuration.mac[0]);
    bndl.add(PacketHeaderString "/mac1").add((int)configuration.mac[1]);
    bndl.add(PacketHeaderString "/mac2").add((int)configuration.mac[2]);
    bndl.add(PacketHeaderString "/mac3").add((int)configuration.mac[3]);
    bndl.add(PacketHeaderString "/mac4").add((int)configuration.mac[4]);
    bndl.add(PacketHeaderString "/mac5").add((int)configuration.mac[5]);
    bndl.add(PacketHeaderString "/ip").add((String)configuration.ip);
 */
// Assemble UDP Packet
// IP1 IP2 Yaw Pitch Roll aX aY aZ gX gY gZ vBatt
#ifdef OUTPUT_READABLE_YAWPITCHROLL
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/yaw");
    bndl.add(addressString).add((float)(ypr[0] * 180/M_PI));
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/roll");
    bndl.add(addressString).add((float)(ypr[1] * 180/M_PI));
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/pitch");
    bndl.add(addressString).add((float)(ypr[2] * 180/M_PI));
#endif
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/accelX");
    bndl.add(addressString).add(axf);
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/accelY");
    bndl.add(addressString).add(ayf);
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/accelZ");
    bndl.add(addressString).add(azf);
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/gyroX");
    bndl.add(addressString).add((float)gx/16000);
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/gyroY");
    bndl.add(addressString).add((float)gy/16000);
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/gyroZ");
    bndl.add(addressString).add((float)gz/16000);
#ifdef OUTPUT_READABLE_WORLDACCEL
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/rwx");
    bndl.add(addressString).add(aaWorld.x);
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/rwy");
    bndl.add(addressString).add(aaWorld.y);
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/rwz");
    bndl.add(addressString).add(aaWorld.z);
#endif
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/vbat");
    bndl.add(addressString).add(vbat);     // Tack battery voltage onto here. Will want to change this for other sensors
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/freefall");
    bndl.add(addressString).add(freefall);
   // UDP Packet
    Udp.beginPacket(configuration.ip_broadcast, 9436);
      bndl.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
   
   // empty the bundle to free room for a new one
    bndl.empty(); 
    
}

void calMPU6050()
{
   // reset offsets
       accelgyro.setXAccelOffset(0);
       accelgyro.setYAccelOffset(0);
       accelgyro.setZAccelOffset(0);
       accelgyro.setXGyroOffset(0);
       accelgyro.setYGyroOffset(0);
       accelgyro.setZGyroOffset(0);

  if (state==0){

    meansensors();
    state++;
    delay(1000);
  }

  if (state==1) {
#if DEBUG == 1
    Serial.println("\nCalculating offsets...");
#endif
    calibration();
    configuration.checksum = memValidationValue;
    state++;
    delay(1000);
  }

  if (state==2) {
    meansensors();
    // Save Sample Period into EEPROM for next power-up
//    EEPROM_writeAnything(0, configuration);
#if DEBUG == 1
    Serial.println("\nFINISHED!");
    Serial.print("\nSensor readings with offsets:\t");
    Serial.print(mean_ax); 
    Serial.print("\t");
    Serial.print(mean_ay); 
    Serial.print("\t");
    Serial.print(mean_az); 
    Serial.print("\t");
    Serial.print(mean_gx); 
    Serial.print("\t");
    Serial.print(mean_gy); 
    Serial.print("\t");
    Serial.println(mean_gz);
    Serial.print("Your offsets:\t");
    Serial.print(configuration.ax_offset); 
    Serial.print("\t");
    Serial.print(configuration.ay_offset); 
    Serial.print("\t");
    Serial.print(configuration.az_offset); 
    Serial.print("\t");
    Serial.print(configuration.gx_offset); 
    Serial.print("\t");
    Serial.print(configuration.gy_offset); 
    Serial.print("\t");
    Serial.println(configuration.gz_offset);
#endif
  state = 0; // reset state flag
  }
}

void meansensors()
{
  long i=0,buff_ax=0,buff_ay=0,buff_az=0,buff_gx=0,buff_gy=0,buff_gz=0;

  while (i<(buffersize+101)){
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    if (i>100 && i<=(buffersize+100)){ //First 100 measures are discarded
      buff_ax=buff_ax+ax;
      buff_ay=buff_ay+ay;
      buff_az=buff_az+az;
      buff_gx=buff_gx+gx;
      buff_gy=buff_gy+gy;
      buff_gz=buff_gz+gz;
    }
    if (i==(buffersize+100)){
      mean_ax=buff_ax/buffersize;
      mean_ay=buff_ay/buffersize;
      mean_az=buff_az/buffersize;
      mean_gx=buff_gx/buffersize;
      mean_gy=buff_gy/buffersize;
      mean_gz=buff_gz/buffersize;
    }
    i++;
    delay(2); //Needed so we don't get repeated measures
  }
}

void calibration(){
  configuration.ax_offset=-mean_ax/8;
  configuration.ay_offset=-mean_ay/8;
  configuration.az_offset=(16384-mean_az)/8;

  configuration.gx_offset=-mean_gx/4;
  configuration.gy_offset=-mean_gy/4;
  configuration.gz_offset=-mean_gz/4;
  while (1){
    int ready=0;
    accelgyro.setXAccelOffset(configuration.ax_offset);
    accelgyro.setYAccelOffset(configuration.ay_offset);
    accelgyro.setZAccelOffset(configuration.az_offset);

    accelgyro.setXGyroOffset(configuration.gx_offset);
    accelgyro.setYGyroOffset(configuration.gy_offset);
    accelgyro.setZGyroOffset(configuration.gz_offset);

    meansensors();
#if DEBUG == 1
    Serial.println("...");
#endif

    if (abs(mean_ax)<=acel_deadzone) ready++;
    else configuration.ax_offset=configuration.ax_offset-mean_ax/acel_deadzone;

    if (abs(mean_ay)<=acel_deadzone) ready++;
    else configuration.ay_offset=configuration.ay_offset-mean_ay/acel_deadzone;

    if (abs(16384-mean_az)<=acel_deadzone) ready++;
    else configuration.az_offset=configuration.az_offset+(16384-mean_az)/acel_deadzone;

    if (abs(mean_gx)<=giro_deadzone) ready++;
    else configuration.gx_offset=configuration.gx_offset-mean_gx/(giro_deadzone+1);

    if (abs(mean_gy)<=giro_deadzone) ready++;
    else configuration.gy_offset=configuration.gy_offset-mean_gy/(giro_deadzone+1);

    if (abs(mean_gz)<=giro_deadzone) ready++;
    else configuration.gz_offset=configuration.gz_offset-mean_gz/(giro_deadzone+1);

    if (ready==6) break;
  }
}

#endif // Send serial i2c subroutines
