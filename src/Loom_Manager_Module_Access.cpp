
// This is given its own file for the sake of separating the section of code that
// will change with module additions from the rest of the implementation of Loom_Manager

#include "Loom_Module.h"
#include "Loom_Manager.h"
#include "Loom_Module_Stub.h"

// Global stub to reference if user tries to access 
// object that does not exist
Loom_Module_Stub global_stub;




LoomModule*	LoomManager::find_module(ModuleType type, int idx, LoomModule** modules, int count)
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



Loom_Neopixel& LoomManager::Neopixel(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Neopixel, idx, (LoomModule**)actuator_modules, actuator_count);
	return (*(Loom_Neopixel*)( (tmp) ? tmp : &global_stub ));
}

Loom_Relay& LoomManager::Relay(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Relay, idx, (LoomModule**)actuator_modules, actuator_count);
	return (*(Loom_Relay*)( (tmp) ? tmp : &global_stub ));

	// return find_module(ModuleType::Relay, idx, (LoomModule**)actuator_modules, actuator_count);
}

Loom_Servo& LoomManager::Servo(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Servo, idx, (LoomModule**)actuator_modules, actuator_count);
	return (*(Loom_Servo*)( (tmp) ? tmp : &global_stub ));
}

Loom_Stepper& LoomManager::Stepper(int idx) 
{
	LoomModule* tmp = find_module(ModuleType::Stepper, idx, (LoomModule**)actuator_modules, actuator_count);
	return (*(Loom_Stepper*)( (tmp) ? tmp : &global_stub ));
}
















