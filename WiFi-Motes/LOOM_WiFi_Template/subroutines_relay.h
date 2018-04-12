// --- RELAY SETUP ---
// Called by main setup
// Sets pin modes for relays (on pins 5 and 6)
// Initializes relays to off
// Arguments: none
// Return:    none
void relay_setup() 
{
    pinMode(RELAY_PIN0,OUTPUT);
    pinMode(RELAY_PIN1,OUTPUT);
    relay_on[0] = false;
    relay_on[1] = false;
}


// --- HANDLE RELAY ---
// Updates stored state of relays
// Arguments: msg (OSC message with data of state to set particular relay to)
// Return:    none
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


// --- WRTIE RELAY STATES ---
// Writes current relay states to the physical relays
// Called after handleRelay, which sets the variables holding the state values
// Arguments: none
// Return:    none
void write_relay_states()
{
  digitalWrite(RELAY_PIN0,(relay_on[0]==true) ? HIGH : LOW);
  digitalWrite(RELAY_PIN1,(relay_on[1]==true) ? HIGH : LOW);
}

