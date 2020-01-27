///////////////////////////////////////////////////////////////////////////////
///
/// @file		Power.h
/// @brief  Header file for Loom Module Power mixin, which provides an interface
///         for modules to specify a specific behavior for power up and power down.
/// @author		C. Greyston Brady
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

class LoomManager;

class Power {
  public:
    // Constructor
    Power(LoomManager* manager);

    // State Getter
    bool get_power();

    // State Setter
    virtual void power_up() = 0;
    virtual void power_down() = 0;

  protected:
    // State of interfaced module, either on or off
    bool powered;
};