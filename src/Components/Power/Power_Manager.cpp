#include "Power_Manager.h"

Power_Manager::Power_Manager(LoomManager* manager)
  : LoomModule(manager, __FUNCTION__, LoomModule::Type::Power_Manager) 
  {
    
}

Power_Manager::Power_Manager(LoomManager* manager, JsonArrayConst p) 
  : Power_Manager(manager)
  {

}