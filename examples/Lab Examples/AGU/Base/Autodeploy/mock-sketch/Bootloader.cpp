#include "Bootloader.h"

constexpr char STATUS = 'R';
constexpr char DIAGNOSE = 'D';
constexpr char START_LOOM = 'S';
constexpr char FLASH_LOOM = 'F';
constexpr char TIME[] = __TIMESTAMP__;
constexpr char FILE_NAME = 'cfg.json';

struct BootProps {
    bool is_written;
    char last_write[25];
    char json_secrets[512];
}

FlashStorage(flash_keys, BootProps);
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
						else if (value[val_index] == '\\') escape_next = true;
						else output[output_index++] = value[val_index];
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

static bool run_flash_cmd(const char* input, const size_t max) {
    // find the splitting point
	size_t idx = find_keys_in_cmd(input, max);
	if (!idx)
		return false;
	// serialize the substitution keys into a "dictionary"
	StaticJsonDocument<512> sub_doc;
	JsonObject keys = sub_doc.to<JsonObject>();
	if (!parse_keys(&input[idx], sizeof(input) - idx, keys))
		return false;
    const char* stamp = keys["stamp"].as<const char*>();
    if (stamp == nullptr)
        return false;
    // flash storage update
    {
        BootProps props = {};
        props.is_written = true;
        strncpy(props.last_write, stamp, sizeof(props.last_write));
        if(!serializeJson(sub_doc, props.json_secrets, sizeof(props.json_secrets)))
            return false;
        flash_keys.write(&props);
    }
    // SD card update
    {
        SdFat card;
        File file;
        sd.begin(10, SD_SCK_MHZ(50));
        if(!file.open(FILE_NAME, O_WRONLY))
            return false;
        file.write(&input[1], idx - 3);
        if (!file.sync() || file.getWriteError())
            return false
        file.close();
    }
    return true;
}

static void load_flash_keys() {
    flash_keys.read(&mem_keys);
}

void Bootloader::get_config(JsonDocument& doc) {
    if (!mem.is_written)
        load_flash_keys();
    // load the config from SD
    char json[2048];
    int read;
    {
        SdFat card;
        File file;
        sd.begin(10, SD_SCK_MHZ(50));
        if(!file.open(FILE_NAME, O_READ))
            return false;
        read = file.read(json, sizeof(json);
        if (read < 1)
            return false;
        file.close();
    }
    // deserialize the substitution keys
    StaticJsonDocument<512> sub_keys;
    deserializeJson(sub_keys, mem.json_secrets);
    // run the substitution
	char output[2048];
	if (!json_sub(json, read, keys, output))
		return false;
	// deserialize the JSON
	deserializeJson(doc, static_cast<const char*>(output));
	return !doc.as<JsonObjectConst>().isNull();
}

bool Bootloader::run_bootloader() {
    if (!mem.is_written)
        load_flash_keys();
    // check the serial bus for tranmissions so we can respond!
    if (Serial && Serial.available()) {
        const char first = Serial.read();
        if (first == STATUS)
            Serial << "R:\"mydevice\",0,\"" << TIME << "\",\"" << mem.last_write << "\"\r\n";
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
            StaticJsonDocument<2048> doc;
            if (input[0] != ':' || !run_flash_cmd(&input[1], sizeof(input) - 1, doc))
                Serial << "ERROR\r\n";
            else
                Serial << "F:OK\r\n";
        } else if (first == START_LOOM){
            Serial << "S:OK\r\n";
            Serial.flush();
            Serial.end();
            // reset!
            RESET();
        }
        else
            Serial << "ERROR\r\n";
        while (Serial.available() && Serial.read() != '\n');
    }
}