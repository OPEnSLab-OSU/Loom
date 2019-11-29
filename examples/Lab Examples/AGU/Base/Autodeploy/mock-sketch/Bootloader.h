#include <cstdio>
#include "ArduinoJson.h"
#include "SdFat.h"
#include "FlashStorage.h"

namespace Bootloader {
    bool get_config(JsonDocument& doc);
    void run_bootloader();
}