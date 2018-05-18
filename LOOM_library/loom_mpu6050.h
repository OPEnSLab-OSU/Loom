// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
// Include libraries for serial and i2c devices
//#include "S_message.h"
#include "I2Cdev.h"
#include "Wire.h"
#include "MPU6050_6Axis_MotionApps20.h"


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================
#define INTERRUPT_PIN 11

#define i2c_addr_mpu6050 0x68													 //0x68, 0x69

// Uncoment one or more of these to determine which readings and format to send
#define OUTPUT_READABLE_YAWPITCHROLL
//#define OUTPUT_BINARY_YAWPITCHROLL
//#define OUTPUT_READABLE_REALACCEL
//#define OUTPUT_READABLE_WORLDACCEL  // this is pretty cool
//#define OUTPUT_BINARY_ACCELGYRO


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct config_mpu6050_t {
  int   ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset;
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
struct config_mpu6050_t * config_mpu6050;
void link_config_mpu6050(struct config_mpu6050_t *flash_setup_mpu6050){
    config_mpu6050 = flash_setup_mpu6050;
}


bool dmpReady = false; //set true if DMP init was successful

MPU6050 mpu;             // Create instance of MPU6050 called mpu
MPU6050 accelgyro;       // Another instance called accelgyro


uint8_t  mpuIntStatus;   // Holds actual interrupt status byte from MPU
uint8_t  devStatus;      // Return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;     // Expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;      // Count of all bytes currently in FIFO
uint8_t  fifoBuffer[64]; // FIFO storage buffer

// Orientation/motion vars
Quaternion  q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;          // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;      // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;     // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;     // [x, y, z]            gravity vector
float       euler[3];    // [psi, theta, phi]    Euler angle container
float       ypr[3];      // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// Place to save accel gyro mpu6050 readings
int16_t ax, ay, az;
int16_t gx, gy, gz;

// MPU calibration vars:
int mean_ax, mean_ay, mean_az, mean_gx, mean_gy, mean_gz, state = 0;
//int ax_offset,ay_offset,az_offset,gx_offset,gy_offset,gz_offset;

//Change these 3 variables if you want to fine tune the skecth to your needs.
int buffersize = 1000;   // Amount of readings used to average, make it higher to get more precision but sketch will be slower  (default:1000)
int acel_deadzone = 8;   // Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
int giro_deadzone = 1;   // Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
bool setup_mpu6050();
void measure_mpu6050();
void meansensors();
void calibration();
void package_mpu6050(OSCBundle *, char[]);
void package_mpu6050(OSCBundle *, char[], uint8_t);
void calMPU6050();
void calMPU6050_OSC(OSCMessage &);


// ================================================================ 
// ===                          SETUP                           === 
// ================================================================

// INTERRUPT DETECTION ROUTINE 
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
  mpuInterrupt = true;
}






// --- I2C SETUP ---
// Called by main setup
// Arguments:
// Return:
bool setup_mpu6050()
{
  #if LOOM_DEBUG == 1
    Serial.println("starting mpu6050 initialization");
  #endif
 
  // Join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
  
  pinMode(INTERRUPT_PIN, INPUT);
  
  #if LOOM_DEBUG == 1
    Serial.println("Using i2c");
  #endif
  
  // *** Init MPU 6050 and serial stuff
    accelgyro.initialize();
    mpu.initialize();
    devStatus = mpu.dmpInitialize();
    
    // Supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip
    
    // Make sure it worked (returns 0 if so)
    if (devStatus == 0) {
      // turn on the DMP, now that it's ready
      #if LOOM_DEBUG == 1
        Serial.println(F("Enabling DMP..."));
      #endif
      
      mpu.setDMPEnabled(true);

        // Uncomment following 2 lines if using enable Arduino Uno, MO, or Trinket interrupt detection
      #if LOOM_DEBUG == 1
        Serial.println(F("Enabling MPU interrupt detection (Arduino external interrupt 0)..."));
      #endif
      
      attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);

      // Uncomment following 2 lines if using Adafruit Feather 32u4
      // enable interrupt for PCINT7...
      #if LOOM_DEBUG == 1
        //Serial.println(F("Enabling MPU interrupt detection PCINT 7 (pin 11)"));
      #endif
      
      //pciSetup(INTERRUPT_PIN);
        
        mpuIntStatus = mpu.getIntStatus();

      // Set our DMP Ready flag so the main loop() function knows it's okay to use it
      #if LOOM_DEBUG == 1
          Serial.println(F("DMP ready! Waiting for first interrupt..."));
      #endif
      dmpReady = true;

      // get expected DMP packet size for later comparison
      packetSize = mpu.dmpGetFIFOPacketSize();
    } else { // of if(defStatus == 0)
      // ERROR!
      // 1 = initial memory load failed
      // 2 = DMP configuration updates failed
      // (if it's going to break, usually the code will be 1)
      
      #if LOOM_DEBUG == 1
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
      #endif
    } // of else
		return dmpReady;
}


// ================================================================ 
// ===                        FUNCTIONS                         === 
// ================================================================






// --- MEASURE MPU6050 ---
// Generic subroutine for reading MPU6050 i2c Data
// Arguments:
// Return:
void measure_mpu6050(void)
{
  // wait for MPU interrupt or extra packet(s) available
  while (!mpuInterrupt && fifoCount < packetSize) {
    // other program behavior stuff here
    // .
    // if you are really paranoid you can frequently test in between other
    // stuff to see if mpuInterrupt is true, and if so, "break;" from the
    // while() loop to immediately process the MPU data
    // .
  }

  // Read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gy, &gx, &gz);

  // Reset interrupt flag and get INT_STATUS byte
  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  // Get current FIFO count
  fifoCount = mpu.getFIFOCount();

  // Check for overflow (this should never happen unless our code is too inefficient)
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();      // Reset so we can continue cleanly
    
    #if LOOM_DEBUG == 1
      Serial.println(F("FIFO overflow!"));    // NOTE: If you get this message, MPU6050 library, file "MPU6050_6Axis_MotionApps20.h" modify last byte of line 305 0x07 to 0x09
    #endif
    
    // Otherwise, check for DMP data ready interrupt (this should happen frequently)
  } else if (mpuIntStatus & 0x02) {
    // Wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // Read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // Track FIFO count here in case there is > 1 packet available
    // (This lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;


    // Now format and compile readings based on definitions from top of sketch
    #ifdef OUTPUT_READABLE_YAWPITCHROLL
      // display Euler angles in degrees
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      
      #if LOOM_DEBUG == 1
  /*      Serial.print("ypr\t");
          Serial.print(ypr[0] * 180/M_PI);
          Serial.print("\t");
          Serial.print(ypr[1] * 180/M_PI);
          Serial.print("\t");
          Serial.println(ypr[2] * 180/M_PI); */
      #endif
    #endif
    

    #ifdef OUTPUT_BINARY_YAWPITCHROLL
      // Display Euler angles in degrees
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      yaw   = ypr[0] * 180 / M_PI;
      pitch = ypr[1] * 180 / M_PI;
      roll  = ypr[2] * 180 / M_PI;
      
      #if LOOM_DEBUG == 1
        Serial.write((uint8_t)(yaw >> 8));   Serial.write((uint8_t)(yaw & 0xFF));
        Serial.write((uint8_t)(pitch >> 8)); Serial.write((uint8_t)(pitch & 0xFF));
        Serial.write((uint8_t)(roll >> 8));  Serial.write((uint8_t)(roll & 0xFF));
      #endif
    #endif


    #ifdef OUTPUT_READABLE_REALACCEL
      // Display real acceleration, adjusted to remove gravity
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetAccel(&aa, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
      
      #if LOOM_DEBUG == 1
        Serial.print("areal\t");
        Serial.print(aaReal.x);
        Serial.print("\t");
        Serial.print(aaReal.y);
        Serial.print("\t");
        Serial.println(aaReal.z);
      #endif
    #endif


    #ifdef OUTPUT_READABLE_WORLDACCEL
      // Display initial world-frame acceleration, adjusted to remove gravity
      // and rotated based on known orientation from quaternion
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetAccel(&aa, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
      mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
      
      #if LOOM_DEBUG == 1
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
      // Display tab-separated accel/gyro x/y/z values
      #if LOOM_DEBUG == 1
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
      #if LOOM_DEBUG == 1
        Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
        Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
        Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
        Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
        Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
        Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
      #endif
    #endif
  } // of else if
  //   return (void);
} // of measure_mpu6050() 




// --- MEAN SENSOR MEASUREMENTS ---
// 
// Arguments:
// Return:
void meansensors()
{
  long i = 0, buff_ax = 0, buff_ay = 0, buff_az = 0, buff_gx = 0, buff_gy = 0, buff_gz = 0;

  while (i < (buffersize + 101)) {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);   // Read raw accel/gyro measurements from device
    
    if (i > 100 && i <= (buffersize + 100)) {             //First 100 measures are discarded
      buff_ax = buff_ax + ax;
      buff_ay = buff_ay + ay;
      buff_az = buff_az + az;
      buff_gx = buff_gx + gx;
      buff_gy = buff_gy + gy;
      buff_gz = buff_gz + gz;
    }
    if (i == (buffersize + 100)) {
      mean_ax = buff_ax / buffersize;
      mean_ay = buff_ay / buffersize;
      mean_az = buff_az / buffersize;
      mean_gx = buff_gx / buffersize;
      mean_gy = buff_gy / buffersize;
      mean_gz = buff_gz / buffersize;
    }
    i++;
    delay(2);   // Needed so we don't get repeated measures
  }
}



// --- CALIBRATION ---
// 
// Arguments:
// Return:
void calibration() {
  config_mpu6050->ax_offset = -mean_ax / 8;
  config_mpu6050->ay_offset = -mean_ay / 8;
  config_mpu6050->az_offset = (16384 - mean_az) / 8;

  config_mpu6050->gx_offset = -mean_gx / 4;
  config_mpu6050->gy_offset = -mean_gy / 4;
  config_mpu6050->gz_offset = -mean_gz / 4;
  
  while (1) {
    int ready = 0;
    accelgyro.setXAccelOffset(config_mpu6050->ax_offset);
    accelgyro.setYAccelOffset(config_mpu6050->ay_offset);
    accelgyro.setZAccelOffset(config_mpu6050->az_offset);

    accelgyro.setXGyroOffset(config_mpu6050->gx_offset);
    accelgyro.setYGyroOffset(config_mpu6050->gy_offset);
    accelgyro.setZGyroOffset(config_mpu6050->gz_offset);

    meansensors();
    #if LOOM_DEBUG == 1
			
      Serial.println("...");
    #endif

    if (abs(mean_ax) <= acel_deadzone) ready++;
    else config_mpu6050->ax_offset = config_mpu6050->ax_offset - mean_ax / acel_deadzone;

    if (abs(mean_ay) <= acel_deadzone) ready++;
    else config_mpu6050->ay_offset = config_mpu6050->ay_offset - mean_ay / acel_deadzone;

    if (abs(16384 - mean_az) <= acel_deadzone) ready++;
    else config_mpu6050->az_offset = config_mpu6050->az_offset + (16384 - mean_az) / acel_deadzone;

    if (abs(mean_gx) <= giro_deadzone) ready++;
    else config_mpu6050->gx_offset = config_mpu6050->gx_offset - mean_gx / (giro_deadzone + 1);

    if (abs(mean_gy) <= giro_deadzone) ready++;
    else config_mpu6050->gy_offset = config_mpu6050->gy_offset - mean_gy / (giro_deadzone + 1);

    if (abs(mean_gz) <= giro_deadzone) ready++;
    else config_mpu6050->gz_offset = config_mpu6050->gz_offset - mean_gz / (giro_deadzone + 1);
		
		
    if (ready == 6) break;
  }
} // pf calibration



// --- UPD MPU6050 ---
// 
// Arguments:
// Return:
void package_mpu6050(OSCBundle *bndl, char packet_header_string[])
{
  package_mpu6050(bndl, packet_header_string, NULL); 
}


// --- UPD MPU6050 ---
// 
// Arguments:
// Return:
void package_mpu6050(OSCBundle *bndl, char packet_header_string[], uint8_t port)
{
  char addressString[255];    // Declare address string buffer
	if (port != NULL) {
		sprintf(addressString, "%s%s%d%s", packet_header_string, "/port", port, "/tsl2591/data");
	}
	else {
		sprintf(addressString, "%s%s", packet_header_string, "/tsl2591/data");
	}

  // Messages want an OSC address as first argument
  // Compile bundle

  float axf = (float)ax / 16000;
  float ayf = (float)ay / 16000;
  float azf = (float)az / 16000;

  bool freefall = false;
  // Evaluate if accelerometers are all around zero-G, if so, set freefall bool to true
  if ((axf > -0.2 && axf < 0.2) && (ayf > -0.2 && ayf < 0.2) && (azf > -0.2 && azf < 0.2))
    freefall = true;
  
  // Assemble UDP Packet
  // IP1 IP2 Yaw Pitch Roll aX aY aZ gX gY gZ vBatt
  #ifdef OUTPUT_READABLE_YAWPITCHROLL
    sprintf(addressString, "%s%s", packet_header_string, "/yaw");
    bndl->add(addressString).add((float)(ypr[0] * 180 / M_PI));
    sprintf(addressString, "%s%s", packet_header_string, "/roll");
    bndl->add(addressString).add((float)(ypr[1] * 180 / M_PI));
    sprintf(addressString, "%s%s", packet_header_string, "/pitch");
    bndl->add(addressString).add((float)(ypr[2] * 180 / M_PI));
  #endif
  
  sprintf(addressString, "%s%s", packet_header_string, "/accelX");
  bndl->add(addressString).add(axf);
  sprintf(addressString, "%s%s", packet_header_string, "/accelY");
  bndl->add(addressString).add(ayf);
  sprintf(addressString, "%s%s", packet_header_string, "/accelZ");
  bndl->add(addressString).add(azf);
  sprintf(addressString, "%s%s", packet_header_string, "/gyroX");
  bndl->add(addressString).add((float)gx / 16000);
  sprintf(addressString, "%s%s", packet_header_string, "/gyroY");
  bndl->add(addressString).add((float)gy / 16000);
  sprintf(addressString, "%s%s", packet_header_string, "/gyroZ");
  bndl->add(addressString).add((float)gz / 16000);
  
  #ifdef OUTPUT_READABLE_WORLDACCEL
    sprintf(addressString, "%s%s", packet_header_string, "/rwx");
    bndl->add(addressString).add(aaWorld.x);
    sprintf(addressString, "%s%s", packet_header_string, "/rwy");
    bndl->add(addressString).add(aaWorld.y);
    sprintf(addressString, "%s%s", packet_header_string, "/rwz");
    bndl->add(addressString).add(aaWorld.z);
  #endif
  
  sprintf(addressString, "%s%s", packet_header_string, "/freefall");
  bndl->add(addressString).add(freefall);
}



// --- CALIBRATE MPU6050 ---
// 
// Arguments:
// Return:
void calMPU6050()
{
  // Reset offsets
  accelgyro.setXAccelOffset(0);
  accelgyro.setYAccelOffset(0);
  accelgyro.setZAccelOffset(0);
  accelgyro.setXGyroOffset(0);
  accelgyro.setYGyroOffset(0);
  accelgyro.setZGyroOffset(0);

  if (state == 0) {
    meansensors();
    state++;
    delay(1000);
  }

  if (state == 1) {
    #if LOOM_DEBUG == 1
      Serial.println("\nCalculating offsets...");
    #endif
    calibration();
    //configuration.checksum = memValidationValue;
    state++;
    delay(1000);
  }

  if (state == 2) {
    meansensors();
    // Save Sample Period into EEPROM for next power-up
    //    EEPROM_writeAnything(0, configuration);
    
    #if LOOM_DEBUG == 1
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
      Serial.print(config_mpu6050->ax_offset);
      Serial.print("\t");
      Serial.print(config_mpu6050->ay_offset);
      Serial.print("\t");
      Serial.print(config_mpu6050->az_offset);
      Serial.print("\t");
      Serial.print(config_mpu6050->gx_offset);
      Serial.print("\t");
      Serial.print(config_mpu6050->gy_offset);
      Serial.print("\t");
      Serial.println(config_mpu6050->gz_offset);
    #endif
    state = 0; // reset state flag
  } // of if (state == 2)
} // of calMPU6050()




// --- CALIBRATE MPU6050 OSC ---
// 
// Arguments:
// Return:
void calMPU6050_OSC(OSCMessage &msg) 
{
  #if LOOM_DEBUG == 1
    Serial.println("Command received to calibrate MPU6050");
  #endif
  
  calMPU6050();
  
  #if LOOM_DEBUG == 1
    Serial.println("New calibration values written to non-volatile memory");
  #endif
}



