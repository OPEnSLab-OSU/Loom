#pragma once

#include "Loom_Module.h"

///////////////////////////////////////////////////////////////////////////////


// ### (LoomModule) | dependencies: [] | conflicts: []
/// Used to synchronize temperatures between sensors that read sensors
/// and modules that need it
// ###
class LoomTempSync : public LoomModule
{

protected:

public:

//=============================================================================
///@name	CONSTRUCTORS / DESTRUCTOR
/*@{*/ //======================================================================

	/// Temp Sync module constructor.
	///
	/// \param[in]  source		
	/// \param[in]  dependant		
	LoomTempSync(
		const LoomModule::Type		source		= LoomModule::Type::MS5803,
		const LoomModule::Type		dependant	= LoomModule::Type::Analog		// might be an array in the future
	);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	LoomTempSync(JsonArrayConst p);

	/// Destructor
	~LoomTempSync() = default;

	/// Verify that source and dependant exist, sync temperatures
	void	second_stage_ctor() override;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	void		measure();
	void 		package(JsonObject json) override { /* do nothing */ };
	bool		dispatch(JsonObject) override { /* do nothing */}

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;

private:

	void		sync_temp();
	
	LoomModule*	source;
	LoomModule* dependant;

	LoomModule::Type source_type;
	LoomModule::Type dependant_type;
};


