

// This file contains functions used to build the commands for 
// actuators supported by Loom.
// These commands are equivalent to the ones sent from the Max
// patch interfaces

// ================================================================ 
// ===                   FUNCTION PROTOTYPES                    === 
// ================================================================

void build_neopixel_cmd(OSCBundle *bndl, char* device, int port, int pixelNum, int red, int green, int blue); 
void build_servo_cmd(OSCBundle *bndl, char* device, int servoNum, int degree); 
void build_relay_cmd(OSCBundle *bndl, char* device, int relayNum, int state); 
void build_stepper_cmd(OSCBundle *bndl, char* device, int motor, int direction, int steps, int speed); 

// ================================================================ 
// ===                    ACTUATOR FUNCTIONS                    === 
// ================================================================


// --- NEOPIXEL ---
// 
// Takes 5 ints, representing:
//  port, neopixel number, red val, green val, blue val
// Fills the bundle with the equivalent command
// Device is the header of the device that the message will be sent to
//
void build_neopixel_cmd(OSCBundle *bndl, char* device, int port, int pixelNum, int red, int green, int blue) 
{
	char address_string[80];
	sprintf(address_string, "%s%s", device, "/Neopixel");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add((int32_t)port).add((int32_t)pixelNum).add((int32_t)red).add((int32_t)green).add((int32_t)blue);
	
	bndl->add(msg);
}


// --- SERVO ---
// 
// Takes 2 ints, representing:
//  servo number and the degree to rotate to
// Fills the bundle with the equivalent command
// Device is the header of the device that the message will be sent to
//
void build_servo_cmd(OSCBundle *bndl, char* device, int servoNum, int degree) 
{
	char address_string[80];
	sprintf(address_string, "%s%s", device, "/Servo/Set");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add((int32_t)servoNum).add((int32_t)degree);
	
	bndl->add(msg);
}


// --- RELAY ---
// 
// Takes 2 ints, representing:
//  relay number and the state to set to
// Fills the bundle with the equivalent command
// Device is the header of the device that the message will be sent to
//
void build_relay_cmd(OSCBundle *bndl, char* device, int relayNum, int state) 
{
	char address_string[80];
	sprintf(address_string, "%s%s", device, "/Relay/State");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add((int32_t)relayNum).add((int32_t)state);
	
	bndl->add(msg);
}


// --- STEPPER MOTOR ---
// 
// Takes 4 ints, representing:
//  steeper motor, direction, steps, and speed
// Fills the bundle with the equivalent command
// Device is the header of the device that the message will be sent to
//
void build_stepper_cmd(OSCBundle *bndl, char* device, int motor, int direction, int steps, int speed) 
{
	char address_string[80];
	sprintf(address_string, "%s%s", device, "/Stepper/Set");
	
	OSCMessage msg = OSCMessage(address_string);
	msg.add((int32_t)motor).add((int32_t)direction).add((int32_t)steps).add((int32_t)speed);
	
	bndl->add(msg);
}





