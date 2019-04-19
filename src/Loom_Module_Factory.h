

#ifndef LOOM_MODULE_FACTORY_h
#define LOOM_MODULE_FACTORY_h



#include "Loom_Module.h"

#include <ArduinoJson.h>


// Should overload to just take 'const char*' in which use code provided defaults
// multiplexer might make use of this


LoomModule* module_factory_aux(JsonVariant module);


#endif // of LOOM_MODULE_FACTORY_h