///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_WarmUp_Manager.h
/// @brief  Header file for Loom Module WarmUp Manager, which is responsible 
///         for handeling any WarmUp related behaviors.
/// @author		C. Greyston Brady
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#include "Module.h"

#include <ArduinoJson.h>

#undef min;
#undef max;
#include <vector>;

class WarmUp;

class Loom_WarmUp_Manager : public LoomModule {
  public:
    ///Constructor
    Loom_WarmUp_Manager(LoomManager* manager);
    Loom_WarmUp_Manager(LoomManager* manager, JsonArrayConst);

    /// Registers a module as a WarmUp Interface providing access;
    template<typename T>
    void Register_WarmUp(T t) {
      Interfaces.push_back(static_cast<WarmUp*>(t));
    }

    /// Start a warming timer
    void warming_Begin();

    /// Reset warming to false, indicates a need to reheat in the near future
    void warming_Reset();

    /// Updates and returns state
    bool is_warm();

    /// An unfortunate consequence of inheriting from a bloated base class
    void package(JsonObject) {return;}

  private:
    /// Collection of LoomModule* cast to WarmUp* to access members in an encapsulated fasion
    std::vector<WarmUp*> Interfaces;

    /// State of system, either containing 'cold' items or completely warmed up
    bool warm;

    /// Lenth of longest items to warm up
    int warm_duration;

    /// Time stamp for begining of warming period
    unsigned long start_time;
};