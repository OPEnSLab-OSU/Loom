#include <cstdio>
#include "ArduinoJson.h"
#include "SdFat.h"
#include "FlashStorage.h"

namespace Bootloader {
    bool is_sketch();
    void get_config(JsonDocument& doc);
    void run_bootloader();
}