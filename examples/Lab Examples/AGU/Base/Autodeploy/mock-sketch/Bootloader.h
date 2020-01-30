#include <cstdio>
#include "Arduino.h"
#include "ArduinoJson.h"
#include "SdFat.h"
#include "FlashStorage.h"
#include "FormatSD.h"

namespace Bootloader {
    bool get_config(JsonDocument& doc);
    void run_bootloader();
}