// START OF SERVO FUNCTIONS AND DECLARATIONS
//#ifdef is_servo


#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150     // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600     // This is the 'maximum' pulse length count (out of 4096)

#if (num_servos==1)
  int predeg[1] = {0};
  double pre_pulselength[1] = {0.0};
#elif (num_servos==2)
  int predeg[2] = {0, 0};
  double pre_pulselength[2] = {0.0, 0.0};
#elif (num_servos==3)
  int predeg[3] = {0, 0, 0};
  double pre_pulselength[3] = {0.0, 0.0, 0.0};
#elif (num_servos==4)
  int predeg[4] = {0, 0, 0, 0};
  double pre_pulselength[4] = {0.0, 0.0, 0.0, 0.0};
#elif (num_servos==5)
  int predeg[5] = {0, 0, 0, 0, 0};
  double pre_pulselength[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
#elif (num_servos==6)
  int predeg[6] = {0, 0, 0, 0, 0, 0};
  double pre_pulselength[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
#elif (num_servos==7)
  int predeg[7] = {0, 0, 0, 0, 0, 0, 0};
  double pre_pulselength[7] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
#elif (num_servos==8)
  int predeg[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  double pre_pulselength[8] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
#endif




// Function called by setup()
void servo_setup() 
{
  pwm.begin();
  pwm.setPWMFreq(60);
}


// Set Servo Degree
// Changes specified servo (number) to provided position (degree)
void set_servo_degree(int set_degree, int servo_choice) 
{
  uint16_t pulselength = map(set_degree, 0, 180, SERVOMIN, SERVOMAX);
  
  if (set_degree < predeg[servo_choice]) {
    for (double pulselen = pre_pulselength[servo_choice]; pulselen >= pulselength; pulselen--) {
      pwm.setPWM(servo_choice, 0, pulselen);
    }
  }
  
  //When input degree is greater than previous degree, it increases
  if (set_degree > predeg[servo_choice]) {
    for (double pulselen = pre_pulselength[servo_choice]; pulselen < pulselength; pulselen++) {
      pwm.setPWM(servo_choice, 0, pulselen);
    }
  }
  
  predeg[servo_choice] = set_degree;
  pre_pulselength[servo_choice] = pulselength;
}



// Set Servo
// Parses OSC message for which servo and position to call set_servo_degree() with
void set_servo(OSCMessage &msg) 
{
  int servo_num  = msg.getInt(0);
  int set_degree = msg.getInt(1);
  
  #if DEBUG == 1
    Serial.print("received message for servo ");
    Serial.print(servo_num);
    Serial.print(" with degree ");
    Serial.println(set_degree);
  #endif
  
  set_servo_degree(set_degree, servo_num);
}


//#endif //END OF SERVO FUNCTIONS AND DECLARATIONS



