///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_WarmUp.h
/// @brief  Header file for Loom Module WarmUp mixin, which provides an interface
///         for modules to specify a delay before sensor is ready to take measurements.
/// @author		C. Greyston Brady
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

// Forward Declarations
class WarmUp_Manager;
class LoomManager;

class WarmUp {
  public:
    /// Constructor
    WarmUp(LoomManager* manager, int period = 0);

    int get_period();

  private:
    /// MS till sensor is ready to take readings
    int period;

};