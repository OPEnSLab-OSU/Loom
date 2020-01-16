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

#include "Loom_Module.h"

#include <ArduinoJson.h>

#undef min;
#undef max;
#include <vector>;

class WarmUp;

class Loom_WarmUp_Manager : public LoomModule {
  public:
    ///Constructor
    Loom_WarmUp_Manager();
    Loom_WarmUp_Manager(JsonArrayConst);

    /// Registers a module as a WarmUp Interface providing access;
    template<typename T>
    void Register_WarmUp(T t) {
      Interfaces.push_back(static_cast<WarmUp*>(t));
    }

    void package(JsonObject) {return;}

  private:
    /// Collection of LoomModule* cast to WarmUp* to access members in an encapsulated fasion
    std::vector<WarmUp*> Interfaces;
};