///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_Temperature_Sync.h
/// @brief		File for the LoomTempSync definition.
/// @author		Luke Goertzen
/// @date		2019
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

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

	/// Constructor.
	///
	/// \param[in]  source		Module type to get temperature from
	/// \param[in]  dependant	Module type to forward temperature to
	LoomTempSync(
		const LoomModule::Type		source		= LoomModule::Type::MS5803,
		const LoomModule::Type		dependant	= LoomModule::Type::Analog		// might be an array in the future
	);

	/// Constructor that takes Json Array, extracts args
	/// and delegates to regular constructor
	/// \param[in]	p		The array of constuctor args to expand
	LoomTempSync(JsonArrayConst p);

	/// Destructor.
	~LoomTempSync() = default;

	/// Verify that source and dependant modules exist, sync temperatures
	void	second_stage_ctor() override;

//=============================================================================
///@name	OPERATION
/*@{*/ //======================================================================

	/// Sync the temperature.
	/// Simply calls synchronization implementation LoomTempSync::sync_temp().
	/// Allows the module to run regularly by emulating a sensor, which have
	/// thier measure methods called regularly.
	void		measure() { sync_temp(); }

	/// No package necessary.
	/// Implement with empty body.
	void 		package(JsonObject json) override { /* do nothing */ };

//=============================================================================
///@name	PRINT INFORMATION
/*@{*/ //======================================================================

	void		print_config() override;

private:

	void		sync_temp();
	
	LoomModule*	source;			///< Pointer to module to get temperature from
	LoomModule* dependant;		///< Pointer to module to forward temperature to

	LoomModule::Type source_type;		///< Type of source module
	LoomModule::Type dependant_type;	///< Type of dependant module
};


