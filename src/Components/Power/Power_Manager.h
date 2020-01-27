///////////////////////////////////////////////////////////////////////////////
///
/// @file		Power_Manager.h
/// @brief  Header file for Loom Module Power Manager, which is responsible 
///         for handeling any power up or down behaviors.
/// @author		C. Greyston Brady
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Module.h"

#include <ArduinoJson.h>

#undef min
#undef max
#include <vector>

class Power;

class Power_Manager : public LoomModule {
  public:

    // Constructors
    Power_Manager(LoomManager*);
    Power_Manager(LoomManager*, JsonArrayConst p);

    // Interface Registration
    void Register_Power(Power* registree);

    // === Macromanagement ===
    // Power up all registered interfaces
    void power_up();
    // Derivative functions

    // Power down all registered interfaces
    void power_down();
    // Derivative functions

  protected:
    // Macro state of power interfaces
    bool powered;

    // List of Interfaces to manager
    std::vector<Power*> Interfaces;
};