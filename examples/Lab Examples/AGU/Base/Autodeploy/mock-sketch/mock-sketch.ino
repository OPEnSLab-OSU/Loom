#include <cstdio>
#include "ArduinoJson.h"
#include "Bootloader.h"


void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
	StaticJsonDocument<2048> doc;
}

void loop() {
    // put your main code here, to run repeatedly:
    while (!Serial.available());
    const char first = Serial.read();
    if (first == STATUS)
        Serial << "R:\"mydevice\",1,\"" << TIME << "\",\"" << TIME << "\"\r\n";
    else if (first == DIAGNOSE) {
        Serial << "D:\"mysensor\",1,\"\"\r\n";
        Serial << "D:\"othersensor\",0,\"Device Id Incorrect\"\r\n";
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
    } else if (first == START_LOOM)
        Serial << "S:OK\r\n";
    else
        Serial << "ERROR 3\r\n";
    while (Serial.available() && Serial.read() != '\n');
}