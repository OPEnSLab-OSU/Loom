
// This is given its own file for the sake of separating the section of code that
// will change with module additions from the rest of the implementation of Loom_Manager

#include "Loom_Module.h"
#include "Loom_Manager.h"

#include "Loom_Module_Stub.h"

// class Loom_Module_Stub;


Loom_Module_Stub global_stub;



LoomModule*	LoomManager::module_access_aux(ModuleType type, int idx, LoomModule** modules, int count)
{
	int current = 0;

	for (int i = 0; i < count; i++) {
		if (type == modules[i]->get_module_type()) {
			
			if (current == idx) {
				return modules[i];
			} else {
				current++;
			}

		}
	}

	print_device_label();
	Println("No match found");
	return nullptr;
}





Loom_Relay& LoomManager::Relay(int idx) 
{
	LoomModule* tmp = module_access_aux(ModuleType::Relay, idx, (LoomModule**)actuator_modules, sensor_count);
	return (*(Loom_Relay*)( (tmp) ? tmp : &global_stub ));
}

