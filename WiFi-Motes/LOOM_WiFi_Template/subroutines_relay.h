
// Function called by setup()
void relay_setup() 
{
    pinMode(RELAY_PIN0,OUTPUT);
    pinMode(RELAY_PIN1,OUTPUT);
    relay_on[0] = false;
    relay_on[1] = false;
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
  digitalWrite(RELAY_PIN0,(relay_on[0]==true) ? HIGH : LOW);
  digitalWrite(RELAY_PIN1,(relay_on[1]==true) ? HIGH : LOW);
}

