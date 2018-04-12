// RELAY SETUP
// Called by main setup
// Arguments:
// Return:
void relay_setup() 
{
    pinMode(RELAY_PIN0,OUTPUT);
    pinMode(RELAY_PIN1,OUTPUT);
    relay_on[0] = false;
    relay_on[1] = false;
}


// HANDLE RELAY
// Updates stored state of relays
// Arguments:
// Return:
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


// WRTIE RELAY STATES
// Writes updated relay states to the physical relays
// Arguments:
// Return:
void write_relay_states()
{
  digitalWrite(RELAY_PIN0,(relay_on[0]==true) ? HIGH : LOW);
  digitalWrite(RELAY_PIN1,(relay_on[1]==true) ? HIGH : LOW);
}

