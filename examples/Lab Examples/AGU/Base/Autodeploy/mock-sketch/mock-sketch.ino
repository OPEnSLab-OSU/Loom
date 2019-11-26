// Generic template
template<class T>
inline Print &operator <<(Print &stream, T arg)
{ stream.print(arg); return stream; }

constexpr char STATUS = 'R';
constexpr char DIAGNOSE = 'D';
constexpr char START_LOOM = 'S';
constexpr char TIME[] = __TIMESTAMP__;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!Serial.available());
  const char first = Serial.read();
  if (first == STATUS)
    Serial << "R:\"mydevice\",1,\"" << TIME << "\",\"" << TIME << "\"\r\n";
  else if (first == DIAGNOSE) {
    Serial << "D:\"mysensor\",1,\"\"\r\n";
    Serial << "D:\"othersensor\",0,\"Device Id Incorrect\"\r\n";
  }
  else if (first == START_LOOM)
    Serial << "S:OK\r\n";
  else
    Serial << "ERROR\r\n";
  while(Serial.available() && Serial.read() != '\n');   
}