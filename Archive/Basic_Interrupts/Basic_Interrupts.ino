const byte ledPin = 13;
const byte interruptPin = 9;
volatile byte state = LOW;

void setup() {

Serial.begin(9600);
while(!Serial);
	
  pinMode(ledPin, OUTPUT);
  pinMode(interruptPin, INPUT_PULLUP);
    pinMode(10, INPUT_PULLUP);
    pinMode(11, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(interruptPin), blink, CHANGE);
  attachInterrupt(digitalPinToInterrupt(10), on, LOW);
  attachInterrupt(digitalPinToInterrupt(11), off, LOW);


Serial.println("Ready");
}

void loop() {
  digitalWrite(ledPin, state);
}

void blink() {
  state = !state;
  Serial.print("State: ");
  Serial.println(state);
}

void off() {
	state = LOW;
}

void on() {
	state = HIGH;
}


