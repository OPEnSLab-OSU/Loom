// ================================================================ 
// ===                        LIBRARIES                         === 
// ================================================================
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Adafruit_PWMServoDriver.h>


// ================================================================ 
// ===                       DEFINITIONS                        === 
// ================================================================


// ================================================================ 
// ===                        STRUCTURES                        === 
// ================================================================
struct state_stepper_t {
	Adafruit_MotorShield *AFMS;
	Adafruit_StepperMotor *myMotors[num_steppers];
	int rpms[num_steppers];
};


// ================================================================ 
// ===                   GLOBAL DECLARATIONS                    === 
// ================================================================
state_stepper_t state_stepper;


// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================
void setup_stepper();
void handle_stepper_msg(OSCMessage &);
void set_stepper_steps(int, int, int, int);


// ================================================================
// ===                          SETUP                           ===
// ================================================================
//
// Creates new object to manage stepper motors
// Sets initial speed for stepper motors
//
void setup_stepper()
{
	state_stepper.AFMS = new Adafruit_MotorShield();
	for (int i = 0; i < num_steppers; i++){
		state_stepper.myMotors[i] = state_stepper.AFMS->getStepper(200, i+1);
	}

	state_stepper.AFMS->begin();

	for (int i = 0; i < num_steppers; i++){
		state_stepper.myMotors[i]->setSpeed(50);
	}
	
	yield();
	set_stepper_steps(0,0,60,50);
	set_stepper_steps(1,0,60,50);
}


// ================================================================
// ===                        FUNCTIONS                         ===
// ================================================================

// --- SET STEPPER DEGREE ---
//
// Called by handle_stepper_msg
// Sets specified motor to move in a direction a number of steps at a given speed
//
// @param motor_choice       Which stepper motor to control
// @param stepper_direction  Which direction to move (0 being clockwise)
// @param steps              How many steps to move (200 steps per full rotation)
// @param stepper_speed      Speed to move at (small values, e.g. < 20,  may cause jittering
//                             Max speed of 250)
//
void set_stepper_steps(int motor_choice, int stepper_direction, int steps, int stepper_speed)
{
	LOOM_DEBUG_Println("Setting stepper...");

	//int set_degree = map(degree,0,360,0,200);
	if (stepper_speed > 0){
		state_stepper.myMotors[motor_choice]->setSpeed(stepper_speed);
	}
	state_stepper.myMotors[motor_choice]->step(steps,stepper_direction == 0 ? FORWARD : BACKWARD,SINGLE);
	yield();
}



// --- HANDLE STEPPER MSG ---
//
// Parses OSC message for which stepper and position to call set_stepper_steps() on
//
// @param msg  OSC message holding a stepper number rotation information
// 
void handle_stepper_msg(OSCMessage &msg)
{
	int motor     = msg.getInt(0);
	int direction = msg.getInt(1);
	int steps     = msg.getInt(2);
	int speed     = msg.getInt(3);

	LOOM_DEBUG_Print("Received message to move the motor ");
	LOOM_DEBUG_Println2(steps, " stepss");

	set_stepper_steps(motor,direction,steps,speed);
	
	LOOM_DEBUG_Println("Processed stepper request");

	// Reply that stepper is done
	OSCBundle bndl;
	bndl.empty();
	char address_string[80];
	sprintf(address_string, "%s%s", global_packet_header_string, "/StepperDone");
	// sprintf(address_string, "%s%s", configuration.packet_header_string, "/StepperDone");

	bndl.add(address_string);

	// delay(5000);

	#if is_wifi == 1
		wifi_send_bundle(&bndl);
	#endif
	// #if is_lora == 1
	// 	lora_send_bundle(&bndl);
	// #endif
	// #if is_nrf == 1
	// 	nrf_send_bundle(&bndl);
	// #endif
}







