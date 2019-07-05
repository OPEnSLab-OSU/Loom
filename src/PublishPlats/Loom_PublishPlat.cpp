
#include "Loom_PublishPlat.h"
#include "Loom_Manager.h"


///////////////////////////////////////////////////////////////////////////////
LoomPublishPlat::LoomPublishPlat(	
		const char* module_name,
		uint internet_index
	) 
	: LoomModule( module_name )
	, m_internet( nullptr )
	, m_internet_index( internet_index ) 
{
	this->module_type = LoomModule::Type::Publish;
}

///////////////////////////////////////////////////////////////////////////////
void LoomPublishPlat::second_stage_ctor() 
{
    // check to see if we have a device manager
    if (device_manager == nullptr) { print_module_label(); LPrint("No Device Manager!\n"); return; }
    // check if internet platform exist
    LoomModule* temp = (LoomModule*)&(device_manager->InternetPlat(m_internet_index));
    if (temp->get_module_type() == LoomModule::Type::Internet) m_internet = (LoomInternetPlat*)temp;
    else {
        print_module_label();
        LPrint("Unable to find internet platform, intstead got: ", (int)(temp->get_module_type()), " using index ", m_internet_index, "\n");
        return;
    }
    // made it here, guess we're good to go!
    print_module_label();
    LPrint("Ready\n");
}

///////////////////////////////////////////////////////////////////////////////
bool LoomPublishPlat::publish(const JsonObject json) 
{
	// check validity
	if(m_internet == nullptr  || json.isNull()){
		print_module_label();
		LPrint("Could not publish without ");
		if(m_internet == nullptr) LPrint("internet module\n");
		else LPrint("valid data.\n");
		return false;
	}
	// guess we're good to go
	return send_to_internet(json, m_internet);
}

///////////////////////////////////////////////////////////////////////////////
void LoomPublishPlat::print_config() 
{
	LoomModule::print_config();
	// print internet stuff
	print_module_label();
	LPrint("\tInternet Index: ", m_internet_index);
}

///////////////////////////////////////////////////////////////////////////////
void LoomPublishPlat::print_state() 
{
	LoomModule::print_state();
	// print internet stuff
	print_module_label();
	LPrint("\tInternet Connected: ", m_internet != nullptr && m_internet->is_connected());
}

///////////////////////////////////////////////////////////////////////////////