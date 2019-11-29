#include "Bootloader.h"

// Generic template
template < class T >
    static inline Print & operator << (Print & stream, T arg) {
        stream.print(arg);
        return stream;
    }

constexpr char STATUS = 'R';
constexpr char DIAGNOSE = 'D';
constexpr char START_LOOM = 'S';
constexpr char FLASH_LOOM = 'F';
constexpr char TIME[] = __TIMESTAMP__;
constexpr char FILE_NAME[] = "cfg.json";

typedef struct {
    bool is_written;
    int instance;
    char last_write[25];
    char json_secrets[512];
}  BootProps;

FlashStorage(flashkeys, BootProps);
static BootProps mem_keys = {};

// system reset definitions
static constexpr auto SYSRESETREQ = (1<<2);
static constexpr auto VECTKEY = (0x05fa0000UL);
static constexpr auto VECTKEY_MASK = (0x0000ffffUL);
/** Trigger a software reset. Only use if in unrecoverable state */
[[ noreturn ]] static void RESET() {
    (*(uint32_t*)0xe000ed0cUL)=((*(uint32_t*)0xe000ed0cUL)&VECTKEY_MASK)|VECTKEY|SYSRESETREQ;
    while(1) { }
}

static size_t find_keys_in_cmd(const char* input, const size_t max) {
	// find where the key/value pairs start, and convert them to JSON
	unsigned int key_index = 0;
	// find two quotes, and then a comma
	bool found_quote = false;
	while (true) {
		if (input[key_index++] == '\'') {
			if (key_index >= max)
				return 0;
			if (found_quote)
				return key_index + 1;
			found_quote = true;
		}
	}
}

static bool parse_keys(const char* input, const size_t max, JsonObject& keys) {
	unsigned int key_index = 0;
	// once we're there, scan the keys and values into json
	while (key_index < max && input[key_index] != '\0') {
		// walk past the comma
		if (input[key_index] == ',') key_index++;
		char key[64];
		char value[1024];
		int read = sscanf(&(input[key_index]), "'%63[^']':'%1023[^']'", key, value);
		if (read == 1)
			value[0] = '\0';
		else if (read < 2)
			return false;
		keys[key] = value;
		while (++key_index < max && input[key_index] != ',' && input[key_index] != '\0');
	}
	return true;
}

static bool json_sub(const char* input, const size_t max, const JsonObjectConst& sub_obj, char* output) {
	// copy the input JSON into the output, until we hid a substitution "${" or EOF
	unsigned int input_index = 0;
	unsigned int output_index = 0;
	while (input_index < max && output_index < max - 1) {
		// copy JSON into the buffer until we encounter a '$'
		if (input[input_index] == '$' && input[input_index + 1] == '{') {
			// find the key in the JSON
			// skip the '${'
			input_index += 2;
			char key[64]; 
			{
				unsigned int key_index = 0;
				while (input[input_index] != '}') {
					if (input_index >= max - 2 || key_index >= sizeof(key) - 2) 
						return false;
					key[key_index++] = input[input_index++];
				}
				key[key_index] = '\0';
				// skip end curly brace
				input_index++;
			}
			// copy the respective value into the output buffer
			{
				const char* value = sub_obj[key];
				if (value == nullptr) 
					return false;
				if (strnlen(value, 1023) + output_index > max) 
					return false;
				// for loop copy, replacing '\r' and '\n' with the real character
				{
					size_t val_index = 0;
					bool escape_next = false;
					while (value[val_index] != '\0') {
						if (escape_next) {
							if (value[val_index] == 'r')
								output[output_index++] = '\r';
							if (value[val_index] == 'n')
								output[output_index++] = '\n';
							escape_next = false;
						}
						else if (value[val_index] == '\\') 
                            escape_next = true;
						else 
                            output[output_index++] = value[val_index];
						val_index++;
					}
				}
			}
		}
		else output[output_index++] = input[input_index++];
	}
	output[output_index] = '\0';
	return true;
}

static bool print_error(const char* error) {
    Serial.println(error);
    return false;
}

static bool run_flash_cmd(const char* input, const size_t max) {
    // find the splitting point
	size_t idx = find_keys_in_cmd(input, max);
	if (!idx)
		return print_error("Could not split keys and json");
	// serialize the substitution keys into a "dictionary"
	StaticJsonDocument<512> sub_doc;
	JsonObject keys = sub_doc.to<JsonObject>();
	if (!parse_keys(&input[idx], sizeof(input) - idx, keys))
		return print_error("Could not parse keys");
    const char* stamp = keys["stamp"].as<const char*>();
    const int instance = keys["instance"];
    if (stamp == nullptr)
        return print_error("No timestamp present");
    // SD card update
    {
        SdFat sd;
        File file;
        if(!sd.begin(10, SD_SCK_MHZ(50)))
            return print_error("Could not open SDcard");
        if(!file.open(FILE_NAME, O_WRONLY | O_CREAT))
            return print_error("Could not open file");
        file.write(&input[1], idx - 3);
        if (!file.sync() || file.getWriteError())
            return print_error("Writing to SD card failed");
        file.close();
    }
    // flash storage update
    {
        BootProps props = {};
        props.is_written = true;
        props.instance = instance;
        strncpy(props.last_write, stamp, sizeof(props.last_write));
        if(!serializeJson(sub_doc, props.json_secrets, sizeof(props.json_secrets)))
            return print_error("JSON serialization failed");
        flashkeys.write(props);
    }
    return true;
}

static void load_flashkeys() {
    flashkeys.read(&mem_keys);
}

bool Bootloader::get_config(JsonDocument& doc) {
    if (!mem_keys.is_written)
        load_flashkeys();
    if (!mem_keys.is_written)
        return false;
    // load the config from SD
    char json[2048];
    int read;
    {
        SdFat sd;
        File file;
        sd.begin(10, SD_SCK_MHZ(50));
        if(!file.open(FILE_NAME, O_READ))
            return false;
        read = file.read(json, sizeof(json));
        if (read < 1)
            return false;
        file.close();
    }
    // deserialize the substitution keys
    StaticJsonDocument<512> sub_keys;
    deserializeJson(sub_keys, mem_keys.json_secrets);
    // run the substitution
    {
        char output[2048];
        const JsonObjectConst sub_obj = sub_keys.as<JsonObjectConst>();
	    if (!json_sub(json, read, sub_obj, output))
		    return false;
	    // deserialize the JSON
	    deserializeJson(doc, static_cast<const char*>(output));
    }
	return !doc.as<JsonObjectConst>().isNull();
}

void Bootloader::run_bootloader() {
    if (!mem_keys.is_written)
        load_flashkeys();
    // check the serial bus for tranmissions so we can respond!
    if (Serial && Serial.available()) {
        const char first = Serial.read();
        if (first == STATUS) {
            Serial << "R:\"" << mem_keys.instance << "\"," << mem_keys.is_written << ",\"" << TIME << "\",\"";
            Serial << static_cast<const char*>(mem_keys.is_written ? mem_keys.last_write : TIME) << "\"\r\n";
        }
        else if (first == DIAGNOSE) {
            // Serial << "D:\"mysensor\",1,\"\"\r\n";
            // Serial << "D:\"othersensor\",0,\"Device Id Incorrect\"\r\n";
            // TODO: this
            Serial << "ERROR\r\n";
        } else if (first == FLASH_LOOM) {
            char input[2048];
            uint16_t index = 0;
            while (index < sizeof(input)) {
                if (Serial.available()) {
                    char c = static_cast<char>(Serial.read());
                    if (c == '\n') {
                        // clear the carrage return and newline
                        if (index > 0 && input[index - 1] == '\r')
                            input[index - 1] = '\0';
                        else
                            input[index] = '\0';
                        // all done
                        break;
                    }
                    // else add it to the input buffer
                    input[index++] = c;
                }
            }
            // parse the input, and make sure it's all valid
            if (input[0] != ':' || !run_flash_cmd(&input[1], sizeof(input) - 1))
                Serial << "ERROR\r\n";
            else
                Serial << "F:OK\r\n";
            // return b/c we already flushed the input buffer
            return;
        } else if (first == START_LOOM){
            Serial << "S:OK\r\n";
            Serial.flush();
            Serial.end();
            // reset!
            RESET();
            return;
        }
        else
            Serial << "ERROR\r\n";
        while (Serial.available() && Serial.read() != '\n');
    }
}