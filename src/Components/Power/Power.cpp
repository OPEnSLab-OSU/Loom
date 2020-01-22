#include "Power.h"

#include "Power_Manager.h"
#include "Manager.h"

Power::Power(LoomManager* manager)
  : powered(false)
  {
    static_cast<Power_Manager*>(manager->find_module(LoomModule::Type::Power_Manager))->Register_Power(this);
}

bool Power::get_power() {
  return powered;
}