#include <cstdio>
#include "ArduinoJson.h"

// Generic template
template<class T>
inline Print &operator <<(Print &stream, T arg)
{ stream.print(arg); return stream; }

constexpr char STATUS = 'R';
constexpr char DIAGNOSE = 'D';
constexpr char START_LOOM = 'S';
constexpr char FLASH_LOOM = 'F';
constexpr char TIME[] = __TIMESTAMP__;

static unsigned int get_keys_from_cmd(const char* input, const size_t max, JsonObject& keys) {
	// find where the key/value pairs start, and convert them to JSON
	unsigned int key_index = 0;
	unsigned int key_start = 0;
	// find two quotes, and then a comma
	bool found_quote = false;
	while (true) {
		if (input[key_index++] == '\'') {
			if (key_index >= max) 
				return 0;
			if (found_quote) {
				key_start = key_index;
				break;
			}
			found_quote = true;
		}
	}
	if (input[key_index] != ',') 
		return 0;
	// once we're there, scan the keys and values into json
	while (key_index < max - 5 && input[key_index] == ',') {
		char key[64];
		char value[64];
		int read = sscanf(&(input[key_index]), ",'%31[^']':'%31[^']'", key, value);
		if (read < 2) 
			return 0;
		keys[key] = value;
		while (++key_index < max && input[key_index] != ',' && input[key_index] != '\0');
	}
	return key_start;
}

static bool json_sub(const char* input, char* output, const size_t max) {
	StaticJsonDocument<512> doc;
	JsonObject sub_obj = doc.to<JsonObject>();
	// put the key/value pairs into the json document
	unsigned int key_start = get_keys_from_cmd(input, max, sub_obj);
	if (!key_start) return false;
	// copy the input JSON into the output, until we hid a substitution "${" or EOF
	unsigned int input_index = 2;
	unsigned int output_index = 0;
	while (input_index < key_start - 1 && output_index < max) {
		// copy JSON into the buffer until we encounter a '$'
		if (input[input_index] == '$' && input[input_index + 1] == '{') {
			// find the key in the JSON
			// skip the '${'
			input_index += 2;
			char key[64];
			{
				unsigned int key_index = 0;
				while (input[input_index] != '}') {
					if (input_index >= key_start - 2 || key_index >= sizeof(key) - 2) return false;
					key[key_index++] = input[input_index++];
				}
				key[key_index] = '\0';
				// skip end curly brace
				input_index++;
			}
			// copy the respective value into the output buffer
			{
				const char* value = sub_obj[key];
				if (value == nullptr) return false;
				if (strnlen(value, 63) + output_index > max) return false;
				// for loop copy, replacing '\r' and '\n' with the real character
				{
					size_t val_index = 0;
					bool escape_next = true;
					while (value[val_index] != '\0') {
						if (escape_next) {
							if (value[val_index] == 'r')
								output[output_index++] = '\r';
							if (value[val_index] == 'n')
								output[output_index++] = '\n';
							escape_next = false;
						}
						else if (value[val_index] == '\\') escape_next = true;
						else output[output_index++] = value[val_index];
						val_index++;
					}
				}
			}
		}
		else output[output_index++] = input[input_index++];
	}
	return true;
}

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
  else if (first == FLASH_LOOM) {
    char input[4096];
    char json[4096];
    unit8_t index = 0;
    while (index < sizeof(input)) {
      if (Serial.available()) {
        char c = static_cast<char>(Serial.read());
        if (c == '\n') {
          // clear the carrage return
          if (index > 0 && input[index - 1] == '\r') 
            input[index - 1] = '\0';
          // all done
          break;
        }
        // else add it to the input buffer
        input[index++] = c;
      }
    }
    // parse the input, and make sure it's all valid
    if (input[0] == ':' && json_sub(&(input[0]), json, sizeof(json)))
      Serial.println("F:OK"); 
    else 
      Serial.println("ERROR");
  }
  else if (first == START_LOOM)
    Serial << "S:OK\r\n";
  else
    Serial << "ERROR\r\n";
  while(Serial.available() && Serial.read() != '\n');   
}