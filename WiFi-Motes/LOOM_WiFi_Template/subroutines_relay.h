
// Define which physical pins each relay would be plugged into 
#define RELAY_PIN0 9
#define RELAY_PIN1 10
#define RELAY_PIN2 11
#define RELAY_PIN3 14 //A0
#define RELAY_PIN4 15 //A1
#define RELAY_PIN5 16 //A2

// Array to hold relay states (on/off)
bool relay_on[6];



// Function called by setup()
void relay_setup() 
{
    pinMode(RELAY_PIN0,OUTPUT);
    pinMode(RELAY_PIN1,OUTPUT);
    pinMode(RELAY_PIN2,OUTPUT);
    pinMode(RELAY_PIN3,OUTPUT);
    pinMode(RELAY_PIN4,OUTPUT);
    pinMode(RELAY_PIN5,OUTPUT);
    relay_on[0] = false;
    relay_on[1] = false;
    relay_on[2] = false;
    relay_on[3] = false;
    relay_on[4] = false;
    relay_on[5] = false;
}


// Set relay state
void handleRelay(OSCMessage &msg) 
{
  int relay  = msg.getInt(0);
  int set_to = msg.getInt(1);
  relay_on[relay] = (set_to == 1);
  
  #if DEBUG == 1
    Serial.print("Set ");
    Serial.print(relay);
    Serial.print(" to ");
    Serial.println((relay_on[relay]) ? "ON" : "OFF");
  #endif
}



// Update relay states
void write_relay_states()
{
  //digitalWrite(RELAY_PIN0,(relay_on[0]==true) ? HIGH : LOW); //NOTE: pin 9 is bad news
  digitalWrite(RELAY_PIN1,(relay_on[1]==true) ? HIGH : LOW);
  digitalWrite(RELAY_PIN2,(relay_on[2]==true) ? HIGH : LOW);
  digitalWrite(RELAY_PIN3,(relay_on[3]==true) ? HIGH : LOW);
  digitalWrite(RELAY_PIN4,(relay_on[4]==true) ? HIGH : LOW);
  digitalWrite(RELAY_PIN5,(relay_on[5]==true) ? HIGH : LOW);
}

