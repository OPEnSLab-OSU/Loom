


// SERVO SETUP
// Called by main setup
// Arguments:
// Return:
void servo_setup() 
{
  pwm.begin();
  pwm.setPWMFreq(60);
}


//  SET SERVO DEGREE
// Changes specified servo (number) to provided position (degree)
// Arguments:
// Return:
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



// SET SERVO
// Parses OSC message for which servo and position to call set_servo_degree() with
// Arguments:
// Return:
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


