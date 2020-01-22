
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