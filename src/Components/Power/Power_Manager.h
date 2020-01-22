#include <ArduinoJson.h>

#include "Module.h"

#undef min
#undef max
#include <vector>

class LoomManager;
class Power;

class Power_Manager : public LoomModule {
  public:

    // Constructors
    Power_Manager(LoomManager*);
    Power_Manager(LoomManager*, JsonArrayConst p);

    // Interface Registration
    template<typename T>
    void Register_Power(T t) {
      Interfaces.push_back(static_cast<Power*>(t));
    };

    // === Macromanagement ===
    // Power up all registered interfaces
    void power_up();
    // Derivative functions

    // Power down all registered interfaces
    void power_down();
    // Derivative functions

  protected:
    // List of Interfaces to manager
    std::vector<Power*> Interfaces;
};