#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>

#define num_steppers 2

struct state_stepper_t {
  Adafruit_MotorShield *AFMS;
  Adafruit_StepperMotor *myMotors[num_steppers];
  int rpms[num_steppers];
};

state_stepper_t state_stepper;
void setup_stepper();
void handleStepper(OSCMessage &);
void set_stepper_degree(int, int, int, int);

void setup_stepper(){
  state_stepper.AFMS = new Adafruit_MotorShield();
  for (int i = 0; i < num_steppers; i++){
    state_stepper.myMotors[i] = state_stepper.AFMS->getStepper(200, i+1);
  }
  state_stepper.AFMS->begin();
  for (int i = 0; i < num_steppers; i++){
    state_stepper.myMotors[i]->setSpeed(50);
  }
  yield();
  set_stepper_degree(0,0,60,50);
  set_stepper_degree(1,0,60,50);
}

void set_stepper_degree(int motor_choice, int stepper_direction, int degree, int stepper_speed){
  
  Serial.println("setting stepper...");
  int set_degree = map(degree,0,360,0,200);
  if (stepper_speed > 0){
    state_stepper.myMotors[motor_choice]->setSpeed(stepper_speed);
  }
  state_stepper.myMotors[motor_choice]->step(set_degree,stepper_direction == 0 ? FORWARD : BACKWARD,SINGLE);
  yield();
}
  
void handleStepper(OSCMessage &msg){
  int motor = msg.getInt(0);
  int stepper_direction = msg.getInt(1);
  int degree = msg.getInt(2);
  int stepper_speed = msg.getInt(3);
  #if LOOM_DEBUG == 1
    Serial.print("received message to move the motor ");
    Serial.print(degree);
    Serial.println(" degrees");
  #endif
  set_stepper_degree(motor,stepper_direction,degree,stepper_speed);
  #if LOOM_DEBUG == 1
    Serial.println("processed stepper request");
  #endif
}
